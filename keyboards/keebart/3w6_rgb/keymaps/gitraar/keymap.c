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
#define CLIP_HIST G(A(KC_BSLS))
#define LOCK_SCR C(G(KC_Q))
#define SPC_LEFT C(KC_LEFT)
#define SPC_RIGHT C(KC_RIGHT)
#define UNDO G(KC_Z)
#define REDO G(S(KC_Z))

// Web Browser commands
#define BACK G(KC_LBRC)
#define FORWARD G(KC_RBRC)
#define TAB_UP G(S(KC_LBRC))
#define TAB_DOWN G(S(KC_RBRC))

// Home row and top row mods
#define HRM_A LCTL_T(KC_A)
#define HRM_R LALT_T(KC_R)
#define HRM_S LGUI_T(KC_S)
#define HRM_T LSFT_T(KC_T)
#define HRM_N LSFT_T(KC_N)
#define HRM_E RGUI_T(KC_E)
#define HRM_I LALT_T(KC_I)
#define HRM_O RCTL_T(KC_O)
#define TRM_L HYPR_T(KC_L)
#define TRM_P HYPR_T(KC_P)

// Layers
#define _BASE 0
#define _NAV 1
#define _MOUSE 2
#define _MEDIA 3
#define _NUM 4
#define _SYM 5
#define _FUN 6
// #define _CEXT 7
#define _ACCENTED 7

// Layer taps
#define LT_1 LT(_NAV,KC_BSPC)
#define LT_2 LT(_MOUSE,KC_TAB)
#define LT_3 LT(_MEDIA,KC_ESC)
#define LT_4 LT(_NUM,KC_SPC)
#define LT_5 LT(_SYM,KC_ENT)
#define LT_6 LT(_FUN,KC_DEL)
#define LT_7 LT(_ACCENTED,KC_TAB)

// Tap dances
#define TD_DOT TD(DOT)
// #define TD_QUOTE TD(QUOTE)
// #define TD_TILDE TD(TILDE)
#define TD_PGUP TD(PGUP)
#define TD_PGDN TD(PGDOWN)
#define TD_HOME TD(HOME)
#define TD_END TD(END)
#define TD_N TD(U_TD_N)
#define TD_E TD(U_TD_E)
#define TD_O TD(U_TD_O)
#define TD_L TD(U_TD_L)
// #define TD_COMMA TD(U_TD_COMMA)

// Settings
#define IDLE_TIMEOUT_MS 600000 // Idle timeout in milliseconds.

// Custom keycodes
enum custom_keycodes {
    BASE = SAFE_RANGE,
    NAV,
    MOUSE,
    MEDIA,
    NUM,
    SYM,
    FUN,
    CEXT,
    ACCENTED,
    SELWORD,
    U_CC,
    U_AC_I, U_AC_U,
    U_CAO, U_COES,
};

// Tap Dance stuff.
enum tap_dances {
    DOT,
    // QUOTE,
    // TILDE,
    PGUP,
    PGDOWN,
    HOME,
    END,
    U_TD_N,
    U_TD_E,
    U_TD_O,
    U_TD_L,
    // U_TD_COMMA,
};

// Select Word keycode.
uint16_t SELECT_WORD_KEYCODE = SELWORD;

/*
##################
### Tap Dances ###
##################
*/

// Function to provide three taps on the Dot key.
void dot_taps(tap_dance_state_t *state, void *user_data) {
    if (state->count == 1) {
        tap_code16(KC_DOT);
    } else if (state->count == 2) {
        tap_code16(KC_COLN);
    } else if (state->count == 3) {
        register_mods(MOD_BIT(KC_LALT));
        tap_code16(KC_SCLN); // This produces an ellipsis.
        unregister_mods(MOD_BIT(KC_LALT));
    } else {
        reset_tap_dance(state);
    }
}

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
void n_taps(tap_dance_state_t *state, void *user_data) {
    uint8_t mod_state = get_mods();
    if (state->count == 1) {
        clear_mods();
        clear_weak_mods();
        tap_code(KC_QUOTE);
        set_mods(mod_state);
        tap_code(KC_A);
    } else if (state->count == 2) {
        clear_mods();
        clear_weak_mods();
        tap_code16(KC_CIRC);
        set_mods(mod_state);
        tap_code(KC_A);
    } else {
        reset_tap_dance(state);
    }
}

