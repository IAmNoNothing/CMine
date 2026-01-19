#pragma once

#include <gl/glew.h>
#include <error_codes.h>
#include <stdio.h>
#include <cglm/cglm.h>
#include <log.h>

typedef struct shader_t {
    GLuint id;
} shader_t;

error_code_t shader_init(shader_t* shader, const char* vertex_src, const char* fragment_src);
error_code_t shader_init_file(shader_t* shader, const char* vertex_path, const char* fragment_path);
void shader_use(shader_t* shader);
error_code_t shader_uniform_mat4(shader_t* shader, const char* name, mat4 mat);
GLint shader_get_uniform(shader_t* shader, const char* name);
error_code_t shader_uniform_1i(shader_t* shader, const char* name, int value);
error_code_t shader_uniform_1f(shader_t* shader, const char* name, float value);