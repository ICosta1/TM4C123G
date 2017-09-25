/* @owner ICosta
 *
 * Copyright of this program is the property of Ivo Costa, without
 * whose written permission reproduction in whole or in part is prohibited.
 */
/**
 * @file Uart.c
 *
 * @brief Services for Uart Handling
 *
 * @author I. Costa
 * @date 2017-19-09
 */
 
 /* -- Includes */
/* Associated header */
#include "uart.h"

/* Standard headers */
#include <stdbool.h>
#include <string.h>

/* Functional headers */
#include "main.h"
#include "driverlib/gpio.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "inc/hw_memmap.h"
#include "tm4c123gh6pm.h"

/* -- Constants */
#define UART_MAX_DATA					0x14 /** Max Size 20 */
/** Uart Buffer Size = Header(1) + Length(1)+ Data(20) + ChkSum (2) */
#define UART_BUFFER_SIZE			(1 + 1 + UART_MAX_DATA + 2)
#define UART_HEADER						0xAB


/** Define System message */
#define UART_SYS_MSG					0x0A
	#define UART_SYS_GET_FW_VERSION				0x01
		#define UART_SYS_GET_FW_VERSION_LEN				0x05
	#define UART_SYS_DIS_CHKSUM						0x02
	
	
	
/** Define Acknowledge message */	
#define UART_SYS_ACK					0xAC
	#define UART_SYS_ACK_LEN							0x03 /** Message Response Header + Sub Message + Ack */
	
	
/** Define Error Messages */
#define UART_ERR							0x0F
	#define UART_ERR_MSG_LEN							0x02
	#define UART_ERR_WRONG_HEADER					0x01
	#define UART_ERR_MAX_DATA_EXCEEDED		0x02
	#define UART_ERR_WRONG_CHKSUM					0x03
	#define UART_ERR_UNK_MSG							0x04
	#define UART_ERR_VALUE_OUT_OF_RANGE		0x05

/* -- Types */
//** Uart Rcv Buffer */
volatile uint8_t UartRcvBuffer[UART_MAX_DATA];
//** Uart Trx Buffer */
volatile uint8_t UartSndBuffer[UART_BUFFER_SIZE];
//** Disable CheckSum, Default = Enable */
static bool bDisableChkSum = false;
//** Threat Uart Message on LPT */
static bool bUartMailBox = false;


void UART_Send(volatile uint8_t *pui8Buffer);
static bool UART_ChkSumChk(volatile const uint8_t *pui8Buffer);
static uint16_t UART_CalcChkSum(volatile const uint8_t *pui8Buffer);
static void UART_DisableChkSum(bool bEnable);
void UART_MessageParser(void);

/**
 * @brief Initialize Uart0
 *
 * This function handles all the initialization needed 
 * for the UART0 (Clock,PinConfig,UARTBaudRate)
 *
 * @return None
 */
void Uart0_Init(void){
	
  ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
  ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);

	/** Enable processor interrupts. */
  ROM_IntMasterEnable();
   
	ROM_GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
  ROM_UARTConfigSetExpClk(UART0_BASE, 
													ROM_SysCtlClockGet(), 
													115200,
                         (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));

	/** Enable the UART interrupt. */
  ROM_IntEnable(INT_UART0);
  ROM_UARTIntEnable(UART0_BASE, UART_INT_RX | UART_INT_RT);
}


/**
 * @brief UART Messages.Interrupt Handler
 *
 * This function handles interrupts from the UART0.  It will copy data from the
 * UART receive FIFO to the receive buffer if data is available, and set Mailbox to true
 *
 * @return None
 */
