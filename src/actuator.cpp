#include "actuator.h"

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

Actuator::Actuator()
{
	pin_state = 0; // current port B
	old_pin_state = 0;
}

Actuator::~Actuator()
{
}

void Actuator::connect(avr_t *n_avr, int i)
{
	avr = n_avr;
	index = i;

	avr_irq_register_notify(
		avr_io_getirq(avr, AVR_IOCTL_IOPORT_GETIRQ('D'), index),
		hook,
		this);
}

void Actuator::hook(struct avr_irq_t *irq, uint32_t value, void *param)
{
	Actuator *actuator = (Actuator *)(param);
	actuator->pin_state = value;
}

void Actuator::createBody(b2World *world, int i, b2Body *chasis_body)
{
	m_world = world;
	m_chasis_body = chasis_body;

	jointDef.bodyA = m_chasis_body;
	jointDef.enableLimit = true;
	jointDef.lowerAngle = 0;
	jointDef.upperAngle = 0;
	jointDef.localAnchorB.SetZero(); //center of tire

	//back left tire
	tire.createBody(m_world);
	tire.setCharacteristics(15, -15, 20);

	jointDef.bodyB = tire.m_body;
	if (i == 0)
		jointDef.localAnchorA.Set(-3, 0.75f);
	if (i == 1)
		jointDef.localAnchorA.Set(3, 0.75f);

	world->CreateJoint(&jointDef);
}

void Actuator::apply()
{

	if (pin_state == 0)
		tire.step(0);
	else
		tire.step(TDC_UP);

	// m_body->SetLinearVelocity(b2Vec2(0.0f, 6.0f));

	// b2Vec2 p = m_body->GetWorldCenter();
	// m_body->ApplyForce(b2Vec2(0.0f, 4.0f), p, true);
}