#include "action.h"
#include "modifiers.h"
#include QMK_KEYBOARD_H
#include "features/achordion.h"
#include "features/sentence_case.h"
#include "features/select_word.h"

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
#define _EXT 1
#define _NAV 2
#define _MOU 3
#define _NUM 4
#define _SYM 5
#define _FUN 6

// Mod taps
#define MT_QF LT(0, KC_F)

// Tap dances
#define TD_DOT TD(DOT)
#define TD_PGUP TD(PGUP)
#define TD_PGDN TD(PGDOWN)
#define TD_HOME TD(HOME)
#define TD_END TD(END)
#define TD_AA TD(U_TD_AA)
#define TD_AE TD(U_TD_AE)
#define TD_AO TD(U_TD_AO)

// Key matrix assigments

/*
    ,———————————————————————————————————————.    ,———————————————————————————————————————.
    |  LT5  |  LT4  |  LT3  |  LT2  |  LT1  |    |  RT1  |  RT2  |  RT3  |  RT4  |  RT5  |
    |———————+———————+———————+———————+———————|    |———————+———————+———————+———————+———————|
    |  LM5  |  LM4  |  LM3  |  LM2  |  LM1  |    |  RM1  |  RM2  |  RM3  |  RM4  |  RM5  |
    |———————+———————+———————+———————+———————|    |———————+———————+———————+———————+———————|
    |  LB5  |  LB4  |  LB3  |  LB2  |  LB1  |    |  RB1  |  RB2  |  RB3  |  RB4  |  RB5  |
    `———————+———————+———————+———————+———————|    |———————+———————+———————+———————+———————'
                    |  LH3  |  LH2  |  LH1  |    |  RH2  |  RH2  |  RH3  |
                    `———————————————————————'    `———————————————————————'
*/

#define UM_LT5 MT_QF
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
#define UM_LB2 LT(_MOU, KC_M)
#define UM_LB1 KC_J

#define UM_LH3 LT(0, KC_BACKSPACE)  // hold is intercepted to output Escape
#define UM_LH2 LT(_NAV, KC_R)
#define UM_LH1 LT(_EXT, KC_TAB)

#define UM_RT1 KC_DELETE
#define UM_RT2 HYPR_T(KC_U)
#define UM_RT3 KC_O
#define UM_RT4 KC_Y
#define UM_RT5 KC_B

#define UM_RM1 KC_MINUS
#define UM_RM2 RSFT_T(KC_A)
#define UM_RM3 RGUI_T(KC_E)
#define UM_RM4 LALT_T(KC_I)
#define UM_RM5 RCTL_T(KC_C)

#define UM_RB1 KC_COLN
#define UM_RB2 KC_DOT
#define UM_RB3 KC_COMMA
#define UM_RB4 U_QUOTE
#define UM_RB5 KC_KP_SLASH

#define UM_RH1 LT(_SYM, KC_ENTER)
#define UM_RH2 LT(_NUM, KC_SPACE)
#define UM_RH3 LT(_FUN, KC_BACKSPACE)  // tap is intercepted to output Alternate Repeat

// Settings
#define IDLE_TIMEOUT_MS 600000 // Idle timeout in milliseconds.

// Custom keycodes
enum custom_keycodes {
    BASE = SAFE_RANGE,
    EXT,
    NAV,
    MOU,
    NUM,
    SYM,
    FUN,
    SELWORD,
    U_QU,  // "Qu"
    U_GR_A, U_TIL_A, U_TIL_O, // accented characters
    U_AC_I, U_AC_U, // accented characters
    U_CC, // "ç"
    U_QUOTE, U_TILDE, U_CIRC, // accents with space to never act like dead keys
    U_MP, U_PS, // "adaptives"
    U_RGB_T  // macro for RGB toggle with extra info
};

// Tap Dance stuff.
enum tap_dances {
    PGUP,
    PGDOWN,
    HOME,
    END,
    U_TD_AA,
    U_TD_AE,
    U_TD_AO,
};

// Select Word keycode.
uint16_t SELECT_WORD_KEYCODE = SELWORD;

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

// Code for tap dances with advanded keycodes
void an_taps(tap_dance_state_t *state, void *user_data) {
    uint8_t mod_state = get_mods();
    if (state->count == 1) {
        if (is_caps_word_on()) {
            clear_mods();
            clear_weak_mods();
            tap_code(KC_QUOTE);
            set_mods(mod_state);
            tap_code16(S(KC_A));
            caps_word_on();
        } else {
            clear_mods();
            clear_weak_mods();
            tap_code(KC_QUOTE);
            set_mods(mod_state);
            tap_code(KC_A);
        }
    } else if (state->count == 2) {
        if (is_caps_word_on()) {
            clear_mods();
            clear_weak_mods();
            tap_code16(KC_CIRCUMFLEX);
            set_mods(mod_state);
            tap_code16(S(KC_A));
            caps_word_on();
        } else {
            clear_mods();
            clear_weak_mods();
            tap_code16(KC_CIRCUMFLEX);
            set_mods(mod_state);
            tap_code(KC_A);
        }
    } else {
        reset_tap_dance(state);
    }
    layer_clear();  // I never want two accented characters in a row
}

