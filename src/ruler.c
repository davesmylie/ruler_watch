#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"


#define MY_UUID { 0xE7, 0x5C, 0xA6, 0xAE, 0x04, 0xC8, 0x48, 0x35, 0xAD, 0x9F, 0xE0, 0xDB, 0xEC, 0x3F, 0x16, 0x74 }
PBL_APP_INFO(MY_UUID,
             "Template App", "Your Company",
             1, 0, /* App version */
             DEFAULT_MENU_ICON,
             APP_INFO_STANDARD_APP);

#define INVERT_COLORS

#ifndef INVERT_COLORS
#define COLOR_FOREGROUND GColorBlack
#define COLOR_BACKGROUND GColorWhite
#else
#define COLOR_FOREGROUND GColorWhite
#define COLOR_BACKGROUND GColorBlack
#endif


Window window;

Layer rulerLayer; // The board/grid

int hour = 3;
int min  = 34



void rulerLayer_update_callback (Layer *me, GContext* ctx) {
  (void)me; // Prevents "unused" warnings.

  graphics_context_set_stroke_color(ctx, COLOR_FOREGROUND);

  // Draw playing board grid
  // TODO: Change this to allow for a nicely centred circle in each square.

  // Vertical lines
  graphics_draw_line(ctx, GPoint(54, 19), GPoint(54, 123));
  graphics_draw_line(ctx, GPoint(89, 19), GPoint(89, 123));

  // Horizontal lines
  graphics_draw_line(ctx, GPoint(19, 54), GPoint(123, 54));
  graphics_draw_line(ctx, GPoint(19, 89), GPoint(123, 89));


}

void handle_init(AppContextRef ctx) {
  (void)ctx;

  window_init(&window, "Window Name");
  window_stack_push(&window, true /* Animated */);

  layer_init(&rulerLayer, window.layer.frame); // Associate with layer object and set dimensions
  rulerLayer.update_proc = &rulerLayer_update_callback; // Set the drawing callback function for the layer.
  layer_add_child(&window.layer, &rulerLayer); // Add the child to the app's base window


}


void pbl_main(void *params) {
  PebbleAppHandlers handlers = {
    .init_handler = &handle_init
  };
  app_event_loop(params, &handlers);
}
