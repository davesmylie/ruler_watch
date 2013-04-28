#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"


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

Window window;

Layer rulerLayer; // The board/grid
Layer textLayer; // The board/grid

TextLayer hourLayer0;
TextLayer hourLayer1;
TextLayer hourLayer2;
TextLayer hourLayer3;
TextLayer hourLayer4;


int hour = 3;
int min  = 34;
int _y = 0; //for testing


// can only see hours less than 3 at the moment
int hour_visible(int _hour) {
  if  (_hour < 4 ) 
    return true;
  else
    return false;
}

//// draws the hours on the screen, offset to the right position
//void draw_hour(TextLayer layer, int y) {
//  text_layer_init(&layer, GRect(120,y,30,40));
//  layer_add_child(&window.layer, &layer.layer);
// // text_layer_set_text_color(&layer, COLOR_FOREGROUND);
//  //text_layer_set_text(&layer, "12");
//}
//
// draws thVje hours on the screen, offset to the right position
void draw_hour(TextLayer *layer, int y) {
  text_layer_set_text(layer, "12");
}


void text_layer_update_callback() {

  layer_set_frame(&textLayer, GRect(0, _y ,144,168));
  _y++;
  draw_hour(&hourLayer0, 20);
  draw_hour(&hourLayer1, 50);
  draw_hour(&hourLayer2, 80);
  draw_hour(&hourLayer3, 120);
}

void init_hour(TextLayer *layer, int y) {
  text_layer_init(layer, GRect(80, y ,30,40));
  layer_add_child(&textLayer, &layer->layer);
  text_layer_set_text_color(layer, COLOR_FOREGROUND);
  text_layer_set_text(layer, "3");
}

void init_hours() {
  init_hour(&hourLayer0, 20);
  init_hour(&hourLayer1, 50);
  init_hour(&hourLayer2, 80);
  init_hour(&hourLayer3, 120);
}


void rulerLayer_update_callback (Layer *me, GContext* ctx) {
  (void)me; // Prevents "unused" warnings.
  int x = 0;
  int y = 0;

  graphics_context_set_stroke_color(ctx, COLOR_FOREGROUND);

  for (int j = 0; j < 23; j++ ) {

    for (int i = 0; i < 12; i ++ ) {
      x = x + 3;
      if  (i  == 0)  {
        y = 70;
      } else if  (i % 3 == 0 ) 
        y = 40;
      else
        y = 30;
      graphics_draw_line(ctx, GPoint(19, x), GPoint(y, x));
    }
  }
  
}



void handle_init_app(AppContextRef ctx) {
  (void)ctx;

  window_init(&window, "Window Name");
  window_stack_push(&window, true /* Animated */);
  window_set_background_color(&window, COLOR_BACKGROUND);
  layer_init(&rulerLayer, window.layer.frame); // Associate with layer object and set dimensions
  layer_init(&textLayer, window.layer.frame); // Associate with layer object and set dimensions

  rulerLayer.update_proc = &rulerLayer_update_callback; // Set the drawing callback function for the layer.
  layer_add_child(&window.layer, &rulerLayer); // Add the child to the app's base window
  layer_add_child(&window.layer, &textLayer); // Add the child to the app's base window

  textLayer.update_proc = &text_layer_update_callback;
  init_hours();
  text_layer_update_callback();

  //draw_hours();

  timer_handle = app_timer_send_event(ctx, 1000, DEBUG_TIMER); // and loop again

  //draw_hour(&hourLayer1, 100);
  //text_layer_init(hourLayer1, GRect(120,30,30,40));
  //layer_add_child(&window.layer, &hourLayer1.layer);

}

static void handle_second_tick(AppContextRef ctx, PebbleTickEvent *t) {
  (void)ctx;
  (void)t;

  // Causes a redraw of the layer (via the
  // associated layer update callback)
  // Note: This will cause the entire layer to be cleared first so it needs
  //       to be redrawn in its entirety--if you want to preserve drawn
  //       content you must have it on a different layer. e.g. board vs player layers.
//  layer_mark_dirty(&playersLayer);

  // TODO: Find out when the redraw actually occurs. Does it make any difference about order of these calls?
 // update_time_display();
}

void debug_timer(AppContextRef ctx, AppTimerHandle handle, uint32_t cookie) {
    if (cookie == DEBUG_TIMER) {
     text_layer_update_callback();
      _y = _y + 10;
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
