#include "robot.h"

Robot::Robot()
{
	avr = NULL;
	is_paused = false;
}

Robot::~Robot()
{
}

void Robot::keyPress(unsigned char key) /* called on key press */
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

void Robot::runInterfaceThread()
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

void Robot::runAvrThread()
{
	while (true)
	{
		avr_run(avr);
	}
}

// path "./src/atmega_firmware.elf";
int Robot::createMCU(const char *firmware_path)
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
	avr->sleep = avr_callback_sleep_sync;
	avr->aref = ADC_VREF_V256;

	sensor.connect(avr);

	for (int i = 0; i < 2; i++)
		actuator_array[i].connect(avr, i);
}

void Robot::createBody(b2World *n_m_world)
{
	m_world = n_m_world;

	//create car body
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	m_body = m_world->CreateBody(&bodyDef);
	m_body->SetAngularDamping(3);

	b2Vec2 vertices[8];
	vertices[0].Set(1.5, 0);
	vertices[1].Set(3, 2.5);
	vertices[2].Set(2.8, 5.5);
	vertices[3].Set(1, 10);
	vertices[4].Set(-1, 10);
	vertices[5].Set(-2.8, 5.5);
	vertices[6].Set(-3, 2.5);
	vertices[7].Set(-1.5, 0);
	b2PolygonShape polygonShape;
	polygonShape.Set(vertices, 8);
	b2Fixture *fixture = m_body->CreateFixture(&polygonShape, 0.1f); //shape, density

	for (int i = 0; i < 2; i++)
		actuator_array[i].createBody(m_world, i, m_body);
}

void Robot::runSim()
{
	interface_thread = thread(&Robot::runInterfaceThread, this);
	avr_thread = thread(&Robot::runAvrThread, this);

	interface_thread.detach();
	avr_thread.detach();
}

/*
Simavr's default sleep callback results in simulated time and
wall clock time to diverge over time. This replacement tries to
keep them in sync by sleeping for the time required to match the
expected sleep deadline in wall clock time.
*/
void Robot::avr_callback_sleep_sync(
	avr_t *avr,
	avr_cycle_count_t how_long)
{
	struct timespec tp;

	/* figure out how long we should wait to match the sleep deadline */
	uint64_t deadline_ns = avr_cycles_to_nsec(avr, avr->cycle + how_long);
	clock_gettime(CLOCK_MONOTONIC_RAW, &tp);
	uint64_t runtime_ns = avr_get_time_stamp(avr);
	if (runtime_ns >= deadline_ns)
	{
		return;
	}

	uint64_t sleep_us = (deadline_ns - runtime_ns) / 1000;
	usleep(sleep_us);
	return;
}
