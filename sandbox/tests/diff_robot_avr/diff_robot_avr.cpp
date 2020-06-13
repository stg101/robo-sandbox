#include "test.h"
#include "settings.h"
#include "differential_robot.h"
#include "differential_robot_body.h"

#define MAX_ROBOTS 15

// This is a fun demo that shows off the wheel joint
class DiffRobotAVR : public Test
{
public:
	DiffRobotAVR()
	{
		robots_counter = 0;
		m_world->SetGravity(b2Vec2(0, 0));
	}

	void Keyboard(int key) override
	{
		switch (key)
		{
		case GLFW_KEY_G:

			if (robots_counter < MAX_ROBOTS)
			{
				robot_body_array[robots_counter].createBody(m_world);
				robot_array[robots_counter].createMCU("./sandbox/tests/diff_robot_avr/atmega_firmware.hex");
				robot_array[robots_counter].createBody(m_world, &robot_body_array[robots_counter]);
				robot_array[robots_counter].setIsReady(true);
				robots_counter++;
			}
			break;

		default:
			break;
		}
	}

	void EnvStep(Settings &settings) override
	{
		run_ns = (1000000000 / settings.m_hertz);

		for (int i = 0; i < robots_counter; i++)
		{
			if (robot_array[i].is_ready)
			{
				threads.emplace_back(std::thread(&robotRun, &robot_array[i], run_ns));
			}
		}
		//wait for them to complete
		for (auto &th : threads)
			th.join();

		threads.clear();
	}

	static void robotRun(DifferentialRobot *robot, uint64_t run_ns)
	{
		robot->runTimeBatch(run_ns);
	}

	void Step(Settings &settings) override
	{
		g_debugDraw.DrawString(5, m_textLine, "Press G to create a new robot");
		m_textLine += m_textIncrement;

		g_debugDraw.DrawString(5, m_textLine, "goal ns = %d", run_ns);
		m_textLine += m_textIncrement;

		Test::Step(settings);
	}

	static Test *Create()
	{
		return new DiffRobotAVR;
	}

	uint64_t run_ns;
	int robots_counter;
	DifferentialRobot robot_array[MAX_ROBOTS];
	DifferentialRobotBody robot_body_array[MAX_ROBOTS];
	std::vector<std::thread> threads;
};

static int testIndex = RegisterTest("Examples", "DiffRobotAVR", DiffRobotAVR::Create);
