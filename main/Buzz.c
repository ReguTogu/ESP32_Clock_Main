#include "Buzz.h"

void buzzer_init(void){

     // Prepare and then apply the LEDC PWM timer configuration
    const ledc_timer_config_t buzzer_timer = {
        .speed_mode       = BUZZER_MODE,
        .duty_resolution  = BUZZER_DUTY_RES,
        .timer_num        = BUZZER_TIMER,
        .freq_hz          = BUZZER_FREQUENCY,  // Set output frequency at 4 kHz
        .clk_cfg          = LEDC_AUTO_CLK
    };

    ESP_ERROR_CHECK(ledc_timer_config(&buzzer_timer));

     // Prepare and then apply the LEDC PWM channel configuration
    ledc_channel_config_t buzzer_channel = {
        .gpio_num       = BUZZER_OUTPUT_IO,
        .speed_mode     = BUZZER_MODE,
        .channel        = BUZZER_CHANNEL,
        .intr_type      = LEDC_INTR_DISABLE,
        .timer_sel      = BUZZER_TIMER,
        .duty           = 0, // Set duty to 0%
        .hpoint         = 0
    };

    ESP_ERROR_CHECK(ledc_channel_config(&buzzer_channel));
}


/// @brief Set duty to 50% : Have sound
void buzzer_SetDuty_On(){ 
    ESP_ERROR_CHECK(ledc_set_duty(BUZZER_MODE, BUZZER_CHANNEL, BUZZER_DUTY_ON));
     // Update duty to apply the new value
    ESP_ERROR_CHECK(ledc_update_duty(BUZZER_MODE, BUZZER_CHANNEL));
    vTaskDelay(100/portTICK_PERIOD_MS);
}

/// @brief Set duty to 0% : No sound
void buzzer_SetDuty_Off(){
    ESP_ERROR_CHECK(ledc_set_duty(BUZZER_MODE, BUZZER_CHANNEL, BUZZER_DUTY_OFF));
     // Update duty to apply the new value
    ESP_ERROR_CHECK(ledc_update_duty(BUZZER_MODE, BUZZER_CHANNEL));
}



