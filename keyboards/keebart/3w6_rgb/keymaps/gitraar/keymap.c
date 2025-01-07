#include QMK_KEYBOARD_H
#include "features/achordion.h"

enum custom_keycodes {
    BASE = SAFE_RANGE,
    NAV,
    ACCENTS,
    MEDIA,
    NUM,
    SYM,
    FUN,
    GRV_A,
    CIRC_A,
    CIRC_E,
    CIRC_O,
};

bool process_record_user(uint16_t keycode, keyrecord_t* record) {
    if (!process_achordion(keycode, record)) { return false; }
    // Your macros ...
    switch (keycode) {
        case GRV_A:
        if (record->event.pressed) {
            if (MOD_MASK_SHIFT) { // Detect the activation of either shift keys
            del_mods(MOD_MASK_SHIFT);  // Disable Shift to prevent issues
            register_code(KC_GRV);
            add_mods(MOD_MASK_SHIFT);  // Enable Shift because it is needed for the Alpha and leave it because it was enabled at the start
            register_code(KC_A);
            } else {
            register_code(KC_GRV);
            register_code(KC_A);
            }
            break;
        }
        case CIRC_A:
        if (record->event.pressed) {
            if (MOD_MASK_SHIFT) {
            del_mods(MOD_MASK_SHIFT);
            tap_code16(KC_CIRC);
            add_mods(MOD_MASK_SHIFT);
            register_code(KC_A);
            } else {
            tap_code16(KC_CIRC); // tap_code16() used instead of register_code() because KC_CIRC uses a modifier, which requires 16 bits instead of 8
            register_code(KC_A);
            }
            break;
        }
        case CIRC_E:
        if (record->event.pressed) {
            if (MOD_MASK_SHIFT) {
            del_mods(MOD_MASK_SHIFT);
            tap_code16(KC_CIRC);
            add_mods(MOD_MASK_SHIFT);
            register_code(KC_E);
            } else {
            tap_code16(KC_CIRC);
            register_code(KC_E);
            }
            break;
        }
        case CIRC_O:
        if (record->event.pressed) {
            if (MOD_MASK_SHIFT) {
            del_mods(MOD_MASK_SHIFT);
            tap_code16(KC_CIRC);
            add_mods(MOD_MASK_SHIFT);
            register_code(KC_O);
            } else {
            tap_code16(KC_CIRC);
            register_code(KC_O);
            }
            break;
        }
        }
    return true;
}

void matrix_scan_user(void) {
  achordion_task();
}

bool achordion_eager_mod(uint8_t mod) {
    switch (mod) {
        case MOD_LSFT:
        case MOD_RSFT:
        case MOD_LCTL:
        case MOD_RCTL:
        return true;  // Eagerly apply Shift and Ctrl mods.

        default:
        return false;
    }
}

// Tap Dance declarations
enum tap_dances {
    TD_CW_CAPS,
    TD_COMMA_HYPHEN,
};

// Tap Dance definitions
tap_dance_action_t tap_dance_actions[] = {
    // Tap once for Caps Word and twice for Caps Lock
    [TD_CW_CAPS] = ACTION_TAP_DANCE_DOUBLE(CW_TOGG, KC_CAPS),
    // Tap once for Comma and twice for Hyphen
    [TD_COMMA_HYPHEN] = ACTION_TAP_DANCE_DOUBLE(KC_COMM, KC_MINS)
};


// Definitions to include in layouts
#define PRT_SCR G(S(KC_4))
#define COPY G(KC_C)
#define CUT G(KC_X)
#define PASTE G(KC_V)
#define CLIP_HIST G(A(KC_BSLS))
#define CEDILLA A(KC_C)

#define HRM_A LCTL_T(KC_A)
#define HRM_R LALT_T(KC_R)
#define HRM_S LGUI_T(KC_S)
#define HRM_T LSFT_T(KC_T)
#define HRM_N RSFT_T(KC_N)
#define HRM_E RGUI_T(KC_E)
#define HRM_I LALT_T(KC_I)
#define HRM_O RCTL_T(KC_O)
#define BRM_X RALT_T(KC_X)
#define BRM_DOT RALT_T(KC_DOT)

#define LT_1 LT(_NAV,KC_SPC)
#define LT_2 LT(_ACCENTS,KC_SPC)
#define LT_3 LT(_MEDIA,KC_SPC)
#define LT_4 LT(_NUM,KC_SPC)
#define LT_5 LT(_SYM,KC_SPC)
#define LT_6 LT(_FUN,KC_SPC)

#define COMMA TD(TD_COMMA_HYPHEN)
#define CW TD(TD_CW_CAPS)

