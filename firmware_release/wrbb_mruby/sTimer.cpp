/*
 * MsTimer2関連
 *
 * Copyright (c) 2018 Wakayama.rb Ruby Board developers
 *
 * This software is released under the MIT License.
 * https://github.com/wakayamarb/wrbb-v2lib-firm/blob/master/MITL
 *
 */
#include <Arduino.h>
#include <MsTimer2.h>

#include <mruby.h>
#include <mruby/string.h>
#include <mruby/array.h>
#include <mruby/data.h>
#include <mruby/variable.h>


typedef struct eh_ctx_t {
  mrb_state *mrb;
  mrb_value timer_block;
} eh_ctx_t;

eh_ctx_t ehc;

void
timer_handler() {
    int arena_index = mrb_gc_arena_save(ehc.mrb);

    if (!mrb_nil_p(ehc.timer_block)) {
        mrb_assert(mrb_type(ehc.timer_block) == MRB_TT_PROC);
        mrb_yield_argv(ehc.mrb, ehc.timer_block, 0, NULL);
    }

    mrb_gc_arena_restore(ehc.mrb, arena_index);
}

mrb_value
mrb_timer_set(mrb_state *mrb, mrb_value self) {
    int ms;
    mrb_value block;

    mrb_get_args(mrb, "i&", &ms, &block);
    if (mrb_nil_p(block)) {
        mrb_raise(mrb, E_ARGUMENT_ERROR, "no block given");
    }

    mrb_iv_set(mrb, self, mrb_intern_lit(mrb, "@timer_block"), block);
    ehc.timer_block = block;

    MsTimer2::set(ms, timer_handler);

    return self;
}

mrb_value
mrb_timer_start(mrb_state *mrb, mrb_value self) {

    MsTimer2::start();

    return mrb_nil_value();
}

mrb_value
mrb_timer_stop(mrb_state *mrb, mrb_value self) {

    MsTimer2::stop();

    return mrb_nil_value();
}

void timer_Init(mrb_state *mrb)
{
    ehc.mrb = mrb;
    ehc.timer_block = mrb_nil_value();

    struct RClass *timerModule = mrb_define_module(mrb, "Timer");
    mrb_define_module_function(mrb, timerModule, "set", mrb_timer_set, MRB_ARGS_REQ(1) | MRB_ARGS_BLOCK());
    mrb_define_module_function(mrb, timerModule, "start", mrb_timer_start, MRB_ARGS_NONE());
    mrb_define_module_function(mrb, timerModule, "stop", mrb_timer_stop, MRB_ARGS_NONE());
}
