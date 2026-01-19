#include "app.h"

app_t* g_app;

int app_init(app_t *app, int argc, char *argv[])
{
    if (!glfwInit()) {
        log_error("Failed to initialize GLFW");
        return -1;
    }

    if (window_init(&app->window, 800, 600, "CMine") != 0) {
        log_error("Failed to create window");
        return -1;
    }

    window_make_context_current(&app->window);
    glfwSetFramebufferSizeCallback(app->window.glfw_window, app_window_resized_callback);
    glfwSetCursorPosCallback(app->window.glfw_window, app_mouse_position_callback);
    glfwSetKeyCallback(app->window.glfw_window, app_key_callback);
    glfwSetInputMode(app->window.glfw_window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

    if (glewInit() != GLEW_OK) {
        log_error("Failed to initialize GLEW");
        window_destroy(&app->window);
        return -1;
    }

    log_info("Initializing buffers...");
    vb_init(&app->vertex_buffer);

    log_info("Initializing shaders...");
    error_code_t result = shader_init_file(&app->shader,"shaders/vertex.glsl", "shaders/fragment.glsl");
    if (result != OK) {
        return result;
    }

    app_generate_cube(app);

    log_info("Application initialized successfully");

    glEnable(GL_DEPTH_TEST /*| GL_CULL_FACE*/);
    // glCullFace(GL_BACK);
    // glFrontFace(GL_CCW);

    if (gl_texture_create_from_file(&app->texture, "assets/testTexture.png")) {
        log_error("Failed to load texture");
        return -1;
    }

    shader_use(&g_app->shader);
    shader_uniform_1i(&g_app->shader, "width", app->window.width);
    shader_uniform_1i(&g_app->shader, "height", app->window.height);

    camera_init(&app->camera, 0.2f, 10.0f);

    app->window_center[0] = app->window.width / 2.0f;
    app->window_center[1] = app->window.height / 2.0f;

    app->mouse_delta[0] = 0;
    app->mouse_delta[1] = 0;

    app->cursor_captured = 0;

    return 0;
}

int app_run(app_t *app)
{
    double last_time = glfwGetTime();

    app_pre_run(app);
    log_info("Starting main loop.");
    while (!window_should_close(&app->window)) {
        double current_time = glfwGetTime();
        app->delta_time = (float)(current_time - last_time);
        last_time = current_time;

        glfwPollEvents();
        window_clear(&app->window, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        app_update(app);
        window_swap_buffers(&app->window);
    }
    log_info("Exiting main loop.");

    app_cleanup(app);
    log_info("Application cleaned up successfully.");
    return 0;
}

void app_cleanup(app_t *app)
{
    gl_texture_destroy(&app->texture);
    vb_free(&app->vertex_buffer);
    window_destroy(&app->window);
    glfwTerminate(); 
}

void app_update(app_t *app)
{
    shader_use(&app->shader);

    gl_texture_bind(&app->texture, GL_TEXTURE0);
    GLuint loc_tex = glGetUniformLocation(app->shader.id, "tex");
    glUniform1i(loc_tex, 0);

    double mx, my;
    glfwGetCursorPos(app->window.glfw_window, &mx, &my);
    app->mouse_position[0] = (float)mx;
    app->mouse_position[1] = (float)my;

    if (app->cursor_captured) {
        glfwSetCursorPos(app->window.glfw_window, app->window_center[0], app->window_center[1]);
        glm_vec2_sub(app->mouse_position, app->window_center, app->mouse_delta);
    } else {
        app->mouse_delta[0] = 0;
        app->mouse_delta[1] = 0;
    }

    mat4 view;

    camera_rotate_with_mouse(&app->camera, app->mouse_delta, app->delta_time);
    camera_update_basis_if_needed(&app->camera);
    app_move_camera(app);
    // printf("%f %f %f\n", app->camera.position[0], app->camera.position[1], app->camera.position[2]);
    camera_update(&app->camera);
    camera_get_view_matrix(&app->camera, view);
    shader_uniform_mat4(&app->shader, "view", view);

    vb_render(&app->vertex_buffer, GL_TRIANGLES);
}

void app_pre_run(app_t *app)
{
    mat4 projection;
    glm_perspective(glm_rad(90.0f), (float)(app->window.width) / app->window.height, 0.01f, 100.0f, projection);
    
    mat4 model;
    glm_mat4_identity(model);

    shader_use(&app->shader);

    shader_uniform_mat4(&app->shader, "projection", projection);
    shader_uniform_mat4(&app->shader, "model", model);
}

void app_window_resized_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
    g_app->window.width = width;
    g_app->window.height = height;

    mat4 projection;
    glm_perspective(glm_rad(90.0f), (float)(g_app->window.width) / g_app->window.height, 0.1f, 100.0f, projection);
    shader_uniform_mat4(&g_app->shader, "projection", projection);

    g_app->window_center[0] = g_app->window.width / 2.0f;
    g_app->window_center[1] = g_app->window.height / 2.0f;
}

void app_mouse_position_callback(GLFWwindow *window, double xpos, double ypos)
{
    
}

void app_key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_G && action == GLFW_PRESS) {
        if (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            g_app->cursor_captured = 0;
        } else {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            g_app->cursor_captured = 1;
        }
    }
}

