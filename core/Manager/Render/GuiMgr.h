#pragma once

#include "common.h"
#include "Manager/Manager.h"

class GuiMgr : public Manager
{
public:
    static GuiMgr *getSingleton() { return s_singleton; }
    GuiMgr();
    ~GuiMgr();

    // Function
    void init();
    void process(const float dt);
    void end();
    void paint();

    void CreateUI(GLFWwindow *window, const char *glslVersion = NULL);
    static void glfwErrorCallback(int error, const char *description);
    static void ResizeWindowCallback(GLFWwindow *, int width, int height);
    static void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
    static void CharCallback(GLFWwindow *window, unsigned int c);
    static void MouseButtonCallback(GLFWwindow *window, int32 button, int32 action, int32 mods);
    static void MouseMotionCallback(GLFWwindow *, double xd, double yd);
    static void ScrollCallback(GLFWwindow *window, double dx, double dy);
    void UpdateUI();

    void SortTests();
    static inline bool CompareTests(const TestEntry &a, const TestEntry &b);
    void RestartTest();

    char buffer[128];
    static GLFWwindow *g_mainWindow;
    static int32 s_testSelection;
    static Test *s_test;
    static Settings s_settings;
    static bool s_rightMouseDown;
    static b2Vec2 s_clickPointWS;

protected:
private:
    static GuiMgr *s_singleton;

    bool m_displayGui;
};
