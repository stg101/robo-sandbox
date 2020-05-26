#ifndef SENSOR_H
#define SENSOR_H

#include <stdio.h>
#include <stdbool.h>
#include "box2d/box2d.h"

extern "C"
{
#include "sim_avr.h"
#include "avr_ioport.h"
#include "button.h"
}

using namespace std;

class Sensor
{
public:
	Sensor(){};
	virtual ~Sensor(){};

	virtual void connect(avr_t *n_avr){};
	virtual void createBody(b2World *n_m_world){};

	virtual void apply(){};

protected:
};

#endif