void e_taps(tap_dance_state_t *state, void *user_data) {
    uint8_t mod_state = get_mods();
    if (state->count == 1) {
        clear_mods();
        clear_weak_mods();
        tap_code(KC_QUOTE);
        set_mods(mod_state);
        tap_code(KC_E);
    } else if (state->count == 2) {
        clear_mods();
        clear_weak_mods();
        tap_code16(KC_CIRC);
        set_mods(mod_state);
        tap_code(KC_E);
    } else {
        reset_tap_dance(state);
    }
}

void o_taps(tap_dance_state_t *state, void *user_data) {
    uint8_t mod_state = get_mods();
    if (state->count == 1) {
        clear_mods();
        clear_weak_mods();
        tap_code(KC_QUOTE);
        set_mods(mod_state);
        tap_code(KC_O);
    } else if (state->count == 2) {
        clear_mods();
        clear_weak_mods();
        tap_code16(KC_CIRC);
        set_mods(mod_state);
        tap_code(KC_O);
    } else if (state->count == 3) {
        clear_mods();
        clear_weak_mods();
        tap_code16(KC_TILDE);
        set_mods(mod_state);
        tap_code(KC_O);
    } else {
        reset_tap_dance(state);
    }
}

void l_taps(tap_dance_state_t *state, void *user_data) {
    uint8_t mod_state = get_mods();
    if (state->count == 1) {
        clear_mods();
        clear_weak_mods();
        tap_code16(KC_TILDE);
        set_mods(mod_state);
        tap_code(KC_A);
    } else if (state->count == 2) {
        clear_mods();
        clear_weak_mods();
        tap_code(KC_GRAVE);
        set_mods(mod_state);
        tap_code(KC_A);
    } else {
        reset_tap_dance(state);
    }
}

// void comma_taps(tap_dance_state_t *state, void *user_data) {
//     if (state->count == 1) {
//         tap_code16(A(KC_C));
//         tap_code16(KC_TILDE);
//         tap_code(KC_A);
//         tap_code(KC_O);
//     } else if (state->count == 2) {
//         tap_code16(A(KC_C));
//         tap_code16(KC_TILDE);
//         tap_code(KC_O);
//         tap_code(KC_E);
//         tap_code(KC_S);
//     } else {
//         reset_tap_dance(state);
//     }
// }

// Definition for each tap dance using the functions above.
tap_dance_action_t tap_dance_actions[] = {
    [DOT] = ACTION_TAP_DANCE_FN(dot_taps),
    // [QUOTE] = ACTION_TAP_DANCE_DOUBLE(KC_QUOTE, KC_GRAVE),
    // [TILDE] = ACTION_TAP_DANCE_DOUBLE(KC_TILD, KC_CIRC),
    [PGUP] = ACTION_TAP_DANCE_TAP_HOLD(KC_PGUP, G(KC_UP)),
    [PGDOWN] = ACTION_TAP_DANCE_TAP_HOLD(KC_PGDN, G(KC_DOWN)),
    [HOME] = ACTION_TAP_DANCE_TAP_HOLD(A(KC_LEFT), KC_HOME),
    [END] = ACTION_TAP_DANCE_TAP_HOLD(A(KC_RIGHT), KC_END),
    [U_TD_N] = ACTION_TAP_DANCE_FN(n_taps),
    [U_TD_E] = ACTION_TAP_DANCE_FN(e_taps),
    [U_TD_O] = ACTION_TAP_DANCE_FN(o_taps),
    [U_TD_L] = ACTION_TAP_DANCE_FN(l_taps),
    // [U_TD_COMMA] = ACTION_TAP_DANCE_FN(comma_taps),
};

