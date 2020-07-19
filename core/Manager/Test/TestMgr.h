#pragma once

#include "Manager/Manager.h"

class TestMgr : public Manager
{
public:
    static TestMgr *getSingleton() { return s_singleton; }
    TestMgr();
    ~TestMgr();

    // Function
    void init();
    void process(const float dt);
    void end();
    void paint();
    void showImGuiWindow(bool *window);

protected:
private:
    // Function
    static TestMgr *s_singleton;
};
