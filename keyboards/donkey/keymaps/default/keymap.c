#include QMK_KEYBOARD_H
#include "debug.h"
#include "action_layer.h"
#include "version.h"

#define BASE 0 // default layer
#define NORMAN 1 // symbols
#define FUN 2 // symbols

enum custom_keycodes {
    PLACEHOLDER = SAFE_RANGE, // can always be here
    SHRUGGIE
};


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
/* Keymap 0: Basic layer
 *
 * ,--------------------------------------------------.           ,--------------------------------------------------.
 * |   =    |   1  |   2  |   3  |   4  |   5  | LEFT |           | RIGHT|   6  |   7  |   8  |   9  |   0  |   -    |
 * |--------+------+------+------+------+-------------|           |------+------+------+------+------+------+--------|
 * | Del    |   Q  |   W  |   E  |   R  |   T  |  L1  |           |  L1  |   Y  |   U  |   I  |   O  |   P  |   \    |
 * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * | BkSp   |   A  |   S  |   D  |   F  |   G  |------|           |------|   H  |   J  |   K  |   L  |; / L2|' / Cmd |
 * |--------+------+------+------+------+------| Hyper|           | Meh  |------+------+------+------+------+--------|
 * | LShift |Z/Ctrl|   X  |   C  |   V  |   B  |      |           |      |   N  |   M  |   ,  |   .  |//Ctrl| RShift |
 * `--------+------+------+------+------+-------------'           `-------------+------+------+------+------+--------'
 *   |Grv/L1|  '"  |AltShf| Left | Right|                                       |  Up  | Down |   [  |   ]  | ~L1  |
 *   `----------------------------------'                                       `----------------------------------'
 *                                        ,-------------.       ,-------------.
 *                                        | App  | LGui |       | Alt  |Ctrl/Esc|
 *                                 ,------|------|------|       |------+--------+------.
 *                                 |      |      | Home |       | PgUp |        |      |
 *                                 | Space|Backsp|------|       |------|  Tab   |Enter |
 *                                 |      |ace   | End  |       | PgDn |        |      |
 *                                 `--------------------'       `----------------------'
 */
// If it accepts an argument (i.e, is a function), it doesn't need KC_.
// Otherwise, it needs KC_*
    [BASE] = LAYOUT_donkey(  // layer 0 : default
        // left hand
        KC_EQL,     KC_1,        KC_2,          KC_3,    KC_4,     KC_5,            DF(NORMAN),
        KC_GRAVE,      KC_Q,        KC_W,          KC_E,    KC_R,     KC_T,            LSFT(KC_TAB),
        MO(FUN),      KC_A,        KC_S,          KC_D,    KC_F,     KC_G,            KC_TAB,
        KC_HOME,    KC_Z,        KC_X,          KC_C,    KC_V,     KC_B,            LGUI_T(KC_ESC),
        MO(FUN),    KC_NO,    KC_NO,      KC_NO, KC_LCTRL, LALT_T(KC_BSPACE), KC_LSFT,

        // right hand

        KC_PGUP,          KC_6,             KC_7,    KC_8,    KC_9,    KC_0,       KC_MINS,
        KC_PGDN,       KC_Y,             KC_U,    KC_I,    KC_O,    KC_P,       KC_BSLASH,
        KC_BSPC,          KC_H,             KC_J,    KC_K,    KC_L,    KC_SCLN,    KC_QUOT,
        LGUI_T(KC_TAB),           KC_N,             KC_M,    KC_COMM, KC_DOT,  KC_SLSH,    KC_END,
        KC_RSFT, KC_ENTER, KC_SPACE, KC_NO, KC_LBRC, KC_RBRC,    MO(FUN)
        ),
// NORMAN LAYOUT
    [NORMAN] = LAYOUT_donkey(
        // left hand
        _______,     _______,    _______,      _______, _______, _______, DF(BASE),
        _______,     KC_Q,       KC_W,         KC_D,    KC_F,    KC_K,    _______,
        _______,     KC_A,       KC_S,         KC_E,    KC_T,    KC_G,    _______,
        _______,     KC_Z,       KC_X,         KC_C,    KC_V,    KC_B,    _______,
        _______,     _______,    _______,      _______, _______, _______, _______,

        // right hand
        _______,  _______, _______, _______, _______, _______,  _______,
        _______,  KC_J,    KC_U,    KC_R,    KC_L,    KC_SCLN,  KC_BSLASH,
        _______,  KC_Y,    KC_N,    KC_I,    KC_O,    KC_H,     KC_QUOT,
        _______,  KC_P,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,  _______,
        _______,  _______, _______, _______, _______, _______,  _______
        ),
    [FUN] = LAYOUT_donkey(
        // left hand
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
        _______, LCA(KC_L), KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
        _______, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,

        // right hand
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, MAGIC_SWAP_ALT_GUI,
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, MAGIC_UNSWAP_ALT_GUI,
        KC_NO, KC_LEFT, KC_DOWN, KC_UP, KC_RIGHT, KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO, KC_VOLD, KC_VOLU, KC_MUTE, KC_NO, _______
        ),
