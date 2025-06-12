#include "sentence_case.h"
#include QMK_KEYBOARD_H

/*
###################
### Definitions ###
###################
*/

// macOS commands
#define PRT_SCR G(S(KC_4))
#define COPY G(KC_C)
#define CUT G(KC_X)
#define PASTE G(KC_V)
#define CLIP_HIST G(A(KC_BACKSLASH))
#define LOCK_SCR C(G(KC_Q))
#define SPC_LEFT C(KC_LEFT)
#define SPC_RIGHT C(KC_RIGHT)
#define UNDO G(KC_Z)
#define REDO G(S(KC_Z))
#define RAYCAST G(KC_SPC)

// Web Browser commands
#define BACK G(KC_LEFT_BRACKET)
#define FORWARD G(KC_RIGHT_BRACKET)
#define TAB_UP G(S(KC_LEFT_BRACKET))
#define TAB_DOWN G(S(KC_RIGHT_BRACKET))

// Layers
#define _BASE 0
#define _NAV 1
#define _MOU 2
#define _NUM 3
#define _SYM 4
#define _FUN 5
#define _EXT 6

// Tap dances
#define TD_PGUP TD(PGUP)
#define TD_PGDN TD(PGDOWN)
#define TD_HOME TD(HOME)
#define TD_END TD(END)

// Key matrix assigments
/*
    ,———————————————————————————————————————.                   ,———————————————————————————————————————.
    |  LT5  |  LT4  |  LT3  |  LT2  |  LT1  |                   |  RT1  |  RT2  |  RT3  |  RT4  |  RT5  |
    |———————+———————+———————+———————+———————|                   |———————+———————+———————+———————+———————|
    |  LM5  |  LM4  |  LM3  |  LM2  |  LM1  |                   |  RM1  |  RM2  |  RM3  |  RM4  |  RM5  |
    |———————+———————+———————+———————+———————|                   |———————+———————+———————+———————+———————|
    |  LB5  |  LB4  |  LB3  |  LB2  |  LB1  |                   |  RB1  |  RB2  |  RB3  |  RB4  |  RB5  |
    `———————————————————————+———————+———————+———————.   ,———————+———————+———————+———————————————————————'
                            |  LH3  |  LH2  |  LH1  |   |  RH2  |  RH2  |  RH3  |
                            `———————————————————————'   `———————————————————————'
*/

#define UM_LT5 LT(0, KC_F)
#define UM_LT4 KC_P
#define UM_LT3 KC_D
#define UM_LT2 HYPR_T(KC_L)
#define UM_LT1 KC_X

#define UM_LM5 LCTL_T(KC_S)
#define UM_LM4 LALT_T(KC_N)
#define UM_LM3 LGUI_T(KC_T)
#define UM_LM2 LSFT_T(KC_H)
#define UM_LM1 KC_K

#define UM_LB5 KC_V
#define UM_LB4 KC_W
#define UM_LB3 KC_G
#define UM_LB2 KC_M
#define UM_LB1 KC_J

#define UM_LH3 LT(_MOU, KC_BACKSPACE)
#define UM_LH2 LT(_NAV, KC_R)
#define UM_LH1 KC_TAB

#define UM_RT1 QK_ALT_REPEAT_KEY
#define UM_RT2 HYPR_T(KC_U)
#define UM_RT3 KC_O
#define UM_RT4 KC_Y
#define UM_RT5 KC_B

#define UM_RM1 KC_MINUS
#define UM_RM2 RSFT_T(KC_A)
#define UM_RM3 RGUI_T(KC_E)
#define UM_RM4 LALT_T(KC_I)
#define UM_RM5 RCTL_T(KC_C)

#define UM_RB1 LT(0,KC_KP_SLASH)
#define UM_RB2 KC_DOT
#define UM_RB3 KC_COMMA
#define UM_RB4 OSL(_EXT)
#define UM_RB5 KC_Z

#define UM_RH1 LT(_SYM, KC_ENTER)
#define UM_RH2 LT(_NUM, KC_SPACE)
#define UM_RH3 LT(_FUN, KC_DELETE)

// Settings
#define IDLE_TIMEOUT_MS 600000 // Idle timeout in milliseconds for RGB Matrix.

// Custom keycodes
enum custom_keycodes {
    BASE = SAFE_RANGE,
    NAV,
    MOU,
    NUM,
    SYM,
    FUN,
    EXT,
    U_TILDE_A, // 'ã'
    U_GR_A, // 'à'
    U_AC_E,// 'é'
    U_CIRC_E, // 'ê'
    U_AC_I, // 'í'
    U_AC_O, // 'ó'
    U_CIRC_O, // 'ô'
    U_TILDE_O, // 'õ'
    U_AC_U, // 'ú'
    U_TILDE_N, // ''
    U_CIRC, // Circumflex with space to not act like a dead key.
    U_TILDE, // Tilde with space to not act like a dead key.
    U_QUOTE, // Single quote with space to not act like a dead key.
    U_QU, // "qu"
    U_RGB_R, // Reset RGB Matrix to orange.
    U_RGB_T, // Macro for RGB toggle with extra info.
};

/*
####################
### Magic String ###
####################
*/

// An enhanced version of SEND_STRING. If Caps Word is active, the Shift key is held while sending the string.
#define MAGIC_STRING(str) magic_send_string_P(PSTR(str))
static void magic_send_string_P(const char* str) {
    uint8_t saved_mods = 0;
    // If Caps Word is on, save the mods and hold Shift.
    if (is_caps_word_on()) {
        saved_mods = get_mods();
        register_mods(MOD_BIT_LSHIFT);
    }

    send_string_P(str); // Send the string.

    // If Caps Word is on, restore the mods.
    if (is_caps_word_on()) {
        set_mods(saved_mods);
    }
}

/*
##################
### Tap Dances ###
##################
*/

// Code to enable tap-and-hold tap dances.
typedef struct {
    uint16_t tap;
    uint16_t hold;
    uint16_t held;
} tap_dance_tap_hold_t;

void tap_dance_tap_hold_finished(tap_dance_state_t *state, void *user_data) {
    tap_dance_tap_hold_t *tap_hold = (tap_dance_tap_hold_t *)user_data;
    if (state->pressed) {
        if (state->count == 1
        #ifndef PERMISSIVE_HOLD
        && !state->interrupted
        #endif
        ) {
            register_code16(tap_hold->hold);
            tap_hold->held = tap_hold->hold;
        } else {
            register_code16(tap_hold->tap);
            tap_hold->held = tap_hold->tap;
        }
    }
}

