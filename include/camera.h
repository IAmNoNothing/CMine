#pragma once

#include <cglm/cglm.h>

typedef struct camera_t {
    vec3 position;
    vec2 direction;
    
    vec3 forward;
    vec3 right;
    vec3 up;

    int needs_basis_update;

    float sensitivity;
    float speed;
} camera_t;

void camera_init(camera_t* camera, float sensitivity, float speed);
void camera_get_view_matrix(camera_t* camera, mat4 out);
void camera_update(camera_t* camera);
void camera_update_basis(camera_t* camera);
void camera_move(camera_t* camera, vec3 diff);
void camera_rotate(camera_t* camera, vec2 diff);
void camera_rotate_with_mouse(camera_t* camera, vec2 mouse_delta, float delta_time);
inline void camera_update_basis_if_needed(camera_t* camera);