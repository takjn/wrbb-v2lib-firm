/*
 * NeoPixel Library
 *
 * Copyright (c) 2016 Wakayama.rb Ruby Board developers
 *
 * This software is released under the MIT License.
 * https://github.com/wakayamarb/wrbb-v2lib-firm/blob/master/MITL
 *
 */
#include <Arduino.h>

#include <mruby.h>
#include <mruby/array.h>
#include <mruby/data.h>
#include <mruby/class.h>
#include <mruby/string.h>

#include <Wire.h>
#include <Adafruit_NeoPixel.h>

#include "../wrbb.h"

// NeoPixel
Adafruit_NeoPixel pixels = Adafruit_NeoPixel();

mrb_value mrb_neopixel_begin(mrb_state *mrb, mrb_value self)
{
	int pin;
	int num_pixels;

	mrb_get_args(mrb, "ii", &pin, &num_pixels);
	pixels.begin();
	pixels.setPin(pin);
	pixels.updateLength(num_pixels);
	pixels.updateType(NEO_GRB + NEO_KHZ800);
	return mrb_nil_value();
}

mrb_value mrb_neopixel_set_pixel_color(mrb_state *mrb, mrb_value self)
{
	int n, r, g, b;

	mrb_get_args(mrb, "iiii", &n, &r, &g, &b);
	pixels.setPixelColor(n, pixels.Color(r, g, b));

	return mrb_nil_value();
}

mrb_value mrb_neopixel_show(mrb_state *mrb, mrb_value self)
{
	pixels.show();
	return mrb_nil_value();
}

mrb_value mrb_neopixel_clear(mrb_state *mrb, mrb_value self)
{
	pixels.clear();
	return mrb_nil_value();
}

mrb_value mrb_neopixel_set_brightness(mrb_state *mrb, mrb_value self)
{
	int b;

	mrb_get_args(mrb, "i", &b);
	pixels.setBrightness(b);

	return mrb_nil_value();
}



void neopixel_Init(mrb_state* mrb) {

	struct RClass *neopixelModule = mrb_define_module(mrb, "NeoPixel");

	mrb_define_module_function(mrb, neopixelModule, "begin", mrb_neopixel_begin, MRB_ARGS_REQ(2));
	mrb_define_module_function(mrb, neopixelModule, "set_pixel_color", mrb_neopixel_set_pixel_color, MRB_ARGS_REQ(4));
	mrb_define_module_function(mrb, neopixelModule, "begin", mrb_neopixel_begin, MRB_ARGS_REQ(2));
	mrb_define_module_function(mrb, neopixelModule, "show", mrb_neopixel_show, MRB_ARGS_NONE());
	mrb_define_module_function(mrb, neopixelModule, "clear", mrb_neopixel_clear, MRB_ARGS_NONE());
	mrb_define_module_function(mrb, neopixelModule, "set_brightness", mrb_neopixel_set_brightness, MRB_ARGS_REQ(1));
}
