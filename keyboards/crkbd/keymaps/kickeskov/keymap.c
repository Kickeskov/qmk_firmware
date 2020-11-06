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

#include QMK_KEYBOARD_H


#ifdef RGBLIGHT_ENABLE
//Following line allows macro to read current RGB settings
extern rgblight_config_t rgblight_config;
#endif

extern uint8_t is_master;

// Each layer gets a name for readability, which is then used in the keymap matrix below.
// The underscores don't mean anything - you can have a layer called STUFF or any other name.
// Layer names don't all need to be of the same length, obviously, and you can also skip them
// entirely and just use numbers.
#define _QWERTY 0
#define _LOWER 1
#define _RAISE 2
#define _ADJUST 3
#define _GAME 4
#define _LGAME 5
#define _MEDIA 6
#define _RGAME 7

#define MDTAP ACTION_TAP_DANCE_DOUBLE(LOWER, MEDIA)

enum custom_keycodes {
  QWERTY = SAFE_RANGE,
  LOWER,
  RAISE,
  ADJUST,
  BACKLIT,
  RGBRST,
  GAME,
  LGAME,
  DEFAULT,
  MEDIA
};

enum macro_keycodes {
  KC_SAMPLEMACRO,
};

enum {
  TD_LOWER_MEDIA = 0,
  TD_GAMING_MOD
};

void dance_egg(qk_tap_dance_state_t *state, void *user_data) {
    if (state->count == 1) {
        SEND_STRING("Safety dance!");
        reset_tap_dance(state);
    }
}

void dance_tdmda_finished(qk_tap_dance_state_t *state, void *user_data) {
    if (state->count == 1) {
        layer_on(_LOWER);
    } else {
        layer_on(_MEDIA);
    }
}

void dance_tdmda_reset(qk_tap_dance_state_t *state, void *user_data) {
    if (state->count == 1) {
        layer_off(_LOWER);
    } else {
        layer_off(_MEDIA);
    }
}

void dance_game_finished(qk_tap_dance_state_t *state, void *user_data) {
    if (state->count == 1) {
        layer_on(_LGAME);
    } else if (state->count == 2) {
        layer_on(_RGAME);
    } else {
        layer_on(_MEDIA);
    }
}

void dance_game_reset(qk_tap_dance_state_t *state, void *user_data) {
    if (state->count == 1) {
        layer_off(_LGAME);
    } else if (state->count == 2) {
        layer_off(_RGAME);
    } else {
        layer_off(_MEDIA);
    }
}



qk_tap_dance_action_t tap_dance_actions[] = {
  [TD_LOWER_MEDIA] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, dance_tdmda_finished, dance_tdmda_reset),
  [TD_GAMING_MOD] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, dance_game_finished, dance_game_reset),
};

