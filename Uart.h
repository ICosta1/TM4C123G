/* @owner ICosta
 *
 * Copyright of this program is the property of Ivo Costa, without
 * whose written permission reproduction in whole or in part is prohibited.
 */
/**
 * @file Uart.h
 *
 * @brief Prototypes for Uart Handling
 *
 * @author I. Costa
 * @date 2017-19-09
 */
 
 
#ifndef __Uart_H__
#define __Uart_H__

 /* -- Includes */

/* Associated header */

/* Standard headers */
#include <stdint.h>

/* Functional headers */

/* -- Constants */
/* -- Variables */
/* -- Macros */
/* -- Types */

/**
 * @brief Initialize Uart0
 *
 * This function handles all the initialization needed 
 * for the UART0 (Clock,PinConfig,UARTBaudRate)
 *
 * @return None
 */
void Uart0_Init(void);

/**
 * @brief Send UART Messages.
 *
 * @param[in]	pui8Buffer	Pointer to the buffer to send
 *
 * @return None
 */
void UART_Send(volatile uint8_t *pui8Buffer);

/**
 * @brief Parse Uart Received message.
 *
 * Parse data on store on UartRcvBuffer
 *
 * @return None
 */
void UART_MessageParser(void);




#endif /* __Uart_H__ */
