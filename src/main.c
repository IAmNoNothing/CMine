#define STB_IMAGE_IMPLEMENTATION
#include <app.h>

int main(int argc, char *argv[]) {
    g_app = (app_t*) malloc(sizeof(app_t));

    if (app_init(g_app, argc, argv)) {
        free(g_app);
        return -1;
    }

    int result = app_run(g_app);
    free(g_app);
    g_app = NULL;
    
    return result;
}