#include "shader_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

char *read_file(const char *filename)
{
    int current_character;
    int filesize = 0;
    int buffer_capacity = 16;
    char *file_content = malloc(buffer_capacity);
    FILE *file = fopen(filename, "rb");

    while ((current_character = fgetc(file)) != EOF)
    {
        if (filesize >= buffer_capacity - 1)
        {
            buffer_capacity *= 2;
            file_content = realloc(file_content, buffer_capacity);
        }

        file_content[filesize++] = current_character;
    }
    file_content[filesize++] = '\0';
    fclose(file);
    return file_content;
}

GLuint load_shader(const char *filename, GLenum shader_type)
{
    GLchar *file_content = read_file(filename);

    // Compiling the shader
    GLuint shader = glCreateShader(shader_type);
    glShaderSource(shader, 1, &file_content, NULL);
    glCompileShader(shader);

    // Checking for errors
    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (status != GL_TRUE)
    {
        const int buffer_length = 512;
        char log[buffer_length];
        glGetShaderInfoLog(shader, buffer_length, NULL, log);

        printf("Shader compilation failed.\nLog:\n%s\nSource code:\n%s", log, file_content);

        exit(EXIT_FAILURE);
    }

    return shader;
}

// Returns the linked shader
GLuint load_shader_program() {
    
    // Load, compile, and link shaders
	GLuint vertex_shader = load_shader("vertex.glsl", GL_VERTEX_SHADER);
	GLuint fragment_shader = load_shader("fragment.glsl", GL_FRAGMENT_SHADER);
	GLuint shader_program = glCreateProgram();
	glAttachShader(shader_program, vertex_shader);
	glAttachShader(shader_program, fragment_shader);
	glLinkProgram(shader_program);

	// Check for linking errors
	GLint success;
	glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
	if (!success)
	{
		char info_log[512];
		glGetProgramInfoLog(shader_program, 512, NULL, info_log);
		printf("Shader linking failed:\n%s", info_log);
	}

	// Standalone vertex and fragment shader objects are no longer required as they're linked into the shader program
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);
    
    return shader_program;
}