void app_generate_cube(app_t *app)
{
    // FRONT
    vb_push_vertex(&app->vertex_buffer, (vertex_t){ .pos={ 0.5, 0.5, 0.5}, .uv={ 1, 1 } });
    vb_push_vertex(&app->vertex_buffer, (vertex_t){ .pos={ 0.5,-0.5, 0.5}, .uv={ 1, 0 } });
    vb_push_vertex(&app->vertex_buffer, (vertex_t){ .pos={-0.5, 0.5, 0.5}, .uv={ 0, 1 } });
    vb_push_vertex(&app->vertex_buffer, (vertex_t){ .pos={-0.5,-0.5, 0.5}, .uv={ 0, 0 } });

    vb_push_index(&app->vertex_buffer, 0);
    vb_push_index(&app->vertex_buffer, 1);
    vb_push_index(&app->vertex_buffer, 2);
    vb_push_index(&app->vertex_buffer, 1);
    vb_push_index(&app->vertex_buffer, 3);
    vb_push_index(&app->vertex_buffer, 2);

    // BACK
    vb_push_vertex(&app->vertex_buffer, (vertex_t){ .pos={ 0.5, 0.5,-0.5}, .uv={ 1, 1 } });
    vb_push_vertex(&app->vertex_buffer, (vertex_t){ .pos={ 0.5,-0.5,-0.5}, .uv={ 1, 0 } });
    vb_push_vertex(&app->vertex_buffer, (vertex_t){ .pos={-0.5, 0.5,-0.5}, .uv={ 0, 1 } });
    vb_push_vertex(&app->vertex_buffer, (vertex_t){ .pos={-0.5,-0.5,-0.5}, .uv={ 0, 0 } });

    vb_push_index(&app->vertex_buffer, 4);
    vb_push_index(&app->vertex_buffer, 5);
    vb_push_index(&app->vertex_buffer, 6);
    vb_push_index(&app->vertex_buffer, 5);
    vb_push_index(&app->vertex_buffer, 7);
    vb_push_index(&app->vertex_buffer, 6);

    // LEFT
    vb_push_vertex(&app->vertex_buffer, (vertex_t){ .pos={-0.5, 0.5,  0.5}, .uv={ 1, 1 } });
    vb_push_vertex(&app->vertex_buffer, (vertex_t){ .pos={-0.5,-0.5,  0.5}, .uv={ 1, 0 } });
    vb_push_vertex(&app->vertex_buffer, (vertex_t){ .pos={-0.5, 0.5, -0.5}, .uv={ 0, 1 } });
    vb_push_vertex(&app->vertex_buffer, (vertex_t){ .pos={-0.5,-0.5, -0.5}, .uv={ 0, 0 } });

    vb_push_index(&app->vertex_buffer, 8);
    vb_push_index(&app->vertex_buffer, 9);
    vb_push_index(&app->vertex_buffer,10);
    vb_push_index(&app->vertex_buffer, 9);
    vb_push_index(&app->vertex_buffer,11);
    vb_push_index(&app->vertex_buffer,10);

    // RIGHT
    vb_push_vertex(&app->vertex_buffer, (vertex_t){ .pos={ 0.5, 0.5,  0.5}, .uv={ 1, 1 } });
    vb_push_vertex(&app->vertex_buffer, (vertex_t){ .pos={ 0.5,-0.5,  0.5}, .uv={ 1, 0 } });
    vb_push_vertex(&app->vertex_buffer, (vertex_t){ .pos={ 0.5, 0.5, -0.5}, .uv={ 0, 1 } });
    vb_push_vertex(&app->vertex_buffer, (vertex_t){ .pos={ 0.5,-0.5, -0.5}, .uv={ 0, 0 } });

    vb_push_index(&app->vertex_buffer,12);
    vb_push_index(&app->vertex_buffer,13);
    vb_push_index(&app->vertex_buffer,14);
    vb_push_index(&app->vertex_buffer,13);
    vb_push_index(&app->vertex_buffer,15);
    vb_push_index(&app->vertex_buffer,14);

    // TOP
    vb_push_vertex(&app->vertex_buffer, (vertex_t){ .pos={ 0.5, 0.5,  0.5}, .uv={ 1, 1 } });
    vb_push_vertex(&app->vertex_buffer, (vertex_t){ .pos={ 0.5, 0.5, -0.5}, .uv={ 1, 0 } });
    vb_push_vertex(&app->vertex_buffer, (vertex_t){ .pos={-0.5, 0.5,  0.5}, .uv={ 0, 1 } });
    vb_push_vertex(&app->vertex_buffer, (vertex_t){ .pos={-0.5, 0.5, -0.5}, .uv={ 0, 0 } });

    vb_push_index(&app->vertex_buffer,16);
    vb_push_index(&app->vertex_buffer,17);
    vb_push_index(&app->vertex_buffer,18);
    vb_push_index(&app->vertex_buffer,17);
    vb_push_index(&app->vertex_buffer,19);
    vb_push_index(&app->vertex_buffer,18);

    // BOTTOM
    vb_push_vertex(&app->vertex_buffer, (vertex_t){ .pos={ 0.5,-0.5,  0.5}, .uv={ 1, 1 } });
    vb_push_vertex(&app->vertex_buffer, (vertex_t){ .pos={ 0.5,-0.5, -0.5}, .uv={ 1, 0 } });
    vb_push_vertex(&app->vertex_buffer, (vertex_t){ .pos={-0.5,-0.5,  0.5}, .uv={ 0, 1 } });
    vb_push_vertex(&app->vertex_buffer, (vertex_t){ .pos={-0.5,-0.5, -0.5}, .uv={ 0, 0 } });

    vb_push_index(&app->vertex_buffer,20);
    vb_push_index(&app->vertex_buffer,21);
    vb_push_index(&app->vertex_buffer,22);
    vb_push_index(&app->vertex_buffer,21);
    vb_push_index(&app->vertex_buffer,23);
    vb_push_index(&app->vertex_buffer,22);

    vb_upload(&app->vertex_buffer);
}

void app_move_camera(app_t *app)
{
    int forward = glfwGetKey(app->window.glfw_window, GLFW_KEY_W) != GLFW_RELEASE;
    int left = glfwGetKey(app->window.glfw_window, GLFW_KEY_A) != GLFW_RELEASE;
    int back = glfwGetKey(app->window.glfw_window, GLFW_KEY_S) != GLFW_RELEASE;
    int right = glfwGetKey(app->window.glfw_window, GLFW_KEY_D) != GLFW_RELEASE;

    vec3 movement = { 0, 0, 0 };

    if (forward && !back) {
        glm_vec3_add(movement, app->camera.forward, movement);
    }
    
    if (back && !forward) {
        glm_vec3_sub(movement, app->camera.forward, movement);        
    }

    if (left && !right) {
        glm_vec3_add(movement, app->camera.right, movement);
    }

    if (right && !left) {
        glm_vec3_sub(movement, app->camera.right, movement);
    }
    
    if (glm_vec3_norm2(movement) != 0) {
        glm_vec3_normalize(movement);
        glm_vec3_scale(movement, app->camera.speed * app->delta_time, movement);
        camera_move(&app->camera, movement);
    }
}