void ae_taps(tap_dance_state_t *state, void *user_data) {
    uint8_t mod_state = get_mods();
    if (state->count == 1) {
        if (is_caps_word_on()) {
            clear_mods();
            clear_weak_mods();
            tap_code(KC_QUOTE);
            set_mods(mod_state);
            tap_code16(S(KC_E));
            caps_word_on();
        } else {
            clear_mods();
            clear_weak_mods();
            tap_code(KC_QUOTE);
            set_mods(mod_state);
            tap_code(KC_E);
        }
    } else if (state->count == 2) {
        if (is_caps_word_on()) {
            clear_mods();
            clear_weak_mods();
            tap_code16(KC_CIRCUMFLEX);
            set_mods(mod_state);
            tap_code16(S(KC_E));
            caps_word_on();
        } else {
            clear_mods();
            clear_weak_mods();
            tap_code16(KC_CIRCUMFLEX);
            set_mods(mod_state);
            tap_code(KC_E);
        }
    } else {
        reset_tap_dance(state);
    }
    layer_clear();  // I never want two accented characters in a row
}

void ao_taps(tap_dance_state_t *state, void *user_data) {
    uint8_t mod_state = get_mods();
    if (state->count == 1) {
        if (is_caps_word_on()) {
            clear_mods();
            clear_weak_mods();
            tap_code(KC_QUOTE);
            set_mods(mod_state);
            tap_code16(S(KC_O));
            caps_word_on();
        } else {
            clear_mods();
            clear_weak_mods();
            tap_code(KC_QUOTE);
            set_mods(mod_state);
            tap_code(KC_O);
        }
    } else if (state->count == 2) {
        if (is_caps_word_on()) {
            clear_mods();
            clear_weak_mods();
            tap_code16(KC_CIRCUMFLEX);
            set_mods(mod_state);
            tap_code16(S(KC_O));
            caps_word_on();
        } else {
            clear_mods();
            clear_weak_mods();
            tap_code16(KC_CIRCUMFLEX);
            set_mods(mod_state);
            tap_code(KC_O);
        }
    } else {
        reset_tap_dance(state);
    }
    layer_clear();  // I never want two accented characters in a row
}