#define TDMDA TD(TD_LOWER_MEDIA)
#define GAMEL TD(TD_GAMING_MOD)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [_QWERTY] = LAYOUT_split_3x6_3( \
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
       KC_TAB,    KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,                         KC_Y,    KC_U,    KC_I,    KC_O,   KC_P,  KC_LBRC,\
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_LCTL,    KC_A,    KC_S,    KC_D,    KC_F,    KC_G,                         KC_H,    KC_J,    KC_K,    KC_L, KC_SCLN, KC_QUOT,\
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_LSFT,    KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,                         KC_N,    KC_M, KC_COMM,  KC_DOT, KC_SLSH, TDMDA,\
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          KC_LGUI,   TDMDA, KC_LALT,     KC_SPC,  RAISE,  KC_RALT \
                                      //`--------------------------'  `--------------------------'

  ),

  [_LOWER] = LAYOUT_split_3x6_3( \
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
       KC_GRV,    KC_1,    KC_2,    KC_3,    KC_4,    KC_5,                         KC_6,    KC_7,    KC_8,    KC_9,    KC_0, KC_BSPC,\
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_LALT, XXXXXXX, XXXXXXX,  KC_TAB,  KC_SPC,  KC_ESC,                      KC_LEFT, KC_DOWN,   KC_UP,KC_RIGHT, XXXXXXX, XXXXXXX,\
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_LGUI,    GAME, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                       KC_END, KC_HOME,KC_PGDOWN,  KC_PGUP, XXXXXXX, XXXXXXX,\
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          KC_LCTL,   LOWER, KC_LSFT,     KC_ENT,   RAISE, KC_RALT \
                                      //`--------------------------'  `--------------------------'
    ),

  [_RAISE] = LAYOUT_split_3x6_3( \
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
       KC_GRV, KC_EXLM,   KC_AT, KC_HASH,  KC_DLR, KC_PERC,                      KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, KC_BSPC,\
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_LCTL, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      KC_MINS,  KC_EQL, KC_LCBR, KC_RCBR, KC_PIPE,  KC_ENT,\
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_LSFT, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      KC_UNDS, KC_PLUS, KC_LBRC, KC_RBRC, KC_BSLS, KC_TILD,\
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          KC_LGUI,   LOWER, KC_LALT,     KC_ENT,   RAISE, KC_RALT \
                                      //`--------------------------'  `--------------------------'
  ),

  [_ADJUST] = LAYOUT_split_3x6_3( \
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
        RESET,  RGBRST, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                        KC_F1,   KC_F2,   KC_F3,   KC_F4, XXXXXXX, KC_BSPC,\
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      RGB_TOG, RGB_HUI, RGB_SAI, RGB_VAI, XXXXXXX, XXXXXXX,                        KC_F5,   KC_F6,   KC_F7,   KC_F8,    GAME, XXXXXXX,\
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      RGB_MOD, RGB_HUD, RGB_SAD, RGB_VAD, XXXXXXX, XXXXXXX,                        KC_F9,  KC_F10,  KC_F11,  KC_F12, XXXXXXX, XXXXXXX,\
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          KC_LGUI,   TDMDA, KC_LALT,     KC_ENT,   RAISE, KC_RALT \
                                      //`--------------------------'  `--------------------------'
  ),

  [_GAME] = LAYOUT_split_3x6_3( \
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
       KC_TAB,    KC_1,    KC_2,    KC_3,    KC_4,    KC_5,                         KC_Y,    KC_U,    KC_I,    KC_O,   KC_P,  KC_LBRC,\
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_LCTL,    KC_T,    KC_Q,    KC_W,    KC_E,    KC_R,                         KC_H,    KC_J,    KC_K,    KC_L, KC_SCLN, KC_QUOT,\
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_LSFT,    KC_C,    KC_A,    KC_S,    KC_D,    KC_F,                         KC_N,    KC_M, KC_COMM,  KC_DOT, KC_SLSH, KC_F9,\
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          KC_LALT,   GAMEL,  KC_SPC,    DEFAULT, XXXXXXX, XXXXXXX \
                                      //`--------------------------'  `--------------------------'
  ),

  [_LGAME] = LAYOUT_split_3x6_3( \
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
       KC_GRV,   KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,                         KC_Y,    KC_U,    KC_I,    KC_O,   KC_P,  KC_LBRC,\
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_LCTL,    KC_X,    KC_6,    KC_W,    KC_7,  KC_ESC,                         KC_H,    KC_J,    KC_K,    KC_L, KC_SCLN, KC_QUOT,\
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_LSFT,    KC_Z,    KC_A,    KC_S,    KC_D,    KC_V,                         KC_N,    KC_M, KC_COMM,  KC_DOT, KC_SLSH, KC_RSFT,\
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          KC_LALT,   GAMEL,  KC_SPC,    DEFAULT, XXXXXXX, XXXXXXX \
                                      //`--------------------------'  `--------------------------'
  ),

  [_MEDIA] = LAYOUT_split_3x6_3( \
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
      XXXXXXX, XXXXXXX, KC_MPLY, KC_MNXT, KC_MPRV, DEFAULT,                      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,\
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      XXXXXXX, XXXXXXX, KC_VOLD, KC_VOLU, XXXXXXX,    GAME,                      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,\
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,\
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          KC_LGUI,   LOWER,  KC_SPC,     KC_ENT,   RAISE, KC_RALT \
                                      //`--------------------------'  `--------------------------'
  ),

  [_RGAME] = LAYOUT_split_3x6_3( \
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
         KC_B,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,                         KC_Y,    KC_U,    KC_I,    KC_O,   KC_P,  KC_LBRC,\
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_LCTL,    KC_H,    KC_J,    KC_K,    KC_L, KC_SCLN,                         KC_H,    KC_J,    KC_K,    KC_L, KC_SCLN, KC_QUOT,\
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_LSFT,    KC_N,    KC_M,    KC_G, KC_COMM,    KC_B,                         KC_N,    KC_M, KC_COMM,  KC_DOT, KC_SLSH, KC_RSFT,\
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          KC_LALT,   GAMEL,  KC_SPC,    DEFAULT, XXXXXXX, XXXXXXX \
                                      //`--------------------------'  `--------------------------'
  ),


};

int RGB_current_mode;

void persistent_default_layer_set(uint16_t default_layer) {
  eeconfig_update_default_layer(default_layer);
  default_layer_set(default_layer);
}