void UARTIntHandler(void){
	
  uint32_t	Status;
	uint8_t		RcvIndex 	= 0;
	uint8_t		OrigArrayIndex 	= 0;
	uint8_t		DestArrayIndex 	= 0;
	//uint8_t 	UartSndBuffer[5];
	volatile 	uint8_t 	UartTmpRcvBuffer[UART_BUFFER_SIZE];
	
  /** Get the interrrupt status. */
  Status = ROM_UARTIntStatus(UART0_BASE, true);

  /** Acknowledge Interrupr. */
	ROM_UARTIntClear(UART0_BASE, Status);

  /** Loop while there are characters in the receive FIFO. */
  while(ROM_UARTCharsAvail(UART0_BASE)){	
		/** Read the next character from the UART and write it back to the UART. */
		UartTmpRcvBuffer[RcvIndex] = ROM_UARTCharGetNonBlocking(UART0_BASE);
		RcvIndex++;
	}	

	if((UartTmpRcvBuffer[0]) != UART_HEADER){
		/** Wrong Header */
		UartSndBuffer[1] = UART_ERR_MSG_LEN;
		UartSndBuffer[2] = UART_ERR;
		UartSndBuffer[3] = UART_ERR_WRONG_HEADER;	
		UART_Send(UartSndBuffer);
	}else if((UartTmpRcvBuffer[1])> UART_MAX_DATA){
		/** Message To big */
		UartSndBuffer[1] = UART_ERR_MSG_LEN;
		UartSndBuffer[2] = UART_ERR;
		UartSndBuffer[3] = UART_ERR_MAX_DATA_EXCEEDED;	
		UART_Send(UartSndBuffer);
	}else	if(!UART_ChkSumChk(UartTmpRcvBuffer)){
		/** Wrong ChkSum */
		UartSndBuffer[1] = UART_ERR_MSG_LEN;
		UartSndBuffer[2] = UART_ERR;
		UartSndBuffer[3] = UART_ERR_WRONG_CHKSUM;	
		UART_Send(UartSndBuffer);
	}else{	
	/* Copying data from array "UartTmpRcvBuffer" to array "UartRcvBuffer" */
   for (OrigArrayIndex = 2; OrigArrayIndex < (UartTmpRcvBuffer[1]); OrigArrayIndex++) {
      UartRcvBuffer[DestArrayIndex] = UartTmpRcvBuffer[OrigArrayIndex];
			DestArrayIndex++;
   }
	 /** Set MailBox UartRcv to true */
	 bUartMailBox = true;
	}
		
}


/**
 * @brief Send UART Messages.
 *
 * @param[in]	pui8Buffer	Pointer to the buffer to send
 *
 * @return None
 */
void UART_Send(volatile uint8_t *pui8Buffer){
	
	uint16_t	ComputedChkSum = 0;
	uint8_t 	ui8Count			 = 1 + (pui8Buffer[1]) + 1 + 2; /** Header + Data + Length + ChkSum*/
	
	/** Add Uart Header*/
	pui8Buffer[0] = UART_HEADER;
	/** Add ChkSum To Length */
	pui8Buffer[1] += 2;
	
	/** Calculate Message Check Sum */
	ComputedChkSum = UART_CalcChkSum(pui8Buffer);
	
	/** Copy LSB to send */
	pui8Buffer[ui8Count-2] = (ComputedChkSum >> 8);
	/** Copy MSB to send */
	pui8Buffer[ui8Count-1] = (ComputedChkSum & 0x00FF);
		
	/** Loop while there are more characters to send.*/
  while(ui8Count--){
		/** Write the next character to the UART */
    ROM_UARTCharPutNonBlocking(UART0_BASE, *pui8Buffer++);
  }
	
	/** Clear Buffer */
	memset(&pui8Buffer, 0, sizeof pui8Buffer);
	
}


/**
 * @brief Verify Uart Message Rcv ChkSum
 *
 * @param[in]	pui8Buffer	Pointer to the buffer Rcv
 *
 * @return true if ChkSum Equal, false otherwise
 */
static bool UART_ChkSumChk(volatile const uint8_t *pui8Buffer){
	
	uint16_t	ComputedChkSum = 0;
	uint16_t	RcvChkSum = 0;
	
	/** Calculate Message Check Sum */
	ComputedChkSum = UART_CalcChkSum(pui8Buffer);
	
	/** Retrieve Message ChekSum */	
	RcvChkSum = (pui8Buffer[pui8Buffer[1]]) << 8;
	RcvChkSum |= pui8Buffer[pui8Buffer[1]+1];
		
	/** Compare RcvChkSum with Compute ChkSum*/
	if(RcvChkSum != ComputedChkSum){
		/** ChkSum !OK*/
		return false;
	}
		
	/** ChkSum Ok*/
	return true;
}


