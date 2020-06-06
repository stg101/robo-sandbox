
#include "distance_sensor.h"

DistanceSensor::DistanceSensor()
{
}

DistanceSensor::~DistanceSensor()
{
}

void DistanceSensor::connect(avr_t *n_avr, int index)
{
    avr = n_avr;
    // initialize our 'peripheral'
    // button_init(avr, &button, "button");
    // // "connect" the output irw of the button to the port pin of the AVR
    // avr_connect_irq(
    //     button.irq + IRQ_BUTTON_OUT,
    //     avr_io_getirq(avr, AVR_IOCTL_IOPORT_GETIRQ('C'), 0));

    // // 'raise' it, it's a "pullup"
    // avr_raise_irq(button.irq + IRQ_BUTTON_OUT, 1);
}

void DistanceSensor::createBody(b2World *world, int i, b2Body *chasis_body, Body *distanceBody)
{
    m_world = world;

    m_chasis_body = chasis_body;
    m_distanceBody = dynamic_cast<DistanceBody *>(distanceBody);

    jointDef.bodyA = m_chasis_body;
    jointDef.enableLimit = true;
    jointDef.lowerAngle = 0;
    jointDef.upperAngle = 0;
    jointDef.localAnchorB.SetZero(); //center of tire

    m_distanceBody->createBody(m_world);

    jointDef.bodyB = m_distanceBody->m_body;
    if (i == 0)
        jointDef.localAnchorA.Set(-3, 2.75f);
    if (i == 1)
        jointDef.localAnchorA.Set(3, 2.75f);

    world->CreateJoint(&jointDef);
}

void DistanceSensor::apply()
{
    button_press(&button, 1000000);
}