// Setting ADJUST layer RGB back to default
void update_tri_layer_RGB(uint8_t layer1, uint8_t layer2, uint8_t layer3) {
  if (IS_LAYER_ON(layer1) && IS_LAYER_ON(layer2)) {
    layer_on(layer3);
  } else {
    layer_off(layer3);
  }
}

void matrix_init_user(void) {
    #ifdef RGBLIGHT_ENABLE
      RGB_current_mode = rgblight_config.mode;
    #endif
    //SSD1306 OLED init, make sure to add #define SSD1306OLED in config.h
    #ifdef SSD1306OLED
        iota_gfx_init(!has_usb());   // turns on the display
    #endif
}

//SSD1306 OLED update loop, make sure to add #define SSD1306OLED in config.h
#ifdef SSD1306OLED

// When add source files to SRC in rules.mk, you can use functions.
const char *read_layer_state(void);
const char *read_logo(void);
void set_keylog(uint16_t keycode, keyrecord_t *record);
const char *read_keylog(void);
const char *read_keylogs(void);

// const char *read_mode_icon(bool swap);
// const char *read_host_led_state(void);
// void set_timelog(void);
// const char *read_timelog(void);

void matrix_scan_user(void) {
   iota_gfx_task();
}

void matrix_render_user(struct CharacterMatrix *matrix) {
  if (is_master) {
    // If you want to change the display of OLED, you need to change here
    matrix_write_ln(matrix, read_layer_state());
    matrix_write_ln(matrix, read_keylog());
    //matrix_write_ln(matrix, read_keylogs());
    //matrix_write_ln(matrix, read_mode_icon(keymap_config.swap_lalt_lgui));
    //matrix_write_ln(matrix, read_host_led_state());
    //matrix_write_ln(matrix, read_timelog());
  } else {
    matrix_write(matrix, read_logo());
  }
}

void matrix_update(struct CharacterMatrix *dest, const struct CharacterMatrix *source) {
  if (memcmp(dest->display, source->display, sizeof(dest->display))) {
    memcpy(dest->display, source->display, sizeof(dest->display));
    dest->dirty = true;
  }
}

void iota_gfx_task_user(void) {
  struct CharacterMatrix matrix;
  matrix_clear(&matrix);
  matrix_render_user(&matrix);
  matrix_update(&display, &matrix);
}
#endif//SSD1306OLED

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  if (record->event.pressed) {
#ifdef SSD1306OLED
    set_keylog(keycode, record);
#endif
    // set_timelog();
  }

  switch (keycode) {
    case QWERTY:
      if (record->event.pressed) {
        persistent_default_layer_set(1UL<<_QWERTY);
      }
      return false;
    case LOWER:
      if (record->event.pressed) {
        layer_on(_LOWER);
        update_tri_layer_RGB(_LOWER, _RAISE, _ADJUST);
      } else {
        layer_off(_LOWER);
        update_tri_layer_RGB(_LOWER, _RAISE, _ADJUST);
      }
      return false;
    case RAISE:
      if (record->event.pressed) {
        layer_on(_RAISE);
        update_tri_layer_RGB(_LOWER, _RAISE, _ADJUST);
      } else {
        layer_off(_RAISE);
        update_tri_layer_RGB(_LOWER, _RAISE, _ADJUST);
      }
      return false;
    case ADJUST:
        if (record->event.pressed) {
          layer_on(_ADJUST);
        } else {
          layer_off(_ADJUST);
        }
        return false;
    // KICKESKOV CUSTOM START
    case GAME:
        if (record->event.pressed) {
          layer_move(_GAME);
        }
        return false;
    case DEFAULT:
        // I don't understand the case: QWERTY so I do this instead when resetting to default layer lol
        if (record->event.pressed) {
          layer_move(_QWERTY);
        }
        return false;
    case LGAME:
        if (record->event.pressed) {
          layer_on(_LGAME);
        } else {
          layer_off(_LGAME);
        }
        return false;
    case MEDIA:
        if (record->event.pressed) {
          layer_on(_MEDIA);
        } else {
          layer_off(_MEDIA);
        }
        return false;
    // KICKESKOV CUSTOM END
    case RGB_MOD:
      #ifdef RGBLIGHT_ENABLE
        if (record->event.pressed) {
          rgblight_mode(RGB_current_mode);
          rgblight_step();
          RGB_current_mode = rgblight_config.mode;
        }
      #endif
      return false;
    case RGBRST:
      #ifdef RGBLIGHT_ENABLE
        if (record->event.pressed) {
          eeconfig_update_rgblight_default();
          rgblight_enable();
          RGB_current_mode = rgblight_config.mode;
        }
      #endif
      break;
  }
  return true;
}