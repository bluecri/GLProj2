#ifndef __3RD_TEXTURE_H__
#define __3RD_TEXTURE_H__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

GLuint loadBMP_custom(const char * imagepath);
GLuint loadDDS(const char * imagepath);

#endif