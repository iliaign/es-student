#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/regs/addressmap.h"
#include "hardware/regs/sio.h"


const uint LED_PIN = 25;





int main(){
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);


    volatile uint32_t *gpio_out_set = (uint32_t *)(SIO_BASE + SIO_GPIO_OUT_SET_OFFSET);
    volatile uint32_t *gpio_out_clear = (uint32_t *)(SIO_BASE + SIO_GPIO_OUT_CLR_OFFSET);
    const uint32_t led_mask = 1u<<LED_PIN;


    while(1){
        *gpio_out_set = led_mask;
        sleep_ms(500);
        *gpio_out_clear = led_mask;
        sleep_ms(500);
    }
}