#include <pebble.h>

static Window *window;
static TextLayer *text_layer;

static GBitmap *s_res_image_lock;
static BitmapLayer *s_bitmaplayer_1;

static TextLayer *s_time_layer;
static char time_text[] = "00:00";


static void handle_tick(struct tm *tick_time, TimeUnits units_changed) {
	if(clock_is_24h_style()) {
		strftime(time_text, sizeof(time_text), "%H:%M", tick_time);
	}
	else {
		strftime(time_text, sizeof(time_text), "%I:%M", tick_time);	
		if (time_text[0] == '0') {
			memmove(&time_text[0], &time_text[1], sizeof(time_text) - 1); //remove leading zero
		}
	}    
	text_layer_set_text(s_time_layer, time_text);
}


static void locked_click_handler(ClickRecognizerRef recognizer, void *context) {
  //text_layer_set_text(text_layer, "LOCKED");
	
}

static void click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_SELECT, locked_click_handler);
  window_single_click_subscribe(BUTTON_ID_UP, locked_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, locked_click_handler);
	window_single_click_subscribe(BUTTON_ID_BACK, locked_click_handler);
}

static void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);
	
  s_res_image_lock = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_LOCK);
  // s_bitmaplayer_1
  s_bitmaplayer_1 = bitmap_layer_create(GRect(0, 5, 144, 144));
  bitmap_layer_set_bitmap(s_bitmaplayer_1, s_res_image_lock);
  layer_add_child(window_get_root_layer(window), (Layer *)s_bitmaplayer_1);	

  text_layer = text_layer_create((GRect) { .origin = { 0, 95 }, .size = { bounds.size.w, 20 } });
  //text_layer_set_text(text_layer, "Press a button");
  text_layer_set_text_alignment(text_layer, GTextAlignmentCenter);
	text_layer_set_text_color(text_layer, GColorWhite);
	text_layer_set_background_color(text_layer, GColorClear);
  //layer_add_child(window_layer, text_layer_get_layer(text_layer));
	
  //s_time_layer
  s_time_layer = text_layer_create(GRect(0, 95, 144, 40));
  text_layer_set_background_color(s_time_layer, GColorClear);
  text_layer_set_text_color(s_time_layer, GColorWhite);
  text_layer_set_text(s_time_layer, "00:00");
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);
  text_layer_set_font(s_time_layer, fonts_get_system_font(FONT_KEY_BITHAM_34_MEDIUM_NUMBERS));
  layer_add_child(window_layer, (Layer *)s_time_layer);	
	
	//Somebody set us up the CLOCK
	time_t now = time(NULL);
	struct tm *tick_time = localtime(&now);  

	handle_tick(tick_time, MINUTE_UNIT);
	tick_timer_service_subscribe(MINUTE_UNIT, handle_tick);		
	
}

static void window_unload(Window *window) {
  text_layer_destroy(text_layer);
	text_layer_destroy(s_time_layer);
  bitmap_layer_destroy(s_bitmaplayer_1);
  gbitmap_destroy(s_res_image_lock);	
}

static void init(void) {
  window = window_create();
	window_set_fullscreen(window, true);
  window_set_click_config_provider(window, click_config_provider);
  window_set_window_handlers(window, (WindowHandlers) {
	.load = window_load,
    .unload = window_unload,
  });
	
  const bool animated = true;
  window_stack_push(window, animated);
}

static void deinit(void) {
  window_destroy(window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}