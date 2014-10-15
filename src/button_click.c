#include <pebble.h>

static Window *window;
static TextLayer *text_layer;

static GBitmap *s_res_image_lock;
static BitmapLayer *s_bitmaplayer_1;

static void locked_click_handler(ClickRecognizerRef recognizer, void *context) {
  text_layer_set_text(text_layer, "LOCKED");
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
  layer_add_child(window_layer, text_layer_get_layer(text_layer));
}

static void window_unload(Window *window) {
  text_layer_destroy(text_layer);
  bitmap_layer_destroy(s_bitmaplayer_1);
  gbitmap_destroy(s_res_image_lock);	
}

static void init(void) {
  window = window_create();
	window_set_fullscreen(window, false);
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