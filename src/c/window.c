/**
 * Sets the Window up and initializes the watchface
 */
#include "window.h"
#include "face.h"

void deinit_window() {
    window_destroy(window);
}

static void tap_handler(AccelAxisType axis, int32_t direction) {
    face_begin_animation();
}

void init_window() {
    window = window_create();
    window_set_window_handlers(window, (WindowHandlers) {
            .load = window_load,
            .unload = window_unload
    });
    window_set_background_color(window, GColorBlack);

    window_stack_push(window, true);
}

void window_unload(Window *window) {
    face_deinit();
    accel_tap_service_unsubscribe();
}

void window_load(Window *window) {
    face_init(window);
    accel_tap_service_subscribe(&tap_handler);
}