/*
##############
### Combos ###
##############
*/

// Combo definitions.
// Left-side vertical combos.
const uint16_t PROGMEM cut_combo[] = {HRM_R, KC_W, COMBO_END};
const uint16_t PROGMEM copy_combo[] = {HRM_S, KC_F, COMBO_END};
const uint16_t PROGMEM paste_combo[] = {HRM_T, TRM_P, COMBO_END};
const uint16_t PROGMEM clip_hist_combo[] = {KC_G, KC_B, COMBO_END};

const uint16_t PROGMEM percentage_combo[] = {HRM_S, KC_C, COMBO_END};
const uint16_t PROGMEM at_combo[] = {HRM_T, KC_D, COMBO_END};
const uint16_t PROGMEM astr_combo[] = {KC_G, KC_V, COMBO_END};

// Right-side vertical combos.
const uint16_t PROGMEM lprn_combo[] = {HRM_N, TRM_L, COMBO_END};
const uint16_t PROGMEM rprn_combo[] = {HRM_E, KC_U, COMBO_END};
const uint16_t PROGMEM super_o_combo[] = {HRM_I, KC_Y, COMBO_END};

// Left-side horizontal combos.
const uint16_t PROGMEM caps_word_combo[] = {HRM_T, KC_G, COMBO_END};
// const uint16_t PROGMEM cedilla_combo[] = {KC_X, KC_C, COMBO_END};
// const uint16_t PROGMEM cao_combo[] = {KC_X, KC_C, KC_D, COMBO_END};
// const uint16_t PROGMEM coes_combo[] = {KC_Z, KC_X, KC_C, KC_D, COMBO_END};

// Right-side horizontal combos.
// const uint16_t PROGMEM minus_combo[] = {HRM_N, KC_M, COMBO_END};
const uint16_t PROGMEM quote_combo[] = {HRM_N, KC_M, COMBO_END};
const uint16_t PROGMEM tilde_combo[] = {KC_H, KC_COMMA, COMBO_END};
// const uint16_t PROGMEM slash_combo[] = {KC_H, KC_COMMA, COMBO_END};
const uint16_t PROGMEM semicolon_combo[] = {KC_COMMA, TD_DOT, COMBO_END};

// Used combos.
combo_t key_combos[] = {
    COMBO(cut_combo, CUT),
    COMBO(copy_combo, COPY),
    COMBO(paste_combo, PASTE),
    COMBO(clip_hist_combo, CLIP_HIST),
    COMBO(percentage_combo, KC_PERCENT),
    COMBO(at_combo, KC_AT),
    COMBO(astr_combo, KC_PAST),
    COMBO(lprn_combo, KC_LPRN),
    COMBO(rprn_combo, KC_RPRN),
    COMBO(super_o_combo, A(KC_0)),
    COMBO(caps_word_combo, CW_TOGG),
    // COMBO(cedilla_combo, C_CEDILLA),
    // COMBO(cao_combo, C_CAO),
    // COMBO(coes_combo, C_COES),
    // COMBO(minus_combo, KC_MINS),
    COMBO(quote_combo, KC_QUOTE),
    COMBO(tilde_combo, KC_TILDE),
    // COMBO(slash_combo, KC_PSLS),
    COMBO(semicolon_combo, KC_SCLN),
};

/*
#####################
### Key Overrides ###
#####################
*/

