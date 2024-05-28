#include <stdio.h>
#include <driver/ledc.h>
#include <driver/gpio.h>
#include "esp_system.h"
#include "freertos/FreeRTOS.h"
#include "liquid_crystal.h"
#include <freertos/queue.h>
#include "freertos/task.h"
#include "freertos/event_groups.h"

#define BUZZER_TIMER              LEDC_TIMER_1
#define BUZZER_MODE               LEDC_HIGH_SPEED_MODE
#define BUZZER_OUTPUT_IO          (33) // Define the output GPIO
#define BUZZER_CHANNEL            LEDC_CHANNEL_0
#define BUZZER_DUTY_RES           LEDC_TIMER_13_BIT // Set duty resolution to 13 bits
#define BUZZER_DUTY_ON            (4096) // Set duty to 50%. (2 ** 13) * 50% = 4096
#define BUZZER_DUTY_OFF           (0) // Set duty to 50%. (2 ** 13) * 50% = 4096
#define BUZZER_FREQUENCY          (4000) // Frequency in Hertz. Set frequency at 4 kHz

void buzzer_init(void);
void buzzer_SetDuty_Off();
void buzzer_SetDuty_On();
