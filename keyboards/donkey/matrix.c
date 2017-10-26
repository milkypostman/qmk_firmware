/*
  Copyright 2016 Fred Sundvik <fsundvik@gmail.com>
  Jun Wako <wakojun@gmail.com>

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "hal.h"
#include "timer.h"
#include "wait.h"
#include "print.h"
#include "debug.h"
#include "matrix.h"
#include "serial_link/system/serial_link.h"

static const I2CConfig i2ccfg = {
    400000 // clock speed (Hz);
};

/*
 * Infinity ErgoDox Pinusage:
 * Column pins are input with internal pull-down. Row pins are output and strobe with high.
 * Key is high or 1 when it turns on.
 *
 *     col: { PTD1, PTD4, PTD5, PTD6, PTD7 }
 *     row: { PTB2, PTB3, PTB18, PTB19, PTC0, PTC9, PTC10, PTC11, PTD0 }
 */
/* matrix state(1:on, 0:off) */
static matrix_row_t matrix[MATRIX_ROWS];
static matrix_row_t matrix_debouncing[MATRIX_ROWS];
static bool debouncing = false;
static uint16_t debouncing_time = 0;

// Initialize to failure state.
static msg_t mcp23017_status = MSG_OK;

#define SXADDR 0x20
#define SXTIMEOUT 5000

typedef enum {
    REG_IODIRA = 0,
    REG_IODIRB,
    REG_IPOLA,
    REG_IPOLB,
    REG_GPINTENA,
    REG_GPINTENB,
    REG_DEFVALA,
    REG_DEFVALB,
    REG_INTCONA,
    REG_INTCONB,
    REG_IOCONA,
    REG_IOCONB,
    REG_GPPUA,
    REG_GPPUB,
    REG_INTFA,
    REG_INTFB,
    REG_INTCAPA,
    REG_INTCAPB,
    REG_GPIOA,
    REG_GPIOB,
    REG_OLATA,
    REG_OLATB,
} MCP23017_Registers_t;

void print_status(msg_t response) {
    switch (response) {
    case MSG_OK:
        print("MSG_OK\n");
        break;
    case MSG_RESET:
        print("MSG_RESET\n");
        break;
    case MSG_TIMEOUT:
        print("MSG_TIMEOUT\n");
        break;
    };
}

msg_t write_byte(MCP23017_Registers_t reg, uint8_t byte) {
    uint8_t data[] = { reg, byte};
    return i2cMasterTransmitTimeout(&I2CD1, SXADDR, data, sizeof(data), NULL, 0, US2ST(1000));
}

msg_t write_word(MCP23017_Registers_t reg, uint8_t byte1, uint8_t byte2) {
    uint8_t data[] = { reg, byte1, byte2};
    return i2cMasterTransmitTimeout(&I2CD1, SXADDR, data, sizeof(data), NULL, 0, US2ST(1000));
}

// Fully setup the MCP23017.
msg_t InitializeMCP23017(void) {
    msg_t response;
    response = write_word(REG_IODIRA, 0xFF, 0xFF);
    if (response != MSG_OK) { return response; }
    response = write_word(REG_GPPUA, 0x00, 0xFF);
    if (response != MSG_OK) { return response; }
    return MSG_OK;
}

