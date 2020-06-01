/**
 * Entrypoint
 */
#include "pride.h"

void init() {
    init_window();
}

void deinit() {
    deinit_window();
}

int main(void) {
    init();
    app_event_loop();
    deinit();
}
