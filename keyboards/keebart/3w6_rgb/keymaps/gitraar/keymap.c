#include QMK_KEYBOARD_H
#include "features/achordion.h"

enum custom_keycodes {
    BASE = SAFE_RANGE,
    NAV,
    MOUSE,
    MEDIA,
    NUM,
    SYM,
    FUN,
};

// Tap Dance declarations
enum tap_dances {
    // TD_CW_CAPS,
    TD_DOT_ELLIPSIS,
};
// Tap Dance definitions
tap_dance_action_t tap_dance_actions[] = {
    // Tap once for Dot and twice for Ellipsis
    [TD_DOT_ELLIPSIS] = ACTION_TAP_DANCE_DOUBLE(KC_DOT, A(KC_SCLN)),
    // Tap once for Caps Word and twice for Caps Lock
    // [TD_CW_CAPS] = ACTION_TAP_DANCE_DOUBLE(CW_TOGG, KC_CAPS),
};

const uint16_t PROGMEM caps_word_combo[] = {KC_H, KC_COMMA, COMBO_END};
combo_t key_combos[] = {
    COMBO(caps_word_combo, CW_TOGG), // keycodes with modifiers are possible too!
};

// Definitions to include in layouts
#define PRT_SCR G(S(KC_4))
#define COPY G(KC_C)
#define CUT G(KC_X)
#define PASTE G(KC_V)
#define CLIP_HIST G(A(KC_BSLS))
#define LOCK_SCR C(G(KC_Q))
#define SPC_LEFT C(KC_LEFT)
#define SPC_RIGHT C(KC_RIGHT)

#define HRM_A LCTL_T(KC_A)
#define HRM_R LALT_T(KC_R)
#define HRM_S LGUI_T(KC_S)
#define HRM_T LSFT_T(KC_T)
#define HRM_N LSFT_T(KC_N)
#define HRM_E RGUI_T(KC_E)
#define HRM_I LALT_T(KC_I)
#define HRM_O RCTL_T(KC_O)
// #define BRM_X RALT_T(KC_X)
// #define BRM_DOT RALT_T(KC_DOT)

#define LT_1 LT(_NAV,KC_BSPC)
#define LT_2 LT(_MOUSE,KC_TAB)
#define LT_3 LT(_MEDIA,KC_ESC)
#define LT_4 LT(_NUM,KC_SPC)
#define LT_5 LT(_SYM,KC_ENT)
#define LT_6 LT(_FUN,KC_DEL)

#define TD_DOT TD(TD_DOT_ELLIPSIS)
// #define CW TD(TD_CW_CAPS)

#define _BASE 0
#define _NAV 1
#define _MOUSE 2
#define _MEDIA 3
#define _NUM 4
#define _SYM 5
#define _FUN 6

// Key overrides ko_make_with_layers_and_negmods >>><<>>
const key_override_t grave = ko_make_with_layers_and_negmods(MOD_MASK_ALT, HRM_O, KC_GRV, ~0, MOD_MASK_CSG);
const key_override_t colon = ko_make_with_layers_and_negmods(MOD_MASK_ALT, TD_DOT, KC_COLN, ~0, MOD_MASK_CSG);
const key_override_t semicolon = ko_make_with_layers_and_negmods(MOD_MASK_ALT, KC_COMM, KC_SCLN, ~0, MOD_MASK_CSG);
const key_override_t exclamation = ko_make_with_layers_and_negmods(MOD_MASK_SHIFT, TD_DOT, KC_EXLM, ~0, MOD_MASK_CAG);
const key_override_t inv_exclamation = ko_make_with_layers_and_negmods(MOD_MASK_SA, TD_DOT, A(KC_1), ~0, MOD_MASK_CG);
const key_override_t question = ko_make_with_layers_and_negmods(MOD_MASK_SHIFT, KC_COMM, KC_QUES, ~0, MOD_MASK_CAG);
const key_override_t inv_question = ko_make_with_layers_and_negmods(MOD_MASK_SA, KC_COMM, A(S(KC_SLSH)), ~0, MOD_MASK_CG);
const key_override_t double_quote = ko_make_with_layers_and_negmods(MOD_MASK_SHIFT, KC_SLSH, KC_DQT, ~0, MOD_MASK_CAG);
const key_override_t gte = ko_make_with_layers_and_negmods(MOD_MASK_SHIFT, KC_GT, A(KC_DOT), ~0, MOD_MASK_CAG);
const key_override_t lte = ko_make_with_layers_and_negmods(MOD_MASK_SHIFT, KC_LT, A(KC_COMMA), ~0, MOD_MASK_CAG);