void matrix_init(void)
{
    /* Column(sense) */
    palSetPadMode(TEENSY_PIN0_IOPORT, TEENSY_PIN0,  PAL_MODE_INPUT_PULLUP);
    palSetPadMode(TEENSY_PIN1_IOPORT, TEENSY_PIN1,  PAL_MODE_INPUT_PULLUP);
    palSetPadMode(TEENSY_PIN2_IOPORT, TEENSY_PIN2,  PAL_MODE_INPUT_PULLUP);
    palSetPadMode(TEENSY_PIN3_IOPORT, TEENSY_PIN3,  PAL_MODE_INPUT_PULLUP);
    palSetPadMode(TEENSY_PIN4_IOPORT, TEENSY_PIN4,  PAL_MODE_INPUT_PULLUP);
    palSetPadMode(TEENSY_PIN5_IOPORT, TEENSY_PIN5,  PAL_MODE_INPUT_PULLUP);
    palSetPadMode(TEENSY_PIN6_IOPORT, TEENSY_PIN6,  PAL_MODE_INPUT_PULLUP);

    /* Row(strobe) */
    palSetPadMode(TEENSY_PIN8_IOPORT, TEENSY_PIN8,  PAL_MODE_INPUT);
    palSetPadMode(TEENSY_PIN9_IOPORT, TEENSY_PIN9,  PAL_MODE_INPUT);
    palSetPadMode(TEENSY_PIN10_IOPORT, TEENSY_PIN10, PAL_MODE_INPUT);
    palSetPadMode(TEENSY_PIN11_IOPORT, TEENSY_PIN11, PAL_MODE_INPUT);
    palSetPadMode(TEENSY_PIN12_IOPORT, TEENSY_PIN12,  PAL_MODE_INPUT);

    memset(matrix, 0, MATRIX_ROWS);
    memset(matrix_debouncing, 0, MATRIX_ROWS);

    palSetPadMode(TEENSY_PIN19_IOPORT, TEENSY_PIN19, PAL_MODE_ALTERNATIVE_2);
    palSetPadMode(TEENSY_PIN18_IOPORT, TEENSY_PIN18, PAL_MODE_ALTERNATIVE_2);
    /* palSetPadMode(TEENSY_PIN19_IOPORT, TEENSY_PIN19, PAL_MODE_OUTPUT_OPENDRAIN); */
    /* palSetPadMode(TEENSY_PIN18_IOPORT, TEENSY_PIN18, PAL_MODE_OUTPUT_OPENDRAIN); */

    i2cStart(&I2CD1, &i2ccfg);

    mcp23017_status = InitializeMCP23017();

    matrix_init_quantum();
    wait_ms(500);
}

msg_t get_byte(MCP23017_Registers_t reg, uint8_t *buffer) {
    uint8_t cmd[] = { reg };
    return i2cMasterTransmitTimeout(&I2CD1, SXADDR, cmd, sizeof(cmd), buffer, 1, US2ST(1000));
}

void GetAllRegisters(uint8_t* registers) {
    uint8_t getRegisters[] = { REG_IODIRA };
    i2cMasterTransmitTimeout(&I2CD1, SXADDR, getRegisters, sizeof(getRegisters), registers, 22, US2ST(1000));
}

msg_t get_mcp23017_row(int row, matrix_row_t *data) {
    if (mcp23017_status != MSG_OK) {return mcp23017_status;}

    // bits are reversed from pins...
    uint8_t mask = 0xFF & ~(1 << (7 - row));
    mcp23017_status = write_byte(REG_IODIRA, mask);
    if (mcp23017_status != MSG_OK) { return mcp23017_status; }

    mcp23017_status = write_byte(REG_GPIOA, mask);
    if (mcp23017_status != MSG_OK) { return mcp23017_status; }

    uint8_t reg_data = 0;
    mcp23017_status = get_byte(REG_GPIOB, &reg_data);
    if (mcp23017_status != MSG_OK) { return mcp23017_status; }
#if (DONKEY_VERSION == 2)
    reg_data >>= 1;
#endif
    *data = 0x7F & ~reg_data;
    /* xprintf("%d %X\n", row, *data); */
    /* wait_ms(1000); */
    /* data = 0; */

    mcp23017_status = write_byte(REG_IODIRA, 0xFF);
    if (mcp23017_status != MSG_OK) { return mcp23017_status; }

    mcp23017_status = write_byte(REG_GPIOA, 0xFF);
    if (mcp23017_status != MSG_OK) { return mcp23017_status; }

    return mcp23017_status;
}

