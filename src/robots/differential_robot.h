#ifndef DIFFERENTIAL_ROBOT_H
#define DIFFERENTIAL_ROBOT_H

#include <stdlib.h>
#include <iostream>
#include <stdio.h>
#include <chrono>
#include <unistd.h>
#include <stdbool.h>
#include <thread>
#include "box2d/box2d.h"
#include "motor_actuator.h"
#include "motor_body.h"
#include "button_sensor.h"

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
#include "robot.h"
#include "body.h"
#include "differential_robot_body.h"

#define MHZ_16 (16000000)

using namespace std;

class DifferentialRobot : public Robot
{
public:
	DifferentialRobot();
	virtual ~DifferentialRobot();

	int createMCU(const char *) override;
	void createBody(b2World *world, Body *body) override;
	void keyPress(unsigned char key) override;

	void runSim() override;
	void runTimeBatch(uint64_t run_ns) override;
	void setIsReady(bool is_ready_value);
	bool is_ready;

	avr_t *avr;
	const char *fname;

protected:
	void runInterfaceThread();
	void runAvrThread();
	static void sleepHook(avr_t *avr, avr_cycle_count_t how_long);

	MotorBody motorBody_array[2];
	MotorActuator actuator_array[2];
	ButtonSensor sensor;
	b2World *m_world;
	DifferentialRobotBody *m_body;
	elf_firmware_t f;
	bool is_paused;
	thread interface_thread;
	thread avr_thread;

	uint64_t ns_debt;

	b2RevoluteJoint *flJoint, *frJoint;
};

#endif
