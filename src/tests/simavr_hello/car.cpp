// MIT License

// Copyright (c) 2019 Erin Catto

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
#include <stdio.h>

#include "test.h"
#include "sim_elf.h"

// This is a fun demo that shows off the wheel joint
class Simavr : public Test
{
public:
    Simavr()
    {
        m_hz = 4.0f;
        m_zeta = 0.7f;
        m_speed = 50.0f;

        b2Body *ground = NULL;

        {

            elf_firmware_t f;
            const char *fname = "./src/my_firmware.elf";
            //char path[256];

            //	sprintf(path, "%s/%s", dirname(argv[0]), fname);
            //	printf("Firmware pathname is %s\n", path);
            printf("Firmware pathname is s\n");
            elf_read_firmware(fname, &f);
        }
        {
            b2BodyDef bd;
            ground = m_world->CreateBody(&bd);

            b2EdgeShape shape;

            b2FixtureDef fd;
            fd.shape = &shape;
            fd.density = 0.0f;
            fd.friction = 0.6f;

            shape.Set(b2Vec2(-20.0f, 0.0f), b2Vec2(20.0f, 0.0f));
            ground->CreateFixture(&fd);

            float hs[10] = {0.25f, 1.0f, 4.0f, 0.0f, 0.0f, -1.0f, -2.0f, -2.0f, -1.25f, 0.0f};

            float x = 20.0f, y1 = 0.0f, dx = 5.0f;

            for (int32 i = 0; i < 10; ++i)
            {
                float y2 = hs[i];
                shape.Set(b2Vec2(x, y1), b2Vec2(x + dx, y2));
                ground->CreateFixture(&fd);
                y1 = y2;
                x += dx;
            }

            for (int32 i = 0; i < 10; ++i)
            {
                float y2 = hs[i];
                shape.Set(b2Vec2(x, y1), b2Vec2(x + dx, y2));
                ground->CreateFixture(&fd);
                y1 = y2;
                x += dx;
            }

            shape.Set(b2Vec2(x, 0.0f), b2Vec2(x + 40.0f, 0.0f));
            ground->CreateFixture(&fd);

            x += 80.0f;
            shape.Set(b2Vec2(x, 0.0f), b2Vec2(x + 40.0f, 0.0f));
            ground->CreateFixture(&fd);

            x += 40.0f;
            shape.Set(b2Vec2(x, 0.0f), b2Vec2(x + 10.0f, 5.0f));
            ground->CreateFixture(&fd);

            x += 20.0f;
            shape.Set(b2Vec2(x, 0.0f), b2Vec2(x + 40.0f, 0.0f));
            ground->CreateFixture(&fd);

            x += 40.0f;
            shape.Set(b2Vec2(x, 0.0f), b2Vec2(x, 20.0f));
            ground->CreateFixture(&fd);
        }

        // Teeter
        {
            b2BodyDef bd;
            bd.position.Set(140.0f, 1.0f);
            bd.type = b2_dynamicBody;
            b2Body *body = m_world->CreateBody(&bd);

            b2PolygonShape box;
            box.SetAsBox(10.0f, 0.25f);
            body->CreateFixture(&box, 1.0f);

            b2RevoluteJointDef jd;
            jd.Initialize(ground, body, body->GetPosition());
            jd.lowerAngle = -8.0f * b2_pi / 180.0f;
            jd.upperAngle = 8.0f * b2_pi / 180.0f;
            jd.enableLimit = true;
            m_world->CreateJoint(&jd);

            body->ApplyAngularImpulse(100.0f, true);
        }

        // Bridge
        {
            int32 N = 20;
            b2PolygonShape shape;
            shape.SetAsBox(1.0f, 0.125f);

            b2FixtureDef fd;
            fd.shape = &shape;
            fd.density = 1.0f;
            fd.friction = 0.6f;

            b2RevoluteJointDef jd;

            b2Body *prevBody = ground;
            for (int32 i = 0; i < N; ++i)
            {
                b2BodyDef bd;
                bd.type = b2_dynamicBody;
                bd.position.Set(161.0f + 2.0f * i, -0.125f);
                b2Body *body = m_world->CreateBody(&bd);
                body->CreateFixture(&fd);

                b2Vec2 anchor(160.0f + 2.0f * i, -0.125f);
                jd.Initialize(prevBody, body, anchor);
                m_world->CreateJoint(&jd);

                prevBody = body;
            }

            b2Vec2 anchor(160.0f + 2.0f * N, -0.125f);
            jd.Initialize(prevBody, ground, anchor);
            m_world->CreateJoint(&jd);
        }

        // Boxes
        {
            b2PolygonShape box;
            box.SetAsBox(0.5f, 0.5f);

            b2Body *body = NULL;
            b2BodyDef bd;
            bd.type = b2_dynamicBody;

            bd.position.Set(230.0f, 0.5f);
            body = m_world->CreateBody(&bd);
            body->CreateFixture(&box, 0.5f);

            bd.position.Set(230.0f, 1.5f);
            body = m_world->CreateBody(&bd);
            body->CreateFixture(&box, 0.5f);

            bd.position.Set(230.0f, 2.5f);
            body = m_world->CreateBody(&bd);
            body->CreateFixture(&box, 0.5f);

            bd.position.Set(230.0f, 3.5f);
            body = m_world->CreateBody(&bd);
            body->CreateFixture(&box, 0.5f);

            bd.position.Set(230.0f, 4.5f);
            body = m_world->CreateBody(&bd);
            body->CreateFixture(&box, 0.5f);
        }

        // Simavr
        {
            b2PolygonShape chassis;
            b2Vec2 vertices[8];
            vertices[0].Set(-1.5f, -0.5f);
            vertices[1].Set(1.5f, -0.5f);
            vertices[2].Set(1.5f, 0.0f);
            vertices[3].Set(0.0f, 0.9f);
            vertices[4].Set(-1.15f, 0.9f);
            vertices[5].Set(-1.5f, 0.2f);
            chassis.Set(vertices, 6);

            b2CircleShape circle;
            circle.m_radius = 0.4f;

            b2BodyDef bd;
            bd.type = b2_dynamicBody;
            bd.position.Set(0.0f, 1.0f);
            m_Simavr = m_world->CreateBody(&bd);
            m_Simavr->CreateFixture(&chassis, 1.0f);

            b2FixtureDef fd;
            fd.shape = &circle;
            fd.density = 1.0f;
            fd.friction = 0.9f;

            bd.position.Set(-1.0f, 0.35f);
            m_wheel1 = m_world->CreateBody(&bd);
            m_wheel1->CreateFixture(&fd);

            bd.position.Set(1.0f, 0.4f);
            m_wheel2 = m_world->CreateBody(&bd);
            m_wheel2->CreateFixture(&fd);

            b2WheelJointDef jd;
            b2Vec2 axis(0.0f, 1.0f);

            jd.Initialize(m_Simavr, m_wheel1, m_wheel1->GetPosition(), axis);
            jd.motorSpeed = 0.0f;
            jd.maxMotorTorque = 20.0f;
            jd.enableMotor = true;
            jd.frequencyHz = m_hz;
            jd.dampingRatio = m_zeta;
            m_spring1 = (b2WheelJoint *)m_world->CreateJoint(&jd);

            jd.Initialize(m_Simavr, m_wheel2, m_wheel2->GetPosition(), axis);
            jd.motorSpeed = 0.0f;
            jd.maxMotorTorque = 10.0f;
            jd.enableMotor = false;
            jd.frequencyHz = m_hz;
            jd.dampingRatio = m_zeta;
            m_spring2 = (b2WheelJoint *)m_world->CreateJoint(&jd);
        }
    }