#define _BASE 0
#define _NAV 1
#define _ACCENTS 2
#define _MEDIA 3
#define _NUM 4
#define _SYM 5
#define _FUN 6

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

/* Base
 * ,----------------------------------.    ,----------------------------------.
 * |   Q  |   W  |   F  |   P  |   B  |    |   J  |   L  |   U  |   Y  |   '  |
 * |------+------+------+------+------|    |------+------+------+------+------|
 * |   A  |   R  |   S  |   T  |   G  |    |   M  |   N  |   E  |   I  |   O  |
 * |------+------+------+------+------|    |------+------+------+------+------|
 * |   Z  |   X  |   C  |   D  |   V  |    |   K  |   H  |   ,  |   .  |   /  |
 * `------+------+------+------+------|    |------+------+------+------+------'
 *               |  Esc |  Spc |  Tab |    |  Ent | Bspc |  Del |
 *               `--------------------'    `--------------------'
 */

[_BASE] = LAYOUT_split_3x5_3(
   //|——————————|——————————|——————————|——————————|——————————|      |——————————|——————————|——————————|——————————|——————————|
      KC_Q,      KC_W,      KC_F,      KC_P,      KC_B,             KC_J,      KC_L,      KC_U,      KC_Y,      KC_QUOT,
   //|——————————|——————————|——————————|——————————|——————————|      |——————————|——————————|——————————|——————————|——————————|
      HRM_A,     HRM_R,     HRM_S,     HRM_T,     KC_G,             KC_M,      HRM_N,     HRM_E,     HRM_I,     HRM_O,
   //|——————————|——————————|——————————|——————————|——————————|      |——————————|——————————|——————————|——————————|——————————|
      KC_Z,      BRM_X,     KC_C,      KC_D,      KC_V,             KC_K,      KC_H,      COMMA,     BRM_DOT,   KC_SLSH,
   //|——————————|——————————|——————————|——————————|——————————|      |——————————|——————————|——————————|——————————|——————————|
                            LT_3,      LT_1,      LT_2,             LT_5,      LT_4,      LT_6
   //|                     |——————————|——————————|——————————|      |——————————|——————————|——————————|
),

/* Navigation
 * ,----------------------------------.    ,----------------------------------.
 * | PstHi| Paste| Copy |  Cut |      |    |  Ins | Home |  Up  |  End | PgUp |
 * |------+------+------+------+------|    |------+------+------+------+------|
 * | Ctrl |  Alt |  GUI | Shift|      |    | Caps | Left | Down | Right| PgDo |
 * |------+------+------+------+------|    |------+------+------+------+------|
 * |      | Ralt |      |      |      |    |      |  Cut | Copy | Paste| CbHst|
 * `------+------+------+------+------|    |------+------+------+------+------'
 *               |      |      |      |    |  Ent | Bspc |  Del |
 *               `--------------------'    `--------------------'
 */

[_NAV] = LAYOUT_split_3x5_3(
   //|——————————|——————————|——————————|——————————|——————————|      |——————————|——————————|——————————|——————————|——————————|
      CLIP_HIST, PASTE,     COPY,      CUT,       KC_NO,            KC_INS,    KC_HOME,   KC_UP,     KC_END,    KC_PGUP,
   //|——————————|——————————|——————————|——————————|——————————|      |——————————|——————————|——————————|——————————|——————————|
      KC_LCTL,   KC_LALT,   KC_LGUI,   KC_LSFT,   KC_NO,            CW,        KC_LEFT,   KC_DOWN,   KC_RIGHT,  KC_PGDN,
   //|——————————|——————————|——————————|——————————|——————————|      |——————————|——————————|——————————|——————————|——————————|
      KC_NO,     KC_RALT,   KC_NO,     KC_NO,     KC_NO,            KC_NO,     CUT,       COPY,      PASTE,     CLIP_HIST,
   //|——————————|——————————|——————————|——————————|——————————|      |——————————|——————————|——————————|——————————|——————————|
                            KC_NO,     KC_NO,     KC_NO,            KC_ENT,    KC_BSPC,   KC_DEL
   //|                     |——————————|——————————|——————————|      |——————————|——————————|——————————|
),

/* Dead Keys
 * ,----------------------------------.    ,----------------------------------.
 * |      |      |      |      |      |    |      |      |      |      |      |
 * |------+------+------+------+------|    |------+------+------+------+------|
 * |   à  |  Alt |  GUI | Shift|   â  |    |      |      |      |   ê  |  ô   |
 * |------+------+------+------+------|    |------+------+------+------+------|
 * |      | Ralt |   ç  |      |      |    |      |      |      |      |      |
 * `------+------+------+------+------|    |------+------+------+------+------'
 *               |      |      |      |    |   `  |   ~  |   ^  |
 *               `--------------------'    `--------------------'
 */