// Key overrides.
const key_override_t super_a = ko_make_with_layers_and_negmods(MOD_MASK_SHIFT, A(KC_0), A(KC_9), ~0, MOD_MASK_CAG);
const key_override_t exclamation = ko_make_with_layers_and_negmods(MOD_MASK_SHIFT, TD_DOT, KC_EXLM, ~0, MOD_MASK_CAG);
const key_override_t inv_exclamation = ko_make_with_layers_and_negmods(MOD_MASK_ALT, TD_DOT, A(KC_1), ~0, MOD_MASK_CSG);
const key_override_t question = ko_make_with_layers_and_negmods(MOD_MASK_SHIFT, KC_COMMA, KC_QUES, ~0, MOD_MASK_CAG);
const key_override_t inv_question = ko_make_with_layers_and_negmods(MOD_MASK_ALT, KC_COMMA, A(S(KC_SLSH)), ~0, MOD_MASK_CSG);
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
        tap_code16(G(S(KC_C)));
        tap_code16(G(S(KC_C)));
        wait_ms(500);
        tap_code(KC_ENTER);
    } else if (leader_sequence_two_keys(KC_T, KC_T)) {
        // Leader, t, t => Process ten segments
        for (int i = 0; i < 10; i++) {
            tap_code16(G(KC_A));
            wait_ms(500);
            tap_code16(G(S(KC_C)));
            tap_code16(G(S(KC_C)));
            wait_ms(500);
            tap_code(KC_ENTER);
            wait_ms(5000);
            tap_code(KC_DOWN);
        }
    } else if (leader_sequence_three_keys(KC_T, KC_T, KC_T)) {
        // Leader, t, t, t => Process fifty segments
        for (int i = 0; i < 50; i++) {
            tap_code16(G(KC_A));
            wait_ms(500);
            tap_code16(G(S(KC_C)));
            tap_code16(G(S(KC_C)));
            wait_ms(500);
            tap_code(KC_ENTER);
            wait_ms(5000);
            tap_code(KC_DOWN);
        }
    }
}

/*
##################
### RGB Matrix ###
##################
*/

