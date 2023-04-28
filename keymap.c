/*
Copyright 2019 @foostan
Copyright 2020 Drashna Jaelre <@drashna>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifdef PIMORONI_TRACKBALL_ENABLE
#include "drivers/sensors/pimoroni_trackball.h"
#endif

#include QMK_KEYBOARD_H
#include <string.h>
#include "lib/lib8tion/lib8tion.h"

#define ENT_CTL LCTL_T(KC_ENT)        // enter / left ctrl
#define SPC_CTL RCTL_T(KC_SPC)

/* home row mods */
#define A_GUI LGUI_T(KC_A)
#define R_ALT LALT_T(KC_R)
#define S_CTL LCTL_T(KC_S)

#define O_GUI LGUI_T(KC_O)
#define I_ALT LALT_T(KC_I)
#define E_CTL LCTL_T(KC_E)

/* corner shifts */
#define Z_SFT LSFT_T(KC_Z)
#define EQL_SFT LSFT_T(KC_EQL)
#define BS_SFT RSFT_T(KC_BSLS)
#define SL_SFT RSFT_T(KC_SLSH)

/* layer taps */
#define L1_BSPC LT(1, KC_BSPC)
#define L3_TAB LT(3, KC_TAB)
#define L4_ESC LT(4, KC_ESC)

enum custom_keycodes {
  DRG_SCR = SAFE_RANGE,
  LR_PRN,
  LR_CBR,
  LR_BRC
};

#ifdef POINTING_DEVICE_ENABLE
static bool           set_scrolling = true;
static uint32_t       last_mouse_activity = 0;
static report_mouse_t last_mouse_report   = {0};
#endif

enum combo_events {
  JL_EQL,
  LU_MINS,
  UY_SCLN,
  YQ_GRV,
  EM_EMAIL,
  HK_MB1,
  JH_MB2,
  H_BTN1,
  H_BTN2,
  H_BTN3,
  MIDDLE_CLICK,
  MOUSE_LAYER,
  CM_COPY,
  CM_PASTE,
  CM_CUT,
  CM_ALL,
  COMBO_LENGTH,
};

uint16_t COMBO_LEN = COMBO_LENGTH;

const uint16_t PROGMEM jl_eql[] = {KC_J, KC_L, COMBO_END};
const uint16_t PROGMEM lu_mins[] = {KC_L, KC_U, COMBO_END};
const uint16_t PROGMEM uy_scln[] = {KC_U, KC_Y, COMBO_END};
const uint16_t PROGMEM yq_grv[] = {KC_Y, KC_QUOT, COMBO_END};

// const uint16_t PROGMEM email_combo[] = {E_CTL, KC_M, COMBO_END};

const uint16_t PROGMEM hk_mb1[] = {KC_H, KC_K, COMBO_END};
const uint16_t PROGMEM jh_mb2[] = {KC_H, KC_J, COMBO_END};

const uint16_t PROGMEM h_btn1[] = {KC_H, KC_N, COMBO_END};
const uint16_t PROGMEM h_btn2[] = {KC_N, E_CTL, COMBO_END};
const uint16_t PROGMEM h_btn3[] = {KC_H, KC_N, E_CTL, COMBO_END};
const uint16_t PROGMEM middle_click[] = {KC_BTN1, KC_BTN2, COMBO_END};
const uint16_t PROGMEM mouse_layer[] = {MO(1), MO(2), COMBO_END};

const uint16_t PROGMEM cm_copy[] = {ENT_CTL, KC_C, COMBO_END};
const uint16_t PROGMEM cm_paste[] = {ENT_CTL, KC_V, COMBO_END};
const uint16_t PROGMEM cm_cut[] = {ENT_CTL, KC_X, COMBO_END};
const uint16_t PROGMEM cm_all[] = {ENT_CTL, A_GUI, COMBO_END};

combo_t key_combos[] = {
  [JL_EQL] = COMBO(jl_eql, KC_EQL),
  [LU_MINS] = COMBO(lu_mins, KC_MINS),
  [UY_SCLN] = COMBO(uy_scln, KC_SCLN),
  [YQ_GRV] = COMBO(yq_grv, KC_GRV),
  // [EM_EMAIL] = COMBO_ACTION(email_combo),
  [HK_MB1] = COMBO(hk_mb1, KC_BTN1),
  [JH_MB2] = COMBO(jh_mb2, KC_BTN2),
  [H_BTN1] = COMBO(h_btn1, KC_BTN1),
  [H_BTN2] = COMBO(h_btn2, KC_BTN2),
  [H_BTN3] = COMBO(h_btn3, KC_BTN3),
  [MIDDLE_CLICK] = COMBO(middle_click, KC_BTN3),
  [CM_COPY] = COMBO(cm_copy, LCTL(KC_C)),
  [CM_PASTE] = COMBO(cm_paste, LCTL(KC_V)),
  [CM_CUT] = COMBO(cm_cut, LCTL(KC_X)),
  [CM_ALL] = COMBO(cm_all, LCTL(KC_A)),
  [MOUSE_LAYER] = COMBO_ACTION(mouse_layer),
};

