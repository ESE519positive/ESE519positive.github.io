#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "hardware/pio.h"
#include "hardware/i2c.h"
#include "pio_i2c.h"
#include "hardware/dma.h"
#include "pico/multicore.h"

#include "ws2812.h"
#include "hardware/clocks.h"
#include "ws2812.pio.h"
#include "registers.h"

// #include "sequencer.h"

#include <stdlib.h>
#include "hardware/structs/bus_ctrl.h"

const uint DRAW_PIN_1 = 2;
const uint DRAW_PIN_2 = 3;
const uint DRAW_PIN_3 = 4;

const uint DRINK_PIN_1 = 6;
const uint DRINK_PIN_2 = 7;

/*
const uint SDA_PIN = 22;
const uint SCL_PIN = 23;
*/

/*const int addr = 0x39;
PIO pio = pio0;
uint sm = 1;*/

enum States
{
    IDLE,
    DRAW,
    END
};

struct Drinks {
   int ingre_num;
   int ingredient[3];
};

/*
static void APDS9960_reset() {
    // Two byte reset. First byte register, second byte data
    // There are a load more options to set up the device in different ways that could be added here
    uint8_t buf[] = {0x80, 0x27};
    pio_i2c_write_blocking(pio, sm, addr, buf, 2, false);
}

static void receive_command(uint8_t *command) {
    // For this particular device, we send the device the register we want to read
    // first, then subsequently read from the device. The register is auto incrementing
    // so we don't need to keep sending the register we want, just the first.

    uint8_t buffer[1];

    // Start reading acceleration registers from register 0x3B for 6 bytes
    uint8_t val = 0x9C;
    pio_i2c_write_blocking(pio, sm, addr, &val, 1, true); // true to keep master control of bus
    pio_i2c_read_blocking(pio, sm, addr, buffer, 1, false);

    *command = buffer[0];
}
*/

void rp_init() {
    stdio_init_all();
    PIO pio = pio0;
    int sm = 0;
    uint offset = pio_add_program(pio, &ws2812_program);
    ws2812_program_init(pio, sm, offset, WS2812_PIN, 800000, IS_RGBW);
}

void make_drink(int amount, int material) {
    // Keep the pump on for a amount of time
    uint pin = 0;
    switch(material) 
    {
        case 0: pin = DRAW_PIN_1; 
                break;
        case 1: pin = DRAW_PIN_2; 
                break;
        case 2: pin = DRAW_PIN_2; 
                break; 
        default:
            break;
    }
    gpio_put(pin, 1);
    sleep_ms(200*amount);
    gpio_put(pin, 0);
    sleep_ms(1000);
}

void main_core1() {
    // ...
}

int main() {

    stdio_init_all();
    rp_init();
/*
    uint offset = pio_add_program(pio, &i2c_program);
    i2c_program_init(pio, sm, offset, SDA_PIN, SCL_PIN);
    bi_decl(bi_2pins_with_func(SDA_PIN, SCL_PIN, GPIO_FUNC_I2C))
*/
    gpio_init(DRAW_PIN_1);
    gpio_set_dir(DRAW_PIN_1, GPIO_OUT);
    gpio_put(DRAW_PIN, 0);

    gpio_init(DRAW_PIN_2);
    gpio_set_dir(DRAW_PIN_2, GPIO_OUT);
    gpio_put(DRAW_PIN, 0);

    gpio_init(DRAW_PIN_3);
    gpio_set_dir(DRAW_PIN_3, GPIO_OUT);
    gpio_put(DRAW_PIN, 0);

    gpio_init(DRINK_PIN_1);
    gpio_set_dir(DRINK_PIN_1, GPIO_IN);

    gpio_init(DRINK_PIN_2);
    gpio_set_dir(DRINK_PIN_2, GPIO_IN);

    enum States state = IDLE;
    enum States next_state = state;
    int command = 0;
    int pos_before = 0;
    struct Drinks drink1;
    struct Drinks drink2;
    int iteration = 0;

    drink1.ingre_num = 3;
    drink1.ingredient[0] = 30;
    drink1.ingredient[1] = 20;
    drink1.ingredient[2] = 30;

    drink2.ingre_num = 2;
    drink2.ingredient[0] = 10;
    drink2.ingredient[1] = 30;

    while (true) {     
        if(state == IDLE) {
            iteration = 0;
            if(DRINK_PIN_1 == 1) command = 1;
            else if(DRINK_PIN_2 == 1) command = 2;
            else command = 0;
        }

        switch (state)
        {
        case IDLE:
            if(command != 0) {
                next_state = DRAW
            }
            break;

        case DRAW:
            if(command == 1) {
                make_drink(drink1.ingredient[iteration], iteration); // move the tube by fixed distance
            } else if(command == 2) {
                make_drink(drink2.ingredient[iteration], iteration); // move the tube by fixed distance
            }
            next_state = END;

            break;

        case END:
            if(command == 1) {
                next_state = (iteration < drink1.ingre_num - 1) ? DRAW : IDLE;
            } else if(command == 2) {
                next_state = (iteration < drink2.ingre_num - 1) ? DRAW : IDLE;
            }
            iteration++;
            sleep_ms(1000);
            break;

        default:
            break;
        }
        state = next_state;
    }
}
