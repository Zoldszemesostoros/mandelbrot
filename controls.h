#ifndef CONTROLS_H_INCLUDED
#define CONTROLS_H_INCLUDED
#include <GLFW/glfw3.h>
extern const float ZOOM_SPEED;
extern const float STRIDE_SPEED;
extern float zoom_level;
extern float plot_middle_x;
extern float plot_middle_y;
extern unsigned int max_iterations;
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode);
#endif