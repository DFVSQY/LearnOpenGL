#pragma once

#include "GLFW/glfw3.h"

void glfw_error_callback(int error, const char *description);

void glfw_key_callback(GLFWwindow *win, int key, int scancode, int action, int mods);

void framebuffer_size_callback(GLFWwindow *win, int width, int height);

void APIENTRY glfw_debug_output(GLenum source, GLenum type, unsigned int id, GLenum severity, GLsizei length,
                                const char *message, const void *userParam);
