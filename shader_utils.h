#ifndef SHADER_UTILS_H_INCLUDED
#define SHADER_UTILS_H_INCLUDED
#include <GL/glew.h>
GLuint load_shader(const char *filename, GLenum shader_type);
GLuint load_shader_program(void);
#endif