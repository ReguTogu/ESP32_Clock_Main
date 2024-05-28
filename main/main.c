#include <stdio.h>
#include "liquid_crystal.h"
#include "Date.h"
#include "driver/gpio.h"
#include "esp_timer.h"
#include "esp_log.h"
#include "esp_system.h"
#include "esp_intr_alloc.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "unistd.h"
#include "Buzz.h"
#include "DHT.h"

char *hello = "Hello World!";
char buffer[10];
const uint8_t rs = 5, en = 18, d4 = 19, d5 = 21, d6 = 22, d7 = 23;

int cursor[2] = {0, 0};
bool cursor_flag = 0;

int counter = 0;
int tmp_counter = 0;
int tmp;
int tmp1; //For Stopwatch
int tmp_hour = 0;
int tmp_minute = 0;
int tmp_second = 0;
int tmp_day  ;
int tmp_month ;
int tmp_year ;
int hour, minute, second;

int change_flag = 1;
int press_flag = 1;
int press_flag1 = 1;

int alarm_flag= 0;
int alarm_flag_before = 0;

CDate date;
uint64_t start_time;
uint64_t end_time;
uint64_t elapsed_time;

#define ADD_BUT GPIO_NUM_25
#define SUB_BUT GPIO_NUM_26
//#define CUR_BUT GPIO_NUM_27
#define CHANGE_BUT GPIO_NUM_14
#define START_STOP_BUT GPIO_NUM_12
#define RESET_BUT GPIO_NUM_13
//bool gpio_value = 0;


void Digital_Clock(void *arg);
void Print_Date();

void Print_Number(int write_value,int collumn,int row){
  sprintf(buffer, "%02d", write_value);
  lcd_set_cursor(collumn, row);
  lcd_write_string(buffer);
}

void Print_String(char* str,int collumn,int row){
  lcd_set_cursor(collumn, row);
  lcd_write_string(str);
}

static void IRAM_ATTR gpio_isr_handler_1(void *arg)
{
  gpio_intr_disable(ADD_BUT);
  //Code for ADD

}

static void IRAM_ATTR gpio_isr_handler_2(void *arg)
{
   gpio_intr_disable(SUB_BUT);
  //Code for SUB
}

// static void IRAM_ATTR gpio_isr_handler_3(void *arg)
// {
//   start_add = clock();
//   gpio_intr_disable(ADD_BUT);
//   gpio_isr_handler_remove(ADD_BUT);

//   if (cursor_flag == 0)
//   {
//     cursor_flag = 1;
//     cursor[0] = 12;
//     cursor[1] = 0;
//   }
//   else (cursor_flag == 1 && ())
//   {
//     cursor_flag = 0;
//     cursor[0] = 4;
//     cursor[1] = 0;
//   }
//   {
//     cursor[0] += 1
//     cursor[1] += 1;
//   }
  
//   lcd_cursor();
  
//   gpio_isr_handler_add(ADD_BUT, gpio_isr_handler_3, NULL);
//   gpio_intr_enable(ADD_BUT);

//   end_add = clock();
//   add_time = ((double)(end_add - start_add));
// }

static void IRAM_ATTR gpio_isr_handler_4(void *arg)
{
  gpio_intr_disable(CHANGE_BUT);
  if (change_flag<3){
      change_flag++;
  }else{
      change_flag=1;
  }
}

static void IRAM_ATTR gpio_isr_handler_5(void *arg)
{
  gpio_intr_disable(START_STOP_BUT);  
  if (press_flag1==1){
    press_flag1 = 0;
  }else{
    press_flag1 = 1;
  }
}

static void IRAM_ATTR gpio_isr_handler_6(void *arg)
{
  gpio_intr_disable(RESET_BUT);
  tmp_counter = 0;
  tmp_hour = 0;
  tmp_minute = 0;
  tmp_second = 0;
}

