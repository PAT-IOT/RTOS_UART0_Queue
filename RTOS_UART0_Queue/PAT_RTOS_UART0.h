#ifndef PAT_RTOS_UART0_H
#define PAT_RTOS_UART0_H

extern QueueHandle_t dataQueue_uart0;  // Declaration of the UART data queue handle
extern const size_t _UART0_QUEUE_LENGTH;  // Declaration of the UART data queue length
extern const size_t _UART0_QUEUE_SIZE;    // Declaration of the UART data queue size
extern const int _RX0_BUF_SIZE;   // Declaration of the receive buffer size for UART
extern const int _TX0_BUF_SIZE;   // Declaration of the transmit buffer size for UART

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void uart0_driver_init(void);  // Function prototype for initializing UART driver
void task_uart0_driver(void* parameter);  // Function prototype for UART task
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif  // PAT_RTOS_UART0_H
