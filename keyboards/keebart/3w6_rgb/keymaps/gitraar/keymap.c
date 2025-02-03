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
#define TD_PGUP TD(PGUP)
#define TD_PGDN TD(PGDOWN)
#define TD_HOME TD(HOME)
#define TD_END TD(END)
#define TD_U TD(U_TD_U)

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
    ACCENTED,
    SELWORD,
    U_GR_A, U_TIL_A, U_TIL_O, U_CIRC_A, U_CIRC_E, U_CIRC_O, // accented characters
    U_AC_A, U_AC_E, U_AC_I, U_AC_O, // accented characters
    U_CC, U_CAO, U_COES, // "ç", "ção", and "ções"
};

// Tap Dance stuff.
enum tap_dances {
    DOT,
    PGUP,
    PGDOWN,
    HOME,
    END,
    U_TD_U,
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
void u_taps(tap_dance_state_t *state, void *user_data) {
    uint8_t mod_state = get_mods();
    if (state->count == 1) {
        tap_code(KC_U);
    } else if (state->count == 2) {
        if (is_caps_word_on()) {
            clear_mods();
            clear_weak_mods();
            tap_code(KC_QUOTE);
            set_mods(mod_state);
            tap_code16(S(KC_U));
            caps_word_on();
        } else {
            clear_mods();
            clear_weak_mods();
            tap_code(KC_QUOTE);
            set_mods(mod_state);
            tap_code(KC_U);
        }
    } else {
        reset_tap_dance(state);
    }
}

// Definition for each tap dance using the functions above.
tap_dance_action_t tap_dance_actions[] = {
    [DOT] = ACTION_TAP_DANCE_FN(dot_taps),
    [PGUP] = ACTION_TAP_DANCE_TAP_HOLD(KC_PGUP, G(KC_UP)),
    [PGDOWN] = ACTION_TAP_DANCE_TAP_HOLD(KC_PGDN, G(KC_DOWN)),
    [HOME] = ACTION_TAP_DANCE_TAP_HOLD(A(KC_LEFT), KC_HOME),
    [END] = ACTION_TAP_DANCE_TAP_HOLD(A(KC_RIGHT), KC_END),
    [U_TD_U] = ACTION_TAP_DANCE_FN(u_taps),
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

const uint16_t PROGMEM acute_a_combo[] = {HRM_N, KC_H, COMBO_END};
const uint16_t PROGMEM acute_e_combo[] = {HRM_E, KC_COMMA, COMBO_END};
const uint16_t PROGMEM acute_i_combo[] = {HRM_I, TD_DOT, COMBO_END};
const uint16_t PROGMEM acute_o_combo[] = {HRM_O, KC_PSLS, COMBO_END};

// Left-side horizontal combos.
const uint16_t PROGMEM caps_word_combo[] = {HRM_T, KC_G, COMBO_END};

// Right-side horizontal combos.
const uint16_t PROGMEM quote_combo[] = {HRM_N, KC_M, COMBO_END};
const uint16_t PROGMEM tilde_combo[] = {KC_H, KC_COMMA, COMBO_END};
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
    COMBO(acute_a_combo, U_AC_A),
    COMBO(acute_e_combo, U_AC_E),
    COMBO(acute_i_combo, U_AC_I),
    COMBO(acute_o_combo, U_AC_O),
    COMBO(caps_word_combo, CW_TOGG),
    COMBO(quote_combo, KC_QUOTE),
    COMBO(tilde_combo, KC_TILDE),
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
#####################
### Assorted Code ###
#####################
*/

bool caps_word_press_user(uint16_t keycode) {
    switch (keycode) {
        // Keycodes that continue Caps Word, with shift applied.
        case KC_A ... KC_Z:
        case TD_U: // Added to make Caps Word work with TD_U.
        case U_GR_A:
        case U_TIL_A:
        case U_TIL_O:
        case U_CIRC_A:
        case U_CIRC_E:
        case U_CIRC_O:
        case U_AC_A:
        case U_AC_E:
        case U_AC_I:
        case U_AC_O:
        case U_CC:
        case U_CAO:
        case U_COES:
        case KC_MINS:
            add_weak_mods(MOD_BIT(KC_LSFT));  // Apply shift to next key.
            return true;

        // Keycodes that continue Caps Word, without shifting.
        case KC_1 ... KC_0:
        case KC_QUOTE: // Added to make Caps Word work with TD_U.
        case KC_GRAVE:
        case KC_BSPC:
        case KC_DEL:
        case KC_UNDS:
            return true;

        default:
            return false; // Deactivate Caps Word.
    }
}

// Set tapping term per key.
uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case HRM_A:
        case HRM_O:
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
        // Cedilla
        case U_CC:
            if (record->event.pressed) {
                if (is_caps_word_on()) {
                    tap_code16(S(A(KC_C)));
                } else {
                    tap_code16(A(KC_C));
                }
            }
            break;
        // Code for combos that output multiple characters
        case U_CAO:
            if (record->event.pressed) {
                if (is_caps_word_on()) {
                    tap_code16(S(A(KC_C)));
                    tap_code16(KC_TILDE);
                    tap_code16(S(KC_A));
                    tap_code16(S(KC_O));
                } else {
                    tap_code16(A(KC_C));
                    tap_code16(KC_TILDE);
                    tap_code(KC_A);
                    tap_code(KC_O);
                }
            }
            break;
        case U_COES:
            if (record->event.pressed) {
                if (is_caps_word_on()) {
                    tap_code16(S(A(KC_C)));
                    tap_code16(KC_TILDE);
                    tap_code16(S(KC_O));
                    tap_code16(S(KC_E));
                    tap_code16(S(KC_S));
                } else {
                    tap_code16(A(KC_C));
                    tap_code16(KC_TILDE);
                    tap_code(KC_O);
                    tap_code(KC_E);
                    tap_code(KC_S);
                }
            }
            break;
        // Code for accented characters
        case U_AC_A:
            if (record->event.pressed) {
                if (is_caps_word_on()) {
                    clear_mods();
                    tap_code(KC_QUOTE);
                    set_mods(mod_state);
                    tap_code16(S(KC_A));
                } else {
                    clear_mods();
                    tap_code(KC_QUOTE);
                    set_mods(mod_state);
                    tap_code(KC_A);
                }
            }
            break;
        case U_AC_E:
            if (record->event.pressed) {
                if (is_caps_word_on()) {
                    clear_mods();
                    tap_code(KC_QUOTE);
                    set_mods(mod_state);
                    tap_code16(S(KC_E));
                } else {
                    clear_mods();
                    tap_code(KC_QUOTE);
                    set_mods(mod_state);
                    tap_code(KC_E);
                }
            }
            break;
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
            break;
        case U_AC_O:
            if (record->event.pressed) {
                if (is_caps_word_on()) {
                    clear_mods();
                    tap_code(KC_QUOTE);
                    set_mods(mod_state);
                    tap_code16(S(KC_O));
                } else {
                    clear_mods();
                    tap_code(KC_QUOTE);
                    set_mods(mod_state);
                    tap_code(KC_O);
                }
            }
            break;
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
            break;
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
            break;
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
            break;
        case U_CIRC_A:
            if (record->event.pressed) {
                if (is_caps_word_on()) {
                    clear_mods();
                    tap_code16(KC_CIRCUMFLEX);
                    set_mods(mod_state);
                    tap_code16(S(KC_A));
                } else {
                    clear_mods();
                    tap_code16(KC_CIRCUMFLEX);
                    set_mods(mod_state);
                    tap_code(KC_A);
                }
            }
            break;
        case U_CIRC_E:
            if (record->event.pressed) {
                if (is_caps_word_on()) {
                    clear_mods();
                    tap_code16(KC_CIRCUMFLEX);
                    set_mods(mod_state);
                    tap_code16(S(KC_E));
                } else {
                    clear_mods();
                    tap_code16(KC_CIRCUMFLEX);
                    set_mods(mod_state);
                    tap_code(KC_E);
                }
            }
            break;
        case U_CIRC_O:
            if (record->event.pressed) {
                if (is_caps_word_on()) {
                    clear_mods();
                    tap_code16(KC_CIRCUMFLEX);
                    set_mods(mod_state);
                    tap_code16(S(KC_O));
                } else {
                    clear_mods();
                    tap_code16(KC_CIRCUMFLEX);
                    set_mods(mod_state);
                    tap_code(KC_O);
                }
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
    ,---------------------------------------.    ,---------------------------------------.
    |   q   |   w   |   f   |   p   |   b   |    |   j   |   l   |   u   |   y   |   -   |
    |-------+-------+-------+-------+-------|    |-------+-------+-------+-------+-------|
    |   a   |   r   |   s   |   t   |   g   |    |   m   |   n   |   e   |   i   |   o   |
    |-------+-------+-------+-------+-------|    |-------+-------+-------+-------+-------|
    |   z   |   x   |   c   |   d   |   v   |    |   k   |   h   |   ,   |   .   |   /   |
    `-------+-------+-------+-------+-------|    |-------+-------+-------+-------+-------'
                    |  Esc  |  Bspc |  Tab  |    |  Ent  |  Spc  |  Del  |
                    `-----------------------'    `-----------------------'
*/

    [_BASE] = LAYOUT_split_3x5_3(
        KC_Q,  KC_W,  KC_F,  TRM_P, KC_B,    KC_J,   TRM_L, TD_U,    KC_Y,   KC_MINUS,
        HRM_A, HRM_R, HRM_S, HRM_T, KC_G,    KC_M,   HRM_N, HRM_E,   HRM_I,  HRM_O,
        KC_Z,  KC_X,  KC_C,  KC_D,  KC_V,    KC_K,   KC_H,  KC_COMM, TD_DOT, KC_PSLS,
                              LT_3,  LT_1,  LT_7,    LT_5,   LT_4,  LT_6
    ),

/* Navigation
    ,---------------------------------------.    ,---------------------------------------.
    |-------|-------|-------|-------|-------|    | TabUp | Home  |   Up  |  End  | PgUp  |
    |-------+-------+-------+-------+-------|    |-------+-------+-------+-------+-------|
    |-------|-------|-------|-------|-------|    | TabDo |  Left |  Down | Right |  PgDo |
    |-------+-------+-------+-------+-------|    |-------+-------+-------+-------+-------|
    |-------|-------|-------|-------|-------|    |  Back |SpcLeft|SelWord|SpcRght|Forward|
    `-------+-------+-------+-------+-------|    |-------+-------+-------+-------+-------'
                    |       |OOOOOOO|       |    | Enter |  RayC |  Undo |
                    `-----------------------'    `-----------------------'
*/

    [_NAV] = LAYOUT_split_3x5_3(
        _______, _______, _______, _______, _______,    TAB_UP,    TD_HOME,   KC_UP,   TD_END,    TD_PGUP,
        _______, _______, _______, _______, _______,    TAB_DOWN,  KC_LEFT,   KC_DOWN, KC_RIGHT,  TD_PGDN,
        _______, _______, _______, _______, _______,    BACK,      SPC_LEFT,  SELWORD, SPC_RIGHT, FORWARD,
                                  XXXXXXX, XXXXXXX, XXXXXXX,    G(KC_ENT), G(KC_SPC), UNDO
    ),

/* Mouse Keys
    ,---------------------------------------.    ,---------------------------------------.
    |-------|-------|-------|-------|-------|    |  MWDn |       |  MUp  |       |       |
    |-------+-------+-------+-------+-------|    |-------+-------+-------+-------+-------|
    |-------|-------|-------|-------|-------|    |  MWUp | MLeft | MDown | MRight|       |
    |-------+-------+-------+-------+-------|    |-------+-------+-------+-------+-------|
    |-------|-------|-------|-------|-------|    |       |       |       |       |       |
    `-------+-------+-------+-------+-------|    |-------+-------+-------+-------+-------'
                    |       |       |OOOOOOO|    |   M1  |   M3  |   M2  |
                    `-----------------------'    `-----------------------'
*/

    [_MOUSE] = LAYOUT_split_3x5_3(
        _______, _______, _______, _______, _______,    MS_WHLD, XXXXXXX, MS_UP,   XXXXXXX, XXXXXXX,
        _______, _______, _______, _______, _______,    MS_WHLU, MS_LEFT, MS_DOWN, MS_RGHT, XXXXXXX,
        _______, _______, _______, _______, _______,    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
                                  XXXXXXX, XXXXXXX, XXXXXXX,    MS_BTN1, MS_BTN3, MS_BTN2
    ),

/* Media
    ,---------------------------------------.    ,---------------------------------------.
    |-------|-------|-------|-------|-------|    | RMTog | RMNxt | RMHue |  RMDo |  RMUp |
    |-------+-------+-------+-------+-------|    |-------+-------+-------+-------+-------|
    |-------|-------|-------|-------|-------|    |       |  VolD | VolUp |  Prev |  Next |
    |-------+-------+-------+-------+-------|    |-------+-------+-------+-------+-------|
    |-------|-------|-------|-------|-------|    |       |       |       |       |       |
    `-------+-------+-------+-------+-------|    |-------+-------+-------+-------+-------'
                    |OOOOOOO|       |       |    |  Next |  Play |  Mute |
                    `-----------------------'    `-----------------------'
*/

    [_MEDIA] = LAYOUT_split_3x5_3(
        _______, _______, _______, _______, _______,    RM_TOGG, RM_NEXT, RM_HUEU, RM_VALD, RM_VALU,
        _______, _______, _______, _______, _______,    XXXXXXX, KC_VOLD, KC_VOLU, KC_MPRV, KC_MNXT,
        _______, _______, _______, _______, _______,    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
                                  XXXXXXX, XXXXXXX, XXXXXXX,    KC_MNXT, KC_MPLY, KC_MUTE
    ),

/* Numbers
    ,---------------------------------------.    ,---------------------------------------.
    |   /   |   7   |   8   |   9   |   *   |    |-------|-------|-------|-------|-------|
    |-------+-------+-------+-------+-------|    |-------+-------+-------+-------+-------|
    |   -   |   4   |   5   |   6   |   +   |    |-------|-------|-------|-------|-------|
    |-------+-------+-------+-------+-------|    |-------+-------+-------+-------+-------|
    |   ^   |   1   |   2   |   3   |   =   |    |-------|-------|-------|-------|-------|
    `-------+-------+-------+-------+-------|    |-------+-------+-------+-------+-------'
                    |   0   |  Bspc |  Ent  |    |       |OOOOOOO|       |
                    `-----------------------'    `-----------------------'
*/

    [_NUM] = LAYOUT_split_3x5_3(
        KC_PSLS, KC_7, KC_8,  KC_9,    KC_PAST,    _______, _______, _______, _______, _______,
        KC_PMNS, KC_4, KC_5,  KC_6,    KC_PPLS,    _______, _______, _______, _______, _______,
        KC_CIRC, KC_1, KC_2,  KC_3,    KC_PEQL,    _______, _______, _______, _______, _______,
                               KC_P0, KC_BSPC, KC_ENT,     XXXXXXX, XXXXXXX, XXXXXXX
    ),

/* Symbols
    ,---------------------------------------.    ,---------------------------------------.
    |       |   \   |   <   |   >   |      |    |-------|-------|-------|-------|-------|
    |-------+-------+-------+-------+-------|    |-------+-------+-------+-------+-------|
    |       |   |   |   [   |   ]   |   $   |    |-------|-------|-------|-------|-------|
    |-------+-------+-------+-------+-------|    |-------+-------+-------+-------+-------|
    |       |       |   {   |   }   |   £   |    |-------|-------|-------|-------|-------|
    `-------+-------+-------+-------+-------|    |-------+-------+-------+-------+-------'
                    |   &   |   #   |   €   |    |OOOOOOO|       |       |
                    `-----------------------'    `-----------------------'
*/

    [_SYM] = LAYOUT_split_3x5_3(
        XXXXXXX, KC_BSLS, KC_LT,   KC_GT,   A(S(KC_K)),    _______, _______, _______, _______, _______,
        XXXXXXX, KC_PIPE, KC_LBRC, KC_RBRC, KC_DOLLAR,     _______, _______, _______, _______, _______,
        XXXXXXX, XXXXXXX, KC_LCBR, KC_RCBR, A(KC_3),       _______, _______, _______, _______, _______,
                                  KC_AMPR, KC_HASH, A(KC_AT),      XXXXXXX, XXXXXXX, XXXXXXX
    ),

/* Function
    ,---------------------------------------.    ,---------------------------------------.
    |  F12  |   F7  |   F8  |   F9  | PrScr |    |-------|-------|-------|-------| Flash |
    |-------+-------+-------+-------+-------|    |-------+-------+-------+-------+-------|
    |  F11  |   F4  |   F5  |   F6  | Sleep |    |-------|-------|-------|-------|-------|
    |-------+-------+-------+-------+-------|    |-------+-------+-------+-------+-------|
    |  F10  |   F1  |   F2  |   F3  | RMTog |    |-------|-------|-------|-------|-------|
    `-------+-------+-------+-------+-------|    |-------+-------+-------+-------+-------'
                    | Leader|  Caps |  Tab  |    |       |       |OOOOOOO|
                    `-----------------------'    `-----------------------'
*/

    [_FUN] = LAYOUT_split_3x5_3(
        KC_F12, KC_F7, KC_F8,   KC_F9,   PRT_SCR,     _______, _______, _______, _______, QK_BOOT,
        KC_F11, KC_F4, KC_F5,   KC_F6,   LOCK_SCR,    _______, _______, _______, _______, _______,
        KC_F10, KC_F1, KC_F2,   KC_F3,   RM_TOGG,     _______, _______, _______, _______, _______,
                               QK_LEAD, KC_CAPS, KC_TAB,      XXXXXXX, XXXXXXX, XXXXXXX
    ),

/* Accented Characters
    ,---------------------------------------.    ,---------------------------------------.
    |-------|-------|-------|-------|-------|    |       |   ã   |   â   |       |       |
    |-------+-------+-------+-------+-------|    |-------+-------+-------+-------+-------|
    |-------|-------|-------|-------|-------|    |   à   |   ç   |   ê   |       |   ô   |
    |-------+-------+-------+-------+-------|    |-------+-------+-------+-------+-------|
    |-------|-------|-------|-------|-------|    |       |  ção  |  ções |       |       |
    `-------+-------+-------+-------+-------|    |-------+-------+-------+-------+-------'
                    |       |       |OOOOOOO|    |       |       |       |
                    `-----------------------'    `-----------------------'
*/

    // [_ACCENTED] = LAYOUT_split_3x5_3(
    //     _______, _______, _______, _______, _______,    XXXXXXX, U_TIL_A,  U_AC_U, XXXXXXX, U_TIL_O,
    //     _______, _______, _______, _______, _______,    U_GR_A,  U_AC_A, U_AC_E, U_AC_I, U_AC_O,
    //     _______, _______, _______, _______, _______,    XXXXXXX, U_CC,     U_CAO,    U_COES,  XXXXXXX,
    //                               XXXXXXX, XXXXXXX, XXXXXXX,    XXXXXXX, XXXXXXX,  XXXXXXX
    // ),
    [_ACCENTED] = LAYOUT_split_3x5_3(
        _______, _______, _______, _______, _______,    XXXXXXX, U_TIL_A,  XXXXXXX,  XXXXXXX, U_TIL_O,
        _______, _______, _______, _______, _______,    U_GR_A,  U_CIRC_A, U_CIRC_E, XXXXXXX, U_CIRC_O,
        _______, _______, _______, _______, _______,    XXXXXXX, U_CC,     U_CAO,    U_COES,  XXXXXXX,
                                  XXXXXXX, XXXXXXX, XXXXXXX,    XXXXXXX, XXXXXXX,  XXXXXXX
    ),
};