void Print_Date (int hour,int minute,int second,int day,int month,int year){
  sprintf(buffer, "%02d", hour);
  lcd_set_cursor(4, 0);
  lcd_write_string(buffer);

  sprintf(buffer, "%02d", minute);
  lcd_set_cursor(7, 0);
  lcd_write_string(buffer);

  sprintf(buffer, "%02d", second);
  lcd_set_cursor(10, 0);
  lcd_write_string(buffer);

  sprintf(buffer, "%02d", day);
  lcd_set_cursor(3, 1);
  lcd_write_string(buffer);

  Print_String("/",5,1);

  sprintf(buffer, "%02d", month);
  lcd_set_cursor(6, 1);
  lcd_write_string(buffer);

  Print_String("/",8,1);

  sprintf(buffer, "%04d", year);
  lcd_set_cursor(9, 1);
  lcd_write_string(buffer);
  
  int ret = readDHT();
  if (ret==DHT_OK){
    int temp = getTemperature();
    int hum = getHumidity();
  
  sprintf(buffer, "%d", hum);
  if (temp>=10){
        lcd_set_cursor(13, 0);
    }else{
        lcd_set_cursor(14, 0);
  }
  lcd_write_string(buffer);

   Print_String("%",15,0);

  sprintf(buffer, "%d", temp);

  if (temp>=10){
        lcd_set_cursor(13, 1);
    }else{
        lcd_set_cursor(14, 1);
    }
    lcd_write_string(buffer);
    Print_String("C",15,1);
  }
  
  lcd_set_cursor(cursor[0], cursor[1]);
  
}
 


void Digital_Clock(void *arg)
{
  start_time = esp_timer_get_time();
  counter++;
  tmp = counter;
  hour = tmp / 3600;
  tmp = tmp % 3600;
  minute = tmp / 60;
  tmp = tmp % 60;
  second = tmp;

  // if (change_flag!=1)
  Print_Date(hour,minute,second,date.day,date.month,date.year);  
   
  
  if (counter == 86400)
  {
    counter = 0;
    CDate_Increment(&date);
  }
  end_time = esp_timer_get_time();
  elapsed_time = (double)(end_time - start_time);
}

void Digital_Clock1(void *arg)
{
  start_time = esp_timer_get_time();
  counter++;
  if (press_flag1==0) tmp_counter++;

  if (change_flag==1) { // Che do man hinh chinh
    lcd_set_cursor(1, 0);
    lcd_write_string("00:00:00");
    lcd_set_cursor(0, 1);
    lcd_write_string("00/00/0000");
    Print_String("H:",11,0);
    Print_String("T:",11,1);
    tmp = counter;
    hour = tmp / 3600;
    tmp = tmp % 3600;
    minute = tmp / 60;
    tmp = tmp % 60;
    second = tmp;

    // if (change_flag!=1)
    Print_Date(hour,minute,second,date.day,date.month,date.year);  

    if (counter == 86400)
    {
      counter = 0;
      CDate_Increment(&date);
    }

      gpio_intr_disable(ADD_BUT); 
      gpio_intr_disable(SUB_BUT);  
  }
  else if (change_flag==2) { // Che do dieu chinh thoi gian
    lcd_set_cursor(1, 0);
    lcd_write_string("00:00:00");

    lcd_set_cursor(0, 1);
    lcd_write_string("00/00/0000");

    Print_Date(hour,minute,second,date.day,date.month,date.year); 

    gpio_isr_handler_add(ADD_BUT, gpio_isr_handler_1,NULL);
    gpio_intr_enable(ADD_BUT); 

    gpio_isr_handler_add(SUB_BUT, gpio_isr_handler_2,NULL);
    gpio_intr_enable(SUB_BUT);  
    // gpio_isr_handler_add(CUR_BUT, gpio_isr_handler_3,NULL);
    // gpio_intr_enable(CUR_BUT);  
  }
  else if (change_flag==3){ // Che do bam gio 
    gpio_isr_handler_add(START_STOP_BUT, gpio_isr_handler_5,NULL);
    gpio_intr_enable(START_STOP_BUT);
    lcd_set_cursor(0, 1);
    lcd_write_string("          ");

    if(press_flag1==0){
      tmp1 = tmp_counter;
      tmp_hour = tmp1 / 3600;
      tmp1 = tmp1 % 3600;
      tmp_minute = tmp1 / 60;
      tmp1 = tmp1 % 60;
      tmp_second = tmp1;
      }
      gpio_intr_disable(ADD_BUT);
      gpio_intr_disable(SUB_BUT);
      gpio_isr_handler_add(RESET_BUT, gpio_isr_handler_6, NULL);
      gpio_intr_enable(RESET_BUT);   

      Print_Number(tmp_hour,1,0);
      Print_Number(tmp_minute,4,0);
      Print_Number(tmp_second,7,0);
  }
  gpio_isr_handler_add(CHANGE_BUT, gpio_isr_handler_4, NULL);
  gpio_intr_enable(CHANGE_BUT);
  end_time = esp_timer_get_time();
}

