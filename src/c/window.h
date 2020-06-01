#ifndef PRIDE_WINDOW_H
#define PRIDE_WINDOW_H

#include <pebble.h>
#include "face.h"

static Window * window;

static void window_load(Window *window);

static void window_unload(Window *window);

void init_window();

void deinit_window();

#endif