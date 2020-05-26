#include "test.h"
#include "env.h"
#include "settings.h"

#define MHZ_16 (16000000)

// This is a fun demo that shows off the wheel joint
class EnvTest : public Test
{
public:
    EnvTest()
    {
        env.setFrequency(MHZ_16);
    }

    void Keyboard(int key) override
    {
        switch (key)
        {
        case GLFW_KEY_G:
            break;

        default:
            printf("default");
        }
    }

    void EnvStep(Settings &settings) override
    {
        maxSteps = (1.0 / settings.m_hertz) * MHZ_16;
        env.multiStep(maxSteps);
        env.render();
    }

    void Step(Settings &settings) override
    {

        g_debugDraw.DrawString(5, m_textLine, "Press G to set the environment");
        m_textLine += m_textIncrement;

        g_debugDraw.DrawString(5, m_textLine, "uc steps = %d", maxSteps);
        m_textLine += m_textIncrement;

        Test::Step(settings);
    }

    static Test *Create()
    {
        return new EnvTest;
    }

    Env env;
    int maxSteps;
};

static int testIndex = RegisterTest("Examples", "EnvTest", EnvTest::Create);
