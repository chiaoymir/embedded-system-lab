#include "ThisThread.h"
#include "mbed.h"
#include "mbed_wait_api.h"


#define LD1_ON {led1 = 1;} 
#define LD1_OFF {led1 = 0;}
#define LD1_TOG {led1 = !led1;}
#define LD2_ON {led2 = 1;}
#define LD2_OFF {led2 = 0;}
#define LD2_TOG {led2 = !led2;}
#define LD3_ON {led34.output(); led34 = 1; led3_status = 1;}
#define LD3_OFF {led34.input(); led3_status = 0;}
#define LD3_TOG {if (led3_status) LD3_OFF else LD3_ON;}
#define LD4_ON {led34.output(); led34 = 0; led4_status = 1;}
#define LD4_OFF {led34.input(); led4_status = 0;}
#define LD4_TOG {if (led4_status) LD4_OFF else LD4_ON;}

// #define led_delay 100ms 

Semaphore led_sem(0); 
Thread t1;
Thread t2;
Thread t3;
Thread t4;

DigitalOut led1(LED1);
DigitalOut led2(LED2);
// This object drives both LD3 and LD4 on the board. 
// Only one of these LEDs can be driven at a time. 
DigitalInOut led34(LED3);
InterruptIn button(USER_BUTTON);
int led_nb = 0;
int led3_status = 0;
int led4_status = 0;
double led_delay = 0.1; // 100 ms 
volatile int botton_switch = -1;

void led_thread(void const *name) { 
    while (1) {
        led_sem.acquire(); 
        while (1) {
            if (*((int*)name) == 2) 
            { 
                LD2_TOG; 
                ThisThread::sleep_for(led_delay); 
                // wait_us(led_delay); 
                printf("led2\n"); 
                if(botton_switch % 4 == 1)
                    break; 
            } else if (*((int*)name) == 3) { 
                LD3_TOG;
                wait_us(led_delay); 
                printf("led3\n");
                if (botton_switch % 4 == 2) 
                    break;
            } else if (*((int*)name) == 1) { 
                LD1_TOG;
                wait_us(led_delay); 
                printf("led1\n");
                if (botton_switch % 4 == 3) 
                    break;
            } else if (*((int*)name) == 4) { 
                LD4_TOG;
                wait_us(led_delay);
                printf("led4\n");
                if (botton_switch % 4 == 0) 
                    break;
            } 
        }
        LD1_OFF; 
        LD2_OFF; 
        LD3_OFF; 
        LD4_OFF; 
        led_sem.release();
    } 
}

void button_pressed() {
    if (botton_switch == -1) { 
        led_sem.release();
    } 
}
void button_released() {
    ++botton_switch; 
}

int main() {
    LD1_OFF;
    LD2_OFF;
    LD3_OFF;
    LD4_OFF;
    button.fall(&button_pressed); // Change led delay 
    button.rise(&button_released); // Change led
    const int a1 = 1;
    const int a2 = 2;
    const int a3 = 3; 
    const int a4 = 4; 
    t1.start(callback(led_thread, (void *)&a1)); 
    t2.start(callback(led_thread, (void *)&a2)); 
    t3.start(callback(led_thread, (void *)&a3)); 
    t4.start(callback(led_thread, (void *)&a4)); 
    while (1);
}