// This globally defines all key overrides to be used
const key_override_t *key_overrides[] = {
    &grave,
    &colon,
    &semicolon,
    &exclamation,
    &inv_exclamation,
    &question,
    &inv_question,
    &double_quote,
    &gte,
    &lte
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

/* Base
 * ,----------------------------------.    ,----------------------------------.
 * |   Q  |   W  |   F  |   P  |   B  |    |   J  |   L  |   U  |   Y  |   -  |
 * |------+------+------+------+------|    |------+------+------+------+------|
 * |   A  |   R  |   S  |   T  |   G  |    |   M  |   N  |   E  |   I  |   O  |
 * |------+------+------+------+------|    |------+------+------+------+------|
 * |   Z  |   X  |   C  |   D  |   V  |    |   K  |   H  |   ,  |   .  |   /  |
 * `------+------+------+------+------|    |------+------+------+------+------'
 *               |  Esc | Bspc |  Tab |    |  Ent |  Spc |  Del |
 *               `--------------------'    `--------------------'
 */

[_BASE] = LAYOUT_split_3x5_3(
   //|——————————|——————————|——————————|——————————|——————————|      |——————————|——————————|——————————|——————————|——————————|
      KC_Q,      KC_W,      KC_F,      KC_P,      KC_B,             KC_J,      KC_L,      KC_U,      KC_Y,      KC_MINS,
   //|——————————|——————————|——————————|——————————|——————————|      |——————————|——————————|——————————|——————————|——————————|
      HRM_A,     HRM_R,     HRM_S,     HRM_T,     KC_G,             KC_M,      HRM_N,     HRM_E,     HRM_I,     HRM_O,
   //|——————————|——————————|——————————|——————————|——————————|      |——————————|——————————|——————————|——————————|——————————|
      KC_Z,      KC_X,      KC_C,      KC_D,      KC_V,             KC_K,      KC_H,      KC_COMMA,  TD_DOT,    KC_SLSH,
   //|——————————|——————————|——————————|——————————|——————————|      |——————————|——————————|——————————|——————————|——————————|
                            LT_3,      LT_1,      LT_2,             LT_5,      LT_4,      LT_6
   //|                     |——————————|——————————|——————————|      |——————————|——————————|——————————|
),

/* Navigation
 * ,----------------------------------.    ,----------------------------------.
 * | PstHi|  Cut | Copy | Paste|      |    |      | Home |  Up  |  End | PgUp |
 * |------+------+------+------+------|    |------+------+------+------+------|
 * | Ctrl |  Alt |  GUI | Shift|      |    | Caps | Left | Down | Right| PgDo |
 * |------+------+------+------+------|    |------+------+------+------+------|
 * |      | Ralt |      |      |      |    |      |SpLeft|      |SpRght|      |
 * `------+------+------+------+------|    |------+------+------+------+------'
 *               |      |OOOOOO|      |    | Copy | Paste| CbHst|
 *               `--------------------'    `--------------------'
 */

[_NAV] = LAYOUT_split_3x5_3(
   //|——————————|——————————|——————————|——————————|——————————|      |——————————|——————————|——————————|——————————|——————————|
      CLIP_HIST, CUT,       COPY,      PASTE,     KC_NO,            KC_NO,     KC_HOME,   KC_UP,     KC_END,    KC_PGUP,
   //|——————————|——————————|——————————|——————————|——————————|      |——————————|——————————|——————————|——————————|——————————|
      KC_LCTL,   KC_LALT,   KC_LGUI,   KC_LSFT,   KC_NO,            CW_TOGG,   KC_LEFT,   KC_DOWN,   KC_RIGHT,  KC_PGDN,
   //|——————————|——————————|——————————|——————————|——————————|      |——————————|——————————|——————————|——————————|——————————|
      KC_NO,     KC_NO,     KC_NO,     KC_NO,     KC_NO,            KC_NO,     SPC_LEFT,  KC_NO,     SPC_RIGHT, KC_NO,
   //|——————————|——————————|——————————|——————————|——————————|      |——————————|——————————|——————————|——————————|——————————|
                            KC_NO,     KC_NO,     KC_NO,            COPY,      PASTE,     CLIP_HIST
   //|                     |——————————|——————————|——————————|      |——————————|——————————|——————————|
),

/* Mouse Keys
 * ,----------------------------------.    ,----------------------------------.
 * | PstHi|  Cut | Copy | Paste|      |    | MWDn |      |  MUp |      |      |
 * |------+------+------+------+------|    |------+------+------+------+------|
 * | Ctrl |  Alt |  GUI | Shift|      |    | MWUp | MLeft| MDown|MRight|      |
 * |------+------+------+------+------|    |------+------+------+------+------|
 * |      |      |      |      |      |    |      |   ˜  |   ´  |   ˆ  |   `  |
 * `------+------+------+------+------|    |------+------+------+------+------'
 *               |      |OOOOOO|      |    |  M1  |  M3  |  M2  |
 *               `--------------------'    `--------------------'
 */

[_MOUSE] = LAYOUT_split_3x5_3(
   //|——————————|——————————|——————————|——————————|——————————|      |——————————|——————————|——————————|——————————|——————————|
      CLIP_HIST, CUT,       COPY,      PASTE,     KC_NO,            MS_WHLD,   KC_NO,     MS_UP,     KC_NO,     KC_NO,
   //|——————————|——————————|——————————|——————————|——————————|      |——————————|——————————|——————————|——————————|——————————|
      KC_LCTL,   KC_LALT,   KC_LGUI,   KC_LSFT,   KC_NO,            MS_WHLU,   MS_LEFT,   MS_DOWN,   MS_RGHT,   KC_NO,
   //|——————————|——————————|——————————|——————————|——————————|      |——————————|——————————|——————————|——————————|——————————|
      KC_NO,     KC_NO,     KC_NO,     KC_NO,     KC_NO,            KC_NO,     KC_TILDE,  KC_QUOTE,  KC_CIRC,   KC_GRV,
   //|——————————|——————————|——————————|——————————|——————————|      |——————————|——————————|——————————|——————————|——————————|
                            KC_NO,     KC_NO,     KC_NO,            MS_BTN1,   MS_BTN3,   MS_BTN2
   //|                     |——————————|——————————|——————————|      |——————————|——————————|——————————|
),

/* Media
 * ,----------------------------------.    ,----------------------------------.
 * |      |      |      |      |      |    | RMTog| RMNxt| RMHue| RMDo | RMUp |
 * |------+------+------+------+------|    |------+------+------+------+------|
 * | Ctrl |  Alt |  GUI | Shift|      |    |      | VolD | VolU | Prev | Next |
 * |------+------+------+------+------|    |------+------+------+------+------|
 * |      |      |      |      |      |    |      |      |      |      |      |
 * `------+------+------+------+------|    |------+------+------+------+------'
 *               |OOOOOO|      |      |    | Next | Play | Mute |
 *               `--------------------'    `--------------------'
 */

[_MEDIA] = LAYOUT_split_3x5_3(
   //|——————————|——————————|——————————|——————————|——————————|      |——————————|——————————|——————————|——————————|——————————|
      KC_NO,     KC_NO,     KC_NO,     KC_NO,     KC_NO,            RM_TOGG,   RM_NEXT,   RM_HUEU,   RM_VALD,   RM_VALU,
   //|——————————|——————————|——————————|——————————|——————————|      |——————————|——————————|——————————|——————————|——————————|
      KC_LCTL,   KC_LALT,   KC_LGUI,   KC_LSFT,   KC_NO,            KC_NO,     KC_VOLD,   KC_VOLU,   KC_MPRV,   KC_MNXT,
   //|——————————|——————————|——————————|——————————|——————————|      |——————————|——————————|——————————|——————————|——————————|
      KC_NO,     KC_NO,     KC_NO,     KC_NO,     KC_NO,            KC_NO,     KC_NO,     KC_NO,     KC_NO,     KC_NO,
   //|——————————|——————————|——————————|——————————|——————————|      |——————————|——————————|——————————|——————————|——————————|
                            KC_NO,     KC_NO,     KC_NO,            KC_MNXT,   KC_MPLY,   KC_MUTE
   //|                     |——————————|——————————|——————————|      |——————————|——————————|——————————|
),

/* Numbers
 * ,----------------------------------.    ,----------------------------------.
 * |   =  |   7  |   8  |   9  |   -  |    |      |      |      |      |      |
 * |------+------+------+------+------|    |------+------+------+------+------|
 * |   €  |   4  |   5  |   6  |   +  |    |      | Shift|  GUI |  Alt | Ctrl |
 * |------+------+------+------+------|    |------+------+------+------+------|
 * |   $  |   1  |   2  |   3  |   /  |    |      |      |      |      |      |
 * `------+------+------+------+------|    |------+------+------+------+------'
 *               |   .  |   0  |   *  |    |      |OOOOOO|      |
 *               `--------------------'    `--------------------'
 */

[_NUM] = LAYOUT_split_3x5_3(
   //|——————————|——————————|——————————|——————————|——————————|      |——————————|——————————|——————————|——————————|——————————|
      KC_PEQL,   KC_7,      KC_8,      KC_9,      KC_PMNS,          KC_NO,     KC_NO,     KC_NO,     KC_NO,     KC_NO,
   //|——————————|——————————|——————————|——————————|——————————|      |——————————|——————————|——————————|——————————|——————————|
      A(KC_AT),  KC_4,      KC_5,      KC_6,      KC_PPLS,          KC_NO,     KC_LSFT,   KC_RGUI,   KC_LALT,   KC_RCTL,
   //|——————————|——————————|——————————|——————————|——————————|      |——————————|——————————|——————————|——————————|——————————|
      KC_DLR,    KC_1,      KC_2,      KC_3,      KC_PSLS,          KC_NO,     KC_NO,     KC_NO,     KC_NO,     KC_NO,
   //|——————————|——————————|——————————|——————————|——————————|      |——————————|——————————|——————————|——————————|——————————|
                            KC_DOT,    KC_0,      KC_PAST,          KC_NO,     KC_NO,     KC_NO
   //|                     |——————————|——————————|——————————|      |——————————|——————————|——————————|
),

/* Symbols
 * ,----------------------------------.    ,----------------------------------.
 * |      |   '  |   {  |   }  |      |    |      |      |      |      |      |
 * |------+------+------+------+------|    |------+------+------+------+------|
 * |   #  |   @  |   [  |   ]  |   |  |    |      | Shift|  GUI |  Alt | Ctrl |
 * |------+------+------+------+------|    |------+------+------+------+------|
 * |   \  |   &  |   <  |   >  |   %  |    |      |      |      |      |      |
 * `------+------+------+------+------|    |------+------+------+------+------'
 *               |   (  |   )  |   _  |    |OOOOOO|      |      |
 *               `--------------------'    `--------------------'
 */

[_SYM] = LAYOUT_split_3x5_3(
   //|——————————|——————————|——————————|——————————|——————————|      |——————————|——————————|——————————|——————————|——————————|
      KC_NO,     KC_QUOT,   KC_LCBR,   KC_RCBR,   KC_NO,            KC_NO,     KC_NO,     KC_NO,     KC_NO,     KC_NO,
   //|——————————|——————————|——————————|——————————|——————————|      |——————————|——————————|——————————|——————————|——————————|
      KC_HASH,   KC_AT,     KC_LBRC,   KC_RBRC,   KC_PIPE,          KC_NO,     KC_LSFT,   KC_RGUI,   KC_LALT,   KC_RCTL,
   //|——————————|——————————|——————————|——————————|——————————|      |——————————|——————————|——————————|——————————|——————————|
      KC_BSLS,   KC_AMPR,   KC_LT,     KC_GT,     KC_PERC,          KC_NO,     KC_NO,     KC_NO,     KC_NO,     KC_NO,
   //|——————————|——————————|——————————|——————————|——————————|      |——————————|——————————|——————————|——————————|——————————|
                            KC_LPRN,   KC_RPRN,   KC_UNDS,          KC_NO,     KC_NO,     KC_NO
   //|                     |——————————|——————————|——————————|      |——————————|——————————|——————————|
),

/* Function
 * ,----------------------------------.    ,----------------------------------.
 * |  F12 |  F7  |  F8  |  F9  | PrSc |    |      |      |      |      |      |
 * |------+------+------+------+------|    |------+------+------+------+------|
 * |  F11 |  F4  |  F5  |  F6  | Sleep|    |      | Shift|  GUI |  Alt | Ctrl |
 * |------+------+------+------+------|    |------+------+------+------+------|
 * |  F10 |  F1  |  F2  |  F3  |      |    |      |      |      |      |      |
 * `------+------+------+------+------|    |------+------+------+------+------'
 *               |  MC  |  Spc |  Tab |    |      |      |OOOOOO|
 *               `--------------------'    `--------------------'
 */

[_FUN] = LAYOUT_split_3x5_3(
   //|——————————|——————————|——————————|——————————|——————————|      |——————————|——————————|——————————|——————————|——————————|
      KC_F12,    KC_F7,     KC_F8,     KC_F9,     PRT_SCR,          KC_NO,     KC_NO,     KC_NO,     KC_NO,     KC_NO,
   //|——————————|——————————|——————————|——————————|——————————|      |——————————|——————————|——————————|——————————|——————————|
      KC_F11,    KC_F4,     KC_F5,     KC_F6,     LOCK_SCR,         KC_NO,     KC_LSFT,   KC_RGUI,   KC_LALT,   KC_RCTL,
   //|——————————|——————————|——————————|——————————|——————————|      |——————————|——————————|——————————|——————————|——————————|
      KC_F10,    KC_F1,     KC_F2,     KC_F3,     KC_NO,            KC_NO,     KC_NO,     KC_NO,     KC_NO,     KC_NO,
   //|——————————|——————————|——————————|——————————|——————————|      |——————————|——————————|——————————|——————————|——————————|
                            KC_MCTL,   KC_SPC,    KC_TAB,           KC_NO,     KC_NO,     KC_NO
   //|                     |——————————|——————————|——————————|      |——————————|——————————|——————————|
   )
};

