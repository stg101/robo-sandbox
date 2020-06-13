#include "motor_actuator.h"

#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)       \
	(byte & 0x80 ? '1' : '0'),     \
		(byte & 0x40 ? '1' : '0'), \
		(byte & 0x20 ? '1' : '0'), \
		(byte & 0x10 ? '1' : '0'), \
		(byte & 0x08 ? '1' : '0'), \
		(byte & 0x04 ? '1' : '0'), \
		(byte & 0x02 ? '1' : '0'), \
		(byte & 0x01 ? '1' : '0')

MotorActuator::MotorActuator()
{
	pin_state = 0; // current port B
	old_pin_state = 0;
}

MotorActuator::~MotorActuator()
{
}

void MotorActuator::connect(avr_t *n_avr, int i)
{
	avr = n_avr;
	index = i;

	avr_irq_register_notify(
		avr_io_getirq(avr, AVR_IOCTL_IOPORT_GETIRQ('D'), index),
		hook,
		this);
}

void MotorActuator::hook(struct avr_irq_t *irq, uint32_t value, void *param)
{
	MotorActuator *actuator = (MotorActuator *)(param);
	actuator->pin_state = value;
}

void MotorActuator::createBody(b2World *world, b2Body *chasis_body, Body *motorBody)
{
	m_world = world;
	m_chasis_body = chasis_body;
	m_motorBody = dynamic_cast<MotorBody *>(motorBody);

	jointDef.bodyA = m_chasis_body;
	jointDef.enableLimit = true;
	jointDef.lowerAngle = 0;
	jointDef.upperAngle = 0;
	jointDef.localAnchorB.SetZero(); //center of tire

	//back left tire
	m_motorBody->createBody(m_world);
	m_motorBody->setCharacteristics(600, -600, 1000);

	jointDef.bodyB = m_motorBody->m_body;
	if (index == 0)
		jointDef.localAnchorA.Set(-3, 0.75f);
	if (index == 1)
		jointDef.localAnchorA.Set(3, 0.75f);

	world->CreateJoint(&jointDef);
}

void MotorActuator::apply()
{
	if (pin_state == 0)
		m_motorBody->setState(MotorBody::TDC_DOWN, 1.0f);
	else
		m_motorBody->setState(MotorBody::TDC_UP, 1.0f);
}