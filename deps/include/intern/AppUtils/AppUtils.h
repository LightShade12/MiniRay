#pragma once
#include "common.h"
#include "stb/stb_image.h"
//#include <windows.h>//can be excluded after undefining DEBUG in main


void AppInit(GLFWwindow*& winobj, const char* name, const char* icon_path, float width, float height);


void AppTerminate(GLFWwindow*& winobj);

void AppImguiFrameStart();


void AppImguiFrameEnd();

void AppImguiAddImage(GLuint texID);