#ifndef CONTROLS_H_INCLUDED
#define CONTROLS_H_INCLUDED
#include <GLFW/glfw3.h>

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode);
#endif