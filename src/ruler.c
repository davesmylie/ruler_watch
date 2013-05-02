#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"
#include "mini-printf.h"


#define MY_UUID { 0xE7, 0x5C, 0xA6, 0xAE, 0x04, 0xC8, 0x48, 0x35, 0xAD, 0x9F, 0xE0, 0xDB, 0xEC, 0x3F, 0x16, 0x74 }
PBL_APP_INFO(MY_UUID,
             "Template App", "Your Company",
             1, 0, /* App version */
             DEFAULT_MENU_ICON,
             APP_INFO_STANDARD_APP);

//#define INVERT_COLORS

#ifndef INVERT_COLORS
#define COLOR_FOREGROUND GColorBlack
#define COLOR_BACKGROUND GColorWhite
#else
#define COLOR_FOREGROUND GColorWhite
#define COLOR_BACKGROUND GColorBlack
#endif

AppTimerHandle timer_handle;
#define DEBUG_TIMER 1

#define GRADIENT 3 // distance each 5 min line apart
#define INITIAL_OFFSET  GRADIENT * 6 * 2

Window window;

Layer rulerLayer; // The board/grid
Layer lineLayer; // The board/grid

TextLayer hourLayer0;
TextLayer hourLayer1;
TextLayer hourLayer2;
TextLayer hourLayer3;
TextLayer hourLayer4;
TextLayer hourLayer5;
TextLayer hourLayers[6];


char hourStr0[13];
char hourStr1[13];
char hourStr2[13];
char hourStr3[13];
char hourStr4[13];
char hourStr5[13];
char *hourStrings[6];

int hour = 4;
int min  = 34;


void init_hour(TextLayer *layer, int y) {
  // 12 gradients per hour, subtract 5 to make the number roughly in the middle of the line
  text_layer_init(layer, GRect(80, (y * (12 * GRADIENT)) - 5  ,60,20));
  layer_add_child(&rulerLayer, &layer->layer);
//  text_layer_set_text_color(layer, GColorWhite);
// text_layer_set_background_color(layer, GColorBlack);
  text_layer_set_text(layer, "x");
}


//currently seems to blow up on either line
void set_hour_string(TextLayer *layer, char *str, int _hour) {
  mini_snprintf(str, 20, "'%d' - %d:%d", _hour, hour, min);
  text_layer_set_text(layer, str);
}

void init_hours() {
  hourLayers[0] = hourLayer0;
  hourLayers[1] = hourLayer1;
  hourLayers[2] = hourLayer2;
  hourLayers[3] = hourLayer3;
  hourLayers[4] = hourLayer4;
  hourLayers[5] = hourLayer5;
  hourLayers[6] = hourLayer5;
  hourStrings[0]  = hourStr0;
  hourStrings[1]  = hourStr1;
  hourStrings[2]  = hourStr2;
  hourStrings[3]  = hourStr3;
  hourStrings[4]  = hourStr4;
  hourStrings[5]  = hourStr5;
  hourStrings[6]  = hourStr5;

  for (int i = 0; i <= 6; i++) {
    init_hour(&hourLayers[i], i);
  }
}



// draws the current time line marker
void drawLineLayer() {
  GContext *ctx;
  ctx = app_get_current_graphics_context();
  graphics_context_set_stroke_color(ctx, COLOR_FOREGROUND);
  graphics_draw_line(ctx, GPoint(0, 100), GPoint(144, 100));
  graphics_draw_line(ctx, GPoint(0, 101), GPoint(144, 101));

}


void drawRuler() {
  GContext *ctx;
  ctx = app_get_current_graphics_context();
  int x = 0;
  int y = 0;
  int hour_layer_counter = 0; // the hour being drawn - this must always go 0,1,2,3,4,5 etc (
                              // (regardless of the actual hour)

  graphics_context_set_stroke_color(ctx, COLOR_FOREGROUND);

 for (int _hour = 0; _hour <= 5; _hour++, hour_layer_counter++ ) {
    for (int _min = 0; _min < 59; _min= _min + 5 ) {
      y = y + GRADIENT;
      if  (_min  == 0)  {
        x = 70;
      } else if  (_min % 15 == 0 ) 
        x = 40;
      else
        x = 30;

      graphics_draw_line(ctx, GPoint(19, y), GPoint(x, y));
      set_hour_string(&hourLayers[hour_layer_counter], hourStrings[hour_layer_counter], _hour);
    }
  }
}


void rulerLayer_update_callback (Layer *me, GContext* ctx) {
  (void)me; // Prevents "unused" warnings.
  int offset = ((min / 5) * GRADIENT) - INITIAL_OFFSET;
  layer_set_frame(&rulerLayer, GRect(0, offset ,144,168));
  drawRuler();
}


void handle_init_app(AppContextRef ctx) {
  (void)ctx;

  window_init(&window, "Window Name");
  window_stack_push(&window, true /* Animated */);
  window_set_background_color(&window, COLOR_BACKGROUND);
  layer_init(&rulerLayer, window.layer.frame); // Associate with layer object and set dimensions
  init_hours();

  layer_set_clips(&rulerLayer, false);
  rulerLayer.update_proc = &rulerLayer_update_callback; // Set the drawing callback function for the layer.
  layer_add_child(&window.layer, &rulerLayer); // Add the child to the app's base window


  //drawRuler();
  timer_handle = app_timer_send_event(ctx, 1000, DEBUG_TIMER); // and loop again


}

static void handle_second_tick(AppContextRef ctx, PebbleTickEvent *t) {
  (void)ctx;
  (void)t;

  // Causes a redraw of the layer (via the
  // associated layer update callback)
  // Note: This will cause the entire layer to be cleared first so it needs
  //       to be redrawn in its entirety--if you want to preserve drawn
  //       content you must have it on a different layer. e.g. board vs player layers.
  layer_mark_dirty(&rulerLayer);
  //drawRuler();

}

// in the timer, we fake time moving on
void debug_timer(AppContextRef ctx, AppTimerHandle handle, uint32_t cookie) {
    if (cookie == DEBUG_TIMER) {
      min = min + 5;
      if (min > 55) { 
        hour = hour + 1;
        min = 0;
      }
      if (hour > 23) {
        hour = 0;
      }
     timer_handle = app_timer_send_event(ctx, 1000, DEBUG_TIMER); // and loop again
    }
}

void pbl_main(void *params) {
  PebbleAppHandlers handlers = {

    // Handle app start
    .init_handler = &handle_init_app,
    .timer_handler = &debug_timer,

    // Handle time updates
    .tick_info = {
      .tick_handler = &handle_second_tick,
      .tick_units = SECOND_UNIT
    }
  };

  app_event_loop(params, &handlers);
}