void tap_dance_tap_hold_reset(tap_dance_state_t *state, void *user_data) {
    tap_dance_tap_hold_t *tap_hold = (tap_dance_tap_hold_t *)user_data;
    if (tap_hold->held) {
        unregister_code16(tap_hold->held);
        tap_hold->held = 0;
    }
}

#define ACTION_TAP_DANCE_TAP_HOLD(tap, hold) { .fn = {NULL, tap_dance_tap_hold_finished, tap_dance_tap_hold_reset}, .user_data = (void *)&((tap_dance_tap_hold_t){tap, hold, 0}), }

enum tap_dances {
    PGUP,
    PGDOWN,
    HOME,
    END,
};

// Definition for each tap dance using the functions above.
tap_dance_action_t tap_dance_actions[] = {
    [PGUP] = ACTION_TAP_DANCE_TAP_HOLD(KC_PGUP, G(KC_UP)),
    [PGDOWN] = ACTION_TAP_DANCE_TAP_HOLD(KC_PGDN, G(KC_DOWN)),
    [HOME] = ACTION_TAP_DANCE_TAP_HOLD(A(KC_LEFT), KC_HOME),
    [END] = ACTION_TAP_DANCE_TAP_HOLD(A(KC_RIGHT), KC_END),
};

/*
##############
### Combos ###
##############
*/

// Combo definitions.
// Left-side vertical combos.
const uint16_t PROGMEM cut_combo[] = {UM_LT4, UM_LM4, COMBO_END};
const uint16_t PROGMEM copy_combo[] = {UM_LT3, UM_LM3, COMBO_END};
const uint16_t PROGMEM paste_combo[] = {UM_LT2, UM_LM2, COMBO_END};
const uint16_t PROGMEM clip_hist_combo[] = {UM_LT1, UM_LM1, COMBO_END};

const uint16_t PROGMEM tilde_combo[] = {UM_LM2, UM_LB2, COMBO_END};

// Left-side horizontal combos.
const uint16_t PROGMEM caps_word_combo[] = {UM_LM2, UM_LM1, COMBO_END};

const uint16_t PROGMEM qu_combo[] = {UM_LB2, UM_LB3, COMBO_END};

// Right-side vertical combos.
const uint16_t PROGMEM lprn_combo[] = {UM_RT2, UM_RM2, COMBO_END};
const uint16_t PROGMEM rprn_combo[] = {UM_RT3, UM_RM3, COMBO_END};
const uint16_t PROGMEM super_o_combo[] = {UM_RT4, UM_RM4, COMBO_END};

const uint16_t PROGMEM at_combo[] = {UM_RM2, UM_RB2, COMBO_END};

// Right-side horizontal combos.
const uint16_t PROGMEM esc_combo[] = {UM_RM1, UM_RM2, COMBO_END};
const uint16_t PROGMEM semicolon_combo[] = {UM_RB2, UM_RB3, COMBO_END};

// Right-side horizontal combos on Media layer.
const uint16_t PROGMEM mute_combo[] = {KC_VOLD, KC_VOLU, COMBO_END};

enum combos {
    AT_COMBO, // '@'
    CAPS_WORD_COMBO,
    CLIP_HIST_COMBO,
    COPY_COMBO,
    CUT_COMBO,
    ESC_COMBO,
    LPRN_COMBO, // '('
    MUTE_COMBO,
    PASTE_COMBO,
    QU_COMBO, // 'qu'
    RPRN_COMBO, // ')'
    SEMICOLON_COMBO, // ';'
    SUPER_O_COMBO, // 'º'
    TILDE_COMBO, // '~'
  };

// Used combos.
combo_t key_combos[] = {
    [AT_COMBO] = COMBO(at_combo, KC_AT),
    [CAPS_WORD_COMBO] = COMBO(caps_word_combo, CW_TOGG),
    [CLIP_HIST_COMBO] = COMBO(clip_hist_combo, CLIP_HIST),
    [COPY_COMBO] = COMBO(copy_combo, COPY),
    [CUT_COMBO] = COMBO(cut_combo, CUT),
    [ESC_COMBO] = COMBO(esc_combo, KC_ESC),
    [LPRN_COMBO] = COMBO(lprn_combo, KC_LPRN),
    [MUTE_COMBO] = COMBO(mute_combo, KC_MUTE),
    [PASTE_COMBO] = COMBO(paste_combo, PASTE),
    [QU_COMBO] = COMBO(qu_combo, U_QU),
    [RPRN_COMBO] = COMBO(rprn_combo, KC_RPRN),
    [SEMICOLON_COMBO] = COMBO(semicolon_combo, KC_SEMICOLON),
    [SUPER_O_COMBO] = COMBO(super_o_combo, A(KC_0)),
    [TILDE_COMBO] = COMBO(tilde_combo, U_TILDE),
};

// Combo options.
#ifdef COMBO_MUST_PRESS_IN_ORDER_PER_COMBO
bool get_combo_must_press_in_order(uint16_t combo_index, combo_t *combo) {
    switch (combo_index) {
        case QU_COMBO: // I want 'qu' to trigger on 'mg' but don't want to lose the ability to type 'gm' quickly
            return true;
        default:
            return false;
    }
}
#endif

#ifdef COMBO_TERM_PER_COMBO
uint16_t get_combo_term(uint16_t combo_index, combo_t *combo) {
    switch (combo_index) {
        case QU_COMBO:
            return 200;
    }
    return COMBO_TERM;
}
#endif

/*
#####################
### Key Overrides ###
#####################
*/

// Key overrides.
const key_override_t super_a = ko_make_with_layers_and_negmods(MOD_MASK_SHIFT, A(KC_0), A(KC_9), ~0, MOD_MASK_CAG);
const key_override_t exclamation = ko_make_with_layers_and_negmods(MOD_MASK_SHIFT, UM_RB2, KC_EXLM, ~0, MOD_MASK_CAG);
const key_override_t inv_exclamation = ko_make_with_layers_and_negmods(MOD_MASK_SA, UM_RB2, A(KC_1), ~0, MOD_MASK_CG);
const key_override_t ellipsis = ko_make_with_layers_and_negmods(MOD_MASK_ALT, UM_RB2, A(KC_SCLN), ~0, MOD_MASK_CSG);
const key_override_t question = ko_make_with_layers_and_negmods(MOD_MASK_SHIFT, UM_RB3, KC_QUES, ~0, MOD_MASK_CAG);
const key_override_t inv_question = ko_make_with_layers_and_negmods(MOD_MASK_SA, UM_RB3, A(S(KC_SLSH)), ~0, MOD_MASK_CG);
const key_override_t gte = ko_make_with_layers_and_negmods(MOD_MASK_SHIFT, KC_GT, A(KC_DOT), ~0, MOD_MASK_CAG);
const key_override_t lte = ko_make_with_layers_and_negmods(MOD_MASK_SHIFT, KC_LT, A(KC_COMMA), ~0, MOD_MASK_CAG);
const key_override_t open_fancy_quote = ko_make_with_layers_and_negmods(MOD_MASK_SHIFT, KC_LPRN, A(KC_RBRC), ~0, MOD_MASK_CAG);
const key_override_t close_fancy_quote = ko_make_with_layers_and_negmods(MOD_MASK_SHIFT, KC_RPRN, A(S(KC_RBRC)), ~0, MOD_MASK_CAG);
const key_override_t open_fancy_double_quote = ko_make_with_layers_and_negmods(MOD_MASK_ALT, KC_LPRN, A(KC_LBRC), ~0, MOD_MASK_CSG);
const key_override_t close_fancy_double_quote = ko_make_with_layers_and_negmods(MOD_MASK_ALT, KC_RPRN, A(S(KC_LBRC)), ~0, MOD_MASK_CSG);

