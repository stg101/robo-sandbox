#include "motor_body.h"

MotorBody::MotorBody()
    : Body()
{
}

void MotorBody::createBody(b2World *world)
{

    bodyDef.type = b2_dynamicBody;
    m_body = world->CreateBody(&bodyDef);

    polygonShape.SetAsBox(0.5f, 1.25f);
    m_body->CreateFixture(&polygonShape, 1); //shape density
}

void MotorBody::setCharacteristics(float maxForwardSpeed, float maxBackwardSpeed, float maxDriveForce)
{
    m_maxForwardSpeed = maxForwardSpeed;
    m_maxBackwardSpeed = maxBackwardSpeed;
    m_maxDriveForce = maxDriveForce;
}

b2Vec2 MotorBody::getLateralVelocity()
{
    b2Vec2 currentRightNormal = m_body->GetWorldVector(b2Vec2(1, 0));
    return b2Dot(currentRightNormal, m_body->GetLinearVelocity()) * currentRightNormal;
}

b2Vec2 MotorBody::getForwardVelocity()
{
    b2Vec2 currentForwardNormal = m_body->GetWorldVector(b2Vec2(0, 1));
    return b2Dot(currentForwardNormal, m_body->GetLinearVelocity()) * currentForwardNormal;
}

void MotorBody::updateFriction()
{
    //lateral linear velocity
    float maxLateralImpulse = 2.5f;
    b2Vec2 impulse = m_body->GetMass() * -getLateralVelocity();
    if (impulse.Length() > maxLateralImpulse)
        impulse *= maxLateralImpulse / impulse.Length();
    m_body->ApplyLinearImpulse(impulse, m_body->GetWorldCenter(), true);

    //angular velocity
    m_body->ApplyAngularImpulse(0.1f * m_body->GetInertia() * -m_body->GetAngularVelocity(), true);

    //forward linear velocity
    b2Vec2 currentForwardNormal = getForwardVelocity();
    float currentForwardSpeed = currentForwardNormal.Normalize();
    float dragForceMagnitude = -2 * currentForwardSpeed;
    m_body->ApplyForce(dragForceMagnitude * currentForwardNormal, m_body->GetWorldCenter(), true);
}

void MotorBody::updateDrive(int controlState, float speedRatio)
{

    speedRatio = (speedRatio > 1) ? 1 : speedRatio;
    speedRatio = (speedRatio < 0) ? 0 : speedRatio;

    //find desired speed
    float desiredSpeed = 0;
    switch (controlState & (TDC_UP | TDC_DOWN))
    {
    case TDC_UP:
        desiredSpeed = speedRatio * m_maxForwardSpeed;
        break;
    case TDC_DOWN:
        desiredSpeed = speedRatio * m_maxBackwardSpeed;
        break;
    default:
        return; //do nothing
    }

    //find current speed in forward direction
    b2Vec2 currentForwardNormal = m_body->GetWorldVector(b2Vec2(0, 1));
    float currentSpeed = b2Dot(getForwardVelocity(), currentForwardNormal);

    //apply necessary force
    float force = 0;
    if (desiredSpeed > currentSpeed)
        force = m_maxDriveForce;
    else if (desiredSpeed < currentSpeed)
        force = -m_maxDriveForce;
    else
        return;
    m_body->ApplyForce(force * currentForwardNormal, m_body->GetWorldCenter(), true);
}

void MotorBody::setState(int controlState, float speedRatio)
{
    updateFriction();
    updateDrive(speedRatio, controlState);
}