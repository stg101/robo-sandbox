
#ifndef BUTTON_SENSOR_H
#define BUTTON_SENSOR_H

#include <stdio.h>
#include <stdbool.h>
#include "box2d/box2d.h"
#include "sensor.h"

extern "C"
{
#include "sim_avr.h"
#include "avr_ioport.h"
#include "button.h"
}

using namespace std;

class ButtonSensor : public Sensor
{
public:
    ButtonSensor();
    ~ButtonSensor();

    void connect(avr_t *n_avr) override;
    void createBody(b2World *world) override;

    void apply() override;

protected:
    avr_t *avr;
    button_t button;
    b2World *m_world;
};

#endif
