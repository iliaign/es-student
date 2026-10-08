#include "pico/stdlib.h"
#include "hardware/gpio.h"

#include "led.h"
#include "log.h"
#include "device.h"


const uint BUTTON_PIN = 24;
const uint DEBOUNCE_MS = 20;


bool get_button_debounce(uint pin)
{
    // Кнопка подключена к GND:
    // 0 = нажата, 1 = отпущена
    bool pressed = !gpio_get(pin);

    sleep_ms(DEBOUNCE_MS);

    return pressed && !gpio_get(pin);
}


void handle_command(int command)
{
    if (command == 'e')
    {
        led_set(true);

        LOG_INF("led %s\n",
                led_is_on() ? "on" : "off");
    }
    else if (command == 'd')
    {
        led_set(false);

        LOG_INF("led %s\n",
                led_is_on() ? "on" : "off");
    }
    else if (command == 'v')
    {
        log_version();
    }
    else if (command == 'i')
    {
        device_info();
    }
    else
    {
        LOG_ERR("unknown command: %c\n", command);
    }
}


int main(void)
{
    stdio_init_all();

    led_init();

    gpio_init(BUTTON_PIN);
    gpio_set_dir(BUTTON_PIN, GPIO_IN);
    gpio_pull_up(BUTTON_PIN);

    bool previous = gpio_get(BUTTON_PIN);

    while (1)
    {
        bool current = gpio_get(BUTTON_PIN);

        if (previous == true && current == false)
        {
            if (get_button_debounce(BUTTON_PIN))
            {
                led_toggle();

                LOG_INF("led %s\n",
                        led_is_on() ? "on" : "off");
            }
        }

        previous = current;

        int command = getchar_timeout_us(0);

        if (command != PICO_ERROR_TIMEOUT)
        {
            LOG_DBG("got %c\n", command);
            handle_command(command);
        }

        sleep_ms(1);
    }
}
