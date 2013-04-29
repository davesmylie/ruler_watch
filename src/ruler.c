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

Window window;

Layer rulerLayer; // The board/grid
Layer textLayer; // The board/grid

TextLayer hourLayer0;
TextLayer hourLayer1;
TextLayer hourLayer2;
TextLayer hourLayer3;
TextLayer hourLayer4;
TextLayer hourLayer5;
TextLayer hourLayer6;
TextLayer hourLayer7;
TextLayer hourLayer8;
TextLayer hourLayer9;
TextLayer hourLayer10;
TextLayer hourLayer11;
TextLayer hourLayer12;
TextLayer hourLayer13;
TextLayer hourLayer14;
TextLayer hourLayer15;
TextLayer hourLayer16;
TextLayer hourLayer17;
TextLayer hourLayer18;
TextLayer hourLayer19;
TextLayer hourLayer20;
TextLayer hourLayer21;
TextLayer hourLayer22;
TextLayer hourLayer23;
TextLayer hourLayers[24];


char hourStr0[3];
char hourStr1[3];
char hourStr2[3];
char hourStr3[3];
char hourStr4[3];
char hourStr5[3];
char hourStr6[3];
char hourStr7[3];
char hourStr8[3];
char hourStr9[3];
char hourStr10[3];
char hourStr11[3];
char hourStr12[3];
char hourStr13[3];
char hourStr14[3];
char hourStr15[3];
char hourStr16[3];
char hourStr17[3];
char hourStr18[3];
char hourStr19[3];
char hourStr20[3];
char hourStr21[3];
char hourStr22[3];
char hourStr23[3];
char *hourStrings[24];

int hour = 3;
int min  = 34;
int _y = 80; //for testing


//
// draws thVje hours on the screen, offset to the right position
void draw_hour(TextLayer *layer, int y) {
  text_layer_set_text(layer, "12");
}


void text_layer_update_callback() {
  layer_set_frame(&textLayer, GRect(0, _y ,144,168));
  _y--;
}

void init_hour(TextLayer *layer, int y, char *value) {
 static char _str[10];
  text_layer_init(layer, GRect(80, y * 10 ,30,60));
  layer_add_child(&textLayer, &layer->layer);
  text_layer_set_text_color(layer, COLOR_FOREGROUND);
  mini_snprintf(_str, 20, "%d bpm", y);
  text_layer_set_text(layer, value);
}

/*--------------------------------------------------
* void init_hour2(int hour){
*  static char _str[10];
*   TextLayer _layer;
*   text_layer_init(&_layer, GRect(80, hour * 10 ,30,60));
*   layer_add_child(&textLayer, &_layer.layer);
*   text_layer_set_text_color(&_layer, COLOR_FOREGROUND);
*   mini_snprintf(_str, 20, ".%d bpm", hour);
*   text_layer_set_text(&_layer, _str);
* }
*--------------------------------------------------*/

/*--------------------------------------------------
* void init_hours() {
*   init_hour(&hourLayer0, 20);
*   init_hour(&hourLayer1, 50);
*   init_hour(&hourLayer2, 80);
*   init_hour(&hourLayer3, 120);
*   init_hour(&hourLayer4, 120);
* }
*--------------------------------------------------*/