void config_button(gpio_num_t GPIO_BUT, gpio_isr_t button_isr_handler){
  gpio_reset_pin(GPIO_BUT);
    //interrupt of raising edge
  gpio_config(&(gpio_config_t){
      .pin_bit_mask = (1<<GPIO_BUT),
      .mode = GPIO_MODE_INPUT,
      .pull_up_en = GPIO_PULLUP_ENABLE,
      .pull_down_en = GPIO_PULLDOWN_DISABLE,
  });
  gpio_set_intr_type(GPIO_BUT, GPIO_INTR_POSEDGE);

  //hook isr handler for specific gpio pin
  gpio_isr_handler_add(GPIO_BUT, button_isr_handler, NULL);
  gpio_intr_enable(GPIO_BUT);
}

/// @brief main
void app_main()
{
  // initialize the library by associating any needed LCD interface pin
  // with the pin number it is connected to
  liquid_crystal(rs, en, d4, d5, d6, d7);

  setDHTgpio(GPIO_NUM_17);
  // set up the LCD's number of columns and rows:
  lcd_begin(16, 2);

  date.day = 0;
  date.month = 0;
  date.year = 0;
  //    Khoi tao timer

  config_button(ADD_BUT, gpio_isr_handler_1);
  config_button(SUB_BUT, gpio_isr_handler_2);
  config_button(CHANGE_BUT, gpio_isr_handler_4);
  config_button(START_STOP_BUT, gpio_isr_handler_5);
  config_button(RESET_BUT, gpio_isr_handler_6);
  //install gpio isr service
  gpio_install_isr_service(0); // no flags

  // int intr_alloc_flags1 = ESP_INTR_FLAG_LEVEL1;  // Set the desired priority level (lowest)
  // esp_err_t err1 = esp_intr_alloc(ETS_GPIO_INTR_SOURCE, intr_alloc_flags1,gpio_isr_handler_1, (void*)ADD_BUT , NULL);
  // esp_err_t err2 = esp_intr_alloc(ETS_GPIO_INTR_SOURCE, intr_alloc_flags1,gpio_isr_handler_2, (void*)SUB_BUT , NULL);
  // esp_err_t err3 = esp_intr_alloc(ETS_GPIO_INTR_SOURCE, intr_alloc_flags1,gpio_isr_handler_4, (void*)CHANGE_BUT , NULL);
  // int intr_alloc_flags2 = ESP_INTR_FLAG_LEVEL2;  // Set the desired priority level (lowest)  
  // esp_err_t err4 = esp_intr_alloc(ETS_GPIO_INTR_SOURCE, intr_alloc_flags2,gpio_isr_handler_5, (void*)START_STOP_BUT , NULL);
  // esp_err_t err5 = esp_intr_alloc(ETS_GPIO_INTR_SOURCE, intr_alloc_flags2,gpio_isr_handler_6, (void*)RESET_BUT , NULL);
  

  // gpio_intr_enable(ADD_BUT);
  // gpio_intr_enable(SUB_BUT);
  // gpio_intr_enable(CHANGE_BUT);
  // gpio_intr_enable(START_STOP_BUT);
  // gpio_intr_enable(RESET_BUT);
  
  elapsed_time = 0;

  const esp_timer_create_args_t periodic_timer_args ={
    .callback = &Digital_Clock,
    .name = "periodic"
  };
  esp_timer_handle_t periodic_timer;

  esp_timer_create(&periodic_timer_args,&periodic_timer);
  while (1)
  {
    esp_timer_start_periodic(periodic_timer,1000000 - elapsed_time);
    elapsed_time = 0;
  }
}