// This globally defines all key overrides to be used.
const key_override_t *key_overrides[] = {
    &super_a,
    &exclamation,
    &inv_exclamation,
    &ellipsis,
    &question,
    &inv_question,
    &gte,
    &lte,
    &open_fancy_quote,
    &close_fancy_quote,
    &open_fancy_double_quote,
    &close_fancy_double_quote,
};

/*
##################
### Leader Key ###
##################
*/

void leader_start_user(void) {
}

void leader_end_user(void) {
    if (leader_sequence_one_key(KC_T)) {
        // Leader, t => Process one segment
        tap_code16(G(KC_A));
        wait_ms(500);
        tap_code16(G(S(KC_C)));
        wait_ms(2500);
        tap_code(KC_C);
        wait_ms(500);
        tap_code16(G(KC_V));
    } else if (leader_sequence_two_keys(KC_T, KC_T)) {
        // Leader, t, t => Process ten segments
        for (int i = 0; i < 10; i++) {
            tap_code16(G(KC_A));
            wait_ms(500);
            tap_code16(G(S(KC_C)));
            wait_ms(2500);
            tap_code(KC_C);
            wait_ms(500);
            tap_code16(G(KC_V));
            wait_ms(500);
            tap_code(KC_DOWN);
        }
    } else if (leader_sequence_three_keys(KC_T, KC_T, KC_T)) {
        // Leader, t, t, t => Process fifty segments
        for (int i = 0; i < 50; i++) {
            tap_code16(G(KC_A));
            wait_ms(500);
            tap_code16(G(S(KC_C)));
            wait_ms(2500);
            tap_code(KC_C);
            wait_ms(500);
            tap_code16(G(KC_V));
            wait_ms(500);
            tap_code(KC_DOWN);
        }
    }
}

/*
##################
### RGB Matrix ###
##################
*/

bool rgb_disabled_manually = false;

// Function to do things when the keyboard is idle.
static uint32_t idle_callback(uint32_t trigger_time, void* cb_arg) {
    // If execution gets here, the keyboard has gone idle.
    if (rgb_matrix_is_enabled()) {
        rgb_matrix_disable(); // Disables the RGB Matrix.
    }
    return 0;
}

// Remove light from inactive keys in current layer.
bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
    if (get_highest_layer(layer_state) > 0) {
        uint8_t layer = get_highest_layer(layer_state);
        for (uint8_t row = 0; row < MATRIX_ROWS; ++row) {
            for (uint8_t col = 0; col < MATRIX_COLS; ++col) {
                uint8_t index = g_led_config.matrix_co[row][col];
                if (index >= led_min && index < led_max && index != NO_LED && keymap_key_to_keycode(layer, (keypos_t){col,row}) <= KC_TRANSPARENT) { // Keys from lower layers are ignored.
                    rgb_matrix_set_color(index, RGB_BLACK); // Setting RGB to black is the same as disabling it.
                }
            }
        }
    }
    return false;
}

/*
#####################
### Sentence Case ###
#####################
*/

char sentence_case_press_user(uint16_t keycode, keyrecord_t* record, uint8_t mods) {
    if ((mods & ~(MOD_MASK_SHIFT | MOD_BIT(KC_RALT))) == 0) {
        const bool shifted = mods & MOD_MASK_SHIFT;
        switch (keycode) {
            case KC_A ... KC_Z:
            case U_AC_E:
            case U_AC_I:
            case U_AC_O:
            case U_AC_U:
            case U_GR_A:
            case U_CIRC_E:
            case U_CIRC_O:
            case U_TILDE_A:
            case U_TILDE_O:
                return 'a'; // Letter key.
            case KC_1:
            case KC_SLSH:
                return shifted ? '.' : '#';
            case KC_EXLM:
            case KC_QUES:
                return '.';
            case KC_2 ... KC_0: // 2 3 4 5 6 7 8 9 0
            case KC_AT ... KC_PERC: // @ # $ %
            case KC_AMPR ... KC_RPRN: // & * ( )
            case KC_MINS ... KC_SCLN: // - = [ ] backslash ;
            case KC_UNDS ... KC_COLN: // _ + { } | :
            case KC_GRV:
                return '#'; // Symbol key.
            case KC_COMMA:
                return shifted ? '.' : '#'; // Shift-, is punctuation (?), but , is just a symbol.
            case KC_DOT:
                return shifted ? '.' : '.'; // Both . and ! are punctuation.
            case KC_SPC:
                return ' '; // Space key.
            case KC_QUOTE:
            case KC_DQUO:
            case UM_RB4: // OSL for Extended layer
                return '\''; // Quote key.
        }
    }
    // Otherwise clear Sentence Case to initial state.
    sentence_case_clear();
    return '\0';
}

