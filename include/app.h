#pragma once

#include <gl/glew.h>
#include <window.h>
#include <log.h>
#include <texture.h>
#include <shader.h>
#include <buffers.h>
#include <camera.h>

typedef struct app_t
{
    window_t window;
    vertex_buffer_t vertex_buffer;
    shader_t shader;
    gl_texture_t texture;
    camera_t camera;
    vec2 window_center;
    vec2 mouse_position;
    vec2 mouse_delta;
    int cursor_captured;
    float delta_time;
} app_t;

extern app_t* g_app;

int app_init(app_t *app, int argc, char *argv[]);
int app_run(app_t *app);
void app_cleanup(app_t *app);
void app_update(app_t *app);
void app_pre_run(app_t *app);
void app_window_resized_callback(GLFWwindow* window, int width, int height);
void app_mouse_position_callback(GLFWwindow* window, double xpos, double ypos);
void app_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void app_generate_cube(app_t* app);
void app_move_camera(app_t* app);
