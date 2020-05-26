#include "test.h"
#include "differential_robot.h"
#include "differential_robot_body.h"

// This is a fun demo that shows off the wheel joint
class DiffRobotArduino : public Test
{
public:
	DiffRobotArduino()
	{
		m_world->SetGravity(b2Vec2(0, 0));
	}

	void Keyboard(int key) override
	{
		switch (key)
		{
		case GLFW_KEY_G:
			robot.createMCU("./testbed/tests/diff_robot_arduino/arduino_firmware/arduino_firmware.arduino.avr.uno.hex");
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
		return new DiffRobotArduino;
	}

	DifferentialRobot robot;
	DifferentialRobotBody robotBody;
};

static int testIndex = RegisterTest("Examples", "DiffRobotArduino", DiffRobotArduino::Create);