void init_hours3() {
  hourLayers[0] = hourLayer0;
  hourLayers[1] = hourLayer1;
  hourLayers[2] = hourLayer2;
  hourLayers[3] = hourLayer3;
  hourLayers[4] = hourLayer4;
  hourLayers[5] = hourLayer5;
  hourLayers[6] = hourLayer6;
  hourLayers[7] = hourLayer7;
  hourLayers[8] = hourLayer8;
  hourLayers[9] = hourLayer9;
  hourLayers[10] = hourLayer10;
  hourLayers[11] = hourLayer11;
  hourLayers[12] = hourLayer12;
  hourLayers[13] = hourLayer13;
  hourLayers[14] = hourLayer14;
  hourLayers[15] = hourLayer15;
  hourLayers[16] = hourLayer16;
  hourLayers[17] = hourLayer17;
  hourLayers[18] = hourLayer18;
  hourLayers[19] = hourLayer19;
  hourLayers[20] = hourLayer20;
  hourLayers[21] = hourLayer21;
  hourLayers[22] = hourLayer22;
  hourLayers[23] = hourLayer23;
  hourStrings[0]  = hourStr0;
  hourStrings[1]  = hourStr1;
  hourStrings[2]  = hourStr2;
  hourStrings[3]  = hourStr3;
  hourStrings[4]  = hourStr4;
  hourStrings[5]  = hourStr5;
  hourStrings[6]  = hourStr6;
  hourStrings[7]  = hourStr7;
  hourStrings[8]  = hourStr8;
  hourStrings[9]  = hourStr9;
  hourStrings[10] = hourStr10;
  hourStrings[11] = hourStr11;
  hourStrings[12] = hourStr12;
  hourStrings[13] = hourStr13;
  hourStrings[14] = hourStr14;
  hourStrings[15] = hourStr15;
  hourStrings[16] = hourStr16;
  hourStrings[17] = hourStr17;
  hourStrings[18] = hourStr18;
  hourStrings[19] = hourStr19;
  hourStrings[20] = hourStr20;
  hourStrings[21] = hourStr21;
  hourStrings[22] = hourStr22;
  hourStrings[23] = hourStr23;

  for(int i = 0; i < 24; i++) {
    mini_snprintf(hourStrings[i], 3, "%d x", i);
    init_hour(&hourLayers[i],  i, hourStrings[i]);
  }
}

/*--------------------------------------------------
* void init_hours2() {
*   for (int i = 0; i < 4; i++) {
*     init_hour2(i);
*   }
* }
*--------------------------------------------------*/



void drawRuler() {
  GContext *ctx;
  int x = 0;
  int y = 0;
  ctx = app_get_current_graphics_context();


  graphics_context_set_stroke_color(ctx, COLOR_FOREGROUND);

  for (int j = 0; j < 50; j++ ) {

    for (int i = 0; i < 12; i ++ ) {
      x = x + 3;
      if  (i  == 0)  {
        y = 70;
      } else if  (i % 3 == 0 ) 
        y = 40;
      else
        y = 30;
      graphics_draw_line(ctx, GPoint(19, x), GPoint(y, x));
      //need to draw 24 hours at the hour pointsn of the timer
      //make timerhandler move the view down
    }
  }
}


void rulerLayer_update_callback (Layer *me, GContext* ctx) {
  (void)me; // Prevents "unused" warnings.
  drawRuler();
  layer_set_frame(&rulerLayer, GRect(0, _y ,144,168));
}

void handle_init_app(AppContextRef ctx) {
  (void)ctx;

  window_init(&window, "Window Name");
  window_stack_push(&window, true /* Animated */);
  window_set_background_color(&window, COLOR_BACKGROUND);
  layer_init(&rulerLayer, window.layer.frame); // Associate with layer object and set dimensions
  layer_init(&textLayer, window.layer.frame); // Associate with layer object and set dimensions
layer_set_clips(&rulerLayer, false);
  layer_set_frame(&rulerLayer, GRect(0, _y ,144,1068));
  rulerLayer.update_proc = &rulerLayer_update_callback; // Set the drawing callback function for the layer.
  layer_add_child(&window.layer, &rulerLayer); // Add the child to the app's base window
  layer_add_child(&window.layer, &textLayer); // Add the child to the app's base window

  textLayer.update_proc = &text_layer_update_callback;
  init_hours3();
  text_layer_update_callback();

  drawRuler();

  timer_handle = app_timer_send_event(ctx, 200, DEBUG_TIMER); // and loop again


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
  drawRuler();

      _y = _y - 10;
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
