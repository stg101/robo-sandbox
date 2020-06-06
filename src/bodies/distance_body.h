#ifndef DISTANCE_BODY_H
#define DISTANCE_BODY_H

#include <vector>
#include "box2d/box2d.h"
#include "body.h"

class DistanceBody : public Body
{
public:
    b2Body *m_body;
    b2BodyDef bodyDef;
    b2PolygonShape polygonShape;

    DistanceBody();

    void createBody(b2World *world) override;
};

#endif