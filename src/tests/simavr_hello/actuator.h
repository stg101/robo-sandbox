#ifndef ACTUATOR_H
#define ACTUATOR_H

#include <stdio.h>
#include <stdbool.h>
#include "box2d/box2d.h"

extern "C"
{
#include "sim_avr.h"
#include "avr_ioport.h"
}

using namespace std;

class Actuator
{
public:
	Actuator();
	virtual ~Actuator();

	void connect(avr_t *n_avr, int i);
	void createBody(b2World *n_m_world, int i);

	uint8_t show();
	void apply();

protected:
	avr_t *avr;
	static void hook(struct avr_irq_t *irq, uint32_t value, void *param);
	b2World *m_world;
	b2Body *m_body;
	uint8_t pin_state;
	uint8_t old_pin_state;
	int index;
};

#endif