// Definition for each tap dance using the functions above.
tap_dance_action_t tap_dance_actions[] = {
    [PGUP] = ACTION_TAP_DANCE_TAP_HOLD(KC_PGUP, G(KC_UP)),
    [PGDOWN] = ACTION_TAP_DANCE_TAP_HOLD(KC_PGDN, G(KC_DOWN)),
    [HOME] = ACTION_TAP_DANCE_TAP_HOLD(A(KC_LEFT), KC_HOME),
    [END] = ACTION_TAP_DANCE_TAP_HOLD(A(KC_RIGHT), KC_END),
    [U_TD_AA] = ACTION_TAP_DANCE_FN(an_taps),
    [U_TD_AE] = ACTION_TAP_DANCE_FN(ae_taps),
    [U_TD_AO] = ACTION_TAP_DANCE_FN(ao_taps),
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

const uint16_t PROGMEM percentage_combo[] = {UM_LM3, UM_LB3, COMBO_END};
const uint16_t PROGMEM at_combo[] = {UM_LM2, UM_LB2, COMBO_END};

// Right-side vertical combos.
const uint16_t PROGMEM lprn_combo[] = {UM_RT2, UM_RM2, COMBO_END};
const uint16_t PROGMEM rprn_combo[] = {UM_RT3, UM_RM3, COMBO_END};
const uint16_t PROGMEM super_o_combo[] = {UM_RT4, UM_RM4, COMBO_END};

// Left-side horizontal combos.
const uint16_t PROGMEM caps_word_combo[] = {UM_LM2, UM_LM1, COMBO_END};
const uint16_t PROGMEM z_combo[] = {UM_LB4, UM_LB3, COMBO_END};
const uint16_t PROGMEM qu_combo[] = {UM_LB3, UM_LB2, COMBO_END};

// Right-side horizontal combos.
const uint16_t PROGMEM tilde_combo[] = {UM_RM1, UM_RM2, COMBO_END};
const uint16_t PROGMEM semicolon_combo[] = {UM_RB2, UM_RB3, COMBO_END};

// Combos for "adaptives"
const uint16_t PROGMEM mw_mp_combo[] = {UM_LB2, UM_LB4, COMBO_END};
const uint16_t PROGMEM pf_ps_combo[] = {UM_LT5, UM_LT4, COMBO_END};

// Media combos
const uint16_t PROGMEM mute_combo[] = {KC_VOLD, KC_VOLU, COMBO_END};

enum combos {
    CUT_COMBO,
    COPY_COMBO,
    PASTE_COMBO,
    CLIP_HIST_COMBO,
    PERCENTAGE_COMBO,
    AT_COMBO,
    LPRN_COMBO,
    RPRN_COMBO,
    SUPER_O_COMBO,
    CAPS_WORD_COMBO,
    Z_COMBO,
    QU_COMBO,
    TILDE_COMBO,
    SEMICOLON_COMBO,
    MW_MP_COMBO,
    PF_PS_COMBO,
    MUTE_COMBO
  };

// Used combos.
combo_t key_combos[] = {
    [CUT_COMBO] = COMBO(cut_combo, CUT),
    [COPY_COMBO] = COMBO(copy_combo, COPY),
    [PASTE_COMBO] = COMBO(paste_combo, PASTE),
    [CLIP_HIST_COMBO] = COMBO(clip_hist_combo, CLIP_HIST),
    [PERCENTAGE_COMBO] = COMBO(percentage_combo, KC_PERC),
    [AT_COMBO] = COMBO(at_combo, KC_AT),
    [LPRN_COMBO] = COMBO(lprn_combo, KC_LPRN),
    [RPRN_COMBO] = COMBO(rprn_combo, KC_RPRN),
    [SUPER_O_COMBO] = COMBO(super_o_combo, A(KC_0)),
    [CAPS_WORD_COMBO] = COMBO(caps_word_combo, CW_TOGG),
    [Z_COMBO] = COMBO(z_combo, KC_Z),
    [QU_COMBO] = COMBO(qu_combo, U_QU),
    [TILDE_COMBO] = COMBO(tilde_combo, U_TILDE),
    [SEMICOLON_COMBO] = COMBO(semicolon_combo, KC_SEMICOLON),
    [MW_MP_COMBO] = COMBO(mw_mp_combo, U_MP),
    [PF_PS_COMBO] = COMBO(pf_ps_combo, U_PS),
    [MUTE_COMBO] = COMBO(mute_combo, KC_MUTE),
};

/*
#####################
### Key Overrides ###
#####################
*/

// Key overrides.
const key_override_t super_a = ko_make_with_layers_and_negmods(MOD_MASK_SHIFT, A(KC_0), A(KC_9), ~0, MOD_MASK_CAG);
const key_override_t exclamation = ko_make_with_layers_and_negmods(MOD_MASK_SHIFT, KC_DOT, KC_EXLM, ~0, MOD_MASK_CAG);
const key_override_t inv_exclamation = ko_make_with_layers_and_negmods(MOD_MASK_SA, KC_DOT, A(KC_1), ~0, MOD_MASK_CG);
const key_override_t ellipsis = ko_make_with_layers_and_negmods(MOD_MASK_ALT, KC_DOT, A(KC_SCLN), ~0, MOD_MASK_CSG);
const key_override_t question = ko_make_with_layers_and_negmods(MOD_MASK_SHIFT, KC_COMMA, KC_QUES, ~0, MOD_MASK_CAG);
const key_override_t inv_question = ko_make_with_layers_and_negmods(MOD_MASK_SA, KC_COMMA, A(S(KC_SLSH)), ~0, MOD_MASK_CG);
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
    &close_fancy_double_quote
};

/*
##################
### Leader Key ###
##################
*/

void leader_start_user(void) {
    // Do something when the leader key is pressed
}

void leader_end_user(void) {
    if (leader_sequence_one_key(KC_T)) {
        // Leader, t => Process one segment
        tap_code16(G(KC_A));
        wait_ms(500);
        tap_code16(G(A(KC_C)));
        wait_ms(1000);
        tap_code(KC_ENTER);
    } else if (leader_sequence_two_keys(KC_T, KC_T)) {
        // Leader, t, t => Process ten segments
        for (int i = 0; i < 10; i++) {
            tap_code16(G(KC_A));
            wait_ms(500);
            tap_code16(G(A(KC_C)));
            wait_ms(1000);
            tap_code(KC_ENTER);
            wait_ms(500);
            tap_code(KC_DOWN);
        }
    } else if (leader_sequence_three_keys(KC_T, KC_T, KC_T)) {
        // Leader, t, t, t => Process fifty segments
        for (int i = 0; i < 50; i++) {
            tap_code16(G(KC_A));
            wait_ms(500);
            tap_code16(G(A(KC_C)));
            wait_ms(1000);
            tap_code(KC_ENTER);
            wait_ms(500);
            tap_code(KC_DOWN);
        }
    }
}

