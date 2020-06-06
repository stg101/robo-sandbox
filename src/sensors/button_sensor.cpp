
#include "button_sensor.h"

ButtonSensor::ButtonSensor()
{
}

ButtonSensor::~ButtonSensor()
{
}

void ButtonSensor::connect(avr_t *n_avr, int index)
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

void ButtonSensor::createBody(b2World *world, int i, b2Body *chasis_body, Body *motorBody)
{
    m_world = world;
}

void ButtonSensor::apply()
{
    button_press(&button, 1000000);
}
