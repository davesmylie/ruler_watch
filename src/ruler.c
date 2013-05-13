#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"
#include "mini-printf.h"


#define MY_UUID { 0xE7, 0x5C, 0xA6, 0xAE, 0x04, 0xC8, 0x48, 0x35, 0xAD, 0x9F, 0xE0, 0xDB, 0xEC, 0x3F, 0x16, 0x74 }
PBL_APP_INFO(MY_UUID,
             "Ruler", "Dave Smylie",
             1, 4, /* App version */
             RESOURCE_ID_IMAGE_MENU_ICON,
//             APP_INFO_STANDARD_APP);
             APP_INFO_WATCH_FACE);

#define INVERT_COLORS  0

#ifndef INVERT_COLORS
#define INVERT_COLORS  1
#define COLOR_FOREGROUND GColorWhite
#define COLOR_BACKGROUND GColorBlack
#else
#define COLOR_FOREGROUND GColorBlack
#define COLOR_BACKGROUND GColorWhite
#endif

AppTimerHandle timer_handle;
#define DEBUG_TIMER 1

#define DEBUG_MODE 0

#define GRADIENT 4 // distance each 5 min line apart
#define INITIAL_OFFSET  GRADIENT * 6 * 2

#define HOUR_OFFSET 2 // number of hours the time line marker should be down from the top of the screen



Window window;

Layer rulerLayer; // The board/grid
Layer lineLayer; // The board/grid
Layer bgLayer;   // the bakcground

TextLayer dbgTextLayer;
char dbg[100];

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
TextLayer hourLayer24;
TextLayer hourLayer25;
TextLayer hourLayer26;
TextLayer hourLayer27;
TextLayer hourLayer28;
TextLayer hourLayer29;
TextLayer hourLayers[29];


char hourStr0[13];
char hourStr1[13];
char hourStr2[13];
char hourStr3[13];
char hourStr4[13];
char hourStr5[13];
char hourStr6[13];
char hourStr7[13];
char hourStr8[13];
char hourStr9[13];
char hourStr10[13];
char hourStr11[13];
char hourStr12[13];
char hourStr13[13];
char hourStr14[13];
char hourStr15[13];
char hourStr16[13];
char hourStr17[13];
char hourStr18[13];
char hourStr19[13];
char hourStr20[13];
char hourStr21[13];
char hourStr22[13];
char hourStr23[13];
char hourStr24[13];
char hourStr25[13];
char hourStr26[13];
char hourStr27[13];
char hourStr28[13];
char hourStr29[13];

char *hourStrings[29];

int hour = 9;
int min  = 37;
int dbg_offset = 0;


// manually changes the time (for debugging)
// direction should be 1 or -1
void move_time(int direction) {
    min = min + (1 * direction);

    if (min >= 60) { 
      hour = hour + 1;
      min = 0;
    }

    if (min < 0) { 
      hour = hour - 1;
      min = 55;
    }

    if (hour > 23) {
      hour = 0;
    }

    if (hour < 0) {
      hour = 23;
      min = 59;

    }
  layer_mark_dirty(&bgLayer);
  layer_mark_dirty(&rulerLayer);
  mini_snprintf(dbg, 20, "%d, %d:%d", dbg_offset, hour, min);
  text_layer_set_text(&dbgTextLayer, dbg);
}

