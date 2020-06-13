#include "differential_robot_body.h"

DifferentialRobotBody::DifferentialRobotBody()
    : Body()
{
}

void DifferentialRobotBody::createBody(b2World *world)
{

    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    m_body = world->CreateBody(&bodyDef);
    m_body->SetAngularDamping(3);

    b2Vec2 vertices[6];
    vertices[0].Set(2.0, 0);
    vertices[1].Set(3, 2.5);
    vertices[2].Set(2.2, 5.5);
    vertices[3].Set(-2.2, 5.5);
    vertices[4].Set(-3, 2.5);
    vertices[5].Set(-2.0, 0);
    b2PolygonShape polygonShape;
    polygonShape.Set(vertices, 6);
    b2Fixture *fixture = m_body->CreateFixture(&polygonShape, 0.1f); //shape, density
}
