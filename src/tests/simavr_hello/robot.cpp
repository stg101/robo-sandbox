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

			for (int i = 0; i < 8; i++)
			{
				uint8_t single_pin_state = actuator_array[i].show();
				if (single_pin_state == 1)
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
void Robot::createMCU(const char *firmware_path)
{
	fname = firmware_path;

	elf_read_firmware(fname, &f);

	printf("firmware %s f=%d mmcu=%s\n", fname, (int)f.frequency, f.mmcu);

	avr = avr_make_mcu_by_name(f.mmcu);
	avr_init(avr);
	avr_load_firmware(avr, &f);

	sensor.connect(avr);

	for (int i = 0; i < 8; i++)
		actuator_array[i].connect(avr, i);
}

void Robot::createBody(b2World *n_m_world)
{
	m_world = n_m_world;

	for (int i = 0; i < 8; i++)
		actuator_array[i].createBody(m_world, i);
}

void Robot::runSim()
{
	interface_thread = thread(&Robot::runInterfaceThread, this);
	avr_thread = thread(&Robot::runAvrThread, this);

	interface_thread.detach();
	avr_thread.detach();
}