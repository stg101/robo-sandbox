#include "distance_body.h"

DistanceBody::DistanceBody()
    : Body()
{
    maxDistance = 10.0f;
}

void DistanceBody::createBody(b2World *world)
{

    bodyDef.type = b2_dynamicBody;
    m_body = world->CreateBody(&bodyDef);

    b2Vec2 vertices[4];
    vertices[0].Set(-0.5f, -0.5f);
    vertices[1].Set(0.5f, -0.5f);
    vertices[2].Set(0.5f, 0.5f);
    vertices[3].Set(-0.5f, 0.5f);
    b2PolygonShape polygonShape;
    polygonShape.Set(vertices, 4);

    b2FixtureDef fd;
    fd.shape = &polygonShape;
    fd.density = 0.1f;
    fd.friction = 0.0f;
    m_body->CreateFixture(&fd);

    // b2EdgeShape edgeShape;
    // edgeShape.Set(b2Vec2(-0.0f, 0.0f), b2Vec2(0.0f, maxDistance * 1.0f));
    // fd.shape = &edgeShape;
    // fd.friction = 0.0f;
    // fd.isSensor = true;
    // m_body->CreateFixture(&fd);

    b2Vec2 sensorVertices[4];
    sensorVertices[0].Set(-0.05f, 0.05f);
    sensorVertices[1].Set(0.05f, 0.05f);
    sensorVertices[2].Set(0.05f, maxDistance * 1.0f);
    sensorVertices[3].Set(-0.05f, maxDistance * 1.0f);
    b2PolygonShape sensorPolygonShape;
    sensorPolygonShape.Set(sensorVertices, 4);

    fd.shape = &sensorPolygonShape;
    fd.friction = 0.0f;
    fd.isSensor = true;
    m_body->CreateFixture(&fd); //shape density
}
