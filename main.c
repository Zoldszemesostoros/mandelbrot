#include <stdlib.h>
#include <stdio.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "shader_utils.h"
#include "controls.h"
const char *STATE_EXPORT_FORMAT = "(%la,%la) zoom: %la max iterations: %i";
const double DEFAULT_PLOT_WIDTH = 3.0;
const double DEFAULT_PLOT_HEIGHT = 2.0;
const double ZOOM_SPEED = 1.04;
const double STRIDE_SPEED = 0.03;
double zoom_level = 1.0;
double plot_middle_x = -0.5;
double plot_middle_y = 0.0;
unsigned int max_iterations = 200;

void error_callback(int error, const char *description)
{
	printf("GLFW error: %s\n", description);
}

// 2 triangles, flling the whole screen
const GLfloat vertices[] = {
	// First triangle

	-1.0f, 1.0f,  // top left
	1.0f, 1.0f,	  // top right
	-1.0f, -1.0f, // bottom left

	// Second triangle
	-1.0f, -1.0f, // bottom left
	1.0f, -1.0f,  // bottom right
	1.0f, 1.0f	  // top right
};

void set_uniforms(
	GLint max_iterations_uniform,
	GLint plot_bottom_left_uniform,
	GLint plot_sizes_uniform,
	GLint screen_sizes_uniform,
	GLuint window_width,
	GLuint window_height)
{
	double plot_width = DEFAULT_PLOT_WIDTH / zoom_level;
	double plot_height = plot_width * ((double)window_height / (double)window_width);

	// Fill the uniforms with values
	glUniform1i(max_iterations_uniform, max_iterations);
	glUniform2d(plot_bottom_left_uniform, plot_middle_x - plot_width / 2.0, plot_middle_y - plot_height / 2.0);
	glUniform2d(plot_sizes_uniform, plot_width, plot_height);
	glUniform2d(screen_sizes_uniform, window_width, window_height);
}

int main(int argc, char **argv)
{
	if (argc > 1)
	{
		int validly_converted = sscanf(argv[1], STATE_EXPORT_FORMAT, &plot_middle_x, &plot_middle_y, &zoom_level, &max_iterations);
		if (validly_converted != 4) {
			printf("%i parameters could not be parsed", 4-validly_converted);
		}
	}

	// Init GLFW
	glfwInit();
	glfwSetErrorCallback(error_callback);

	// OpenGL 3.3 with compatibility profile
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Non-resizable window
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// OpenGL context / window creation
	GLFWmonitor *primary_monitor = glfwGetPrimaryMonitor();
	GLuint xpos, ypos, window_width, window_height;
	glfwGetMonitorWorkarea(primary_monitor, &xpos, &ypos, &window_width, &window_height);

	GLFWwindow *window = glfwCreateWindow(window_width, window_height, "Mandelbrot fractal", NULL, NULL);
	glfwMakeContextCurrent(window);

	// User input callbacks
	glfwSetKeyCallback(window, key_callback);
	glfwSetScrollCallback(window, scroll_callback);

	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	glewInit();

	// Define the viewport dimensions

	glViewport(0, 0, window_width, window_height);

	GLuint shader_program = load_shader_program();

	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// The position atribute is a vec2
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind

	glBindVertexArray(0); // Unbind VAO

	// Get identifiers of the uniforms
	GLint max_iterations_uniform = glGetUniformLocation(shader_program, "max_iterations"),
		  plot_bottom_left_uniform = glGetUniformLocation(shader_program, "plot_bottom_left"),
		  plot_sizes_uniform = glGetUniformLocation(shader_program, "plot_sizes"),
		  screen_sizes_uniform = glGetUniformLocation(shader_program, "screen_sizes");

	// Main loop
	while (!glfwWindowShouldClose(window))
	{
		// Render
		// Clear the colorbuffer
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		set_uniforms(max_iterations_uniform,
					 plot_bottom_left_uniform,
					 plot_sizes_uniform,
					 screen_sizes_uniform,
					 window_width,
					 window_height);

		glUseProgram(shader_program);
		glBindVertexArray(VAO);

		// Do the actual drawing
		// 2 triangles, both of them has 2 vertices
		glDrawArrays(GL_TRIANGLES, 0, 6);

		// Unbind VAO after use
		glBindVertexArray(0);

		// Swap the screen buffers
		glfwSwapBuffers(window);

		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwWaitEvents();
	}
	// Properly de-allocate all resources once they've outlived their purpose
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();
	return 0;
}