// void leader_end_user(void) {
//     if (leader_sequence_one_key(KC_T)) {
//         // Leader, t => Process one segment
//         tap_code16(G(KC_A));
//         wait_ms(500);
//         tap_code16(G(S(KC_C)));
//         tap_code16(G(S(KC_C)));
//         wait_ms(500);
//         tap_code(KC_ENTER);
//     } else if (leader_sequence_two_keys(KC_T, KC_T)) {
//         // Leader, t, t => Process ten segments
//         for (int i = 0; i < 10; i++) {
//             tap_code16(G(KC_A));
//             wait_ms(500);
//             tap_code16(G(S(KC_C)));
//             tap_code16(G(S(KC_C)));
//             wait_ms(500);
//             tap_code(KC_ENTER);
//             wait_ms(5000);
//             tap_code(KC_DOWN);
//         }
//     } else if (leader_sequence_three_keys(KC_T, KC_T, KC_T)) {
//         // Leader, t, t, t => Process fifty segments
//         for (int i = 0; i < 50; i++) {
//             tap_code16(G(KC_A));
//             wait_ms(500);
//             tap_code16(G(S(KC_C)));
//             tap_code16(G(S(KC_C)));
//             wait_ms(500);
//             tap_code(KC_ENTER);
//             wait_ms(5000);
//             tap_code(KC_DOWN);
//         }
//     }
// }

/*
##################
### RGB Matrix ###
##################
*/

bool rgb_disabled_manually = false;

// Function to do things when the keyboard is idle.
static uint32_t idle_callback(uint32_t trigger_time, void* cb_arg) {
    // If execution reaches here, the keyboard has gone idle.
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
##########################
### Achordion Settings ###
##########################
*/

void matrix_scan_user(void) {
    achordion_task();
}

bool achordion_chord(uint16_t tap_hold_keycode, keyrecord_t* tap_hold_record, uint16_t other_keycode, keyrecord_t* other_record) {
    // Get matrix positions of tap-hold key and other key.
    uint8_t tap_hold_row = tap_hold_record->event.key.row;
    // uint8_t tap_hold_col = tap_hold_record->event.key.col; // Not needed given the way this keyboard maps sides.
    uint8_t other_row = other_record->event.key.row;
    // uint8_t other_col = other_record->event.key.col; // Not needed given the way this keyboard maps sides.
    if (tap_hold_row == 3) return true; // I want the left thumbs to activate a hold even when used with keys on the same side to make using a mouse easier.
    bool first_key_left = (tap_hold_row >= 0 && tap_hold_row <= 3);
    bool second_key_left = (other_row >= 0 && other_row <= 3);
    return first_key_left != second_key_left;
}

bool achordion_eager_mod(uint8_t mod) {
    switch (mod) {
        case MOD_LSFT:
        case MOD_LALT:
        case MOD_LGUI:
            return true;  // Eagerly apply mods.
        default:
            return false;
    }
}

uint16_t achordion_streak_chord_timeout(uint16_t tap_hold_keycode, uint16_t next_keycode) {
    if (IS_QK_LAYER_TAP(tap_hold_keycode)) {
        return 0; // Disable streak detection on layer-tap keys.
    }
    // Otherwise, tap_hold_keycode is a mod-tap key.
    uint8_t mod = mod_config(QK_MOD_TAP_GET_MODS(tap_hold_keycode));
    if ((mod & MOD_LSFT) != 0) {
        return 100; // A shorter streak timeout for Shift mod-tap keys.
    } else {
        return 240; // A longer timeout otherwise.
    }
}

uint16_t achordion_timeout(uint16_t tap_hold_keycode) {
    switch (tap_hold_keycode) {
        case MT_QF:
        case UM_LH3:
            return 0;  // Bypass Achordion for these keys.
    }
    return 800;  // Otherwise use a timeout of 800 ms.
}

/*
##############################
### Sentence Case Settings ###
##############################
*/

void housekeeping_task_user(void) {
    sentence_case_task();
    // Other tasks…
}

char sentence_case_press_user(uint16_t keycode, keyrecord_t* record, uint8_t mods) {
    if ((mods & ~(MOD_MASK_SHIFT | MOD_BIT(KC_RALT))) == 0) {
        const bool shifted = mods & MOD_MASK_SHIFT;
        switch (keycode) {
            case KC_A ... KC_Z:
                return 'a'; // Letter key.
            // case KC_DOT: // . is punctuation, Shift . is a symbol (>)
                // return !shifted ? '.' : '#';
            case KC_1:
            case KC_SLSH:
                return shifted ? '.' : '#';
            case KC_EXLM:
            case KC_QUES:
                return '.';
            case KC_2 ... KC_0: // 2 3 4 5 6 7 8 9 0
            case KC_AT ... KC_RPRN: // @ # $ % ^ & * ( )
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
            case KC_QUOT:
            case U_QUOTE:
            case KC_DQUO:
                return '\''; // Quote key.
        }
    }
    // Otherwise clear Sentence Case to initial state.
    sentence_case_clear();
    return '\0';
}

/*
##########################
### Caps Word Settings ###
##########################
*/

bool caps_word_press_user(uint16_t keycode) {
    switch (keycode) {
        // Keycodes that continue Caps Word, with shift applied.
        case KC_A ... KC_Z:
        case TD_AA:
        case TD_AE:
        case TD_AO:
        case U_QU:
        case MT_QF:
        case U_GR_A:
        case U_TIL_A:
        case U_TIL_O:
        case U_AC_I:
        case U_AC_U:
        case U_CC:
        case U_MP:
        case U_PS:
        case KC_MINS:
            add_weak_mods(MOD_BIT(KC_LSFT)); // Apply shift to next key.
            return true;

        // Keycodes that continue Caps Word, without shifting.
        case KC_1 ... KC_0:
        case U_QUOTE:
        case KC_QUOTE:
        case KC_DQUO:
        case KC_GRAVE:
        case KC_BSPC:
        case LT(0, KC_BACKSPACE):
        case KC_DEL:
        case KC_UNDS:
            return true;

        default:
            return false; // Deactivate Caps Word.
    }
}

/*
########################
### Tapping Settings ###
########################
*/

// Set tapping term per key.
uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case TD_AA:
        case TD_AE:
        case TD_AO:
            return 200;
        case MT_QF:
            return 175;
        case UM_LM2:
        case UM_LM4:
            return 150;  // these need to be short so the eager mods apply quickly
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
            return QUICK_TAP_TERM;
    }
}

