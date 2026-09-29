#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"

const uint LED_PIN    = 25;
const uint BUTTON_PIN = 24;
const uint DEBOUNCE_MS = 20;


void set_led(bool on)
{
    gpio_put(LED_PIN, on);
    printf("led %s\n", on ? "on" : "off");
}


bool handle_command(int command, bool led)
{
    if (command == 'e')
    {
        led = true;
        set_led(led);
    }
    else if (command == 'd')
    {
        led = false;
        set_led(led);
    }
    else
    {
        printf("unknown command: %c\n", command);
    }

    return led;
}


int main(void)
{
    stdio_init_all();

    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    gpio_init(BUTTON_PIN);
    gpio_set_dir(BUTTON_PIN, GPIO_IN);
    gpio_pull_up(BUTTON_PIN);

    bool led = false;
    bool prev_pressed = false;

    while (true) {
        bool raw = !gpio_get(BUTTON_PIN);  

        if (raw) {
            sleep_ms(DEBOUNCE_MS);
            if (!gpio_get(BUTTON_PIN)) {    // подтверждение нажатия
                if (!prev_pressed) {        // фронт нажатия
                    led = !led;
                    set_led(led);
                }
                prev_pressed = true;
            }
        } else {
            prev_pressed = false;
        }

        sleep_ms(5);



        

        int command = getchar_timeout_us(0);

        if (command == PICO_ERROR_TIMEOUT)
        {
            continue;
        }

        led = handle_command(command, led);





    }
}