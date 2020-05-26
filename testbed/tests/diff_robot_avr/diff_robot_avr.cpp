#include "test.h"
#include "differential_robot.h"
#include "differential_robot_body.h"

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
			robotBody.createBody(m_world);
			robot.createBody(m_world, &robotBody);
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

	DifferentialRobot robot;
	DifferentialRobotBody robotBody;
};

static int testIndex = RegisterTest("Examples", "DiffRobotAVR", DiffRobotAVR::Create);
