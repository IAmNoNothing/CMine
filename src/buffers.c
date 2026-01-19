#include <buffers.h>

void vb_init(vertex_buffer_t *vb) {
    memset(vb, 0, sizeof *vb);

    glGenVertexArrays(1, &vb->vao);
    glGenBuffers(1, &vb->vbo);
    glGenBuffers(1, &vb->ebo);
}

void vb_free(vertex_buffer_t *vb) {
    free(vb->vertices);
    free(vb->indices);

    glDeleteBuffers(1, &vb->vbo);
    glDeleteBuffers(1, &vb->ebo);
    glDeleteVertexArrays(1, &vb->vao);

    memset(vb, 0, sizeof *vb);
}

void vb_push_vertex(vertex_buffer_t *vb, vertex_t v) {
    if (vb->v_count == vb->v_cap) {
        size_t nc = vb->v_cap ? vb->v_cap * 2 : 32;
        vb->vertices = realloc(vb->vertices, nc * sizeof *vb->vertices);
        vb->v_cap = nc;
    }
    vb->vertices[vb->v_count++] = v;
}

void vb_push_index(vertex_buffer_t *vb, index_t i) {
    if (vb->i_count == vb->i_cap) {
        size_t nc = vb->i_cap ? vb->i_cap * 2 : 64;
        vb->indices = realloc(vb->indices, nc * sizeof *vb->indices);
        vb->i_cap = nc;
    }
    vb->indices[vb->i_count++] = i;
}

void vb_upload(vertex_buffer_t *vb) {
    glBindVertexArray(vb->vao);

    /* VBO */
    glBindBuffer(GL_ARRAY_BUFFER, vb->vbo);
    glBufferData(
        GL_ARRAY_BUFFER,
        vb->v_count * sizeof(vertex_t),
        vb->vertices,
        GL_STATIC_DRAW
    );

    /* EBO */
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vb->ebo);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        vb->i_count * sizeof(index_t),
        vb->indices,
        GL_STATIC_DRAW
    );

    /* layout */
    glEnableVertexAttribArray(0); // pos
    glVertexAttribPointer(
        0, 3, GL_FLOAT, GL_FALSE,
        sizeof(vertex_t),
        (void*)offsetof(vertex_t, pos)
    );

    glEnableVertexAttribArray(1); // uv
    glVertexAttribPointer(
        1, 2, GL_FLOAT, GL_FALSE,
        sizeof(vertex_t),
        (void*)offsetof(vertex_t, uv)
    );

    glBindVertexArray(0);
}

void vb_render(vertex_buffer_t* vb, GLenum mode)
{
    glBindVertexArray(vb->vao);
    glDrawElements(mode, (GLsizei)vb->i_count, GL_UNSIGNED_INT, 0);
}
