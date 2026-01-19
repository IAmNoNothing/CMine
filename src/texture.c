#include "texture.h"

int texture_load_from_file(texture_t *texture, const char *file_path)
{
    stbi_set_flip_vertically_on_load(1);
    texture->data = stbi_load(file_path, &texture->info.width, &texture->info.height, &texture->info.channels, 0);

    if (!texture->data) {
        return -1;
    }

    return 0;
}

void texture_free(texture_t *texture)
{
    if (texture->data) {
        stbi_image_free(texture->data);
        texture->data = NULL;
    }
}

int gl_texture_create_from_texture(gl_texture_t *gl_texture, const texture_t *texture)
{
    if (!texture->data) {
        return -1;
    }

    gl_texture->info = texture->info;

    glGenTextures(1, &gl_texture->id);
    glBindTexture(GL_TEXTURE_2D, gl_texture->id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    GLenum format = (texture->info.channels == 4) ? GL_RGBA : GL_RGB;

    glTexImage2D(GL_TEXTURE_2D, 0, format, texture->info.width, texture->info.height,
                0, format, GL_UNSIGNED_BYTE, texture->data);

    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);

    return 0;
}

int gl_texture_create_from_file(gl_texture_t *gl_texture, const char *file_path)
{
    texture_t texture;

    if (texture_load_from_file(&texture, file_path) != 0) {
        return -1;
    }

    int result = gl_texture_create_from_texture(gl_texture, &texture);
    texture_free(&texture);
    return result;
}

void gl_texture_destroy(gl_texture_t *gl_texture)
{
    if (gl_texture->id) {
        glDeleteTextures(1, &gl_texture->id);
        gl_texture->id = 0;
    }
}

void gl_texture_bind(const gl_texture_t *gl_texture, GLenum texture_unit)
{
    glActiveTexture(texture_unit);
    glBindTexture(GL_TEXTURE_2D, gl_texture->id);
}
