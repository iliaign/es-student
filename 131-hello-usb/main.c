#include <stdio.h>
#include "pico/stdlib.h"

stdio_init_all();
int main(){
    while (1)
    {
        printf("Hello, world!\n");
        sleep_ms(1000);
    }
}