#ifndef KEYBOARDS_ERGODOX_INFINITY_INFINITY_H_
#define KEYBOARDS_ERGODOX_INFINITY_INFINITY_H_

#include "quantum.h"

/* void ergodox_board_led_on(void); */
/* void ergodox_right_led_1_on(void); */
/* void ergodox_right_led_2_on(void); */
/* void ergodox_right_led_3_on(void); */

/* inline void ergodox_right_led_on(uint8_t led) { */
/*     switch (led) { */
/*     case 0: */
/*         ergodox_right_led_1_on(); */
/*         break; */
/*     case 1: */
/*         ergodox_right_led_2_on(); */
/*         break; */
/*     case 2: */
/*         ergodox_right_led_3_on(); */
/*         break; */
/*     } */
/* } */

/* void ergodox_board_led_off(void); */
/* void ergodox_right_led_1_off(void); */
/* void ergodox_right_led_2_off(void); */
/* void ergodox_right_led_3_off(void); */
/* inline void ergodox_right_led_off(uint8_t led) { */
/*     switch (led) { */
/*     case 0: */
/*         ergodox_right_led_1_off(); */
/*         break; */
/*     case 1: */
/*         ergodox_right_led_2_off(); */
/*         break; */
/*     case 2: */
/*         ergodox_right_led_3_off(); */
/*         break; */
/*     } */
/* } */

/* inline void ergodox_led_all_on(void) */
/* { */
/*     ergodox_board_led_on(); */
/*     ergodox_right_led_1_on(); */
/*     ergodox_right_led_2_on(); */
/*     ergodox_right_led_3_on(); */
/* } */

/* inline void ergodox_led_all_off(void) */
/* { */
/*     ergodox_board_led_off(); */
/*     ergodox_right_led_1_off(); */
/*     ergodox_right_led_2_off(); */
/*     ergodox_right_led_3_off(); */
/* } */

/* void ergodox_right_led_1_set(uint8_t n); */
/* void ergodox_right_led_2_set(uint8_t n); */
/* void ergodox_right_led_3_set(uint8_t n); */

/* inline void ergodox_right_led_set(uint8_t led, uint8_t n){ */
/*     switch (led) { */
/*     case 0: */
/*         ergodox_right_led_1_set(n); */
/*         break; */
/*     case 1: */
/*         ergodox_right_led_2_set(n); */
/*         break; */
/*     case 2: */
/*         ergodox_right_led_3_set(n); */
/*         break; */
/*     } */
/* } */

/* inline void ergodox_led_all_set(uint8_t n) { */
/*     ergodox_right_led_1_set(n); */
/*     ergodox_right_led_2_set(n); */
/*     ergodox_right_led_3_set(n); */
/* } */

#define KEYMAP( \
    L60, L50, L40, L30, L20, L10, L00, \
    L61, L51, L41, L31, L21, L11, L01, \
    L62, L52, L42, L32, L22, L12, L02, \
    L63, L53, L43, L33, L23, L13, L03, \
    L64, L54, L44, L34, L24, L14, L04, \
    R60, R50, R40, R30, R20, R10, R00, \
    R61, R51, R41, R31, R21, R11, R01, \
    R62, R52, R42, R32, R22, R12, R02, \
    R63, R53, R43, R33, R23, R13, R03, \
    R64, R54, R44, R34, R24, R14, R04 \
) { \
    { L00, L10, L20, L30, L40, L50, L60 }, \
    { L01, L11, L21, L31, L41, L51, L61 }, \
    { L02, L12, L22, L32, L42, L52, L62 }, \
    { L03, L13, L23, L33, L43, L53, L63 }, \
    { L04, L14, L24, L34, L44, L54, L64 }, \
    { R60, R50, R40, R30, R20, R10, R00 }, \
    { R61, R51, R41, R31, R21, R11, R01 }, \
    { R62, R52, R42, R32, R22, R12, R02 }, \
    { R63, R53, R43, R33, R23, R13, R03 }, \
    { R64, R54, R44, R34, R24, R14, R04 } \
}

#define LAYOUT_donkey KEYMAP

#endif /* KEYBOARDS_ERGODOX_INFINITY_INFINITY_H_ */