/*
##############################
### Alternate Key Settings ###
##############################
*/

uint16_t get_alt_repeat_key_keycode_user(uint16_t keycode, uint8_t mods) {
    bool shifted = (mods & MOD_MASK_SHIFT);  // Was Shift held?
    switch (keycode) {
        case UM_LH1:
            if (shifted) {        // If the last key was Shift + Tab,
                return KC_TAB;    // ... the reverse is Tab.
            } else {              // Otherwise, the last key was Tab,
                return S(KC_TAB); // ... and the reverse is Shift + Tab.
            }
            break;
        case UM_LM5: SEND_STRING(/*s*/"sion"); break;
        case UM_LM4: SEND_STRING(/*n*/"ion"); break;
        case UM_LM3: SEND_STRING(/*t*/"heir"); break;
        case UM_LB2: SEND_STRING(/*m*/"ent"); break;
        case UM_RT5: SEND_STRING(/*b*/"ecause"); break;
        case UM_RM2: SEND_STRING(/*a*/"tion"); break;
        case UM_RM4: SEND_STRING(/*i*/"tion"); break;
        case UM_RH2: SEND_STRING(/* */"the"); break;
        case KC_W: SEND_STRING(/*w*/"hich"); break;
    }
    return KC_NO;
}

bool remember_last_key_user(uint16_t keycode, keyrecord_t* record, uint8_t* remembered_mods) {
    if (keycode == UM_RH3) { return false; }
    return true;
}

/*
########################
### One-shot mod-tap ###
########################
*/

static bool oneshot_mod_tap(uint16_t keycode, keyrecord_t* record) {
    if (record->tap.count == 0) {  // Key is being held.
        if (record->event.pressed) {
            const uint8_t mods = (keycode >> 8) & 0x1f;
            add_oneshot_mods(((mods & 0x10) == 0) ? mods : (mods << 4));
        }
        return false;  // Skip default handling.
    }
    return true;  // Continue default handling.
}

/*
###########################
### Process Record User ###
###########################
*/

