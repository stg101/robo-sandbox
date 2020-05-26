#ifndef MOTOR_ACTUATOR_H
#define MOTOR_ACTUATOR_H

#include <stdio.h>
#include <stdbool.h>
#include "box2d/box2d.h"
#include "actuator.h"
#include "motor_body.h"

extern "C"
{
#include "sim_avr.h"
#include "avr_ioport.h"
}

using namespace std;

class MotorActuator : public Actuator
{
public:
	MotorActuator();
	~MotorActuator();

	void connect(avr_t *n_avr, int i) override;
	void createBody(b2World *world, int i, b2Body *chasis_body, Body *motorBody) override;

	void apply() override;

protected:
	avr_t *avr;
	static void hook(struct avr_irq_t *irq, uint32_t value, void *param);
	b2World *m_world;
	b2RevoluteJointDef jointDef;
	b2Body *m_chasis_body;
	MotorBody *m_motorBody;
	uint8_t pin_state;
	uint8_t old_pin_state;
	int index;
};

#endif
