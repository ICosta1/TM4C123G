/**
 * @file main.c
 *
 * @brief AutoSpacer
 * @brief Hardware requirements 
 *									: TM4C123 LaunchPad	
 *
 * @author I. Costa
 * @date 18-09-2017
 */
 
 /* @owner ICosta
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *    Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 *    Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the
 *    distribution.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
*/

/* -- Includes */
/* Associated header */
#include "main.h"

/* Standard headers */
#include <stdbool.h>
#include <stdio.h>  

/* Functional headers */
#include "Uart.h"
#include "Delay.h"
#include "interface.h"
#include "tm4c123gh6pm.h"

/* -- Constants */

/* -- Types */  

/* -- Variables */
char version[] = "v2"; 										/** Version Control*/

	
/* Private function prototypes -----------------------------------------------*/
void PLL_Init(void);											/** Enable System Clock */
 
#define ENA			0x02 	// Yelloe
#define DIR			0x04 	// Blue
#define PULL		0x08	// Green

 
/*
 * Application's entry point
 */

int main(void){
  
	
	PLL_Init();					/** Clock COnfig */
	Uart0_Init();
	volatile unsigned long delay;
  SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOE;     // 1) E Clock
  delay = SYSCTL_RCGC2_R;           				// 2) delay   
	
	/*GPIOE Configuratons*/
  GPIO_PORTE_AMSEL_R &= ~0x0E; 							// 3) disable analog function on PE1-3
  GPIO_PORTE_PCTL_R &= ~0x00FFFFFF; 				// 4) enable regular 
  GPIO_PORTE_DIR_R |= 0x0E;    							// 5) outputs on PE1-3
  GPIO_PORTE_AFSEL_R &= ~0x0E; 							// 6) regular function on PE1-3
	//GPIO_PORTE_PDR_R = 0x0E;
  GPIO_PORTE_DEN_R |= 0x0E;    							// 7) enable digital on PE1-3
	
	//UARTprintf("75634501976281");
	
	
	while(1){
		
		UART_MessageParser();

//    GPIO_PORTE_DATA_R &=~ DIR;								/* Reset  */
//		GPIO_PORTE_DATA_R &=~ ENA;								/* ReSet  */
//		
//for (int i=0; i<400; i++)    //Forward 5000 steps
//  {
//		GPIO_PORTE_DATA_R |= PULL;							/* Set  */
//		Delay_12ns(65000);
//		GPIO_PORTE_DATA_R &=~ PULL;							/* Reset  */
//		Delay_12ns(65000);
//		
//  }
//				
//	GPIO_PORTE_DATA_R |= ENA;									/* Set  */
//	Delay_100ms(50);
//	

//		GPIO_PORTE_DATA_R |= DIR;			
//		GPIO_PORTE_DATA_R &=~ ENA;								/* ReSet  */
//	
//  for (int i=0; i<400; i++)   //Backward 5000 steps
//  {

//		GPIO_PORTE_DATA_R |= PULL;							/* Set  */
//		Delay_12ns(65000);
//		GPIO_PORTE_DATA_R &=~ PULL;							/* Reset  */
//		Delay_12ns(65000);
//  }

//			GPIO_PORTE_DATA_R  |= ENA;								/* Set  */
//	Delay_100ms(50);
	}
}

void PLL_Init(void){
  // 0) configure the system to use RCC2 for advanced features
  //    such as 400 MHz PLL and non-integer System Clock Divisor
  SYSCTL_RCC2_R |= SYSCTL_RCC2_USERCC2;
  // 1) bypass PLL while initializing
  SYSCTL_RCC2_R |= SYSCTL_RCC2_BYPASS2;
  // 2) select the crystal value and oscillator source
  SYSCTL_RCC_R &= ~SYSCTL_RCC_XTAL_M;   // clear XTAL field
  SYSCTL_RCC_R += SYSCTL_RCC_XTAL_16MHZ;// configure for 16 MHz crystal
  SYSCTL_RCC2_R &= ~SYSCTL_RCC2_OSCSRC2_M;// clear oscillator source field
  SYSCTL_RCC2_R += SYSCTL_RCC2_OSCSRC2_MO;// configure for main oscillator source
  // 3) activate PLL by clearing PWRDN
  SYSCTL_RCC2_R &= ~SYSCTL_RCC2_PWRDN2;
  // 4) set the desired system divider and the system divider least significant bit
  SYSCTL_RCC2_R |= SYSCTL_RCC2_DIV400;  // use 400 MHz PLL
  SYSCTL_RCC2_R = (SYSCTL_RCC2_R&~0x1FC00000) // clear system clock divider field
                  + (4<<22);      // configure for 80 MHz clock
  // 5) wait for the PLL to lock by polling PLLLRIS
  while((SYSCTL_RIS_R&SYSCTL_RIS_PLLLRIS)==0){};
  // 6) enable use of PLL by clearing BYPASS
  SYSCTL_RCC2_R &= ~SYSCTL_RCC2_BYPASS2;
}

