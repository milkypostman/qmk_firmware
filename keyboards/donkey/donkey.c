#include QMK_KEYBOARD_H
#include "hal.h"


__attribute__ ((weak))
void matrix_init_user(void) {
}

__attribute__ ((weak))
void matrix_scan_user(void) {
}

void matrix_init_kb(void) {
    // put your keyboard start-up code here
    // runs once when the firmware starts up
    matrix_init_user();
    // The backlight always has to be initialized, otherwise it will stay lit
}

void matrix_scan_kb(void) {
    // put your looping keyboard code here
    // runs every cycle (a lot)

    matrix_scan_user();

}

#ifdef ONEHAND_ENABLE
__attribute__ ((weak))
const keypos_t hand_swap_config[MATRIX_ROWS][MATRIX_COLS] = {
    /* {{0, 9}, {1, 9}, {2, 9}, {3, 9}, {4, 9}}, */
    /* {{0, 10}, {1, 10}, {2, 10}, {3, 10}, {4, 10}}, */
    /* {{0, 11}, {1, 11}, {2, 11}, {3, 11}, {4, 11}}, */
    /* {{0, 12}, {1, 12}, {2, 12}, {3, 12}, {4, 12}}, */
    /* {{0, 13}, {1, 13}, {2, 13}, {3, 13}, {4, 13}}, */
    /* {{0, 14}, {1, 14}, {2, 14}, {3, 14}, {4, 14}}, */
    /* {{0, 15}, {1, 15}, {2, 15}, {3, 15}, {4, 15}}, */
    /* {{0, 16}, {1, 16}, {2, 16}, {3, 16}, {4, 16}}, */
    /* {{0, 17}, {1, 17}, {2, 17}, {3, 17}, {4, 17}}, */
    /* {{0, 0}, {1, 0}, {2, 0}, {3, 0}, {4, 0}}, */
    /* {{0, 1}, {1, 1}, {2, 1}, {3, 1}, {4, 1}}, */
    /* {{0, 2}, {1, 2}, {2, 2}, {3, 2}, {4, 2}}, */
    /* {{0, 3}, {1, 3}, {2, 3}, {3, 3}, {4, 3}}, */
    /* {{0, 4}, {1, 4}, {2, 4}, {3, 4}, {4, 4}}, */
    /* {{0, 5}, {1, 5}, {2, 5}, {3, 5}, {4, 5}}, */
    /* {{0, 6}, {1, 6}, {2, 6}, {3, 6}, {4, 6}}, */
    /* {{0, 7}, {1, 7}, {2, 7}, {3, 7}, {4, 7}}, */
    /* {{0, 8}, {1, 8}, {2, 8}, {3, 8}, {4, 8}}, */
};
#endif
