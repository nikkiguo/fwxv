#include <stdbool.h>

#include "delay.h"
#include "log.h"
#include "task_test_helpers.h"
#include "tasks.h"
#include "FreeRTOS.h"
#include "unity.h"
#include "mutex.h"
#include "spi.h"
#include "spi_mcu.h"

#define LONG_BUFFER_LEN 100
#define SHORT_BUFFER_LEN 2

static Mutex test_mutex = {0};
static bool s_spi_long_started;
static bool s_spi_short_started;

static bool s_spi_long_rx_ended;
static bool s_spi_short_rx_ended;


static uint8_t s_long_buffer[LONG_BUFFER_LEN] = {0};
static uint8_t s_long_buffer_init[LONG_BUFFER_LEN] = {0};

static uint8_t s_short_buffer[SHORT_BUFFER_LEN] = {0};
static uint8_t s_short_buffer_init[SHORT_BUFFER_LEN] = {0};

static const SpiSettings spi_settings = { .mode= SPI_PORT_1 };
static bool s_in_long = false;

void prv_unset_buffer(uint8_t* buff, size_t sz){
  for (uint8_t i=0; i<sz; i++)
    buff[i] = 0;
}

bool prv_comp_buffs(uint8_t* buff1, uint8_t* buff2, size_t sz){
  for (uint8_t i=0; i<sz; i++)
    if (buff1[i] != buff2[i])
      return false;
  
  return true;
}

void setup_test(void) {
  s_spi_long_started= false;
  s_spi_short_started = false;

  s_spi_long_rx_ended = true;
  s_spi_short_rx_ended =  true;
  
}

void teardown_test(void) {}

TASK(spi_long, TASK_STACK_512){
  s_spi_long_started=true;
  while (true) {
    LOG_DEBUG("LONG SPI TX\n");
    spi_tx(SPI_PORT_1, s_long_buffer, LONG_BUFFER_LEN);
    
    prv_unset_buffer(s_long_buffer_init, LONG_BUFFER_LEN);
    prv_unset_buffer(s_long_buffer, LONG_BUFFER_LEN);

    LOG_DEBUG("LONG SPI RX START\n");
    s_spi_long_rx_ended = false;
    spi_rx(SPI_PORT_1, s_long_buffer, LONG_BUFFER_LEN, 0);
    s_spi_long_rx_ended = true;
    LOG_DEBUG("LONG SPI RX END\n");

    if (s_spi_short_rx_ended)
      TEST_ASSERT_FALSE(prv_comp_buffs(s_long_buffer_init, s_long_buffer, LONG_BUFFER_LEN));
    else
      TEST_ASSERT_FALSE(prv_comp_buffs(s_long_buffer_init, s_long_buffer, LONG_BUFFER_LEN));


    vTaskDelay(1000);

  }
}

TASK(spi_short, TASK_STACK_512){
  s_spi_short_started=true;
  while (true) {

    LOG_DEBUG("SHORT SPI TX\n");
    spi_tx(SPI_PORT_1, s_short_buffer, SHORT_BUFFER_LEN);
    
    prv_unset_buffer(s_short_buffer_init, SHORT_BUFFER_LEN);
    prv_unset_buffer(s_short_buffer, SHORT_BUFFER_LEN);

    LOG_DEBUG("SHORT SPI RX START\n");
    s_spi_short_rx_ended = false;
    spi_rx(SPI_PORT_1, s_short_buffer, SHORT_BUFFER_LEN, 0);
    s_spi_short_rx_ended = true;
    LOG_DEBUG("SHORT SPI RX END\n");


    if (s_spi_long_rx_ended)
      TEST_ASSERT_FALSE(prv_comp_buffs(s_short_buffer_init, s_short_buffer, SHORT_BUFFER_LEN));
    else
      TEST_ASSERT_TRUE(prv_comp_buffs(s_short_buffer_init, s_short_buffer, SHORT_BUFFER_LEN));

    vTaskDelay(500);
    
  }
}

TASK_TEST(test_running_task, TASK_STACK_512) {
  
  spi_init(SPI_PORT_1, &spi_settings);
  tasks_init_task(spi_long, TASK_PRIORITY(3), NULL);
  tasks_init_task(spi_short, TASK_PRIORITY(2), NULL);


  // The task doesn't start immediately because the test task has the highest priority.
  TEST_ASSERT_FALSE(s_spi_long_started);
  TEST_ASSERT_FALSE(s_spi_short_started);

  // To let it run, use a delay.
  delay_ms(20000);

  // The task should have run.
  TEST_ASSERT_TRUE(s_spi_long_started);
  TEST_ASSERT_TRUE(s_spi_short_started);
}
