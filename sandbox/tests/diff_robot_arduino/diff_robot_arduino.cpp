#include "test.h"
#include "settings.h"
#include "differential_robot.h"
#include "differential_robot_body.h"

#define MAX_ROBOTS 15

// This is a fun demo that shows off the wheel joint
class DiffRobotArduino : public Test
{
public:
	DiffRobotArduino()
	{
		robots_counter = 0;
		m_world->SetGravity(b2Vec2(0, 0));

		b2Body *ground = NULL;
		{
			b2BodyDef bd;
			ground = m_world->CreateBody(&bd);

			b2EdgeShape shape;

			b2FixtureDef fd;
			fd.shape = &shape;
			fd.density = 0.0f;
			fd.friction = 0.6f;

			shape.Set(b2Vec2(-100.0f, -100.0f), b2Vec2(100.0f, -100.0f));
			ground->CreateFixture(&fd);
			shape.Set(b2Vec2(100.0f, -100.0f), b2Vec2(100.0f, 100.0f));
			ground->CreateFixture(&fd);
			shape.Set(b2Vec2(100.0f, 100.0f), b2Vec2(-100.0f, 100.0f));
			ground->CreateFixture(&fd);
			shape.Set(b2Vec2(-100.0f, 100.0f), b2Vec2(-100.0f, -100.0f));
			ground->CreateFixture(&fd);
		}

		// Boxes
		{
			b2PolygonShape box;
			box.SetAsBox(10.0f, 10.0f);

			b2Body *body = NULL;
			b2BodyDef bd;
			bd.type = b2_staticBody;

			bd.position.Set(20.0f, 20.f);
			body = m_world->CreateBody(&bd);
			body->CreateFixture(&box, 0.5f);

			bd.position.Set(-20.0f, -20.f);
			body = m_world->CreateBody(&bd);
			body->CreateFixture(&box, 0.5f);
		}
	}

	void Keyboard(int key) override
	{
		switch (key)
		{
		case GLFW_KEY_G:

			if (robots_counter < MAX_ROBOTS)
			{
				robot_body_array[robots_counter].createBody(m_world);
				// robot_array[robots_counter].loadDefinition("../sandbox/tests/diff_robot_arduino/robot_def.json");
				robot_array[robots_counter].createMCU("./sandbox/firmware/arduino/firmware/firmware.arduino.avr.uno.hex");
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
		return new DiffRobotArduino;
	}

	uint64_t run_ns;
	int robots_counter;
	DifferentialRobot robot_array[MAX_ROBOTS];
	DifferentialRobotBody robot_body_array[MAX_ROBOTS];
	std::vector<std::thread> threads;
};

static int testIndex = RegisterTest("Examples", "DiffRobotArduino", DiffRobotArduino::Create);
