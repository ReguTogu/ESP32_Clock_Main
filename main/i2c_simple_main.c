#include <stdio.h>
#include "liquid_crystal.h"
#include "Date.h"
#include "esp_timer.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "Buzz.h"

char *hello = "Hello World!";
char buffer[10];
const uint8_t rs = 5, en = 18, d4 = 19, d5 = 21, d6 = 22, d7 = 23;

int cursor[2] = {0, 0};
bool cursor_flag = 0;

int counter = 0;
int tmp;
int hour, minute, second;
CDate date;
clock_t start, end, start_add, end_add;
double cpu_time_used, add_time;

#define ADD_BUT GPIO_NUM_25
#define SUB_BUT GPIO_NUM_26
//#define CUR_BUT GPIO_NUM_27
//bool gpio_value = 0;


void Digital_Clock(void *arg);
void Print_Date();

static void IRAM_ATTR gpio_isr_handler_1(void *arg)
{
  start_add = clock();
  gpio_intr_disable(ADD_BUT);
  gpio_isr_handler_remove(ADD_BUT);

  date.year++;
  Print_Date();

  gpio_isr_handler_add(ADD_BUT, gpio_isr_handler_1, NULL);
  gpio_intr_enable(ADD_BUT);

  end_add = clock();
  add_time = ((double)(end_add - start_add));
}

static void IRAM_ATTR gpio_isr_handler_2(void *arg)
{
  start_add = clock();
  gpio_intr_disable(SUB_BUT);
  gpio_isr_handler_remove(SUB_BUT);

  date.year--;
  Print_Date();

  gpio_isr_handler_add(SUB_BUT, gpio_isr_handler_2, NULL);
  gpio_intr_enable(SUB_BUT);

  end_add = clock();
  add_time = ((double)(end_add - start_add));
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

void Print_Date(){
  sprintf(buffer, "%02d", hour);
  lcd_set_cursor(4, 0);
  lcd_write_string(buffer);

  sprintf(buffer, "%02d", minute);
  lcd_set_cursor(7, 0);
  lcd_write_string(buffer);

  sprintf(buffer, "%02d", second);
  lcd_set_cursor(10, 0);
  lcd_write_string(buffer);

  sprintf(buffer, "%02d", date.day);
  lcd_set_cursor(3, 1);
  lcd_write_string(buffer);

  sprintf(buffer, "%02d", date.month);
  lcd_set_cursor(6, 1);
  lcd_write_string(buffer);

  sprintf(buffer, "%04d", date.year);
  lcd_set_cursor(9, 1);
  lcd_write_string(buffer);

  lcd_set_cursor(cursor[0], cursor[1]);

}
 


void Digital_Clock(void *arg)
{
  start = clock();
  counter++;
  tmp = counter;
  hour = tmp / 3600;
  tmp = tmp % 3600;
  minute = tmp / 60;
  tmp = tmp % 60;
  second = tmp;

  Print_Date();  
  
  if (counter == 86400)
  {
    counter = 0;
    CDate_Increment(&date);
  }
  end = clock();
  cpu_time_used = ((double)(end - start + add_time)) / CLOCKS_PER_SEC;
}

void app_main()
{
  // initialize the library by associating any needed LCD interface pin
  // with the pin number it is connected to
  liquid_crystal(rs, en, d4, d5, d6, d7);

  // set up the LCD's number of columns and rows:
  lcd_begin(16, 2);
  lcd_set_cursor(4, 0);
  lcd_write_string("00:00:00");
  lcd_set_cursor(3, 1);
  lcd_write_string("00/00/0000");
  date.day = 0;
  date.month = 0;
  date.year = 0;
  //    Khoi tao timer

  gpio_reset_pin(ADD_BUT);
  gpio_reset_pin(SUB_BUT);

  gpio_set_direction(ADD_BUT, GPIO_MODE_INPUT);
  gpio_set_direction(SUB_BUT, GPIO_MODE_INPUT);

  gpio_pullup_en(ADD_BUT);
 gpio_pullup_en(SUB_BUT);

  gpio_pulldown_dis(ADD_BUT);
  gpio_pulldown_dis(SUB_BUT);

  gpio_set_intr_type(ADD_BUT, GPIO_INTR_POSEDGE);
  gpio_set_intr_type(SUB_BUT, GPIO_INTR_POSEDGE);

  gpio_install_isr_service(0);

  gpio_isr_handler_add(ADD_BUT, &gpio_isr_handler_1, NULL);
  gpio_isr_handler_add(SUB_BUT, &gpio_isr_handler_2, NULL);

  gpio_intr_enable(ADD_BUT);
  gpio_intr_enable(SUB_BUT);
  
  buzzer_task();
  
  add_time = 0;

  const esp_timer_create_args_t periodic_timer_args ={
    .callback = &Digital_Clock,
    .name = "periodic"
  };
  esp_timer_handle_t periodic_timer;

  esp_timer_create(&periodic_timer_args,&periodic_timer);
  while (1)
  {
    esp_timer_start_periodic(periodic_timer,1000000 - cpu_time_used*1000);
    add_time = 0;
  }
}