bool rgb_auto_disabled = false;
// Function to do things when the keyboard is idle.
static uint32_t idle_callback(uint32_t trigger_time, void* cb_arg) {
    // If execution reaches here, the keyboard has gone idle.
    if (rgb_matrix_is_enabled()) {
        rgb_auto_disabled = true; // Used to inform decision about automatically turning RGB back on.
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
#################
### Achordion ###
#################
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
    if (tap_hold_row == 3) return true; // I want the left thumbs to activate a hold even when used with keys on the same side for mouse usage.
    bool first_key_left = (tap_hold_row >= 0 && tap_hold_row <= 3);
    bool second_key_left = (other_row >= 0 && other_row <= 3);
    return first_key_left != second_key_left;
}

bool achordion_eager_mod(uint8_t mod) {
    switch (mod) {
        case MOD_LSFT:
        case MOD_RSFT:
        case MOD_LALT:
        case MOD_LGUI:
            return true; // Eagerly apply mods.
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

/*
#####################
### Sentence Case ###
#####################
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
            case KC_DOT: // . is punctuation, Shift . is a symbol (>)
                return !shifted ? '.' : '#';
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
            case TD_DOT:
                return shifted ? '.' : '.'; // Both . and ! are punctuation.
            case KC_SPC:
                return ' '; // Space key.
            case KC_QUOT:
                return '\''; // Quote key.
        }
    }
    // Otherwise clear Sentence Case to initial state.
    sentence_case_clear();
    return '\0';
}

/*
######################
### One-Shot Layer ###
######################
*/

// Function to activate a layer temporarily
void activate_layer_temporarily(uint8_t layer) {
    layer_on(layer); // Activate the target layer
}

// Function to deactivate the temporary layer and return to the base layer
void deactivate_temporary_layer(uint8_t layer) {
    layer_off(layer); // Deactivate the target layer
}

/*
#####################
### Assorted Code ###
#####################
*/

// Set tapping term per key.
uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case HRM_A:
        case HRM_O:
            return 250;
        // case TD_QUOTE:
        // case TD_TILDE:
        case TD_DOT:
            return 250;
        default:
            return TAPPING_TERM;
    }
}

bool process_record_user(uint16_t keycode, keyrecord_t* record) {
    if (!process_achordion(keycode, record)) { return false; }
    if (!process_sentence_case(keycode, record)) { return false; }
    if (!process_select_word(keycode, record)) { return false; }
    // On every key event, start or extend the deferred execution to call `idle_callback()` after IDLE_TIMEOUT_MS.
    static deferred_token idle_token = INVALID_DEFERRED_TOKEN;
    if (!extend_deferred_exec(idle_token, IDLE_TIMEOUT_MS)) {
        idle_token = defer_exec(IDLE_TIMEOUT_MS, idle_callback, NULL);
    }
    // One-shot layer tracking.
    // static uint8_t temp_layer = 0; // Track the temporary layer
    // static bool temp_layer_active = false; // Track if the temporary layer is active
    // Restore the RGB matrix when returning from idle.
    if (!rgb_matrix_is_enabled() && rgb_auto_disabled) {
        rgb_matrix_enable();
        rgb_auto_disabled = false;
    }
    uint8_t mod_state = get_mods();
    // uint8_t oneshot_mods = get_oneshot_mods();
    tap_dance_action_t *action;
    switch (keycode) {
        // Code for advanced tap dances
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
        // Code for cedilla, activating a custom one-shot layer
        case U_CC:
            if (record->event.pressed) {
                // temp_layer = _CEXT; // Set the target layer
                tap_code16(A(KC_C));
                // activate_layer_temporarily(temp_layer); // Activate the layer
                // temp_layer_active = true; // Mark the layer as active
                // uprintf("Temporary layer activated. Layer state: %u\n", layer_state);
            }
            break;
        // Codes for "ão" and "ões", used in the ç-extension layer
        // case E_AO:
        //     if (record->event.pressed) {
        //         tap_code16(KC_TILDE);
        //         tap_code(KC_A);
        //         tap_code(KC_O);
        //     }
        //     break;
        // case E_OES:
        //     if (record->event.pressed) {
        //         tap_code16(KC_TILDE);
        //         tap_code(KC_O);
        //         tap_code(KC_E);
        //         tap_code(KC_S);
        //     }
        //     break;
        // Codes for combos that output multiple characters
        case U_CAO:
            if (record->event.pressed) {
                tap_code16(A(KC_C));
                tap_code16(KC_TILDE);
                tap_code(KC_A);
                tap_code(KC_O);
            }
            break;
        case U_COES:
            if (record->event.pressed) {
                tap_code16(A(KC_C));
                tap_code16(KC_TILDE);
                tap_code(KC_O);
                tap_code(KC_E);
                tap_code(KC_S);
            }
            break;
        // // Codes for accented characters
        // case U_AC_A:
        //     if (record->event.pressed) {
        //         clear_mods();
        //         tap_code(KC_QUOTE);
        //         set_mods(mod_state);
        //         tap_code(KC_A);
        //     }
        //     break;
        // case U_AE:
        //     if (record->event.pressed) {
        //         clear_mods();
        //         tap_code(KC_QUOTE);
        //         set_mods(mod_state);
        //         tap_code(KC_E);
        //     }
        //     break;
        case U_AC_I:
            if (record->event.pressed) {
                clear_mods();
                tap_code(KC_QUOTE);
                set_mods(mod_state);
                tap_code(KC_I);
            }
            break;
        // case U_AO:
        //     if (record->event.pressed) {
        //         clear_mods();
        //         tap_code(KC_QUOTE);
        //         set_mods(mod_state);
        //         tap_code(KC_O);
        //     }
        //     break;
        case U_AC_U:
            if (record->event.pressed) {
                clear_mods();
                tap_code(KC_QUOTE);
                set_mods(mod_state);
                tap_code(KC_U);
            }
            break;
        // case U_GA:
        //     if (record->event.pressed) {
        //         clear_mods();
        //         tap_code(KC_GRAVE);
        //         set_mods(mod_state);
        //         tap_code(KC_A);
        //     }
        //     break;
        // case U_TA:
        //     if (record->event.pressed) {
        //         clear_mods();
        //         tap_code16(KC_TILDE);
        //         set_mods(mod_state);
        //         tap_code(KC_A);
        //     }
        //     break;
        // case U_TO:
        //     if (record->event.pressed) {
        //         clear_mods();
        //         tap_code16(KC_TILDE);
        //         set_mods(mod_state);
        //         tap_code(KC_O);
        //     }
        //     break;
        // case U_CA:
        //     if (record->event.pressed) {
        //         clear_mods();
        //         tap_code16(KC_CIRC);
        //         set_mods(mod_state);
        //         tap_code(KC_A);
        //     }
        //     break;
        // case U_CE:
        //     if (record->event.pressed) {
        //         clear_mods();
        //         tap_code16(KC_CIRC);
        //         set_mods(mod_state);
        //         tap_code(KC_E);
        //     }
        //     break;
        // case U_CO:
        //     if (record->event.pressed) {
        //         clear_mods();
        //         tap_code16(KC_CIRC);
        //         set_mods(mod_state);
        //         tap_code(KC_O);
        //     }
        //     break;
    }
    // Deactivate the temporary layer after any key is pressed
    // if (record->event.pressed && temp_layer_active) {
    //     deactivate_temporary_layer(temp_layer); // Deactivate the layer
    //     temp_layer_active = false; // Mark the layer as inactive
    // }
    return true;
}

/*
###############
### Keymaps ###
###############
*/

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

/* Base
    ,----------------------------------.    ,----------------------------------.
    |   Q  |   W  |   F  |   P  |   B  |    |   J  |   L  |   U  |   Y  |   -  |
    |------+------+------+------+------|    |------+------+------+------+------|
    |   A  |   R  |   S  |   T  |   G  |    |   M  |   N  |   E  |   I  |   O  |
    |------+------+------+------+------|    |------+------+------+------+------|
    |   Z  |   X  |   C  |   D  |   V  |    |   K  |   H  |   ,  |   .  |   /  |
    `------+------+------+------+------|    |------+------+------+------+------'
                  |  Esc | Bspc |  Tab |    |  Ent |  Spc |  Del |
                  `--------------------'    `--------------------'
*/

    [_BASE] = LAYOUT_split_3x5_3(
        KC_Q,  KC_W,  KC_F,  TRM_P, KC_B,    KC_J,   TRM_L, KC_U,    KC_Y,   KC_MINUS,
        HRM_A, HRM_R, HRM_S, HRM_T, KC_G,    KC_M,   HRM_N, HRM_E,   HRM_I,  HRM_O,
        KC_Z,  KC_X,  KC_C,  KC_D,  KC_V,    KC_K,   KC_H,  KC_COMM, TD_DOT, KC_PSLS,
                              LT_3,  LT_1,  LT_7,    LT_5,   LT_4,  LT_6
    ),

/* Navigation
    ,----------------------------------.    ,----------------------------------.
    |------|------|------|------|------|    | TabUp| Home |  Up  |  End | PgUp |
    |------+------+------+------+------|    |------+------+------+------+------|
    |------|------|------|------|------|    | TabDo| Left | Down | Right| PgDo |
    |------+------+------+------+------|    |------+------+------+------+------|
    |------|------|------|------|------|    | Back |SpLeft|SelWrd|SpRght|Forwar|
    `------+------+------+------+------|    |------+------+------+------+------'
                  |      |OOOOOO|      |    | Enter| RayC | Undo |
                  `--------------------'    `--------------------'
*/

    [_NAV] = LAYOUT_split_3x5_3(
        _______, _______, _______, _______, _______,    TAB_UP,    TD_HOME,   KC_UP,   TD_END,    TD_PGUP,
        _______, _______, _______, _______, _______,    TAB_DOWN,  KC_LEFT,   KC_DOWN, KC_RIGHT,  TD_PGDN,
        _______, _______, _______, _______, _______,    BACK,      SPC_LEFT,  SELWORD, SPC_RIGHT, FORWARD,
                                  XXXXXXX, XXXXXXX, XXXXXXX,    G(KC_ENT), G(KC_SPC), UNDO
    ),

/* Mouse Keys
    ,----------------------------------.    ,----------------------------------.
    |------|------|------|------|------|    | MWDn |      |  MUp |      |      |
    |------+------+------+------+------|    |------+------+------+------+------|
    |------|------|------|------|------|    | MWUp | MLeft| MDown|MRight|      |
    |------+------+------+------+------|    |------+------+------+------+------|
    |------|------|------|------|------|    |      |      |      |      |      |
    `------+------+------+------+------|    |------+------+------+------+------'
                  |      |      |OOOOOO|    |  M1  |  M3  |  M2  |
                  `--------------------'    `--------------------'
*/

    [_MOUSE] = LAYOUT_split_3x5_3(
        _______, _______, _______, _______, _______,    MS_WHLD, XXXXXXX, MS_UP,   XXXXXXX, XXXXXXX,
        _______, _______, _______, _______, _______,    MS_WHLU, MS_LEFT, MS_DOWN, MS_RGHT, XXXXXXX,
        _______, _______, _______, _______, _______,    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
                                  XXXXXXX, XXXXXXX, XXXXXXX,    MS_BTN1, MS_BTN3, MS_BTN2
    ),

/* Media
    ,----------------------------------.    ,----------------------------------.
    |------|------|------|------|------|    | RMTog| RMNxt| RMHue| RMDo | RMUp |
    |------+------+------+------+------|    |------+------+------+------+------|
    |------|------|------|------|------|    |      | VolD | VolU | Prev | Next |
    |------+------+------+------+------|    |------+------+------+------+------|
    |------|------|------|------|------|    |      |      |      |      |      |
    `------+------+------+------+------|    |------+------+------+------+------'
                  |OOOOOO|      |      |    | Next | Play | Mute |
                  `--------------------'    `--------------------'
*/

    [_MEDIA] = LAYOUT_split_3x5_3(
        _______, _______, _______, _______, _______,    RM_TOGG, RM_NEXT, RM_HUEU, RM_VALD, RM_VALU,
        _______, _______, _______, _______, _______,    XXXXXXX, KC_VOLD, KC_VOLU, KC_MPRV, KC_MNXT,
        _______, _______, _______, _______, _______,    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
                                  XXXXXXX, XXXXXXX, XXXXXXX,    KC_MNXT, KC_MPLY, KC_MUTE
    ),

/* Numbers
    ,----------------------------------.    ,----------------------------------.
    |   /  |   7  |   8  |   9  |   *  |    |------|------|------|------|------|
    |------+------+------+------+------|    |------+------+------+------+------|
    |   -  |   4  |   5  |   6  |   +  |    |------|------|------|------|------|
    |------+------+------+------+------|    |------+------+------+------+------|
    |   ^  |   1  |   2  |   3  |   =  |    |------|------|------|------|------|
    `------+------+------+------+------|    |------+------+------+------+------'
                  | Bspc |   0  |  Ent |    |      |OOOOOO|      |
                  `--------------------'    `--------------------'
*/

    [_NUM] = LAYOUT_split_3x5_3(
        KC_PSLS, KC_7, KC_8,    KC_9,  KC_PAST,    _______, _______, _______, _______, _______,
        KC_PMNS, KC_4, KC_5,    KC_6,  KC_PPLS,    _______, _______, _______, _______, _______,
        KC_CIRC, KC_1, KC_2,    KC_3,  KC_PEQL,    _______, _______, _______, _______, _______,
                               KC_BSPC, KC_P0, KC_ENT,     XXXXXXX, XXXXXXX, XXXXXXX
    ),

/* Symbols
    ,----------------------------------.    ,----------------------------------.
    |      |   \  |   <  |   >  |     |    |------|------|------|------|------|
    |------+------+------+------+------|    |------+------+------+------+------|
    |      |   |  |   [  |   ]  |   $  |    |------|------|------|------|------|
    |------+------+------+------+------|    |------+------+------+------+------|
    |      |      |   {  |   }  |   £  |    |------|------|------|------|------|
    `------+------+------+------+------|    |------+------+------+------+------'
                  |   &  |   #  |   €  |    |OOOOOO|      |      |
                  `--------------------'    `--------------------'
*/

    [_SYM] = LAYOUT_split_3x5_3(
        XXXXXXX, KC_BSLS, KC_LT,   KC_GT,   A(S(KC_K)),    _______, _______, _______, _______, _______,
        XXXXXXX, KC_PIPE, KC_LBRC, KC_RBRC, KC_DOLLAR,     _______, _______, _______, _______, _______,
        XXXXXXX, XXXXXXX, KC_LCBR, KC_RCBR, A(KC_3),       _______, _______, _______, _______, _______,
                                  KC_AMPR, KC_HASH, A(KC_AT),      XXXXXXX, XXXXXXX, XXXXXXX
    ),

/* Function
    ,----------------------------------.    ,----------------------------------.
    |  F12 |  F7  |  F8  |  F9  | PrSc |    |------|------|------|------|------|
    |------+------+------+------+------|    |------+------+------+------+------|
    |  F11 |  F4  |  F5  |  F6  | Sleep|    |------|------|------|------|------|
    |------+------+------+------+------|    |------+------+------+------+------|
    |  F10 |  F1  |  F2  |  F3  | RMTog|    |------|------|------|------|------|
    `------+------+------+------+------|    |------+------+------+------+------'
                  |Leader| Caps |  Tab |    |      |      |OOOOOO|
                  `--------------------'    `--------------------'
*/

    [_FUN] = LAYOUT_split_3x5_3(
        KC_F12, KC_F7, KC_F8,   KC_F9,   PRT_SCR,     _______, _______, _______, _______, _______,
        KC_F11, KC_F4, KC_F5,   KC_F6,   LOCK_SCR,    _______, _______, _______, _______, _______,
        KC_F10, KC_F1, KC_F2,   KC_F3,   RM_TOGG,     _______, _______, _______, _______, _______,
                               QK_LEAD, KC_CAPS, KC_TAB,      XXXXXXX, XXXXXXX, XXXXXXX
    ),

/* Mouse Keys
    ,----------------------------------.    ,----------------------------------.
    |------|------|------|------|------|    | MWDn |      |  MUp |      |      |
    |------+------+------+------+------|    |------+------+------+------+------|
    |------|------|------|------|------|    | MWUp | MLeft| MDown|MRight|      |
    |------+------+------+------+------|    |------+------+------+------+------|
    |------|------|------|------|------|    |      |      |      |      |      |
    `------+------+------+------+------|    |------+------+------+------+------'
                  |      |      |OOOOOO|    |  M1  |  M3  |  M2  |
                  `--------------------'    `--------------------'
*/

    [_ACCENTED] = LAYOUT_split_3x5_3(
        _______, _______, _______, _______, _______,    XXXXXXX, TD_L,    U_AC_U,   XXXXXXX, XXXXXXX,
        _______, _______, _______, _______, _______,    XXXXXXX, TD_N,    TD_E,     U_AC_I,  TD_O,
        _______, _______, _______, _______, _______,    XXXXXXX, U_CC,    U_CAO, U_COES, XXXXXXX,
                                  XXXXXXX, XXXXXXX, XXXXXXX,    XXXXXXX, XXXXXXX, XXXXXXX
    ),
};
