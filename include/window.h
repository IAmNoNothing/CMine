#pragma once

#include <glfw/glfw3.h>
#include <cglm/cglm.h>

typedef struct window_t
{
    GLFWwindow *glfw_window;
    int width;
    int height;
    const char *title;
    vec3 clear_color;
} window_t;

void window_clear_color_3f(window_t *win, float r, float g, float b);
void window_clear_color_3fv(window_t *win, vec3 color);
void window_clear(window_t *win, GLuint mask);
void window_swap_buffers(window_t *win);
int window_init(window_t *win, int width, int height, const char *title);
void window_destroy(window_t *win);
void window_make_context_current(window_t *win);
int window_should_close(window_t *win);