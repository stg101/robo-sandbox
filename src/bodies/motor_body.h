#ifndef MOTOR_BODY_H
#define MOTOR_BODY_H

#include <vector>
#include "box2d/box2d.h"
#include "body.h"
#include <iostream>

class MotorBody : public Body
{
public:
    b2Body *m_body;
    b2BodyDef bodyDef;
    b2PolygonShape polygonShape;

    float m_maxForwardSpeed;
    float m_maxBackwardSpeed;
    float m_maxDriveForce;

    enum
    {
        TDC_UP = 0x1,
        TDC_DOWN = 0x2
    };
    MotorBody();

    void createBody(b2World *world) override;
    void setCharacteristics(float maxForwardSpeed, float maxBackwardSpeed, float maxDriveForce);
    b2Vec2 getLateralVelocity();
    b2Vec2 getForwardVelocity();
    void updateFriction();
    void updateDrive(int controlState, float speedRatio);
    void setState(int controlState, float speedRatio) override;
};

#endif