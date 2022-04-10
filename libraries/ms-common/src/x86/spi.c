#include "spi.h"

#include "log.h"
#include "spi_mcu.h"
#include "mutex.h"
#include "delay.h"

static GpioState s_cs_curr_state = GPIO_STATE_HIGH;

// SPI mutex for simultaneous task handling
#define SPI_MUTEX_WAIT_MS BLOCK_INDEFINITELY
static Mutex spi_mutex = {0};

StatusCode spi_init(SpiPort spi, const SpiSettings *settings) {
  if (spi >= NUM_SPI_PORTS) {
    return status_msg(STATUS_CODE_INVALID_ARGS, "Invalid SPI port.");
  } else if (settings->mode >= NUM_SPI_MODES) {
    return status_msg(STATUS_CODE_INVALID_ARGS, "Invalid SPI mode.");
  }
  mutex_init(&spi_mutex);

  return STATUS_CODE_OK;
}

StatusCode spi_tx(SpiPort spi, uint8_t *tx_data, size_t tx_len) {
  if (mutex_lock(&spi_mutex, SPI_MUTEX_WAIT_MS) == STATUS_CODE_OK){
    mutex_unlock(&spi_mutex);
  }
  return STATUS_CODE_OK;
}

StatusCode spi_rx(SpiPort spi, uint8_t *rx_data, size_t rx_len, uint8_t placeholder) {
  if (mutex_lock(&spi_mutex, SPI_MUTEX_WAIT_MS) == STATUS_CODE_OK){
    for (size_t i = 0; i < rx_len; i++) {
      // Insert dummy data
      if (i % 2 == 0) {
        rx_data[i] = 1;
      }
    }

    //TODO: should find a way around for mutex write and remove this 
    delay_ms(rx_len);

    mutex_unlock(&spi_mutex);
  }

  return STATUS_CODE_OK;
}

StatusCode spi_cs_set_state(SpiPort spi, GpioState state) {
  if (mutex_lock(&spi_mutex, SPI_MUTEX_WAIT_MS) == STATUS_CODE_OK){
    s_cs_curr_state = state;
    mutex_unlock(&spi_mutex);

  }
  return STATUS_CODE_OK;
}

StatusCode spi_cs_get_state(SpiPort spi, GpioState *input_state) {
  if (mutex_lock(&spi_mutex, SPI_MUTEX_WAIT_MS) == STATUS_CODE_OK){
    *input_state = s_cs_curr_state;
    mutex_unlock(&spi_mutex);
  }
  return STATUS_CODE_OK;
}

StatusCode spi_exchange(SpiPort spi, uint8_t *tx_data, size_t tx_len, uint8_t *rx_data,
                        size_t rx_len) {
  if (spi >= NUM_SPI_PORTS) {
    return status_msg(STATUS_CODE_INVALID_ARGS, "Invalid SPI port.");
  }
  spi_cs_set_state(spi, GPIO_STATE_LOW);

  spi_tx(spi, tx_data, tx_len);

  spi_rx(spi, rx_data, rx_len, 0x00);

  spi_cs_set_state(spi, GPIO_STATE_HIGH);

  return STATUS_CODE_OK;
}
