// This code snippet demonstrates the behavior of UART communication and serial output.
// When the user writes "The provided code is a header file named "PAT_RTOS_UART0.h" for managing UART communication in a real-time operating system (RTOS) environment. Here's a summary of what each part of the code does:" over UART,
// after 500 milliseconds, the user writes "0123456789" through the Serial Monitor.
// The expected serial output is as follows, with each line prefixed by "Received=" and displayed every 5 seconds:
// Received= The provided code is a header file named "PAT_RTOS_UART0.h" for managing UART communication in a real-time operating system (RTO
// Received= S) environment. Here's a summary of what each part of the code does:
// Received= 0123456789

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include <Arduino.h>  // Include Arduino core library
#include "PAT_RTOS_UART0.h"  // Include custom UART driver header file
#include "freertos/FreeRTOS.h"  // Include FreeRTOS headers
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/uart.h"  // Include ESP32 UART driver

/////////////////////////////////////////////////////////////////CONFIG////////////////////////////////////////////////////////////////////////////////////////
#define _TXD0_PIN (GPIO_NUM_1)  // Define TX pin for UART0
#define _RXD0_PIN (GPIO_NUM_3)  // Define RX pin for UART0

QueueHandle_t dataQueue_uart0;  // Queue handle for UART0 data
const int _BAUD_RATE = 115200;  // UART baud rate
const size_t _UART0_QUEUE_LENGTH = 10;  // Length of UART0 queue

const size_t _UART0_QUEUE_SIZE = 128;  // Size of UART0 queue
const int _RX0_BUF_SIZE = 128;  // Size of UART0 RX buffer
const int _TX0_BUF_SIZE = 128;  // Size of UART0 TX buffer
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Task to handle UART0 data reading
void task_uart0_driver(void* parameter) {
  char data[_RX0_BUF_SIZE];  // Buffer to store received data
  while (1) {
    // Read data from UART
    const int rxBytes = uart_read_bytes(UART_NUM_0, &data, _RX0_BUF_SIZE, 1 / portTICK_RATE_MS);  // 1ms for waiting
    if (rxBytes > 0) {
      data[rxBytes] = '\0';  // Null-terminate the received data
      //Serial.println(data);
      // Send the data to the queue
      if (xQueueSend(dataQueue_uart0, &data, 1 / portTICK_RATE_MS) != pdTRUE) {
        Serial.println("Failed to send data to queue");
      }
    }
    vTaskDelay(500 / portTICK_PERIOD_MS);  // Delay before reading next data
  }
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Initialize UART0 driver
void uart0_driver_init(void) {
  const uart_config_t uart0Config = {
    .baud_rate = _BAUD_RATE,  // Configure UART baud rate
    .data_bits = UART_DATA_8_BITS,  // Configure data bits
    .parity = UART_PARITY_DISABLE,  // Disable parity
    .stop_bits = UART_STOP_BITS_1,  // Configure stop bits
    .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,  // Disable hardware flow control
  };
  
  uart_param_config(UART_NUM_0, &uart0Config);  // Set UART parameters
  uart_set_pin(UART_NUM_0, _TXD0_PIN, _RXD0_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);  // Set UART pin configuration
  uart_driver_install(UART_NUM_0, _RX0_BUF_SIZE, _TX0_BUF_SIZE, 0, NULL, 0);  // Install UART driver with buffers
  dataQueue_uart0 = xQueueCreate(_UART0_QUEUE_LENGTH, _UART0_QUEUE_SIZE);  // Create UART0 data queue
  xTaskCreatePinnedToCore(task_uart0_driver, "UART0 Task", 1024 * 4, NULL, configMAX_PRIORITIES, NULL, 0);  // Create UART0 task
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