/*     /\*    _______, _______, _______, _______, _______, _______, _______, *\/ */
/*     /\*    _______, _______, _______, KC_MS_U, _______, _______, _______, *\/ */
/*     /\*    _______, _______, KC_MS_L, KC_MS_D, KC_MS_R, _______, *\/ */
/*     /\*    _______, _______, _______, _______, _______, _______, _______, *\/ */
/*     /\*    _______, _______, _______, KC_BTN1, KC_BTN2, *\/ */
/*     /\*                                        _______, _______, *\/ */
/*     /\*                                                 _______, *\/ */
/*     /\*                               _______, _______, _______, *\/ */
/*     /\* // right hand *\/ */
/*     /\*    _______,  _______, _______, _______, _______, _______, _______, *\/ */
/*     /\*    _______,  _______, _______, _______, _______, _______, _______, *\/ */
/*     /\*              _______, _______, _______, _______, _______, KC_MPLY, *\/ */
/*     /\*    _______,  _______, _______, KC_MPRV, KC_MNXT, _______, _______, *\/ */
/*     /\*                       KC_VOLU, KC_VOLD, KC_MUTE, _______, _______, *\/ */
/*     /\*    _______, _______, *\/ */
/*     /\*    _______, *\/ */
/*     /\*    _______, _______, KC_WBAK *\/ */
};

/* const uint16_t PROGMEM fn_actions[] = { */
/*     [1] = ACTION_LAYER_TAP_TOGGLE(SYMB)                // FN1 - Momentary Layer 1 (Symbols) */
/* }; */

/* const macro_t *action_get_macro(keyrecord_t *record, uint8_t id, uint8_t opt) */
/* { */
/*   // MACRODOWN only works in this function */
/*       switch(id) { */
/*         case 0: */
/*         if (record->event.pressed) { */
/*           SEND_STRING (QMK_KEYBOARD "/" QMK_KEYMAP " @ " QMK_VERSION); */
/*         } */
/*         break; */
/*         case 1: */
/*         if (record->event.pressed) { // For resetting EEPROM */
/*           eeconfig_init(); */
/*         } */
/*         break; */
/*       } */
/*     return MACRO_NONE; */
/* }; */

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    return true;
};

/* bool process_record_user(uint16_t keycode, keyrecord_t *record) { */
/*   switch (keycode) { */
/*     // dynamically generate these. */
/*     case EPRM: */
/*       if (record->event.pressed) { */
/*         eeconfig_init(); */
/*       } */
/*       return false; */
/*       break; */
/*     case VRSN: */
/*       if (record->event.pressed) { */
/*         SEND_STRING (QMK_KEYBOARD "/" QMK_KEYMAP " @ " QMK_VERSION); */
/*       } */
/*       return false; */
/*       break; */
/*     case RGB_SLD: */
/*       if (record->event.pressed) { */
/*         #ifdef RGBLIGHT_ENABLE */
/*           rgblight_mode(1); */
/*         #endif */
/*       } */
/*       return false; */
/*       break; */
/*   } */
/*   return true; */
/* } */

// Runs just one time when the keyboard initializes.
void matrix_init_user(void) {

};


// Runs constantly in the background, in a loop.
void matrix_scan_user(void) {

/* uint8_t layer = biton32(layer_state); */

/* ergodox_board_led_off(); */
/* ergodox_right_led_1_off(); */
/* ergodox_right_led_2_off(); */
/* ergodox_right_led_3_off(); */
/* switch (layer) { */
/*   // TODO: Make this relevant to the ErgoDox EZ. */
/*     case 1: */
/*         ergodox_right_led_1_on(); */
/*         break; */
/*     case 2: */
/*         ergodox_right_led_2_on(); */
/*         break; */
/*     default: */
/*         // none */
/*         break; */
/* } */

};
