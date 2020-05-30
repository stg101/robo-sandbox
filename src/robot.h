#ifndef ROBOT_H
#define ROBOT_H

#include <stdlib.h>
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
#include "body.h"

#define MHZ_16 (16000000)

using namespace std;

class Robot
{
public:
	Robot(){};
	virtual ~Robot(){};

	virtual int createMCU(const char *){};
	virtual void createBody(b2World *world, Body *body){};
	virtual void keyPress(unsigned char key){};
	virtual void setIsReady(bool is_ready){};

	virtual void runSim(){};
	virtual void runTimeBatch(uint64_t run_ns){};

protected:
};

#endif
