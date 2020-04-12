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

void Actuator::createBody(b2World *n_m_world, int i)
{
	m_world = n_m_world;

	b2PolygonShape box;
	box.SetAsBox(0.5f, 0.5f);

	b2BodyDef bd;
	bd.type = b2_dynamicBody;

	bd.position.Set(10.0f + i * 1.1f, 0.5f);
	m_body = m_world->CreateBody(&bd);
	m_body->CreateFixture(&box, 0.5f);
}

void Actuator::apply()
{
	m_body->SetLinearVelocity(b2Vec2(0.0f, 6.0f));

	// b2Vec2 p = m_body->GetWorldCenter();
	// m_body->ApplyForce(b2Vec2(0.0f, 4.0f), p, true);
}

uint8_t Actuator::show()
{
	return pin_state;
}