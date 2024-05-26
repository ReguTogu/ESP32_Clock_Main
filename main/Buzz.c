#include "Buzz.h"
EventGroupHandle_t alarm_eventgroup;

const int GPIO_SENSE_BIT = BIT0;
void IRAM_ATTR button_isr_handler(void* arg) {
    uint32_t gpio_num = (uint32_t) arg;
    BaseType_t xHigherPriorityTaskWoken;
    if (gpio_num==BUTTON_INPUT_IO) {
    	xEventGroupSetBitsFromISR(alarm_eventgroup, GPIO_SENSE_BIT, &xHigherPriorityTaskWoken);
    }
}

void button_init(void){
     //interrupt of raising edge
    gpio_config(&(gpio_config_t){
        .pin_bit_mask = (1<<BUTTON_INPUT_IO),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
    });
    gpio_set_intr_type(BUZZER_OUTPUT_IO, GPIO_INTR_POSEDGE);

    //install gpio isr service
    gpio_install_isr_service(0); // no flags
    //hook isr handler for specific gpio pin
    gpio_isr_handler_add(BUZZER_OUTPUT_IO, &button_isr_handler, NULL);
    gpio_intr_enable(BUZZER_OUTPUT_IO);

}

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


void buzzer_SetDuty_On(){
     // Set duty to 50%
    ESP_ERROR_CHECK(ledc_set_duty(BUZZER_MODE, BUZZER_CHANNEL, BUZZER_DUTY_ON));
     // Update duty to apply the new value
    ESP_ERROR_CHECK(ledc_update_duty(BUZZER_MODE, BUZZER_CHANNEL));
    vTaskDelay(100/portTICK_PERIOD_MS);
}

void buzzer_SetDuty_Off(){
     // Set duty to 0%
    ESP_ERROR_CHECK(ledc_set_duty(BUZZER_MODE, BUZZER_CHANNEL, BUZZER_DUTY_OFF));
     // Update duty to apply the new value
    ESP_ERROR_CHECK(ledc_update_duty(BUZZER_MODE, BUZZER_CHANNEL));
}

void buzzer_task(void){
   
    alarm_eventgroup = xEventGroupCreate();
    button_init();
    buzzer_init();
    buzzer_SetDuty_On();
}

void buzzer_check(void){
    EventBits_t bits;
    bits=xEventGroupWaitBits(alarm_eventgroup, GPIO_SENSE_BIT,pdTRUE, pdFALSE, 6000 / portTICK_PERIOD_MS); // max wait 60s
		if(bits!=0) {
            int16_t loop = 3;
            while (loop) {
                buzzer_SetDuty_On();
                loop --;
            }
		xEventGroupClearBits(alarm_eventgroup, GPIO_SENSE_BIT);
        buzzer_SetDuty_Off();
    }
}