bool process_record_user(uint16_t keycode, keyrecord_t* record) {
    if (!process_achordion(keycode, record)) { return false; }
    if (!process_sentence_case(keycode, record)) { return false; }
    if (!process_select_word(keycode, record)) { return false; }
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
        // Hold intercepts
        case UM_LH3:
            if (!record->tap.count && record->event.pressed) {
                tap_code(KC_ESCAPE);
                return false;
            }
            break;
        case UM_RH3:
            if (record->tap.count && record->event.pressed) {
                alt_repeat_key_invoke(&record->event);
                return false;
            }
            break;
        case MT_QF:
            if (!record->tap.count && record->event.pressed) {
                if (is_caps_word_on()) {
                    tap_code16(S(KC_Q));
                } else if (is_sentence_case_primed()) {
                    tap_code16(S(KC_Q));
                    sentence_case_clear();
                } else {
                    tap_code(KC_Q);
                }
                return false;
            }
            break;
        // Macros
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
        case U_QU:
            if (record->event.pressed) {
                if (is_caps_word_on()) {
                    tap_code16(S(KC_Q));
                    tap_code16(S(KC_U));
                } else if (is_sentence_case_primed()) {
                    tap_code16(S(KC_Q));
                    sentence_case_clear();
                    tap_code(KC_U);
                } else {
                    tap_code(KC_Q);
                    clear_mods();
                    tap_code(KC_U);
                    set_mods(mod_state);
                }
                return false;
            }
            break;
        case U_QUOTE:
            if (record->event.pressed) {
                if (mod_state & MOD_MASK_ALT) {
                    tap_code(KC_GRAVE);
                    tap_code(KC_SPACE);
                    tap_code(KC_BACKSPACE);
                } else {
                    tap_code(KC_QUOTE);
                    tap_code(KC_SPACE);
                }
            }
            break;
        case U_TILDE:
            if (record->event.pressed) {
                tap_code16(KC_TILDE);
                tap_code(KC_SPACE);
            }
            break;
        case U_CIRC:
            if (record->event.pressed) {
                tap_code16(KC_CIRC);
                tap_code(KC_SPACE);
            }
            break;
        case U_CC:
            if (record->event.pressed) {
                if (is_caps_word_on()) {
                    tap_code16(S(A(KC_C)));
                } else {
                    tap_code16(A(KC_C));
                }
            }
            break;
        case U_MP:
            if (record->event.pressed) {
                if (is_caps_word_on()) {
                    tap_code16(S(KC_M));
                    tap_code16(S(KC_P));
                } else {
                    tap_code(KC_M);
                    clear_mods();
                    tap_code(KC_P);
                    set_mods(mod_state);
                }
            }
            break;
        case U_PS:
            if (record->event.pressed) {
                if (is_caps_word_on()) {
                    tap_code16(S(KC_P));
                    tap_code16(S(KC_S));
                } else {
                    tap_code(KC_P);
                    clear_mods();
                    tap_code(KC_S);
                    set_mods(mod_state);
                }
            }
            break;
        // Accented characters
        case U_AC_I:
            if (record->event.pressed) {
                if (is_caps_word_on()) {
                    clear_mods();
                    tap_code(KC_QUOTE);
                    set_mods(mod_state);
                    tap_code16(S(KC_I));
                } else {
                    clear_mods();
                    tap_code(KC_QUOTE);
                    set_mods(mod_state);
                    tap_code(KC_I);
                }
            }
            layer_clear();  // I never want two accented characters in a row
            return false;
        case U_AC_U:
            if (record->event.pressed) {
                if (is_caps_word_on()) {
                    clear_mods();
                    tap_code(KC_QUOTE);
                    set_mods(mod_state);
                    tap_code16(S(KC_U));
                } else {
                    clear_mods();
                    tap_code(KC_QUOTE);
                    set_mods(mod_state);
                    tap_code(KC_U);
                }
            }
            layer_clear();  // I never want two accented characters in a row
            return false;
        case U_GR_A:
            if (record->event.pressed) {
                if (is_caps_word_on()) {
                    clear_mods();
                    tap_code(KC_GRAVE);
                    set_mods(mod_state);
                    tap_code16(S(KC_A));
                } else {
                    clear_mods();
                    tap_code(KC_GRAVE);
                    set_mods(mod_state);
                    tap_code(KC_A);
                }
            }
            layer_clear();  // I never want two accented characters in a row
            return false;
        case U_TIL_A:
            if (record->event.pressed) {
                if (is_caps_word_on()) {
                    clear_mods();
                    tap_code16(KC_TILDE);
                    set_mods(mod_state);
                    tap_code16(S(KC_A));
                } else {
                    clear_mods();
                    tap_code16(KC_TILDE);
                    set_mods(mod_state);
                    tap_code(KC_A);
                }
            }
            layer_clear();  // I never want two accented characters in a row
            return false;
        case U_TIL_O:
            if (record->event.pressed) {
                if (is_caps_word_on()) {
                    clear_mods();
                    tap_code16(KC_TILDE);
                    set_mods(mod_state);
                    tap_code16(S(KC_O));
                } else {
                    clear_mods();
                    tap_code16(KC_TILDE);
                    set_mods(mod_state);
                    tap_code(KC_O);
                }
            }
            layer_clear();  // I never want two accented characters in a row
            return false;
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
    ,———————————————————————————————————————.    ,———————————————————————————————————————.
    |   f   |   p   |   d   |   l   |   x   |    |  Del  |   u   |   o   |   y   |   b   |
    |———————+———————+———————+———————+———————|    |———————+———————+———————+———————+———————|
    |   s   |   n   |   t   |   h   |   k   |    |   -   |   a   |   e   |   i   |   c   |
    |———————+———————+———————+———————+———————|    |———————+———————+———————+———————+———————|
    |   v   |   w   |   g   |   m   |   j   |    |   ;   |   .   |   ,   |   '   |   /   |
    `———————+———————+———————+———————+———————|    |———————+———————+———————+———————+———————'
                    |  Bspc |   r   |  Tab  |    | Enter | Space | Magic |
                    `———————————————————————'    `———————————————————————'
*/

    [_BASE] = LAYOUT_split_3x5_3(
        UM_LT5, UM_LT4, UM_LT3, UM_LT2, UM_LT1,    UM_RT1, UM_RT2, UM_RT3, UM_RT4, UM_RT5,
        UM_LM5, UM_LM4, UM_LM3, UM_LM2, UM_LM1,    UM_RM1, UM_RM2, UM_RM3, UM_RM4, UM_RM5,
        UM_LB5, UM_LB4, UM_LB3, UM_LB2, UM_LB1,    UM_RB1, UM_RB2, UM_RB3, UM_RB4, UM_RB5,
                                UM_LH3, UM_LH2, UM_LH1,    UM_RH1, UM_RH2, UM_RH3
    ),

/* Alpha Extension
    ,———————————————————————————————————————.    ,———————————————————————————————————————.
    |  ---  |  ---  |  ---  |  ---  |  ---  |    |       |   ú   |   ó   |       |       |
    |———————+———————+———————+———————+———————|    |———————+———————+———————+———————+———————|
    |  ---  |  ---  |  ---  |  ---  |  ---  |    |   à   |   á   |   é   |   í   |   ç   |
    |———————+———————+———————+———————+———————|    |———————+———————+———————+———————+———————|
    |  ---  |  ---  |  ---  |  ---  |  ---  |    |       |   ã   |   õ   |       |       |
    `———————————————————————————————————————|    |———————————————————————————————————————'
                    |       |OOOOOOO|       |    |       |       |       |
                    `———————————————————————'    `———————————————————————'
*/

[_EXT] = LAYOUT_split_3x5_3(
    _______, _______, _______, _______, _______,    XXXXXXX, U_AC_U,  TD_AO,   XXXXXXX, XXXXXXX,
    _______, _______, _______, _______, _______,    U_GR_A,  TD_AA,   TD_AE,   U_AC_I,  U_CC,
    _______, _______, _______, _______, _______,    XXXXXXX, U_TIL_A, U_TIL_O, XXXXXXX, XXXXXXX,
                              XXXXXXX, XXXXXXX, XXXXXXX,    XXXXXXX, XXXXXXX, XXXXXXX
),

/* Navigation
    ,———————————————————————————————————————.    ,———————————————————————————————————————.
    |  ---  |  ---  |  ---  |  ---  |  ---  |    | TabUp |  Home |   Up  |  End  |  PgUp |
    |———————+———————+———————+———————+———————|    |———————+———————+———————+———————+———————|
    |  ---  |  ---  |  ---  |  ---  |  ---  |    | TabDo |  Left |  Down | Right |  PgDo |
    |———————+———————+———————+———————+———————|    |———————+———————+———————+———————+———————|
    |  ---  |  ---  |  ---  |  ---  |  ---  |    |       |SpcLeft|SelWord|SpcRght|       |
    `———————+———————+———————+———————+———————|    |———————+———————+———————+———————+———————'
                    |       |       |OOOOOOO|    |Confirm|  RayC |  Undo |
                    `———————————————————————'    `———————————————————————'
*/

    [_NAV] = LAYOUT_split_3x5_3(  // Left shift is not left transparent to prevent the one-shot mod-tap from messing with text selection
        _______, _______, _______, _______, _______,    TAB_UP,    TD_HOME,  KC_UP,   TD_END,    TD_PGUP,
        _______, _______, _______, _______, _______,    TAB_DOWN,  KC_LEFT,  KC_DOWN, KC_RIGHT,  TD_PGDN,
        _______, _______, _______, _______, _______,    XXXXXXX,   SPC_LEFT, SELWORD, SPC_RIGHT, XXXXXXX,
                                  XXXXXXX, XXXXXXX, XXXXXXX,    G(KC_ENT), RAYCAST, UNDO
    ),

/* Mouse and Media Keys
    ,———————————————————————————————————————.    ,———————————————————————————————————————.
    |  ---  |  ---  |  ---  |  ---  |  ---  |    |  MWDn |  MB4  |  MUp  |  MB5  |       |
    |———————+———————+———————+———————+———————|    |———————+———————+———————+———————+———————|
    |  ---  |  ---  |  ---  |  ---  |  ---  |    |  MWUp | MLeft | MDown | MRight|       |
    |———————+———————+———————+———————+———————|    |———————+———————+———————+———————+———————|
    |  ---  |  ---  |  ---  |OOOOOOO|  ---  |    |  Play | VolDo | VolUp |  Prev |  Next |
    `———————+———————+———————+———————+———————|    |———————+———————+———————+———————+———————'
                    |       |       |       |    |  MB1  |  MB3  |  MB2  |
                    `———————————————————————'    `———————————————————————'
*/

    [_MOU] = LAYOUT_split_3x5_3(
        _______, _______, _______, _______, _______,    MS_WHLD, MS_BTN4, MS_UP,   MS_BTN5, XXXXXXX,
        _______, _______, _______, _______, _______,    MS_WHLU, MS_LEFT, MS_DOWN, MS_RGHT, XXXXXXX,
        _______, _______, _______, XXXXXXX, _______,    KC_MPLY, KC_VOLD, KC_VOLU, KC_MPRV, KC_MNXT,
                                  XXXXXXX, XXXXXXX, XXXXXXX,    MS_BTN1, MS_BTN3, MS_BTN2
    ),

/* Numbers
    ,———————————————————————————————————————.    ,———————————————————————————————————————.
    |   /   |   7   |   8   |   9   |   *   |    |  ---  |  ---  |  ---  |  ---  |  ---  |
    |———————+———————+———————+———————+———————|    |———————+———————+———————+———————+———————|
    |   -   |   4   |   5   |   6   |   +   |    |  ---  |  ---  |  ---  |  ---  |  ---  |
    |———————+———————+———————+———————+———————|    |———————+———————+———————+———————+———————|
    |   %   |   1   |   2   |   3   |   =   |    |  ---  |  ---  |  ---  |  ---  |  ---  |
    `———————+———————+———————+———————+———————|    |———————+———————+———————+———————+———————'
                    |  Bspc |   0   | LLock |    |       |OOOOOOO|       |
                    `———————————————————————'    `———————————————————————'
*/

    [_NUM] = LAYOUT_split_3x5_3(
        KC_PSLS, KC_7, KC_8,    KC_9,  KC_PAST,    _______, _______, _______, _______, _______,
        KC_PMNS, KC_4, KC_5,    KC_6,  KC_PPLS,    _______, _______, _______, _______, _______,
        KC_PERC, KC_1, KC_2,    KC_3,  KC_PEQL,    _______, _______, _______, _______, _______,
                               KC_BSPC, KC_P0, QK_LLCK,    XXXXXXX, XXXXXXX, XXXXXXX
    ),

/* Symbols
    ,———————————————————————————————————————.    ,———————————————————————————————————————.
    |       |   \   |   <   |   >   |      |    |  ---  |  ---  |  ---  |  ---  |  ---  |
    |———————+———————+———————+———————+———————|    |———————+———————+———————+———————+———————|
    |       |   |   |   [   |   ]   |   $   |    |  ---  | Shift |  ---  |  ---  |  ---  |
    |———————+———————+———————+———————+———————|    |———————+———————+———————+———————+———————|
    |       |   ^   |   {   |   }   |   £   |    |  ---  |  ---  |  ---  |  ---  |  ---  |
    `———————+———————+———————+———————+———————|    |———————+———————+———————+———————+———————'
                    |   &   |   #   |   €   |    |OOOOOOO|       |       |
                    `———————————————————————'    `———————————————————————'
*/

    [_SYM] = LAYOUT_split_3x5_3(
        XXXXXXX, KC_BSLS, KC_LT,   KC_GT,   A(S(KC_K)),    _______, _______, _______, _______, _______,
        XXXXXXX, KC_PIPE, KC_LBRC, KC_RBRC, KC_DLR,        _______, KC_LSFT, _______, _______, _______,
        XXXXXXX, U_CIRC,  KC_LCBR, KC_RCBR, A(KC_3),       _______, _______, _______, _______, _______,
                                  KC_AMPR, KC_HASH, A(KC_AT),      XXXXXXX, XXXXXXX, XXXXXXX
    ),

/* Function
    ,———————————————————————————————————————.    ,———————————————————————————————————————.
    |  F12  |   F7  |   F8  |   F9  | PrScr |    |  ---  |  ---  |  RMUp |  ---  |  ---  |
    |———————+———————+———————+———————+———————|    |———————+———————+———————+———————+———————|
    |  F11  |   F4  |   F5  |   F6  |       |    |  ---  | RMNxt | RMDown| ACTog | Flash |
    |———————+———————+———————+———————+———————|    |———————+———————+———————+———————+———————|
    |  F10  |   F1  |   F2  |   F3  | RMTog |    |  ---  |  ---  |  ---  |  ---  |  ---  |
    `———————+———————+———————+———————+———————|    |———————+———————+———————+———————+———————'
                    | Leader|  Caps |  Lock |    |       |       |OOOOOOO|
                    `———————————————————————'    `———————————————————————'
*/

    [_FUN] = LAYOUT_split_3x5_3(
        KC_F12, KC_F7, KC_F8,   KC_F9,   PRT_SCR,     XXXXXXX, XXXXXXX, RM_VALU, XXXXXXX, XXXXXXX,
        KC_F11, KC_F4, KC_F5,   KC_F6,   XXXXXXX,     XXXXXXX, RM_NEXT, RM_VALD, AC_TOGG, QK_BOOT,
        KC_F10, KC_F1, KC_F2,   KC_F3,   U_RGB_T,     XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
                               QK_LEAD, KC_CAPS, LOCK_SCR,    XXXXXXX, XXXXXXX, XXXXXXX
    ),
};
