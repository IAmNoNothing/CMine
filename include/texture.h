#pragma once

#include <stb_image.h>
#include <gl/glew.h>

typedef struct texture_info_t {
    int width;
    int height;
    int channels;
} texture_info_t;

typedef struct texture_t {
    texture_info_t info;
    unsigned char* data;
} texture_t;

typedef struct gl_texture_t {
    texture_info_t info;
    GLuint id;
} gl_texture_t;

int texture_load_from_file(texture_t* texture, const char* file_path);
void texture_free(texture_t* texture);

int gl_texture_create_from_texture(gl_texture_t* gl_texture, const texture_t* texture);
int gl_texture_create_from_file(gl_texture_t* gl_texture, const char* file_path);
void gl_texture_destroy(gl_texture_t* gl_texture);
void gl_texture_bind(const gl_texture_t* gl_texture, GLenum texture_unit);