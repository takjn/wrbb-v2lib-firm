/*
 * SSD1306 OLED Library
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
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <acknow7pt.h>
#include <pixel_dingbats.h>

#include "../wrbb.h"

// OLED
Adafruit_SSD1306 display(-1);

mrb_value mrb_gfx_begin(mrb_state *mrb, mrb_value self)
{
	int i2c_addr, wire = 0;

	mrb_get_args(mrb, "i|i", &i2c_addr, &wire);
	display.begin(SSD1306_SWITCHCAPVCC, i2c_addr, false, wire);
	display.setTextColor(WHITE);
	display.setFont(&acknowtt7pt7b);

	return mrb_nil_value();
}

mrb_value mrb_gfx_display(mrb_state *mrb, mrb_value self)
{
	display.display();
	return mrb_nil_value();
}

mrb_value mrb_gfx_clearDisplay(mrb_state *mrb, mrb_value self)
{
	display.clearDisplay();
	return mrb_nil_value();
}

mrb_value mrb_gfx_setCursor(mrb_state *mrb, mrb_value self)
{
	int x, y;
	mrb_get_args(mrb, "ii", &x, &y);

	display.setCursor(x, y);
	return mrb_nil_value();
}

mrb_value mrb_gfx_drawLine(mrb_state *mrb, mrb_value self)
{
	int x1, y1, x2, y2;
	mrb_get_args(mrb, "iiii", &x1, &y1, &x2, &y2);

	display.drawLine(x1, y1, x2 , y2, WHITE);
	return mrb_nil_value();
}

mrb_value mrb_gfx_setTextSize(mrb_state *mrb, mrb_value self)
{
	int size;
	mrb_get_args(mrb, "i", &size);

	display.setTextSize(size);
	return mrb_nil_value();
}

mrb_value mrb_gfx_println(mrb_state *mrb, mrb_value self)
{
	mrb_value text;
	int n = mrb_get_args(mrb, "|S", &text);

	if(n > 0){
	  display.println(RSTRING_PTR(text));
	}
	return mrb_nil_value();
}

mrb_value mrb_gfx_print(mrb_state *mrb, mrb_value self)
{
	mrb_value text;
	int n = mrb_get_args(mrb, "|S", &text);

	if(n > 0){
	  display.print(RSTRING_PTR(text));
	}
	return mrb_nil_value();
}

mrb_value mrb_gfx_drawText(mrb_state *mrb, mrb_value self)
{
	int x, y;
	mrb_value text;
	int n = mrb_get_args(mrb, "iiS", &x, &y, &text);

	display.setCursor(x, y);
  display.print(RSTRING_PTR(text));

	return mrb_nil_value();
}

mrb_value mrb_gfx_setTextWrap(mrb_state *mrb, mrb_value self)
{
	mrb_bool flag;
	mrb_get_args(mrb, "b", &flag);

	display.setTextWrap(flag);

	return mrb_nil_value();
}

mrb_value mrb_gfx_resetFont(mrb_state *mrb, mrb_value self)
{
	display.setFont(&acknowtt7pt7b);
	return mrb_nil_value();
}

mrb_value mrb_gfx_useDingbatsFont(mrb_state *mrb, mrb_value self)
{
	display.setFont(&pixel_dingbats_78pt8b);
	return mrb_nil_value();
}

mrb_value mrb_gfx_drawRect(mrb_state *mrb, mrb_value self)
{
	int x, y, w, h;
	mrb_get_args(mrb, "iiii", &x, &y, &w, &h);

	display.drawRect(x, y, w , h, WHITE);
	return mrb_nil_value();
}

mrb_value mrb_gfx_fillRect(mrb_state *mrb, mrb_value self)
{
	int x, y, w, h;
	mrb_get_args(mrb, "iiii", &x, &y, &w, &h);

	display.fillRect(x, y, w , h, WHITE);
	return mrb_nil_value();
}

void ssd1306_Init(mrb_state* mrb) {

	struct RClass *ssd1306Module = mrb_define_module(mrb, "Ssd1306");

	mrb_define_module_function(mrb, ssd1306Module, "begin", mrb_gfx_begin, MRB_ARGS_ARG(1,1));
	mrb_define_module_function(mrb, ssd1306Module, "clear_display", mrb_gfx_clearDisplay, MRB_ARGS_NONE());
	mrb_define_module_function(mrb, ssd1306Module, "display", mrb_gfx_display, MRB_ARGS_NONE());
	mrb_define_module_function(mrb, ssd1306Module, "set_cursor", mrb_gfx_setCursor, MRB_ARGS_REQ(2));
	mrb_define_module_function(mrb, ssd1306Module, "draw_line", mrb_gfx_drawLine, MRB_ARGS_REQ(4));
	mrb_define_module_function(mrb, ssd1306Module, "set_text_size", mrb_gfx_setTextSize, MRB_ARGS_REQ(1));
	mrb_define_module_function(mrb, ssd1306Module, "print", mrb_gfx_print, MRB_ARGS_OPT(1));
	mrb_define_module_function(mrb, ssd1306Module, "println", mrb_gfx_println, MRB_ARGS_OPT(1));
	mrb_define_module_function(mrb, ssd1306Module, "draw_text", mrb_gfx_drawText, MRB_ARGS_REQ(3));
	mrb_define_module_function(mrb, ssd1306Module, "set_text_wrap", mrb_gfx_setTextWrap, MRB_ARGS_REQ(1));
	mrb_define_module_function(mrb, ssd1306Module, "reset_font", mrb_gfx_resetFont, MRB_ARGS_NONE());
	mrb_define_module_function(mrb, ssd1306Module, "use_dingbats_font", mrb_gfx_useDingbatsFont, MRB_ARGS_NONE());
	mrb_define_module_function(mrb, ssd1306Module, "draw_rect", mrb_gfx_drawRect, MRB_ARGS_REQ(4));
	mrb_define_module_function(mrb, ssd1306Module, "fill_rect", mrb_gfx_fillRect, MRB_ARGS_REQ(4));
}