[_ACCENTS] = LAYOUT_split_3x5_3(
   //|——————————|——————————|——————————|——————————|——————————|      |——————————|——————————|——————————|——————————|——————————|
      KC_NO,     KC_NO,     KC_NO,     KC_NO,     KC_NO,            KC_NO,     KC_NO,     KC_NO,     KC_NO,     KC_NO,
   //|——————————|——————————|——————————|——————————|——————————|      |——————————|——————————|——————————|——————————|——————————|
      GRV_A,     KC_LALT,   KC_LGUI,   KC_LSFT,   CIRC_A,           KC_NO,     KC_NO,     KC_NO,     CIRC_E,    CIRC_O,
   //|——————————|——————————|——————————|——————————|——————————|      |——————————|——————————|——————————|——————————|——————————|
      KC_NO,     KC_RALT,   CEDILLA,   KC_NO,     KC_NO,            KC_NO,     KC_NO,     KC_NO,     KC_NO,     KC_NO,
   //|——————————|——————————|——————————|——————————|——————————|      |——————————|——————————|——————————|——————————|——————————|
                            KC_NO,     KC_NO,     KC_NO,            KC_GRV,    KC_TILD,   KC_CIRC
   //|                     |——————————|——————————|——————————|      |——————————|——————————|——————————|
),

/* Media
 * ,----------------------------------.    ,----------------------------------.
 * |      |      |      |      |      |    | RGBT | RGBM | RGBH | RGBS | RGBV |
 * |------+------+------+------+------|    |------+------+------+------+------|
 * | Ctrl |  Alt |  GUI | Shift|      |    | Sleep| VolU | VolD | Prev | Next |
 * |------+------+------+------+------|    |------+------+------+------+------|
 * |      | Ralt |      |      |      |    |      |      |      |      |      |
 * `------+------+------+------+------|    |------+------+------+------+------'
 *               |      |      |      |    | Next | Play | Mute |
 *               `--------------------'    `--------------------'
 */

[_MEDIA] = LAYOUT_split_3x5_3(
   //|——————————|——————————|——————————|——————————|——————————|      |——————————|——————————|——————————|——————————|——————————|
      KC_NO,     KC_NO,     KC_NO,     KC_NO,     KC_NO,            RGB_TOG,   RGB_MOD,   RGB_HUI,   RGB_SAI,   RGB_VAI,
   //|——————————|——————————|——————————|——————————|——————————|      |——————————|——————————|——————————|——————————|——————————|
      KC_LCTL,   KC_LALT,   KC_LGUI,   KC_LSFT,   KC_NO,            KC_SLEP,   KC_VOLU,   KC_VOLD,   KC_MPRV,   KC_MNXT,
   //|——————————|——————————|——————————|——————————|——————————|      |——————————|——————————|——————————|——————————|——————————|
      KC_NO,     KC_RALT,   KC_NO,     KC_NO,     KC_NO,            KC_NO,     KC_NO,     KC_NO,     KC_NO,     KC_NO,
   //|——————————|——————————|——————————|——————————|——————————|      |——————————|——————————|——————————|——————————|——————————|
                            KC_NO,     KC_NO,     KC_NO,            KC_MNXT,   KC_MPLY,   KC_MUTE
   //|                     |——————————|——————————|——————————|      |——————————|——————————|——————————|
),

/* Numbers
 * ,----------------------------------.    ,----------------------------------.
 * |   [  |   7  |   8  |   9  |   ]  |    |      |      |      |      |      |
 * |------+------+------+------+------|    |------+------+------+------+------|
 * |   ;  |   4  |   5  |   6  |   =  |    |      | Shift|  GUI |  Alt | Ctrl |
 * |------+------+------+------+------|    |------+------+------+------+------|
 * |   `  |   1  |   2  |   3  |   \  |    |      |      |      | Ralt |      |
 * `------+------+------+------+------|    |------+------+------+------+------'
 *               |   .  |   0  |   -  |    |      |      |      |
 *               `--------------------'    `--------------------'
 */

