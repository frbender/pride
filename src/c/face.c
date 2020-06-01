#include "face.h"
#include "perlin.h"

/**
 * Config of the circles
 */
static cirle_info circles[] = {
        {
                72,
                84,
                60,
                32,
                0
        },
        {
                72,
                84,
                50,
                32,
                1
        },
        {
                72,
                84,
                40,
                32,
                2
        },
        {
                72,
                84,
                30,
                32,
                3
        },
        {
                72,
                84,
                20,
                32,
                4
        },
        {
                72,
                84,
                10,
                32,
                5
        }
};

static AppTimer *update_timer;

static float strength_buffer = 0.0f;
static float strength_interval = 1.0f;
static float strength = 0.0f;

static int hour = -1;
static int minutes = -1;

void draw_hands(GContext *ctx);

void face_deinit() {
    for (int i = 0; i < 6; i++) {
        circle_destroy(circle_path_info[i]);
        gpath_destroy(circle_path[i]);
    }
    layer_destroy(layer);
    app_timer_cancel(update_timer);
    tick_timer_service_unsubscribe();
}

/**
 * Called to update the wobble. This does not draw the face, this only wobbles the paths!
 * @param data NULL
 */
void face_update(void *data) {
    float t = (float) time_ms(NULL, NULL) / 1000.0f + time(NULL) % 1000;
    for (int i = 0; i < 6; ++i) {
        circle_warp(circle_path_info[i], &circles[i], strength, t + i);
    }
    layer_mark_dirty(layer);
    if (strength_buffer > 0) {
        strength += strength_buffer / 10.0;
        strength_buffer -= strength_interval;
        strength = strength > 30 ? 30 : strength;
    } else {
        strength *= 0.95f;
    }
    if (strength > 0.5) {
        update_timer = app_timer_register(20, &face_update, NULL);
    } else {
        strength = 0;
    }
}

/**
 * Called to adjust the time to the now-time
 * @param tick_time this contains the time
 * @param units_changed ignored
 */
static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
    hour = tick_time->tm_hour;
    minutes = tick_time->tm_min;
    layer_mark_dirty(layer);
}

void face_init(Window *window) {
    for (int i = 0; i < 6; i++) {
        circle_path_info[i] = cirle_create(&circles[i]);
        circle_path[i] = gpath_create(circle_path_info[i]);
    }

    Layer *window_layer = window_get_root_layer(window);
    GRect bounds = layer_get_bounds(window_layer);

    layer = layer_create(bounds);
    layer_set_update_proc(layer, &face_draw);

    layer_add_child(window_layer, layer);

    tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);

    time_t temp = time(NULL);
    struct tm *tick_time = localtime(&temp);
    minutes = tick_time->tm_min;
    hour = tick_time->tm_hour;
    layer_mark_dirty(layer);
}

/**
 * Convert from int to a GColor
 * @param color 0 = red, 5 = purple
 * @return the GColor
 */
GColor color_from_int(int color) {
    switch (color) {
        default:
        case 0:
            return GColorRed;
        case 1:
            return GColorChromeYellow;
        case 2:
            return GColorYellow;
        case 3:
            return GColorGreen;
        case 4:
            return GColorBlueMoon;
        case 5:
            return GColorPurple;
    }
}

/**
 * Draws the watch face.
 * @param layer
 * @param ctx
 */
void face_draw(struct Layer *layer, GContext *ctx) {
    for (int i = 0; i < 6; i++) {
        graphics_context_set_fill_color(ctx, color_from_int(circles[i].color));
        gpath_draw_filled(ctx, circle_path[i]);
    }

    if (minutes != -1 && hour != -1) {
        draw_hands(ctx);
    }
}

/**
 * Draws the two hands
 * @param ctx
 */
void draw_hands(GContext *ctx) {
    GPoint center = GPoint(72, 84);
    GPoint hour_point = GPoint(
            sin_lookup((hour % 12)*TRIG_MAX_ANGLE / 12)*35/TRIG_MAX_RATIO + center.x,
            -cos_lookup((hour % 12)*TRIG_MAX_ANGLE / 12)*35/TRIG_MAX_RATIO + center.y);
    GPoint minute_point = GPoint(
            sin_lookup((minutes % 60)*TRIG_MAX_ANGLE / 60)*55/TRIG_MAX_RATIO + center.x,
            -cos_lookup((minutes % 60)*TRIG_MAX_ANGLE / 60)*55/TRIG_MAX_RATIO + center.y);

    graphics_context_set_stroke_color(ctx, GColorBlack);
    graphics_context_set_stroke_width(ctx, 6);

    graphics_draw_line(ctx, GPoint(72, 84), hour_point);
    graphics_draw_line(ctx, GPoint(72, 84), minute_point);

    graphics_context_set_stroke_color(ctx, GColorWhite);
    graphics_context_set_stroke_width(ctx, 3);

    graphics_draw_line(ctx, GPoint(72, 84), hour_point);
    graphics_draw_line(ctx, GPoint(72, 84), minute_point);
}

/**
 * Sets the strength_buffer to some values. With time the strength buffer slowly pours over into the strength
 * variable. This variable continously looss 5% of its value. This results in a slow speedup of the wobble and a slow
 * slowdown of the wobble.
 *
 * Imagine you have a glass with a hole in it. The amount of water in the glass is the wobble.
 * strength_buffer is how much you fill that glass and strength_interval is the rate at which you fill it.
 */
void face_begin_animation() {
    strength_buffer = 15.0f;
    app_timer_cancel(update_timer);
    update_timer = app_timer_register(20, &face_update, NULL);
    APP_LOG(APP_LOG_LEVEL_INFO, "Tapped");
}