bool sentence_case_check_ending(const uint16_t* buffer) {
    // Don't consider abbreviations like "vs." and "etc." to end the sentence.
    if (SENTENCE_CASE_JUST_TYPED(KC_SPC, KC_A, KC_B, KC_R, KC_DOT) ||
        SENTENCE_CASE_JUST_TYPED(KC_SPC, KC_A, KC_DOT, KC_C, KC_DOT) ||
        SENTENCE_CASE_JUST_TYPED(KC_SPC, KC_A, KC_DOT, KC_M, KC_DOT) ||
        SENTENCE_CASE_JUST_TYPED(KC_SPC, KC_A, KC_G, KC_O, KC_DOT) ||
        SENTENCE_CASE_JUST_TYPED(KC_SPC, KC_A, KC_R, KC_T, KC_DOT) ||
        SENTENCE_CASE_JUST_TYPED(KC_SPC, KC_A, KC_V, KC_DOT) ||
        SENTENCE_CASE_JUST_TYPED(KC_SPC, KC_B, KC_R, KC_O, KC_S, KC_DOT) ||
        SENTENCE_CASE_JUST_TYPED(KC_SPC, KC_C, KC_A, KC_P, KC_DOT) ||
        SENTENCE_CASE_JUST_TYPED(KC_SPC, KC_C, KC_F, KC_DOT) ||
        SENTENCE_CASE_JUST_TYPED(KC_SPC, KC_D, KC_DOT, KC_C, KC_DOT) ||
        SENTENCE_CASE_JUST_TYPED(KC_SPC, KC_D, KC_DOT) ||
        SENTENCE_CASE_JUST_TYPED(KC_SPC, KC_D, KC_E, KC_Z, KC_DOT) ||
        SENTENCE_CASE_JUST_TYPED(KC_SPC, KC_D, KC_O, KC_C, KC_S, KC_DOT) ||
        SENTENCE_CASE_JUST_TYPED(KC_SPC, KC_D, KC_O, KC_M, KC_DOT) ||
        SENTENCE_CASE_JUST_TYPED(KC_SPC, KC_D, KC_R, KC_A, KC_DOT) ||
        SENTENCE_CASE_JUST_TYPED(KC_SPC, KC_D, KC_R, KC_DOT) ||
        SENTENCE_CASE_JUST_TYPED(KC_SPC, KC_E, KC_DOT, KC_G,KC_DOT) ||
        SENTENCE_CASE_JUST_TYPED(KC_SPC, KC_E, KC_N, KC_G, KC_DOT) ||
        SENTENCE_CASE_JUST_TYPED(KC_SPC, KC_E, KC_T, KC_C, KC_DOT) ||
        SENTENCE_CASE_JUST_TYPED(KC_SPC, KC_E, KC_X, KC_A, KC_DOT) ||
        SENTENCE_CASE_JUST_TYPED(KC_SPC, KC_E, KC_X, KC_DOT) ||
        SENTENCE_CASE_JUST_TYPED(KC_SPC, KC_E, KC_X, KC_M, KC_A, KC_DOT) ||
        SENTENCE_CASE_JUST_TYPED(KC_SPC, KC_E, KC_X, KC_M, KC_O, KC_DOT) ||
        SENTENCE_CASE_JUST_TYPED(KC_SPC, KC_F, KC_A, KC_X, KC_DOT) ||
        SENTENCE_CASE_JUST_TYPED(KC_SPC, KC_F, KC_E, KC_M, KC_DOT) ||
        SENTENCE_CASE_JUST_TYPED(KC_SPC, KC_F, KC_E, KC_V, KC_DOT) ||
        SENTENCE_CASE_JUST_TYPED(KC_SPC, KC_F, KC_I, KC_G, KC_DOT) ||
        SENTENCE_CASE_JUST_TYPED(KC_SPC, KC_F, KC_L, KC_DOT) ||
        SENTENCE_CASE_JUST_TYPED(KC_SPC, KC_F, KC_R, KC_E, KC_G, KC_DOT) ||
        SENTENCE_CASE_JUST_TYPED(KC_SPC, KC_H, KC_DOT) ||
        SENTENCE_CASE_JUST_TYPED(KC_SPC, KC_I, KC_B, KC_I, KC_D, KC_DOT) ||
        SENTENCE_CASE_JUST_TYPED(KC_SPC, KC_I, KC_D, KC_DOT) ||
        SENTENCE_CASE_JUST_TYPED(KC_SPC, KC_I, KC_L, KC_M, KC_A, KC_DOT) ||
        SENTENCE_CASE_JUST_TYPED(KC_SPC, KC_I, KC_L, KC_M, KC_O, KC_DOT) ||
        SENTENCE_CASE_JUST_TYPED(KC_SPC, KC_J, KC_A, KC_N, KC_DOT) ||
        SENTENCE_CASE_JUST_TYPED(KC_SPC, KC_J, KC_R, KC_DOT) ||
        SENTENCE_CASE_JUST_TYPED(KC_SPC, KC_J, KC_U, KC_L, KC_DOT) ||
        SENTENCE_CASE_JUST_TYPED(KC_SPC, KC_J, KC_U, KC_N, KC_DOT) ||
        SENTENCE_CASE_JUST_TYPED(KC_SPC, KC_L, KC_D, KC_A, KC_DOT) ||
        SENTENCE_CASE_JUST_TYPED(KC_SPC, KC_L, KC_T, KC_D, KC_A, KC_DOT) ||
        SENTENCE_CASE_JUST_TYPED(KC_SPC, KC_M, KC_A, KC_R, KC_DOT) ||
        SENTENCE_CASE_JUST_TYPED(KC_SPC, KC_M, KC_A, KC_S, KC_C, KC_DOT) ||
        SENTENCE_CASE_JUST_TYPED(KC_SPC, KC_M, KC_I, KC_S, KC_C, KC_DOT) ||
        SENTENCE_CASE_JUST_TYPED(KC_SPC, KC_M, KC_O, KC_N, KC_S, KC_DOT) ||
        SENTENCE_CASE_JUST_TYPED(KC_SPC, KC_M, KC_S, KC_DOT) ||
        SENTENCE_CASE_JUST_TYPED(KC_SPC, KC_M, KC_T, KC_O, KC_DOT) ||
        SENTENCE_CASE_JUST_TYPED(KC_SPC, KC_N, KC_O, KC_DOT) ||
        SENTENCE_CASE_JUST_TYPED(KC_SPC, KC_N, KC_O, KC_V, KC_DOT) ||
        SENTENCE_CASE_JUST_TYPED(KC_SPC, KC_O, KC_B, KC_G, KC_DOT) ||
        SENTENCE_CASE_JUST_TYPED(KC_SPC, KC_O, KC_P, KC_DOT) ||
        SENTENCE_CASE_JUST_TYPED(KC_SPC, KC_O, KC_U, KC_T, KC_DOT) ||
        SENTENCE_CASE_JUST_TYPED(KC_SPC, KC_P, KC_DOT, KC_E, KC_X, KC_DOT) ||
        SENTENCE_CASE_JUST_TYPED(KC_SPC, KC_P, KC_DOT, KC_F, KC_DOT) ||
        SENTENCE_CASE_JUST_TYPED(KC_SPC, KC_P, KC_DOT, KC_M, KC_DOT) ||
        SENTENCE_CASE_JUST_TYPED(KC_SPC, KC_P, KC_DOT, KC_P, KC_DOT) ||
        SENTENCE_CASE_JUST_TYPED(KC_SPC, KC_P, KC_E, KC_DOT) ||
        SENTENCE_CASE_JUST_TYPED(KC_SPC, KC_P, KC_G, KC_DOT) ||
        SENTENCE_CASE_JUST_TYPED(KC_SPC, KC_P, KC_O, KC_V, KC_DOT) ||
        SENTENCE_CASE_JUST_TYPED(KC_SPC, KC_P, KC_P, KC_DOT) ||
        SENTENCE_CASE_JUST_TYPED(KC_SPC, KC_P, KC_R, KC_O, KC_F, KC_DOT) ||
        SENTENCE_CASE_JUST_TYPED(KC_SPC, KC_Q, KC_A, KC_DOT) ||
        SENTENCE_CASE_JUST_TYPED(KC_SPC, KC_Q, KC_U, KC_A, KC_DOT) ||
        SENTENCE_CASE_JUST_TYPED(KC_SPC, KC_Q, KC_U, KC_I, KC_DOT) ||
        SENTENCE_CASE_JUST_TYPED(KC_SPC, KC_R, KC_DOT) ||
        SENTENCE_CASE_JUST_TYPED(KC_SPC, KC_R, KC_E, KC_F, KC_DOT) ||
        SENTENCE_CASE_JUST_TYPED(KC_SPC, KC_R, KC_E, KC_V, KC_D, KC_O, KC_DOT) ||
        SENTENCE_CASE_JUST_TYPED(KC_SPC, KC_R, KC_E, KC_V, KC_DOT) ||
        SENTENCE_CASE_JUST_TYPED(KC_SPC, KC_S, KC_A, KC_B, KC_DOT) ||
        SENTENCE_CASE_JUST_TYPED(KC_SPC, KC_S, KC_DOT, KC_F, KC_DOT, KC_F, KC_DOT) ||
        SENTENCE_CASE_JUST_TYPED(KC_SPC, KC_S, KC_E, KC_G, KC_DOT) ||
        SENTENCE_CASE_JUST_TYPED(KC_SPC, KC_S, KC_E, KC_X, KC_DOT) ||
        SENTENCE_CASE_JUST_TYPED(KC_SPC, KC_S, KC_R, KC_A, KC_DOT) ||
        SENTENCE_CASE_JUST_TYPED(KC_SPC, KC_S, KC_R, KC_DOT) ||
        SENTENCE_CASE_JUST_TYPED(KC_SPC, KC_S, KC_R, KC_T, KC_A, KC_DOT) ||
        SENTENCE_CASE_JUST_TYPED(KC_SPC, KC_S, KC_T, KC_A, KC_DOT) ||
        SENTENCE_CASE_JUST_TYPED(KC_SPC, KC_S, KC_T, KC_O, KC_DOT) ||
        SENTENCE_CASE_JUST_TYPED(KC_SPC, KC_T, KC_E, KC_L, KC_DOT) ||
        SENTENCE_CASE_JUST_TYPED(KC_SPC, KC_T, KC_E, KC_R, KC_DOT) ||
        SENTENCE_CASE_JUST_TYPED(KC_SPC, KC_T, KC_I, KC_M, KC_DOT) ||
        SENTENCE_CASE_JUST_TYPED(KC_SPC, KC_T, KC_L, KC_M, KC_DOT) ||
        SENTENCE_CASE_JUST_TYPED(KC_SPC, KC_V, KC_D, KC_DOT) ||
        SENTENCE_CASE_JUST_TYPED(KC_SPC, KC_V, KC_DOT) ||
        SENTENCE_CASE_JUST_TYPED(KC_SPC, KC_V, KC_S, KC_DOT)) {
        return false; // Not a real sentence ending.
    }
    return true; // Real sentence ending; capitalize next letter.
}