[_NUM] = LAYOUT_split_3x5_3(
   //|——————————|——————————|——————————|——————————|——————————|      |——————————|——————————|——————————|——————————|——————————|
      KC_LBRC,   KC_7,      KC_8,      KC_9,      KC_RBRC,          KC_NO,     KC_NO,     KC_NO,     KC_NO,     KC_NO,
   //|——————————|——————————|——————————|——————————|——————————|      |——————————|——————————|——————————|——————————|——————————|
      KC_SCLN,   KC_4,      KC_5,      KC_6,      KC_EQL,           KC_NO,     KC_RSFT,   KC_RGUI,   KC_LALT,   KC_RCTL,
   //|——————————|——————————|——————————|——————————|——————————|      |——————————|——————————|——————————|——————————|——————————|
      KC_GRV,    KC_1,      KC_2,      KC_3,      KC_BSLS,          KC_NO,     KC_NO,     KC_NO,     KC_RALT,   KC_NO,
   //|——————————|——————————|——————————|——————————|——————————|      |——————————|——————————|——————————|——————————|——————————|
                            KC_DOT,    KC_0,      KC_MINS,          KC_NO,     KC_NO,     KC_NO
   //|                     |——————————|——————————|——————————|      |——————————|——————————|——————————|
),

/* Symbols
 * ,----------------------------------.    ,----------------------------------.
 * |   {  |   &  |   *  |   (  |   }  |    |      |      |      |      |      |
 * |------+------+------+------+------|    |------+------+------+------+------|
 * |   :  |   $  |   %  |   ^  |   +  |    |      | Shift|  GUI |  Alt | Ctrl |
 * |------+------+------+------+------|    |------+------+------+------+------|
 * |   ~  |   !  |   @  |   #  |   |  |    |      |      |      | Ralt |      |
 * `------+------+------+------+------|    |------+------+------+------+------'
 *               |   (  |   )  |   _  |    |      |      |      |
 *               `--------------------'    `--------------------'
 */

[_SYM] = LAYOUT_split_3x5_3(
   //|——————————|——————————|——————————|——————————|——————————|      |——————————|——————————|——————————|——————————|——————————|
      KC_LCBR,   KC_AMPR,   KC_ASTR,   KC_LPRN,   KC_RCBR,          KC_NO,     KC_NO,     KC_NO,     KC_NO,     KC_NO,
   //|——————————|——————————|——————————|——————————|——————————|      |——————————|——————————|——————————|——————————|——————————|
      KC_COLN,   KC_DLR,    KC_PERC,   KC_CIRC,   KC_PLUS,          KC_NO,     KC_RSFT,   KC_RGUI,   KC_LALT,   KC_RCTL,
   //|——————————|——————————|——————————|——————————|——————————|      |——————————|——————————|——————————|——————————|——————————|
      KC_TILD,   KC_EXLM,   KC_AT,     KC_HASH,   KC_PIPE,          KC_NO,     KC_NO,     KC_NO,     KC_RALT,   KC_NO,
   //|——————————|——————————|——————————|——————————|——————————|      |——————————|——————————|——————————|——————————|——————————|
                            KC_LPRN,   KC_RPRN,   KC_UNDS,          KC_NO,     KC_NO,     KC_NO
   //|                     |——————————|——————————|——————————|      |——————————|——————————|——————————|
),

/* Function
 * ,----------------------------------.    ,----------------------------------.
 * |  F12 |  F7  |  F8  |  F9  | PrSc |    |      |      |      |      |      |
 * |------+------+------+------+------|    |------+------+------+------+------|
 * |  F11 |  F4  |  F5  |  F6  |      |    |      | Shift|  GUI |  Alt | Ctrl |
 * |------+------+------+------+------|    |------+------+------+------+------|
 * |  F10 |  F1  |  F2  |  F3  |      |    |      |      |      | Ralt |      |
 * `------+------+------+------+------|    |------+------+------+------+------'
 *               |  MC  |  Spc |  Tab |    |      |      |      |
 *               `--------------------'    `--------------------'
 */

[_FUN] = LAYOUT_split_3x5_3(
   //|——————————|——————————|——————————|——————————|——————————|      |——————————|——————————|——————————|——————————|——————————|
      KC_F12,    KC_F7,     KC_F8,     KC_F9,     PRT_SCR,          KC_NO,     KC_NO,     KC_NO,     KC_NO,     KC_NO,
   //|——————————|——————————|——————————|——————————|——————————|      |——————————|——————————|——————————|——————————|——————————|
      KC_F11,    KC_F4,     KC_F5,     KC_F6,     KC_NO,            KC_NO,     KC_RSFT,   KC_RGUI,   KC_LALT,   KC_RCTL,
   //|——————————|——————————|——————————|——————————|——————————|      |——————————|——————————|——————————|——————————|——————————|
      KC_F10,    KC_F1,     KC_F2,     KC_F3,     KC_NO,            KC_NO,     KC_NO,     KC_NO,     KC_RALT,   KC_NO,
   //|——————————|——————————|——————————|——————————|——————————|      |——————————|——————————|——————————|——————————|——————————|
                            KC_MCTL,   KC_SPC,    KC_TAB,           KC_NO,     KC_NO,     KC_NO
   //|                     |——————————|——————————|——————————|      |——————————|——————————|——————————|
   )
};
