#include <stdio.h>

#include "controls.h"


extern const double ZOOM_SPEED;
extern const double STRIDE_SPEED;
extern double zoom_level;
extern double plot_middle_x;
extern double plot_middle_y;
extern unsigned int max_iterations;
extern const char* STATE_EXPORT_FORMAT;
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
	if (yoffset > 0)
	{
		zoom_level *= ZOOM_SPEED;
	}
	else
	{
		zoom_level /= ZOOM_SPEED;
	}
}
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (action == GLFW_REPEAT || action == GLFW_PRESS)
	{
		switch (key)
		{
		case GLFW_KEY_G:
			max_iterations++;
			break;
		case GLFW_KEY_H:
			if (max_iterations > 1)
				max_iterations--;
			break;
		case GLFW_KEY_I:
			zoom_level *= ZOOM_SPEED;
			break;
		case GLFW_KEY_O:
			zoom_level /= ZOOM_SPEED;
			break;
		case GLFW_KEY_W:
		case GLFW_KEY_UP:
			plot_middle_y += STRIDE_SPEED / zoom_level;
			break;
		case GLFW_KEY_A:
		case GLFW_KEY_LEFT:
			plot_middle_x -= STRIDE_SPEED / zoom_level;
			break;
		case GLFW_KEY_S:
		case GLFW_KEY_DOWN:
			plot_middle_y -= STRIDE_SPEED / zoom_level;
			break;
		case GLFW_KEY_D:
		case GLFW_KEY_RIGHT:
			plot_middle_x += STRIDE_SPEED / zoom_level;
			break;
		case GLFW_KEY_P:
			printf(STATE_EXPORT_FORMAT, plot_middle_x, plot_middle_y, zoom_level, max_iterations);
			printf("\n");
		}
	}
}