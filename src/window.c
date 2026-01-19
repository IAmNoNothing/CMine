#include "window.h"

void window_clear_color_3f(window_t *win, float r, float g, float b)
{
    win->clear_color[0] = r;
    win->clear_color[1] = g;
    win->clear_color[2] = b;

    glClearColor(r, g, b, 1.0f);
}

void window_clear_color_3fv(window_t *win, vec3 color)
{
    win->clear_color[0] = color[0];
    win->clear_color[1] = color[1];
    win->clear_color[2] = color[2];

    glClearColor(color[0], color[1], color[2], 1.0f);
}

void window_clear(window_t *win, GLuint mask)
{
    glClear(mask);
}

void window_swap_buffers(window_t *win)
{
    glfwSwapBuffers(win->glfw_window);
}

int window_init(window_t *win, int width, int height, const char *title)
{
    win->glfw_window = glfwCreateWindow(width, height, title, NULL, NULL);

    if (!win->glfw_window) {
        return -1; // Window creation failed
    }

    win->width = width;
    win->height = height;
    win->title = title;

    win->clear_color[0] = 0.0f;
    win->clear_color[1] = 0.0f;
    win->clear_color[2] = 0.0f;

    return 0; 
}

void window_destroy(window_t *win)
{
    if (win->glfw_window) {
        glfwDestroyWindow(win->glfw_window);
        win->glfw_window = NULL;
    }
}

void window_make_context_current(window_t *win)
{
    glfwMakeContextCurrent(win->glfw_window);
}

int window_should_close(window_t *win)
{
    return glfwWindowShouldClose(win->glfw_window);
}