/*
#################
### Caps Word ###
#################
*/

bool caps_word_press_user(uint16_t keycode) {
    switch (keycode) {
        // Keycodes that continue Caps Word, with shift applied.
        case KC_A ... KC_Z:
        case UM_LT5:
        case KC_MINS:
            add_weak_mods(MOD_BIT(KC_LSFT)); // Apply shift to next key.
            return true;

        // Keycodes that continue Caps Word, without shifting.
        case KC_1 ... KC_0:
        case KC_QUOTE:
        case KC_DQUO:
        case KC_GRAVE:
        case KC_BACKSPACE:
        case KC_DEL:
        case KC_UNDS:
        case KC_CIRC:
        case KC_KP_SLASH:
        case KC_TILD:
        case UM_RB1:
        case UM_RB4:
            return true;

        default:
            return false; // Deactivate Caps Word.
    }
}

/*
###########################
### Tapping and Holding ###
###########################
*/

// Set tapping term per key.
uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case UM_LT5:
        case UM_LH1:
        case UM_RB1:
            return 175;
        default:
            return TAPPING_TERM;
    }
}

uint16_t get_quick_tap_term(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case UM_LH3:
        case UM_RH2:
        case UM_RH3:
            return 120;
        default:
            return QUICK_TAP_TERM; // This is set to zero since I don't want other holds to have key repetition.
    }
}

// Define which keys are on which side for Chordal Hold
const char chordal_hold_layout[MATRIX_ROWS][MATRIX_COLS] PROGMEM =
    LAYOUT_split_3x5_3(
        'L', 'L', 'L', 'L', 'L',  'R', 'R', 'R', 'R', 'R',
        'L', 'L', 'L', 'L', 'L',  'R', 'R', 'R', 'R', 'R',
        'L', 'L', 'L', 'L', 'L',  'R', 'R', 'R', 'R', 'R',
                          'L', '*', '*',  'R', 'R', 'R'
    );

uint16_t get_flow_tap_term(uint16_t keycode, keyrecord_t* record, uint16_t prev_keycode) {
    if (is_flow_tap_key(keycode) && is_flow_tap_key(prev_keycode)) {
        switch (keycode) {
            case UM_LH3:
            case UM_LH1:
            case UM_RH3:
            case UM_RH2:
            case UM_RH1:
                return 0; // Short timeout on these keys.

            default:
                return FLOW_TAP_TERM; // Longer timeout otherwise.
        }
    }
    return 0; // Disable Flow Tap.
}

/*
#####################
### Alternate Key ###
#####################
*/

