#ifndef ACTUATOR_H
#define ACTUATOR_H

#include <stdio.h>
#include <stdbool.h>
#include "box2d/box2d.h"
#include "body.h"

extern "C"
{
#include "sim_avr.h"
#include "avr_ioport.h"
}

using namespace std;

class Actuator
{
public:
	Actuator(){};
	virtual ~Actuator(){};

	virtual void connect(avr_t *n_avr, int i){};
	virtual void createBody(b2World *world, b2Body *chasis_body, Body *motorBody){};

	virtual void apply(){};

protected:
};

#endif
