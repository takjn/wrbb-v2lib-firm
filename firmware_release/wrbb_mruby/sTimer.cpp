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
#include <mruby/data.h>
#include <mruby/variable.h>

typedef struct {
  mrb_state *mrb;
  mrb_value block;
} mrb_event_handler;

mrb_event_handler eh;

void
timer_handler() {
    int arena_index = mrb_gc_arena_save(eh.mrb);

    if (!mrb_nil_p(eh.block)) {
        mrb_assert(mrb_type(eh.block) == MRB_TT_PROC);
        mrb_yield_argv(eh.mrb, eh.block, 0, NULL);
    }

    mrb_gc_arena_restore(eh.mrb, arena_index);
}

//**************************************************
// 指定した周期間隔(ms)で処理するブロックを登録します: Timer.set
// Timer.set(ms) block
// ms: 周期(ms)
// block: 処理するブロック
//**************************************************
mrb_value
mrb_timer_set(mrb_state *mrb, mrb_value self) {
    int ms;
    mrb_value block;

    mrb_get_args(mrb, "i&", &ms, &block);
    if (mrb_nil_p(block)) {
        mrb_raise(mrb, E_ARGUMENT_ERROR, "no block given");
    }

    mrb_iv_set(mrb, self, mrb_intern_lit(mrb, "@event_handler_block"), block);
    eh.block = block;

    MsTimer2::set(ms, timer_handler);

    return mrb_nil_value();
}

//**************************************************
// setしたタイマーをスタートします: Timer.start
// Timer.start()
//**************************************************
mrb_value
mrb_timer_start(mrb_state *mrb, mrb_value self) {

    MsTimer2::start();

    return mrb_nil_value();
}

//**************************************************
// タイマーをストップします: Timer.stop
// Timer.stop()
//**************************************************
mrb_value
mrb_timer_stop(mrb_state *mrb, mrb_value self) {

    MsTimer2::stop();

    return mrb_nil_value();
}

//**************************************************
// ライブラリを定義します
//**************************************************
void
timer_Init(mrb_state *mrb) {
    eh.mrb = mrb;
    eh.block = mrb_nil_value();

    struct RClass *timerModule = mrb_define_module(mrb, "Timer");
    mrb_define_module_function(mrb, timerModule, "set", mrb_timer_set, MRB_ARGS_REQ(1) | MRB_ARGS_BLOCK());
    mrb_define_module_function(mrb, timerModule, "start", mrb_timer_start, MRB_ARGS_NONE());
    mrb_define_module_function(mrb, timerModule, "stop", mrb_timer_stop, MRB_ARGS_NONE());
}

//**************************************************
// ファイナライズをします
//**************************************************
void
timer_final(mrb_state *mrb) {
    MsTimer2::stop();
}