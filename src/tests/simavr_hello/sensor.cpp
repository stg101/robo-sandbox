#include "sensor.h"

Sensor::Sensor()
{
}

Sensor::~Sensor()
{
}

void Sensor::connect(avr_t *n_avr)
{
	avr = n_avr;
	// initialize our 'peripheral'
	button_init(avr, &button, "button");
	// "connect" the output irw of the button to the port pin of the AVR
	avr_connect_irq(
		button.irq + IRQ_BUTTON_OUT,
		avr_io_getirq(avr, AVR_IOCTL_IOPORT_GETIRQ('C'), 0));

	// 'raise' it, it's a "pullup"
	avr_raise_irq(button.irq + IRQ_BUTTON_OUT, 1);
}

void Sensor::createBody(b2World *n_m_world)
{
	m_world = n_m_world;
}

void Sensor::apply()
{
	button_press(&button, 1000000);
}
