#ifndef DIFFERENTIAL_ROBOT_BODY_H
#define DIFFERENTIAL_ROBOT_BODY_H

#include <vector>
#include "box2d/box2d.h"
#include "body.h"

class DifferentialRobotBody : public Body
{
public:
    b2Body *m_body;
    DifferentialRobotBody();

    void createBody(b2World *world) override;
};

#endif