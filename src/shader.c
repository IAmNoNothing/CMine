#include "shader.h"

error_code_t shader_init(shader_t *shader, const char* vertex_src, const char* fragment_src)
{
    GLint ok;
    char infoLog[1024];

    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vertex_src, NULL);
    glCompileShader(vs);

    glGetShaderiv(vs, GL_COMPILE_STATUS, &ok);
    if (!ok) {
        glGetShaderInfoLog(vs, sizeof(infoLog), NULL, infoLog);
        log_error("Vertex shader compilation failed:\n%s", infoLog);
        glDeleteShader(vs);
        return SHADER_COMPILE_VERTEX_FAILED;
    }

    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fragment_src, NULL);
    glCompileShader(fs);

    glGetShaderiv(fs, GL_COMPILE_STATUS, &ok);
    if (!ok) {
        glGetShaderInfoLog(fs, sizeof(infoLog), NULL, infoLog);
        log_error("Fragment shader compilation failed:\n%s", infoLog);
        glDeleteShader(vs);
        glDeleteShader(fs);
        return SHADER_COMPILE_FRAGMENT_FAILED;
    }

    shader->id = glCreateProgram();
    glAttachShader(shader->id, vs);
    glAttachShader(shader->id, fs);
    glLinkProgram(shader->id);

    glGetProgramiv(shader->id, GL_LINK_STATUS, &ok);
    if (!ok) {
        glGetProgramInfoLog(shader->id, sizeof(infoLog), NULL, infoLog);
        log_error("Shader program linking failed:\n%s", infoLog);
        glDeleteProgram(shader->id);
        glDeleteShader(vs);
        glDeleteShader(fs);
        return SHADER_LINKING_FAILED;
    }

    glDeleteShader(vs);
    glDeleteShader(fs);
    log_info("Shader program created successfully.");

    return OK;
}

error_code_t shader_init_file(shader_t *shader, const char *vertex_path, const char *fragment_path)
{
    FILE* file = fopen(vertex_path, "rb");

    if (!file) {
        log_error("Failed to open vertex shader file: %s", vertex_path);
        return FILE_FAILED_TO_OPEN;
    }

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);

    char* vertex_src = (char*)malloc(length + 1);
    fread(vertex_src, 1, length, file);
    vertex_src[length] = '\0';
    fclose(file);

    file = fopen(fragment_path, "rb");

    if (!file) {
        log_error("Failed to open fragment shader file: %s", fragment_path);
        free(vertex_src);
        return FILE_FAILED_TO_OPEN;
    }

    fseek(file, 0, SEEK_END);
    length = ftell(file);
    fseek(file, 0, SEEK_SET);
    
    char* fragment_src = (char*)malloc(length + 1);
    fread(fragment_src, 1, length, file);
    fragment_src[length] = '\0';
    fclose(file);

    error_code_t result = shader_init(shader, vertex_src, fragment_src);

    free(vertex_src);
    free(fragment_src);
    return result;
}

void shader_use(shader_t *shader)
{
    glUseProgram(shader->id);
}

error_code_t shader_uniform_mat4(shader_t *shader, const char *name, mat4 mat)
{
    GLint location = shader_get_uniform(shader, name);
    if (location != -1) {
        glUniformMatrix4fv(location, 1, GL_FALSE, (const GLfloat*)mat);
        return OK;
    }
    return SHADER_UNIFORM_NOT_FOUND;
}

GLint shader_get_uniform(shader_t *shader, const char *name)
{
    return glGetUniformLocation(shader->id, name);
}

error_code_t shader_uniform_1i(shader_t *shader, const char *name, int value)
{
    GLint location = shader_get_uniform(shader, name);
    if (location != -1) {
        glUniform1i(location, value);
        return OK;
    }
    return SHADER_UNIFORM_NOT_FOUND;
}

error_code_t shader_uniform_1f(shader_t *shader, const char *name, float value)
{
    GLint location = shader_get_uniform(shader, name);
    if (location != -1) {
        glUniform1f(location, value);
        return OK;
    }
    return SHADER_UNIFORM_NOT_FOUND;
}
