#include "pebble.h"
#include <time.h>

Window *window;
TextLayer *text_time_layer;
Layer *graphics_layer;

GPoint get_gpoint(int degrees,int radius){
  double x = degrees, y = degrees;
  GPoint a;
  x = radius*sin_lookup((x/360.0)*TRIG_MAX_ANGLE);
  x /= TRIG_MAX_RATIO;
  y = -radius*cos_lookup((y/360.0)*TRIG_MAX_ANGLE);
  y /= TRIG_MAX_RATIO;
  a = GPoint((int)x+72,(int)y+74);
  //APP_LOG(APP_LOG_LEVEL_INFO,"%d %d",(int)x,(int)y);
  return a;
}

void graphics_layer_update_callback(Layer *layer, GContext* ctx) {
  struct tm * timenow;
  time_t timestamp = time(NULL);
  timenow = localtime(&timestamp);

  graphics_context_set_fill_color(ctx, GColorWhite);
  APP_LOG(APP_LOG_LEVEL_INFO,"%d %d %d",timenow->tm_hour,timenow->tm_min,timenow->tm_sec);
  //draw hours
  int i;
  for (i = 0; i < 12*15; i++) {
    if(abs((timenow->tm_hour%12)-(i/15)) > 1) {
      graphics_fill_circle(ctx,get_gpoint(i*2,35),4);
      
    }
  }
  for (i = 0; i < 60*6; i++) {
    if(abs(timenow->tm_min-(i/6)) > 5) {
      graphics_fill_circle(ctx,get_gpoint(i,47),4);
      
    }
  }
  for (i = 0; i < 60*6; i++) {
    if(abs(timenow->tm_sec-(i/6)) > 5) {
      graphics_fill_circle(ctx,get_gpoint(i,60),4);
      
    }
  }

}
//Taken from simplicity project
void handle_minute_tick(struct tm *tick_time, TimeUnits units_changed) {
  // Need to be static because they're used by the system later.
  static char time_text[] = "00:00";
  static char date_text[] = "Xxxxxxxxx 00";

  char *time_format;

  if (clock_is_24h_style()) {
    time_format = "%R";
  } else {
    time_format = "%I:%M";
  }

  strftime(time_text, sizeof(time_text), time_format, tick_time);

  // Kludge to handle lack of non-padded hour format string
  // for twelve hour clock.
  if (!clock_is_24h_style() && (time_text[0] == '0')) {
    memmove(time_text, &time_text[1], sizeof(time_text) - 1);
  }

  text_layer_set_text(text_time_layer, time_text);
}

void handle_deinit(void) {
  tick_timer_service_unsubscribe();
}



void handle_init(void) {
  window = window_create();
  window_stack_push(window, true /* Animated */);
  window_set_background_color(window, GColorBlack);

  Layer *window_layer = window_get_root_layer(window);

  text_time_layer = text_layer_create(GRect(48, 62, 144-48, 168-62));
  text_layer_set_text_color(text_time_layer, GColorWhite);
  text_layer_set_background_color(text_time_layer, GColorClear);
  text_layer_set_font(text_time_layer, fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_ROBOTO_CONDENSED_21)));
  layer_add_child(window_layer, text_layer_get_layer(text_time_layer));

  GRect line_frame = GRect(0, 0, 144, 168);
  graphics_layer = layer_create(line_frame);
  layer_set_update_proc(graphics_layer, graphics_layer_update_callback);
  layer_add_child(window_layer, graphics_layer);

  tick_timer_service_subscribe(SECOND_UNIT, handle_minute_tick);
  // TODO: Update display here to avoid blank display on launch?
}


int main(void) {
  handle_init();

  app_event_loop();
  
  handle_deinit();
}