    void Keyboard(int key) override
    {
        switch (key)
        {
        case GLFW_KEY_A:
            m_spring1->SetMotorSpeed(m_speed);
            break;

        case GLFW_KEY_S:
            m_spring1->SetMotorSpeed(0.0f);
            break;

        case GLFW_KEY_D:
            m_spring1->SetMotorSpeed(-m_speed);
            break;

        case GLFW_KEY_Q:
            m_hz = b2Max(0.0f, m_hz - 1.0f);
            m_spring1->SetSpringFrequencyHz(m_hz);
            m_spring2->SetSpringFrequencyHz(m_hz);
            break;

        case GLFW_KEY_E:
            m_hz += 1.0f;
            m_spring1->SetSpringFrequencyHz(m_hz);
            m_spring2->SetSpringFrequencyHz(m_hz);
            break;
        }
    }

    void Step(Settings &settings) override
    {
        g_debugDraw.DrawString(5, m_textLine, "Keys: left = a, brake = s, right = d, hz down = q, hz up = e");
        m_textLine += m_textIncrement;
        g_debugDraw.DrawString(5, m_textLine, "frequency = %g hz, damping ratio = %g", m_hz, m_zeta);
        m_textLine += m_textIncrement;

        g_camera.m_center.x = m_Simavr->GetPosition().x;
        Test::Step(settings);
    }

    static Test *Create()
    {
        return new Simavr;
    }

    b2Body *m_Simavr;
    b2Body *m_wheel1;
    b2Body *m_wheel2;

    float m_hz;
    float m_zeta;
    float m_speed;
    b2WheelJoint *m_spring1;
    b2WheelJoint *m_spring2;
};

static int testIndex = RegisterTest("Examples", "Simavr", Simavr::Create);