bool process_record_user(uint16_t keycode, keyrecord_t* record) {
    if (!process_achordion(keycode, record)) { return false; }
    // Your macros ...
    return true;
}

void matrix_scan_user(void) {
    achordion_task();
}

bool achordion_chord(uint16_t tap_hold_keycode, keyrecord_t* tap_hold_record, uint16_t other_keycode, keyrecord_t* other_record) {
    // Get matrix positions of tap-hold key and other key.
    uint8_t tap_hold_row = tap_hold_record->event.key.row;
    // uint8_t tap_hold_col = tap_hold_record->event.key.col;
    uint8_t other_row = other_record->event.key.row;
    // uint8_t other_col = other_record->event.key.col;
    if (tap_hold_row == 3) {
        return true;
    }
    bool first_key_left = (tap_hold_row >= 0 && tap_hold_row <= 3);
    bool second_key_left = (other_row >= 0 && other_row <= 3);
    return first_key_left != second_key_left;
}

bool achordion_eager_mod(uint8_t mod) {
    switch (mod) {
        case MOD_LSFT:
        case MOD_RSFT:
        case MOD_LALT:
            return true;  // Eagerly apply Shift mods.
        default:
            return false;
    }
}

uint16_t achordion_streak_chord_timeout(
    uint16_t tap_hold_keycode, uint16_t next_keycode) {
        if (IS_QK_LAYER_TAP(tap_hold_keycode)) {
        return 0;  // Disable streak detection on layer-tap keys.
    }

    // Otherwise, tap_hold_keycode is a mod-tap key.
    uint8_t mod = mod_config(QK_MOD_TAP_GET_MODS(tap_hold_keycode));
    if ((mod & MOD_LSFT) != 0) {
        return 100;  // A shorter streak timeout for Shift mod-tap keys.
    } else {
        return 240;  // A longer timeout otherwise.
    }
}

