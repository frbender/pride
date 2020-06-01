#ifndef PRIDE_FACE_H
#define PRIDE_FACE_H

#include <pebble.h>
#include "circle.h"
#include "window.h"

static Layer * layer;
static GPath * circle_path[6];
static GPathInfo * circle_path_info[6];


void face_draw (struct Layer *layer, GContext* ctx);

void face_init(Window * window);

void face_deinit();

void face_begin_animation();

#endif