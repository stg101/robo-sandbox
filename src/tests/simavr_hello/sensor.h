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
	Sensor();
	virtual ~Sensor();

	void connect(avr_t *n_avr);
	void createBody(b2World *n_m_world);

	void apply();

protected:
	avr_t *avr;
	button_t button;
	b2World *m_world;
};

#endif
