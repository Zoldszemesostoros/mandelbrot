#ifndef CONTROLS_H_INCLUDED
#define CONTROLS_H_INCLUDED
#include <GLFW/glfw3.h>
extern const double ZOOM_SPEED;
extern const double STRIDE_SPEED;
extern double zoom_level;
extern double plot_middle_x;
extern double plot_middle_y;
extern unsigned int max_iterations;
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode);
#endif