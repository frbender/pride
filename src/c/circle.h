#ifndef PRIDE_CIRCLE_H
#define PRIDE_CIRCLE_H

#include <pebble.h>
#include "perlin.h"

typedef struct {
    float center_x;
    float center_y;
    float radius;
    int num_points;
    int color;
} cirle_info;

void circle_warp(GPathInfo *info, cirle_info *circle, float strength, float time);

GPathInfo *cirle_create(cirle_info *info);

void circle_destroy(GPathInfo *info);

#endif