uint16_t get_alt_repeat_key_keycode_user(uint16_t keycode, uint8_t mods) {
    bool shifted = (mods & MOD_MASK_SHIFT); // Was Shift held?
    switch (keycode) {
        case UM_LH1:
            if (shifted) {        // If the last key was Shift + Tab,
                return KC_TAB;    // the reverse is Tab.
            } else {              // Otherwise, the last key was Tab,
                return S(KC_TAB); // and the reverse is Shift + Tab.
            }
            break;
        case UM_LM5: MAGIC_STRING(/*s*/"sion"); break;
        case UM_LM4: MAGIC_STRING(/*n*/"ion"); break;
        case UM_LM3: MAGIC_STRING(/*t*/"heir"); break;
        case UM_LB4: MAGIC_STRING(/*w*/"hich"); break;
        case UM_LB2: MAGIC_STRING(/*m*/"ent"); break;
        case UM_RT5: MAGIC_STRING(/*b*/"ecause"); break;
        case UM_RM2: MAGIC_STRING(/*a*/"tion"); break;
        case UM_RM4: MAGIC_STRING(/*i*/"tion"); break;
        case UM_RH2:
            if (is_sentence_case_primed()) {
                SEND_STRING(/* */"And");
                sentence_case_clear();
            } else {
                MAGIC_STRING(/* */"and");
            }
            break;
        case U_QU:
            tap_code16(KC_CIRCUMFLEX);
            if (is_caps_word_on()) {
                tap_code16(S(KC_E));
            } else {
                tap_code(KC_E);
            }
            break;
    }
    return KC_NO;
}

bool remember_last_key_user(uint16_t keycode, keyrecord_t* record, uint8_t* remembered_mods) {
    if (keycode == UM_RT1) { return false; }
    return true;
}

/*
########################
### One-Shot Mod-Tap ###
########################
*/

static bool oneshot_mod_tap(uint16_t keycode, keyrecord_t* record) {
    if (record->tap.count == 0) { // Key is being held.
        if (record->event.pressed) {
            const uint8_t mods = (keycode >> 8) & 0x1f;
            add_oneshot_mods(((mods & 0x10) == 0) ? mods : (mods << 4));
        }
        return false; // Skip default handling.
    }
    return true; // Continue default handling.
}

/*
######################
### One-Shot Layer ###
######################
*/

// Workaround to make Sentence Case work with OSL
bool temp_shift = false;

void oneshot_layer_changed_user(uint8_t layer) {
  if (!layer) {
    if (temp_shift) {
        del_mods(MOD_MASK_SHIFT);
        temp_shift = false;
    }
  }
}

/*
###########################
### Process Record User ###
###########################
*/

bool process_record_user(uint16_t keycode, keyrecord_t* record) {
    // On every key event, start or extend the deferred execution to call `idle_callback()` after IDLE_TIMEOUT_MS.
    static deferred_token idle_token = INVALID_DEFERRED_TOKEN;
    if (!extend_deferred_exec(idle_token, IDLE_TIMEOUT_MS)) {
        idle_token = defer_exec(IDLE_TIMEOUT_MS, idle_callback, NULL);
    }
    // Restore the RGB matrix when returning from idle.
    if (!rgb_matrix_is_enabled() && !rgb_disabled_manually) {
        rgb_matrix_enable();
    }
    uint8_t mod_state = get_mods();
    tap_dance_action_t *action;
    switch (keycode) {
        // One-shot mod-taps
        case UM_RM2:
            return oneshot_mod_tap(keycode, record);
        // Advanced tap dances
        case TD_PGUP:
            action = &tap_dance_actions[QK_TAP_DANCE_GET_INDEX(keycode)];
            if (!record->event.pressed && action->state.count && !action->state.finished) {
                tap_dance_tap_hold_t *tap_hold = (tap_dance_tap_hold_t *)action->user_data;
                tap_code16(tap_hold->tap);
            }
            break;
        case TD_PGDN:
            action = &tap_dance_actions[QK_TAP_DANCE_GET_INDEX(keycode)];
            if (!record->event.pressed && action->state.count && !action->state.finished) {
                tap_dance_tap_hold_t *tap_hold = (tap_dance_tap_hold_t *)action->user_data;
                tap_code16(tap_hold->tap);
            }
            break;
        case TD_HOME:
            action = &tap_dance_actions[QK_TAP_DANCE_GET_INDEX(keycode)];
            if (!record->event.pressed && action->state.count && !action->state.finished) {
                tap_dance_tap_hold_t *tap_hold = (tap_dance_tap_hold_t *)action->user_data;
                tap_code16(tap_hold->tap);
            }
            break;
        case TD_END:
            action = &tap_dance_actions[QK_TAP_DANCE_GET_INDEX(keycode)];
            if (!record->event.pressed && action->state.count && !action->state.finished) {
                tap_dance_tap_hold_t *tap_hold = (tap_dance_tap_hold_t *)action->user_data;
                tap_code16(tap_hold->tap);
            }
            break;
        // Intercepts
        case UM_LT5:
            if (!record->tap.count && record->event.pressed) {
                if (is_caps_word_on() || is_sentence_case_primed()) {
                    tap_code16(S(KC_Q));
                    sentence_case_clear();
                } else {
                    tap_code(KC_Q);
                }
                return false;
            }
            break;
        case UM_RB1:
            if (!record->tap.count && record->event.pressed) {
                tap_code16(KC_COLN);
                return false;
            }
            break;
        case UM_RB4:
            if (record->event.pressed) {
                if (mod_state & MOD_MASK_SHIFT) {
                    clear_mods();
                    tap_code16(KC_DQUO);
                    tap_code(KC_SPACE);
                    set_mods(mod_state);
                    return false;
                } else if (mod_state & MOD_MASK_ALT) {
                    clear_mods();
                    tap_code(KC_GRAVE);
                    tap_code(KC_SPACE);
                    set_mods(mod_state);
                    return false;
                } else {
                    if (is_sentence_case_primed()) {
                        sentence_case_clear();
                        add_mods(MOD_MASK_SHIFT);
                        temp_shift = true;
                    }
                }
            }
            break;
        // Macros
        case U_QU:
            if (record->event.pressed) {
                MAGIC_STRING("qu");
                return false;
            }
            break;
        case U_RGB_T:
            if (record->event.pressed) {
                if (rgb_matrix_is_enabled()) {
                    rgb_disabled_manually = true;
                    rgb_matrix_disable();
                } else {
                    rgb_disabled_manually = false;
                    rgb_matrix_enable();
                }
            }
            break;
        case U_RGB_R:
            rgb_matrix_sethsv(21, 255, 190);
            break;
        case U_CIRC:
            if (record->event.pressed) {
                clear_mods();
                tap_code16(KC_CIRC);
                tap_code(KC_SPACE);
                set_mods(mod_state);
            }
            break;
        case U_TILDE:
            if (record->event.pressed) {
                clear_mods();
                tap_code16(KC_TILDE);
                tap_code(KC_SPACE);
                set_mods(mod_state);
            }
            break;
        case U_QUOTE:
            if (record->event.pressed) {
                clear_mods();
                tap_code(KC_QUOTE);
                tap_code(KC_SPACE);
                set_mods(mod_state);
            }
            break;
        // Accented characters
        case RSFT_T(KC_H):
            if (record->tap.count && record->event.pressed) {
                clear_mods();
                tap_code(KC_QUOTE);
                set_mods(mod_state);
                MAGIC_STRING("a");
                return false;
            }
            break;
        case U_AC_E:
            if (record->event.pressed) {
                clear_mods();
                tap_code(KC_QUOTE);
                set_mods(mod_state);
                MAGIC_STRING("e");
                return false;
            }
            break;
        case U_AC_I:
            if (record->event.pressed) {
                clear_mods();
                tap_code(KC_QUOTE);
                set_mods(mod_state);
                MAGIC_STRING("i");
                return false;
            }
            break;
        case U_AC_O:
            if (record->event.pressed) {
                clear_mods();
                tap_code(KC_QUOTE);
                set_mods(mod_state);
                MAGIC_STRING("o");
                return false;
            }
            break;
        case U_AC_U:
            if (record->event.pressed) {
                clear_mods();
                tap_code(KC_QUOTE);
                set_mods(mod_state);
                MAGIC_STRING("u");
                return false;
            }
            break;
        case U_GR_A:
            if (record->event.pressed) {
                clear_mods();
                tap_code(KC_GRAVE);
                set_mods(mod_state);
                MAGIC_STRING("a");
                return false;
            }
            break;
        case LSFT_T(KC_A):
            if (record->tap.count && record->event.pressed) {
                clear_mods();
                tap_code16(KC_CIRC);
                set_mods(mod_state);
                MAGIC_STRING("a");
                return false;
            }
            break;
        case U_CIRC_E:
            if (record->event.pressed) {
                clear_mods();
                tap_code16(KC_CIRC);
                set_mods(mod_state);
                MAGIC_STRING("e");
                return false;
            }
            break;
        case U_CIRC_O:
            if (record->event.pressed) {
                clear_mods();
                tap_code16(KC_CIRC);
                set_mods(mod_state);
                MAGIC_STRING("o");
                return false;
            }
            break;
        case U_TILDE_A:
            if (record->event.pressed) {
                clear_mods();
                tap_code16(KC_TILD);
                set_mods(mod_state);
                MAGIC_STRING("a");
                return false;
            }
            break;
        case U_TILDE_O:
            if (record->event.pressed) {
                clear_mods();
                tap_code16(KC_TILD);
                set_mods(mod_state);
                MAGIC_STRING("o");
                return false;
            }
            break;
        case U_TILDE_N:
            if (record->event.pressed) {
                clear_mods();
                tap_code16(KC_TILD);
                set_mods(mod_state);
                MAGIC_STRING("n");
                return false;
            }
            break;
    }
    return true;
}

