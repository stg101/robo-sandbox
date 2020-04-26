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

#include "test.h"
#include "robot.h"

// This is a fun demo that shows off the wheel joint
class DiffRobotAVR : public Test
{
public:
	DiffRobotAVR()
	{
		m_world->SetGravity(b2Vec2(0, 0));
	}

	void Keyboard(int key) override
	{
		switch (key)
		{
		case GLFW_KEY_G:
			robot.createMCU("./testbed/tests/diff_robot_avr/atmega_firmware.hex");
			robot.createBody(m_world);
			robot.runSim();
			break;

		default:
			robot.keyPress(key);
		}
	}

	void Step(Settings &settings) override
	{
		g_debugDraw.DrawString(5, m_textLine, "Press G to create a new robot");
		m_textLine += m_textIncrement;

		Test::Step(settings);
	}

	static Test *Create()
	{
		return new DiffRobotAVR;
	}

	Robot robot;
};

static int testIndex = RegisterTest("Examples", "DiffRobotAVR", DiffRobotAVR::Create);
