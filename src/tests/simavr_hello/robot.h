#ifndef ROBOT_H
#define ROBOT_H

#include <stdlib.h>
#include <stdio.h>
#include <chrono>
#include <stdbool.h>
#include <thread>
#include "box2d/box2d.h"

extern "C"
{
#include "sim_avr.h"
#include "avr_adc.h"
#include "avr_extint.h"
#include "avr_ioport.h"
#include "sim_time.h"
#include "sim_elf.h"
#include "sim_hex.h"
#include "sim_gdb.h"
#include "sim_vcd_file.h"
#include "button.h"
}

#include "actuator.h"
#include "sensor.h"

#define MHZ_16 (16000000)

using namespace std;

class Robot
{
public:
	Robot();
	Robot(const char *);
	virtual ~Robot();

	int createMCU(const char *);
	void createBody(b2World *);
	void keyPress(unsigned char key);

	void runSim();

	avr_t *avr;
	const char *fname;

protected:
	void runInterfaceThread();
	void runAvrThread();

	Actuator actuator_array[8];
	Sensor sensor;
	b2World *m_world;
	elf_firmware_t f;
	bool is_paused;
	thread interface_thread;
	thread avr_thread;
};

#endif