uint8_t matrix_scan(void)
{
    uint8_t right_row_offset = MATRIX_ROWS - LOCAL_MATRIX_ROWS;

    if (mcp23017_status != MSG_OK) {
        mcp23017_status = InitializeMCP23017();
    }

    for (int row = 0; row < LOCAL_MATRIX_ROWS; row++) {
        matrix_row_t data = 0;
        // strobe row
        switch (row) {
        case 0: palSetPadMode(TEENSY_PIN8_IOPORT, TEENSY_PIN8, PAL_MODE_OUTPUT_PUSHPULL);    break;
        case 1: palSetPadMode(TEENSY_PIN9_IOPORT, TEENSY_PIN9, PAL_MODE_OUTPUT_PUSHPULL);    break;
        case 2: palSetPadMode(TEENSY_PIN10_IOPORT, TEENSY_PIN10, PAL_MODE_OUTPUT_PUSHPULL);   break;
        case 3: palSetPadMode(TEENSY_PIN11_IOPORT, TEENSY_PIN11, PAL_MODE_OUTPUT_PUSHPULL);   break;
        case 4: palSetPadMode(TEENSY_PIN12_IOPORT, TEENSY_PIN12, PAL_MODE_OUTPUT_PUSHPULL);    break;
        }

        // need wait to settle pin state
        // if you wait too short, or have a too high update rate
        // the keyboard might freeze, or there might not be enough
        // processing power to update the LCD screen properly.
        // 20us, or two ticks at 100000Hz seems to be OK
        wait_us(20);

        data |= palReadPort(TEENSY_PIN0_IOPORT) >> 16;
        uint8_t r = palReadPort(TEENSY_PIN2_IOPORT);
        data |= (r & 0x10) << 2 | (r & 0x80) >> 2 | (r & 0x1) << 2;
        data |= palReadPort(TEENSY_PIN3_IOPORT) >> 9;

        // un-strobe row
        switch (row) {
        case 0: palSetPadMode(TEENSY_PIN8_IOPORT, TEENSY_PIN8, PAL_MODE_INPUT);    break;
        case 1: palSetPadMode(TEENSY_PIN9_IOPORT, TEENSY_PIN9, PAL_MODE_INPUT);    break;
        case 2: palSetPadMode(TEENSY_PIN10_IOPORT, TEENSY_PIN10, PAL_MODE_INPUT);   break;
        case 3: palSetPadMode(TEENSY_PIN11_IOPORT, TEENSY_PIN11, PAL_MODE_INPUT);   break;
        case 4: palSetPadMode(TEENSY_PIN12_IOPORT, TEENSY_PIN12, PAL_MODE_INPUT);    break;
        }

        data ^= 0x7F;
        if (matrix_debouncing[right_row_offset + row] != data) {
            matrix_debouncing[right_row_offset + row] = data;
            debouncing = true;
            debouncing_time = timer_read();
        }
    }

    for (int row = 0; row < (MATRIX_ROWS - LOCAL_MATRIX_ROWS); row++) {
        matrix_row_t data = 0;
        mcp23017_status = get_mcp23017_row(row, &data);
        /* xprintf("row data: %d %X\n", row, data); */
        if (matrix_debouncing[row] != data) {
            matrix_debouncing[row] = data;
            debouncing = true;
            debouncing_time = timer_read();
        }
    }


    if (debouncing && timer_elapsed(debouncing_time) > DEBOUNCE) {
        for (int row = 0; row < MATRIX_ROWS; row++) {
            matrix[row] = matrix_debouncing[row];
        }
        debouncing = false;
    }
    matrix_scan_quantum();
    return 1;
}

bool matrix_is_on(uint8_t row, uint8_t col)
{
    return (matrix[row] & (1<<col));
}

matrix_row_t matrix_get_row(uint8_t row)
{
    return matrix[row];
}

void matrix_print(void)
{
    xprintf("\nr/c 01234567\n");
    for (uint8_t row = 0; row < MATRIX_ROWS; row++) {
        xprintf("%X0: ", row);
        matrix_row_t data = matrix_get_row(row);
        for (int col = 0; col < MATRIX_COLS; col++) {
            if (data & (1<<col))
                xprintf("1");
            else
                xprintf("0");
        }
        xprintf("\n");
    }
}

void matrix_set_remote(matrix_row_t* rows, uint8_t index) {
    uint8_t offset = 0;
    offset = MATRIX_ROWS - LOCAL_MATRIX_ROWS * (index + 2);
    for (int row = 0; row < LOCAL_MATRIX_ROWS; row++) {
        matrix[offset + row] = rows[row];
    }
}