void process_combo_event(uint16_t combo_index, bool pressed) {
  switch(combo_index) {
  // case EM_EMAIL:
  //   if (pressed) {
  //     SEND_STRING("lee@imre.al");
  //   }
  //   break;
  case MOUSE_LAYER:
    if (pressed) {
      if (IS_LAYER_ON(0)) {
        layer_move(5);
      } else {
        layer_move(0);
      }
    }
    break;
  }
}

bool process_combo_key_release(uint16_t combo_index, combo_t *combo, uint8_t key_index, uint16_t keycode) {
  switch (combo_index) {
  case H_BTN1:
    switch (keycode) {
    case KC_H:
      unregister_code(KC_BTN1);
      break;
    }
    return false;
  case H_BTN2:
    switch (keycode) {
    case KC_H:
    case E_CTL:
      unregister_code(KC_BTN2);
      break;
    }
    return false;
  case H_BTN3:
  case MIDDLE_CLICK:
    switch (keycode) {
    case KC_H:
    case KC_N:
    case E_CTL:
    case KC_BTN1:
    case KC_BTN2:
      unregister_code(KC_BTN3);
      break;
    }
    return false;
  }
  return false;
}

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [0] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
      XXXXXXX,    KC_Q,    KC_W,    KC_F,    KC_P,    KC_G,                         KC_J,    KC_L,    KC_U,    KC_Y, KC_QUOT, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      XXXXXXX,   A_GUI,   R_ALT,   S_CTL,    KC_T,    KC_D,                         KC_H,    KC_N,   E_CTL,   I_ALT,   O_GUI, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      XXXXXXX,   Z_SFT,    KC_X,    KC_C,    KC_V,    KC_B,                         KC_K,    KC_M, KC_COMM,  KC_DOT,  SL_SFT, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                           KC_DEL, L1_BSPC, ENT_CTL,    SPC_CTL,  L3_TAB,  L4_ESC
                                      //`--------------------------'  `--------------------------'
    ),

  [1] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
      _______, _______, _______, _______, _______, _______,                       KC_DLR, KC_HOME, KC_PGUP, KC_PGDN,  KC_END, _______,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      _______, KC_LGUI, KC_LALT, KC_LCTL, _______, _______,                      KC_SCLN, KC_LEFT,   KC_UP, KC_DOWN, KC_RGHT, _______,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      _______, EQL_SFT, _______, _______, _______, _______,                      KC_MINS,  KC_EQL, _______, _______,  BS_SFT, _______,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          _______, _______, _______,    _______, _______, _______
                                      //`--------------------------'  `--------------------------'
    ),

  [2] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
      _______, _______, _______, _______, _______, _______,                      _______, _______, _______, _______, _______, _______,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      _______, _______, _______, _______, _______, _______,                      _______, _______, _______, _______, _______, _______,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      _______, _______, _______, _______, _______, _______,                      _______, _______, _______, _______, _______, _______,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          _______, _______, _______,    _______, _______, _______
                                      //`--------------------------'  `--------------------------'
    ),

  [3] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
      _______,  KC_DLR,    KC_7,    KC_8,    KC_9, KC_LBRC,                       KC_DLR,  KC_EQL, KC_MINS, KC_SCLN,  KC_GRV, _______,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      _______, KC_PIPE,    KC_4,    KC_5,    KC_6, KC_LPRN,                      KC_SCLN, _______, KC_LCTL, KC_LALT, KC_LGUI, _______,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      _______, EQL_SFT,    KC_1,    KC_2,    KC_3, KC_LCBR,                      KC_MINS, _______, _______, _______,  BS_SFT, _______,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          _______,    KC_0, _______,    _______, _______,   MO(5)
                                      //`--------------------------'  `--------------------------'
    ),

  [4] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
      _______, _______,   KC_F7,   KC_F8,   KC_F9, _______,                      _______, _______, _______, _______, _______, _______,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      _______, _______,   KC_F4,   KC_F5,   KC_F6, _______,                      _______, _______, KC_LCTL, KC_LALT, KC_LGUI, _______,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      _______, _______,   KC_F1,   KC_F2,   KC_F3, _______,                      _______, _______, _______, _______, KC_LSFT, _______,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          _______,  KC_F10, _______,    _______,   MO(5), _______
                                      //`--------------------------'  `--------------------------'
    ),

  [5] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
      _______, KC_WH_L, KC_WH_U, KC_WH_D, KC_WH_R, KC_VOLU,                      _______, _______, _______, _______, _______,DRG_SCR,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      _______, KC_MS_L, KC_MS_U, KC_MS_D, KC_MS_R, KC_VOLD,                      _______, KC_BTN1, KC_BTN2, _______, _______, _______,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      _______, _______, _______, _______, _______, KC_MUTE,                      _______, _______, _______, _______, _______, _______,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          _______, _______, _______,    _______, _______, _______
                                      //`--------------------------'  `--------------------------'
  )
};
 
uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
  case SPC_CTL:
  case A_GUI:
    return TAPPING_TERM + 35;
  case O_GUI:
    return TAPPING_TERM + 20;
  case EQL_SFT:
  case SL_SFT:
  case BS_SFT:
  case L1_BSPC:
    return TAPPING_TERM - 30;
  case L3_TAB:
    return TAPPING_TERM - 55;
  case Z_SFT:
    return TAPPING_TERM - 75;
  default:
    return TAPPING_TERM;
  }
}

uint16_t get_combo_term(uint16_t index, combo_t *combo) {
  switch (index) {
  case UY_SCLN:
  case LU_MINS:
  case H_BTN3:
    return COMBO_TERM - 20;
  case H_BTN2:
    return COMBO_TERM - 35;
  case H_BTN1:
  case MOUSE_LAYER:
    return COMBO_TERM - 40;
  }
  return COMBO_TERM;
}

// shift key overrides
const key_override_t delete_key_override = ko_make_with_layers(MOD_MASK_SHIFT, KC_BSPC, KC_DEL, 1<<0);
const key_override_t lbrc_key_override = ko_make_basic(MOD_MASK_SHIFT, KC_LBRC, KC_RBRC);
const key_override_t lprn_key_override = ko_make_basic(MOD_MASK_SHIFT, KC_LPRN, KC_RPRN);
const key_override_t lcbr_key_override = ko_make_basic(MOD_MASK_SHIFT, KC_LCBR, KC_RCBR);

// This globally defines all key overrides to be used
const key_override_t **key_overrides = (const key_override_t *[]){
    &delete_key_override,
    &lbrc_key_override,
    &lprn_key_override,
    &lcbr_key_override,
    NULL // Null terminate the array of overrides!
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
#ifdef POINTING_DEVICE_ENABLE
  case DRG_SCR:
    if (record->event.pressed) {
      set_scrolling = !set_scrolling;
    }
    break;
#endif
  case LR_PRN:
    if (record->event.pressed) {
      tap_code16(KC_RPRN);
      tap_code16(KC_LEFT);
      tap_code16(KC_LPRN);
    }
    break;
  case LR_BRC:
    if (record->event.pressed) {
      tap_code16(KC_RBRC);
      tap_code16(KC_LEFT);
      tap_code16(KC_LBRC);
    }
    break;
  case LR_CBR:
    if (record->event.pressed) {
      tap_code16(KC_RCBR);
      tap_code16(KC_LEFT);
      tap_code16(KC_LCBR);
    }
    break;
  }
  return true;
}

#ifdef POINTING_DEVICE_ENABLE
report_mouse_t smooth_mouse_movement(report_mouse_t mouse_report) {

  static fract8 fract = 0.5;
  int8_t        x     = 0;
  int8_t        y     = 0;
  int8_t        h     = 0;
  int8_t        v     = 0;

  if (set_scrolling) {
    pointing_device_set_cpi(3000);
    h = ease8InOutApprox(lerp8by8(last_mouse_report.x, mouse_report.x, fract));
    v = ease8InOutApprox(lerp8by8(last_mouse_report.y, mouse_report.y, fract));
  } else {
    pointing_device_set_cpi(8000);
    x = ease8InOutApprox(lerp8by8(last_mouse_report.x, mouse_report.x, fract));
    y = ease8InOutApprox(lerp8by8(last_mouse_report.y, mouse_report.y, fract));
  }

  // update the new smoothed report
  mouse_report.x = 2.5*x;
  mouse_report.y = 2.5*y;
  mouse_report.h = h;
  mouse_report.v = -v;

  return mouse_report;
}

layer_state_t layer_state_set_user(layer_state_t state) {
  switch (get_highest_layer(state)) {
  case 1:
    pimoroni_trackball_set_rgbw(235,122,231,0);
    break;
  case 2:
    pimoroni_trackball_set_rgbw(101,255,95,0);
    break;
  case 3:
    pimoroni_trackball_set_rgbw(170,0,254,0);
    break;
  case 4:
    pimoroni_trackball_set_rgbw(0,200,0,0);
    break;
  case 5:
    pimoroni_trackball_set_rgbw(30,200,254,0);
    break;
  default: //  for any other layers, or the default layer
    pimoroni_trackball_set_rgbw(255,255,255,0);
    break;
  }
  return state;
}


report_mouse_t pointing_device_task_user(report_mouse_t mouse_report) {
  if (has_mouse_report_changed(&last_mouse_report, &mouse_report)) {
    last_mouse_activity = timer_read32();
    memcpy(&last_mouse_report, &mouse_report, sizeof(mouse_report));
  }
  return smooth_mouse_movement(mouse_report);
}
#endif
