#define _CRT_SECURE_NO_WARNINGS
#define IMGUI_DISABLE_OBSOLETE_FUNCTIONS 1

#include "imgui/imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "draw.h"
#include "settings.h"
#include "test.h"

#include <algorithm>
#include <stdio.h>
#include <thread>
#include <chrono>

#if defined(_WIN32)
#include <crtdbg.h>
#endif


#include "Manager/Manager.h"