/*
###############
### Keymaps ###
###############
*/

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

/* Base
    ,———————————————————————————————————————.                   ,———————————————————————————————————————.
    |   f   |   p   |   d   |   l   |   x   |                   | Magic |   u   |   o   |   y   |   b   |
    |———————+———————+———————+———————+———————|                   |———————+———————+———————+———————+———————|
    |   s   |   n   |   t   |   h   |   k   |                   |   -   |   a   |   e   |   i   |   c   |
    |———————+———————+———————+———————+———————|                   |———————+———————+———————+———————+———————|
    |   v   |   w   |   g   |   m   |   j   |                   |   /   |   .   |   ,   |  EXT  |   z   |
    `———————————————————————+———————+———————+———————.   ,———————+———————+———————+———————————————————————'
                            |  Bspc |   r   |  Tab  |   | Enter | Space |  Del  |
                            `———————————————————————'   `———————————————————————'
*/

    [_BASE] = LAYOUT_split_3x5_3(
        UM_LT5, UM_LT4, UM_LT3, UM_LT2, UM_LT1,    UM_RT1, UM_RT2, UM_RT3, UM_RT4, UM_RT5,
        UM_LM5, UM_LM4, UM_LM3, UM_LM2, UM_LM1,    UM_RM1, UM_RM2, UM_RM3, UM_RM4, UM_RM5,
        UM_LB5, UM_LB4, UM_LB3, UM_LB2, UM_LB1,    UM_RB1, UM_RB2, UM_RB3, UM_RB4, UM_RB5,
                                UM_LH3, UM_LH2, UM_LH1,    UM_RH1, UM_RH2, UM_RH3
    ),

/* Navigation
    ,———————————————————————————————————————.                   ,———————————————————————————————————————.
    |  ___  |  ___  |  ___  |  ___  |  ___  |                   | TabUp |  Home |   Up  |  End  |  PgUp |
    |———————+———————+———————+———————+———————|                   |———————+———————+———————+———————+———————|
    |  ___  |  ___  |  ___  |  ___  |  ___  |                   | TabDo |  Left |  Down | Right |  PgDo |
    |———————+———————+———————+———————+———————|                   |———————+———————+———————+———————+———————|
    |  ___  |  ___  |  ___  |  ___  |  ___  |                   |       |SpcLeft|SelWord|SpcRght|       |
    `———————————————————————+———————+———————+———————.   ,———————+———————+———————+———————————————————————'
                            |       |  OOO  |       |   |Confirm|  RayC |  Undo |
                            `———————————————————————'   `———————————————————————'
*/

    [_NAV] = LAYOUT_split_3x5_3(
        _______, _______, _______, _______, _______,    TAB_UP,    TD_HOME,  KC_UP,   TD_END,    TD_PGUP,
        _______, _______, _______, _______, _______,    TAB_DOWN,  KC_LEFT,  KC_DOWN, KC_RIGHT,  TD_PGDN,
        _______, _______, _______, _______, _______,    XXXXXXX,   SPC_LEFT, SELWORD, SPC_RIGHT, XXXXXXX,
                                  XXXXXXX, XXXXXXX, XXXXXXX,    G(KC_ENT), RAYCAST,  UNDO
    ),

/* Mouse and Media Keys
    ,———————————————————————————————————————.                   ,———————————————————————————————————————.
    |  ___  |  ___  |  ___  |  ___  |  ___  |                   |  MWDn |  MB4  |  MUp  |  MB5  |       |
    |———————+———————+———————+———————+———————|                   |———————+———————+———————+———————+———————|
    |  ___  |  ___  |  ___  |  ___  |  ___  |                   |  MWUp | MLeft | MDown | MRight|       |
    |———————+———————+———————+———————+———————|                   |———————+———————+———————+———————+———————|
    |  ___  |  ___  |  ___  |  ___  |  ___  |                   |  Play | VolDo | VolUp |  Prev |  Next |
    `———————————————————————+———————+———————+———————.   ,———————+———————+———————+———————————————————————'
                            |  OOO  |       |       |   |  MB1  |  MB3  |  MB2  |
                            `———————————————————————'   `———————————————————————'
*/

    [_MOU] = LAYOUT_split_3x5_3(
        _______, _______, _______, _______, _______,    MS_WHLD, MS_BTN4, MS_UP,   MS_BTN5, XXXXXXX,
        _______, _______, _______, _______, _______,    MS_WHLU, MS_LEFT, MS_DOWN, MS_RGHT, XXXXXXX,
        _______, _______, _______, _______, _______,    KC_MPLY, KC_VOLD, KC_VOLU, KC_MPRV, KC_MNXT,
                                  XXXXXXX, XXXXXXX, XXXXXXX,    MS_BTN1, MS_BTN3, MS_BTN2
    ),