/**
 * @brief Calculate CheckSum
 *
 * @param[in]	pui8Buffer	Pointer to the data to calculate ChkSum
 *
 * @return 16-bit CheckSum
 */
static uint16_t UART_CalcChkSum(volatile const uint8_t *pui8Buffer)
{
	uint8_t		ChkSumLoopIndex = 0; 
	uint16_t	ComputedChkSum = 0;
	
	if(!bDisableChkSum){
		/** Calculate Message Check Sum */
		/** Retrive loop index from Message Length pos1 */
		for(ChkSumLoopIndex = 2; ChkSumLoopIndex < pui8Buffer[1] ; ChkSumLoopIndex++ ){
			ComputedChkSum += pui8Buffer[ChkSumLoopIndex];	
		}
	}
	
	/** Return ChkSum */
	return ComputedChkSum;
	
}


/**
 * @brief Enables or disables ChkSum of Received/Transmitted message.
 *
 * @param[in]	bEnable must be set to \b true to enable ChkSum or \b false to disable it.
 *
 * @return None
 */
static void UART_DisableChkSum(bool bEnable){
	
    bDisableChkSum = !bEnable;
}


/**
 * @brief Parse Uart Received message.
 *
 * Parse data on store on UartRcvBuffer
 *
 * @return None
 */
void UART_MessageParser(){
	
	if(bUartMailBox){
		bUartMailBox = false;
		/** Start Message Parse */
		switch (UartRcvBuffer[0]) {
			/** Parse System Message */
			case UART_SYS_MSG:
				switch (UartRcvBuffer[1]) {
					/** Get Fw Version */
				case UART_SYS_GET_FW_VERSION:
					UartSndBuffer[1] = UART_SYS_GET_FW_VERSION_LEN;
					UartSndBuffer[2] = UART_SYS_MSG;
					UartSndBuffer[3] = UART_SYS_GET_FW_VERSION;
					UartSndBuffer[4] = MAJOR;
					UartSndBuffer[5] = MINOR;
					UartSndBuffer[6] = BUILD;	
					UART_Send(UartSndBuffer);
					break;
					/** Get Disable Uart CheckSum */
				case UART_SYS_DIS_CHKSUM:
					/* Check That Value is in range */
					if((UartRcvBuffer[2] == true ) || (UartRcvBuffer[2] == false )){
						UART_DisableChkSum(UartRcvBuffer[2]);
						UartSndBuffer[1] = UART_SYS_ACK_LEN;
						UartSndBuffer[2] = UART_SYS_MSG;
						UartSndBuffer[3] = UART_SYS_DIS_CHKSUM;
						UartSndBuffer[4] = UART_SYS_ACK;
						UART_Send(UartSndBuffer);
					}else{
						UartSndBuffer[1] = UART_ERR_MSG_LEN;
						UartSndBuffer[2] = UART_ERR;
						UartSndBuffer[3] = UART_ERR_VALUE_OUT_OF_RANGE;
						UART_Send(UartSndBuffer);
					}				
					break;
				default:
					/* Unknown Message */
					UartSndBuffer[1] = UART_ERR_MSG_LEN;
					UartSndBuffer[2] = UART_ERR;
					UartSndBuffer[3] = UART_ERR_UNK_MSG;	
					UART_Send(UartSndBuffer);
					break;
				}
				break;	
		//case UART_SYS_GET_FW_VERSION:
			/** Unused  */
			//break;
		default:
			/* Unknown Message */
			UartSndBuffer[1] = UART_ERR_MSG_LEN;
			UartSndBuffer[2] = UART_ERR;
			UartSndBuffer[3] = UART_ERR_UNK_MSG;	
			UART_Send(UartSndBuffer);
			break;
		}
		
		/** Clear Buffer */
		memset(&UartRcvBuffer, 0, sizeof UartRcvBuffer);
	}		

}
