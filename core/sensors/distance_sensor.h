
#ifndef DISTANCE_SENSOR_H
#define DISTANCE_SENSOR_H

#include <stdio.h>
#include <stdbool.h>
#include "box2d/box2d.h"
#include "sensor.h"
#include "distance_body.h"

extern "C"
{
#include "sim_avr.h"
#include "avr_ioport.h"
#include "button.h"
}

using namespace std;

class DistanceSensor : public Sensor
{
public:
    DistanceSensor();
    ~DistanceSensor();

    void connect(avr_t *n_avr, int index) override;
    void createBody(b2World *world, b2Body *chasis_body, Body *distanceBody) override;

    void apply() override;

protected:
    avr_t *avr;
    button_t button;
    b2World *m_world;
    b2RevoluteJointDef jointDef;
    b2Body *m_chasis_body;
    DistanceBody *m_distanceBody;

    avr_irq_t *irq;
    const char *name;
    uint8_t value;
    uint8_t index;
};

#endif
