#include "distance_body.h"

DistanceBody::DistanceBody()
    : Body()
{
}

void DistanceBody::createBody(b2World *world)
{

    bodyDef.type = b2_dynamicBody;
    m_body = world->CreateBody(&bodyDef);

    polygonShape.SetAsBox(0.5f, 0.5f);
    m_body->CreateFixture(&polygonShape, 0.1f); //shape density
}
