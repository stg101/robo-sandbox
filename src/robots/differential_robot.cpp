#include "differential_robot.h"

DifferentialRobot::DifferentialRobot()
{
	avr = NULL;
	is_paused = false;
	is_ready = false;
	ns_debt = 0;
}

DifferentialRobot::~DifferentialRobot()
{
}

void DifferentialRobot::keyPress(unsigned char key) /* called on key press */
{

	printf("pressed key %c \n", key);
	if (is_paused && key == 'H')
	{
		is_paused = false;
		return;
	}

	switch (key)
	{
	case 'J':
		printf("Message sent\n");
		sensor.apply();
		break;
	case 'H':
		printf("*** PAUSED ***\n");
		is_paused = true;
		break;
	}
}

void DifferentialRobot::runInterfaceThread()
{
	auto start = chrono::steady_clock::now();
	auto current_time = chrono::steady_clock::now();
	int elapsed_time = chrono::duration_cast<chrono::milliseconds>(current_time - start).count();
	uint8_t old_pin_state = 0;

	while (true)
	{
		current_time = chrono::steady_clock::now();
		elapsed_time = chrono::duration_cast<chrono::milliseconds>(current_time - start).count();

		if (elapsed_time >= 1000 / 240)
		{

			if (is_paused)
				continue;

			for (int i = 0; i < 2; i++)
			{
				actuator_array[i].apply();
			}

			start = chrono::steady_clock::now();
		}
	}
}

void DifferentialRobot::runAvrThread()
{
	while (true)
	{
		avr_run(avr);
	}
}

// path "./src/atmega_firmware.elf";
int DifferentialRobot::createMCU(const char *firmware_path)
{
	fname = firmware_path;

	avr = avr_make_mcu_by_name("atmega328p");

	if (!avr)
	{
		printf("mcu name not found\n");
		return -1;
	}
	avr_init(avr);

	avr_extint_set_strict_lvl_trig(avr, EXTINT_IRQ_OUT_INT0, 0);
	avr_extint_set_strict_lvl_trig(avr, EXTINT_IRQ_OUT_INT1, 0);
	{
		/* Load .hex and setup program counter */
		uint32_t boot_base, boot_size;
		uint8_t *boot = read_ihex_file(fname, &boot_size, &boot_base);
		if (!boot)
		{
			fprintf(stderr, "Unable to load %s\n", fname);
			return -1;
		}
		memcpy(avr->flash + boot_base, boot, boot_size);
		free(boot);
		avr->pc = boot_base;
		/* end of flash, remember we are writing /code/ */
		avr->codeend = avr->flashend;
	}

	/* more simulation parameters */
	avr->frequency = MHZ_16;
	avr->aref = ADC_VREF_V256;
	avr->sleep = sleepHook;

	sensor.connect(avr);

	for (int i = 0; i < 2; i++)
		actuator_array[i].connect(avr, i);
}

void DifferentialRobot::createBody(b2World *world, Body *body)
{
	m_world = world;
	m_body = dynamic_cast<DifferentialRobotBody *>(body);

	for (int i = 0; i < 2; i++)
		actuator_array[i].createBody(m_world, i, m_body->m_body, &motorBody_array[i]);
}

void DifferentialRobot::runSim()
{
	interface_thread = thread(&DifferentialRobot::runInterfaceThread, this);
	avr_thread = thread(&DifferentialRobot::runAvrThread, this);

	interface_thread.detach();
	avr_thread.detach();
}

void DifferentialRobot::setIsReady(bool is_ready_value)
{
	is_ready = is_ready_value;
}

void DifferentialRobot::runTimeBatch(uint64_t run_ns)
{
	if (ns_debt > run_ns)
		ns_debt -= run_ns;
	else
	{
		uint64_t start_ns = avr_cycles_to_nsec(avr, avr->cycle);
		uint64_t runtime_ns = avr_cycles_to_nsec(avr, avr->cycle);

		uint64_t total_run_ns = run_ns - ns_debt;
		uint64_t elapsed_ns = runtime_ns - start_ns;

		while (elapsed_ns < total_run_ns)
		{
			avr_run(avr);
			runtime_ns = avr_cycles_to_nsec(avr, avr->cycle);
			elapsed_ns = runtime_ns - start_ns;
		}

		ns_debt = elapsed_ns - total_run_ns;
	}

	for (int i = 0; i < 2; i++)
	{
		actuator_array[i].apply();
	}
}

void DifferentialRobot::sleepHook(
	avr_t *avr,
	avr_cycle_count_t how_long)
{
	return;
}