// RGB LED indicators for active keys in each layer
bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
    if (get_highest_layer(layer_state) > 0) {
        uint8_t layer = get_highest_layer(layer_state);

        for (uint8_t row = 0; row < MATRIX_ROWS; ++row) {
            for (uint8_t col = 0; col < MATRIX_COLS; ++col) {
                uint8_t index = g_led_config.matrix_co[row][col];

                if (index >= led_min && index < led_max && index != NO_LED &&
                keymap_key_to_keycode(layer, (keypos_t){col,row}) > KC_TRNS) {
                    rgb_matrix_set_color(index, RGB_ORANGE);
                }
            }
        }
    }
    return false;
}

// bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
//     uint8_t layer = get_highest_layer(layer_state);
//     for (uint8_t row = 0; row < MATRIX_ROWS; ++row) {
//         for (uint8_t col = 0; col < MATRIX_COLS; ++col) {
//             uint8_t index = g_led_config.matrix_co[row][col];
//             if (index >= led_min && index < led_max && index != NO_LED &&
//             keymap_key_to_keycode(layer, (keypos_t){col,row}) > KC_TRNS) {
//                 switch (layer) {
//                     case 0:
//                         rgb_matrix_set_color(index, HSV_ORANGE);
//                         break;
//                     case 1:
//                         rgb_matrix_set_color(index, HSV_BLUE);
//                         break;
//                     case 2:
//                         rgb_matrix_set_color(index, HSV_GREEN);
//                         break;
//                     default:
//                         rgb_matrix_set_color(index, HSV_BLACK);
//                 }
//             }
//         }
//     }
//     return false;
// }

// This disables matrix effects, leaving only the indicators
// void keyboard_post_init_user(void) {
//     rgb_matrix_mode_noeeprom(RGB_MATRIX_SOLID_COLOR);
//     rgb_matrix_sethsv_noeeprom(HSV_OFF);
// }
