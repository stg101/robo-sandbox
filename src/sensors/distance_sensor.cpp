
#include "distance_sensor.h"

DistanceSensor::DistanceSensor()
{
    name = "distance";
    value = 0;
}

DistanceSensor::~DistanceSensor()
{
}

void DistanceSensor::connect(avr_t *avr, int index)
{
    this->avr = avr;
    this->index = index;

    irq = avr_alloc_irq(&avr->irq_pool, 0, 1, &name);
    avr_connect_irq(irq, avr_io_getirq(avr, AVR_IOCTL_IOPORT_GETIRQ('C'), index));
}

void DistanceSensor::createBody(b2World *world, b2Body *chasis_body, Body *distanceBody)
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
    if (index == 0)
        jointDef.localAnchorA.Set(-3.25, 2.75f);
    if (index == 1)
        jointDef.localAnchorA.Set(3.25, 2.75f);

    world->CreateJoint(&jointDef);
}

void DistanceSensor::apply()
{

    if (!m_distanceBody->m_body)
        return;

    value = 0;
    bool stop = false;

    for (b2ContactEdge *ce = m_distanceBody->m_body->GetContactList(); ce && !stop; ce = ce->next)
    {
        b2Contact *c = ce->contact;

        if (c->IsTouching())
        {
            value = 1;
            stop = true;
        }
    }

    avr_raise_irq(irq, value);
}
