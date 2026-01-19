#pragma once

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <gl/glew.h>
#include <cglm/cglm.h>

typedef uint32_t index_t;

typedef struct vertex_t {
    vec3 pos;
    vec2 uv;
} vertex_t;

typedef struct vertex_buffer_t {
    GLuint vao;
    GLuint vbo;
    GLuint ebo;

    vertex_t *vertices;
    size_t    v_count;
    size_t    v_cap;

    index_t  *indices;
    size_t    i_count;
    size_t    i_cap;
} vertex_buffer_t;

void vb_init(vertex_buffer_t *vb);
void vb_free(vertex_buffer_t *vb);
void vb_push_vertex(vertex_buffer_t *vb, vertex_t v);
void vb_push_index(vertex_buffer_t *vb, index_t i);
void vb_upload(vertex_buffer_t *vb);
void vb_render(vertex_buffer_t* vb, GLenum mode);