void init_hour(TextLayer *layer, int y) {
  // 12 gradients per hour, subtract 5 to make the number roughly in the middle of the line
  text_layer_init(layer, GRect(70, (y * (12 * GRADIENT)) - 15 ,40,30));
  //text_layer_init(layer, GRect(70, (y * (12 * 2)) - 5  ,70,20));
  layer_add_child(&rulerLayer, &layer->layer);
  text_layer_set_font(layer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
  text_layer_set_background_color(layer, GColorClear);
  text_layer_set_text(layer, "x");
}


//currently seems to blow up on either line
void set_hour_string(TextLayer *layer, char *str, int _hour) {
  // convert to 12h format if that's what the phone is set to
  if (!clock_is_24h_style()) {
    _hour = (_hour % 12);
    if (_hour == 0) _hour = 12;
  }
    
  mini_snprintf(str, 20, "%d", _hour);
  text_layer_set_text(layer, str);
}

void init_hours() {
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
  hourLayers[24] = hourLayer24;
  hourLayers[25] = hourLayer25;
  hourLayers[26] = hourLayer26;
  hourLayers[27] = hourLayer27;
  hourLayers[28] = hourLayer28;
  hourLayers[29] = hourLayer29;

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
  hourStrings[10]  = hourStr10;
  hourStrings[11]  = hourStr11;
  hourStrings[12]  = hourStr12;
  hourStrings[13]  = hourStr13;
  hourStrings[14]  = hourStr14;
  hourStrings[15]  = hourStr15;
  hourStrings[16]  = hourStr16;
  hourStrings[17]  = hourStr17;
  hourStrings[18]  = hourStr18;
  hourStrings[19]  = hourStr19;
  hourStrings[20]  = hourStr20;
  hourStrings[21]  = hourStr21;
  hourStrings[22]  = hourStr22;
  hourStrings[23]  = hourStr23;
  hourStrings[24]  = hourStr24;
  hourStrings[25]  = hourStr25;
  hourStrings[26]  = hourStr26;
  hourStrings[27]  = hourStr27;
  hourStrings[28]  = hourStr28;
  hourStrings[29]  = hourStr29;

  for (int i = 0; i < 30; i++) {
    init_hour(&hourLayers[i], i);
  }
}



// draws the current time line marker
void lineLayer_update_callback (Layer *me, GContext* ctx) {
  (void)me; // Prevents "unused" warnings.
  int offset = 120;
  graphics_context_set_stroke_color(ctx, COLOR_FOREGROUND);
  graphics_draw_line(ctx, GPoint(0, offset), GPoint(144, offset));
  offset++;
  graphics_draw_line(ctx, GPoint(0, offset), GPoint(144, offset));
}


void bgLayer_update_callback(Layer *layer, GContext* ctx) {
  //GContext *ctx;
  // ctx = app_get_current_graphics_context();
  int y = 80; // position of marker line
  if (INVERT_COLORS) {
    graphics_context_set_fill_color(ctx, GColorBlack);
  } else {
    graphics_context_set_fill_color(ctx, COLOR_FOREGROUND);
    //graphics_fill_rect(ctx, GRect(0,0,144, 168), 0, GCornersAll);
    graphics_fill_rect(ctx, layer->bounds, 0, GCornersAll);
    graphics_context_set_fill_color(ctx, GColorClear);
    graphics_fill_rect(ctx, GRect(5,5,144 - 10, 168 - 10) , 4, GCornersAll);

  }


  // draw the time marker line
  graphics_context_set_stroke_color(ctx, COLOR_FOREGROUND);
  graphics_draw_line(ctx, GPoint(0, y), GPoint(144, y));
  graphics_draw_line(ctx, GPoint(0, y+1), GPoint(144, y+1));
}

void drawRuler() {
  
  GContext *ctx;
  ctx = app_get_current_graphics_context();

  int x = 0;
  int y = 0;
  int display_hour;

  graphics_context_set_stroke_color(ctx, COLOR_FOREGROUND);
 // draw 29 hours worth of lines as we need to be able to have 23.59 get to the top of the screen
 // whilst still having the next few hours beneath it. (so hours 0-3 are duplicated)
 //for (int _hour = 0; _hour <= 5; _hour++, hour_layer_counter++ ) {
 for (int _hour = 0 ; _hour < 29 ; _hour++ ) {
    for (int _min = 0; _min < 59; _min= _min + 5 ) {
      y = y + GRADIENT;
      if  (_min  == 0)  {
        x =  60; 
      } else if  (_min % 30 == 0 ) 
        x = 45;
       else if  (_min % 15 == 0 ) 
        x = 40;
      else
        x = 30;

      graphics_draw_line(ctx, GPoint(19, y), GPoint(x, y));

      // we are displaying a rolling frame of 29 odd hour markers (to make sure
      // we have extra numbers at the start and end to facillate rollinng around
      // as we are displaying this two hours backwards (to make the time marker
      // in the centre of the screen, we need to adjust the hour - make sure
      // it's less than 24, and make sure it's greeater than one
      // it doesn't display as -1 or -2, or 27 o'clock
      display_hour = (_hour - 2) % 24;
      if (display_hour < 0) display_hour = display_hour + 24;


      set_hour_string(&hourLayers[_hour], hourStrings[_hour], display_hour  );
    }
  }
}


void rulerLayer_update_callback (Layer *me, GContext* ctx) {
  (void)me; // Prevents "unused" warnings.
  int total_mins = ( (hour * 60) + min);
  int offset = (total_mins / 5) * GRADIENT * - 1 ;

  dbg_offset = offset;
  //layer_set_frame(&rulerLayer, GRect(0, offset ,144  ,148));
  //set the frame to be the area on the screen that we want the 
  //ruler lines  to be visible in (this has clipping off so we wont see
  //anything outside this box)
  layer_set_frame(&rulerLayer, GRect(5, 5 ,144-20  ,168-10));
  // offset the bounds of the layer by the length needed to show the current 
  // time
  layer_set_bounds(&rulerLayer, GRect(0, offset ,100 ,100));
  drawRuler();

}


// single click - increment time (for debugging)
void up_single_click_handler(ClickRecognizerRef recognizer, Window *window_2) {
  (void)recognizer;
  (void)window_2;
  move_time(-1);
}


// single click - decrement time (for debugging)
void down_single_click_handler(ClickRecognizerRef recognizer, Window *window) {
  (void)recognizer;
  (void)window;
  move_time(1);
}

// set up buttons
void click_config_provider(ClickConfig **config, Window *window) {
  (void)window;
  config[BUTTON_ID_UP]->click.handler = (ClickHandler) up_single_click_handler;
  config[BUTTON_ID_UP]->click.repeat_interval_ms = 100;

  config[BUTTON_ID_DOWN]->click.handler = (ClickHandler) down_single_click_handler;
  config[BUTTON_ID_DOWN]->click.repeat_interval_ms = 100;
}

void init_bg_layer() {
  layer_init(&bgLayer, window.layer.frame); // Associate with layer object and set dimensions
  lineLayer.update_proc = &bgLayer_update_callback; // Set the drawing callback function for the layer.
  layer_add_child(&window.layer, &bgLayer); // Add the child to the app's base window
}

void init_line_layer() {
  layer_init(&lineLayer, window.layer.frame); // Associate with layer object and set dimensions
  //layer_set_clips(&lineLayer, false);
  lineLayer.update_proc = &lineLayer_update_callback; // Set the drawing callback function for the layer.
  layer_add_child(&window.layer, &lineLayer); // Add the child to the app's base window
}

void init_ruler_layer() {
  rulerLayer.update_proc = &rulerLayer_update_callback; // Set the drawing callback function for the layer.
  layer_add_child(&window.layer, &rulerLayer); // Add the child to the app's base window

}


// gets the system time and sets our hour/min variables
void set_time(){
  // don't use the system time in debug mode
  if (!DEBUG_MODE) {
    PblTm time;
    get_time(&time);
    hour = time.tm_hour;
    min = time.tm_min;
  }

  if (min == 0) {
    vibes_short_pulse();
  }
}

// updates the time counters, marks layers dirty  and draws debug info
void update_screen() {
  set_time();
  layer_mark_dirty(&bgLayer);
  layer_mark_dirty(&rulerLayer);
  if (DEBUG_MODE) {
    mini_snprintf(dbg, 20, "%d, %d:%d", dbg_offset, hour, min);
    text_layer_set_text(&dbgTextLayer, dbg);
  }
}


void handle_init_app(AppContextRef ctx) {
  (void)ctx;

  window_init(&window, "Window Name");
  window_stack_push(&window, true /* Animated */);
  window_set_background_color(&window, COLOR_BACKGROUND);
  layer_init(&rulerLayer, window.layer.frame); // Associate with layer object and set dimensions
  init_hours();

  //line layer isn't being used at all, but if we remove, things don't work????
  //(the line is now drawn on  bg layer...
  init_line_layer();
  init_bg_layer();
  init_ruler_layer();

  if (DEBUG_MODE) {
    text_layer_init(&dbgTextLayer, GRect(40, 40, 80, 20));
    layer_add_child(&window.layer, &dbgTextLayer.layer);
    text_layer_set_text(&dbgTextLayer, "xxx");
    window_set_click_config_provider(&window, (ClickConfigProvider) click_config_provider);
  }

  update_screen();
}




// once a minute update position of the ruler on the screen
static void handle_minute_tick(AppContextRef ctx, PebbleTickEvent *t) {
  (void)ctx;
  (void)t;
  update_screen();
}




void pbl_main(void *params) {
  PebbleAppHandlers handlers = {

    // Handle app start
    .init_handler = &handle_init_app,

    // Handle time updates
    .tick_info = {
      .tick_handler = &handle_minute_tick,
      .tick_units = MINUTE_UNIT
    }
  };

  app_event_loop(params, &handlers);
}
