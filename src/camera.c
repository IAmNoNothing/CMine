#include "camera.h"

void camera_init(camera_t *camera, float sensitivity, float speed)
{
    camera->position[0] = 0;    
    camera->position[1] = 0;        
    camera->position[2] = 0;        
    camera->direction[1] = 0;    
    camera->direction[2] = 0;
    
    camera->sensitivity = sensitivity;
    camera->speed = speed;

    camera_update_basis(camera);
    camera->needs_basis_update = 0;
}

void camera_get_view_matrix(camera_t *camera, mat4 out)
{
    camera_update_basis_if_needed(camera);
    glm_look(camera->position, camera->forward, camera->up, out);
}

void camera_update(camera_t *camera)
{
    if (camera->needs_basis_update) {
        camera_update_basis(camera);
    }
}

void camera_update_basis(camera_t *camera)
{
    float yaw_cos = cosf(camera->direction[0]);
    float pitch_cos = cosf(camera->direction[1]);
    float yaw_sin = sinf(camera->direction[0]);
    float pitch_sin = sinf(camera->direction[1]);

    camera->forward[0] = pitch_cos * yaw_sin;
    camera->forward[1] = pitch_sin;
    camera->forward[2] = -pitch_cos * yaw_cos;
    
    glm_normalize(camera->forward);

    vec3 world_up = { 0, 1, 0 };
    
    glm_cross(world_up, camera->forward, camera->right);
    glm_normalize(camera->right);

    glm_cross(camera->forward, camera->right, camera->up);
    glm_normalize(camera->up);
}

void camera_move(camera_t *camera, vec3 diff)
{
    glm_vec3_add(camera->position, diff, camera->position);
}

void camera_rotate(camera_t *camera, vec2 diff)
{
    camera->needs_basis_update = 1;
    glm_vec2_add(camera->direction, diff, camera->direction);

    camera->direction[0] = fmodf(camera->direction[0], (float)M_PI * 2.0f);
    if (camera->direction[0] < 0.0f) camera->direction[0] += 2.0f * (float)M_PI;

    camera->direction[1] = glm_clamp(camera->direction[1], -(float)M_PI_2 + 0.001f, (float)M_PI_2 - 0.001f);
}

void camera_rotate_with_mouse(camera_t* camera, vec2 mouse_delta, float delta_time)
{
    vec2 angle;

    glm_vec2_scale(mouse_delta, camera->sensitivity * delta_time, angle);

    angle[1] = -angle[1]; 

    camera_rotate(camera, angle);
}

inline void camera_update_basis_if_needed(camera_t *camera)
{
    if (camera->needs_basis_update) camera_update_basis(camera);
}