/* Numbers
    ,———————————————————————————————————————.                   ,———————————————————————————————————————.
    |   /   |   7   |   8   |   9   |   *   |                   |  ___  |  ___  |  ___  |  ___  |  ___  |
    |———————+———————+———————+———————+———————|                   |———————+———————+———————+———————+———————|
    |   -   |   4   |   5   |   6   |   +   |                   |  ___  |  ___  |  ___  |  ___  |  ___  |
    |———————+———————+———————+———————+———————|                   |———————+———————+———————+———————+———————|
    |   %   |   1   |   2   |   3   |   =   |                   |  ___  |  ___  |  ___  |  ___  |  ___  |
    `———————————————————————+———————+———————+———————.   ,———————+———————+———————+———————————————————————'
                            |  Bspc |   0   | LLock |   |       |  OOO  |       |
                            `———————————————————————'   `———————————————————————'
*/

    [_NUM] = LAYOUT_split_3x5_3(
        KC_PSLS, KC_7, KC_8,    KC_9,  KC_PAST,    _______, _______, _______, _______, _______,
        KC_PMNS, KC_4, KC_5,    KC_6,  KC_PPLS,    _______, _______, _______, _______, _______,
        KC_PERC, KC_1, KC_2,    KC_3,  KC_PEQL,    _______, _______, _______, _______, _______,
                               KC_BSPC, KC_P0, QK_LLCK,    XXXXXXX, XXXXXXX, XXXXXXX
    ),

/* Symbols
    ,———————————————————————————————————————.                   ,———————————————————————————————————————.
    |       |   \   |   <   |   >   |      |                   |  ___  |  ___  |  ___  |  ___  |  ___  |
    |———————+———————+———————+———————+———————|                   |———————+———————+———————+———————+———————|
    |       |   |   |   [   |   ]   |   $   |                   |  ___  | Shift |  ___  |  ___  |  ___  |
    |———————+———————+———————+———————+———————|                   |———————+———————+———————+———————+———————|
    |       |   ^   |   {   |   }   |   £   |                   |  ___  |  ___  |  ___  |  ___  |  ___  |
    `———————————————————————+———————+———————+———————.   ,———————+———————+———————+———————————————————————'
                            |   &   |   #   |   €   |   |  OOO  |       |       |
                            `———————————————————————'   `———————————————————————'
*/

    [_SYM] = LAYOUT_split_3x5_3(
        XXXXXXX, KC_BSLS, KC_LT,   KC_GT,   A(S(KC_K)),    _______, _______, _______, _______, _______,
        XXXXXXX, KC_PIPE, KC_LBRC, KC_RBRC, KC_DLR,        _______, KC_RSFT, _______, _______, _______,
        XXXXXXX, U_CIRC,  KC_LCBR, KC_RCBR, A(KC_3),       _______, _______, _______, _______, _______,
                                  KC_AMPR, KC_HASH, A(KC_AT),      XXXXXXX, XXXXXXX, XXXXXXX
    ),

/* Function
    ,———————————————————————————————————————.                   ,———————————————————————————————————————.
    |  F12  |   F7  |   F8  |   F9  | PrScr |                   | RMNxt | RMHUp |  RMUp |  ___  |  ___  |
    |———————+———————+———————+———————+———————|                   |———————+———————+———————+———————+———————|
    |  F11  |   F4  |   F5  |   F6  |       |                   |RMReset|RMHDown| RMDown| ACTog | Flash |
    |———————+———————+———————+———————+———————|                   |———————+———————+———————+———————+———————|
    |  F10  |   F1  |   F2  |   F3  | RMTog |                   |  ___  |  ___  |  ___  |  ___  |  ___  |
    `———————————————————————+———————+———————+———————.   ,———————+———————+———————+———————————————————————'
                            | Leader|  Caps |  Lock |   |       |       |  OOO  |
                            `———————————————————————'   `———————————————————————'
    */

    [_FUN] = LAYOUT_split_3x5_3(
        KC_F12, KC_F7, KC_F8,   KC_F9,   PRT_SCR,     RM_NEXT, RM_HUEU, RM_VALU, XXXXXXX, XXXXXXX,
        KC_F11, KC_F4, KC_F5,   KC_F6,   XXXXXXX,     U_RGB_R, RM_HUED, RM_VALD, AC_TOGG, QK_BOOT,
        KC_F10, KC_F1, KC_F2,   KC_F3,   U_RGB_T,     XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
                               QK_LEAD, KC_CAPS, LOCK_SCR,    XXXXXXX, XXXXXXX, XXXXXXX
    ),

/* Extended
    ,———————————————————————————————————————.                   ,———————————————————————————————————————.
    |  ___  |  ___  |   ô   |  ___  |  ___  |                   |  ___  |   ú   |   ó   |  ___  |  ___  |
    |———————+———————+———————+———————+———————|                   |———————+———————+———————+———————+———————|
    |  ___  |   ñ   |   ê   |   â   |  ___  |                   |  ___  |   á   |   é   |   í   |  ___  |
    |———————+———————+———————+———————+———————|                   |———————+———————+———————+———————+———————|
    |  ___  |  ___  |  ___  |  ___  |  ___  |                   |  ___  |   ã   |   õ   |   '   |  ___  |
    `———————————————————————+———————+———————+———————.   ,———————+———————+———————+———————————————————————'
                            |  ___  |   à   |  ___  |   |  ___  |  ___  |  ___  |
                            `———————————————————————'   `———————————————————————'
*/

    [_EXT] = LAYOUT_split_3x5_3(
        _______, _______,   U_CIRC_O, _______,      _______,    _______, U_AC_U,       U_AC_O,    _______, _______,
        _______, U_TILDE_N, U_CIRC_E, LSFT_T(KC_A), _______,    _______, RSFT_T(KC_H), U_AC_E,    U_AC_I,  _______,
        _______, _______,   _______,  _______,      _______,    _______, U_TILDE_A,    U_TILDE_O, U_QUOTE, _______,
                                    _______,  U_GR_A,       _______,    _______, _______,      _______
    ),
};
