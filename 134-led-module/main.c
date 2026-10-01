#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "led.h"

const uint BUTTON_PIN = 24;
const uint DEBOUNCE_MS = 20;


bool get_button_debounce(uint pin)
{
    // Кнопка подключена к GND, поэтому:
    // 0 = нажата, 1 = отпущена
    bool pressed = !gpio_get(pin);

    sleep_ms(DEBOUNCE_MS);

    // Проверяем состояние ещё раз после debounce
    return pressed && !gpio_get(pin);
}


void handle_command(int command)
{
    if (command == 'e')
    {
        led_set(true);
        printf("led %s\n", led_is_on() ? "on" : "off");
    }
    else if (command == 'd')
    {
        led_set(false);
        printf("led %s\n", led_is_on() ? "on" : "off");
    }
    else
    {
        printf("unknown command: %c\n", command);
    }
}


int main()
{
    // Включаем стандартный ввод-вывод
    stdio_init_all();

    // Инициализируем LED
    led_init();

    // Инициализируем кнопку
    gpio_init(BUTTON_PIN);
    gpio_set_dir(BUTTON_PIN, GPIO_IN);

    // Включаем внутреннюю подтяжку к питанию.
    // Поэтому без нажатия: 1
    // При нажатии кнопки на GND: 0
    gpio_pull_up(BUTTON_PIN);

    // Начальное состояние кнопки
    bool previous = gpio_get(BUTTON_PIN);

    while (1)
    {
        // Читаем состояние кнопки
        bool current = gpio_get(BUTTON_PIN);

        // Обнаруживаем нажатие:
        // было 1 (отпущена), стало 0 (нажата)
        if (previous == true && current == false)
        {
            if (get_button_debounce(BUTTON_PIN))
            {
                led_toggle();

                printf("led %s\n",
                       led_is_on() ? "on" : "off");
            }
        }

        // Запоминаем текущее состояние
        previous = current;

        // Проверяем ввод с USB/UART
        int command = getchar_timeout_us(0);

        if (command != PICO_ERROR_TIMEOUT)
        {
            handle_command(command);
        }

        sleep_ms(1);
    }
}
