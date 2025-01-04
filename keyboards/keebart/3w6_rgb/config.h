// Copyright 2024 Keebart
// Copyright 2021 weteor
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

/* key matrix size */
#define MATRIX_ROWS 8
#define MATRIX_COLS 10

#define MATRIX_ROWS_PER_SIDE (MATRIX_ROWS / 2)
#define MATRIX_COLS_PER_SIDE (MATRIX_COLS / 2)

#define MATRIX_ROW_PINS_L \
    { GP6, GP7, GP8, GP9 }
#define MATRIX_COL_PINS_L \
    { GP14, GP13, GP12, GP11, GP10 }

#define MATRIX_ROW_PINS_R \
    { P13, P14, P15, P16 }
#define MATRIX_COL_PINS_R \
    { P04, P05, P06, P07, P08 }

#define RGB_MATRIX_FRAMEBUFFER_EFFECTS
#define RGB_MATRIX_KEYPRESSES

#define USB_SUSPEND_WAKEUP_DELAY 200

#define ACHORDION_STREAK

#define TAPPING_TERM 175
#define PERMISSIVE_HOLD
#define QUICK_TAP_TERM_PER_KEY
