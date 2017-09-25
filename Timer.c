/* @owner ICosta
 *
 * Copyright of this program is the property of Ivo Costa, without
 * whose written permission reproduction in whole or in part is prohibited.
 */
/**
 * @file Timer.c
 *
 * @brief Services for Timer initialization
 *
 * @author I. Costa
 * @date 2015-16-10
 */

/* -- Includes */

/* Associated header */
#include "Timer.h"

/* Standard headers */           
#include <stdbool.h>

/* Functional headers */
#include "tm4c123gh6pm.h"
/* -- Constants */

/* -- Types */

/**
 * @brief Timer0A Init. (Wlan Connect Timout)
 *
 * @param[in]       Period			Interrupt Period
 *
 * @return None
 */
void Timer0A_Init(unsigned long Period){ 
	unsigned long volatile delay;

  SYSCTL_RCGCTIMER_R |= 0x01;   // 0) activate TIMER0
  delay = SYSCTL_RCGCTIMER_R;
  TIMER0_CTL_R = 0x00000000;    // 1) disable TIMER0A during setup
  TIMER0_CFG_R = 0x00000000;    // 2) configure for 32-bit mode
  TIMER0_TAMR_R = 0x00000002;   // 3) configure for periodic mode, default down-count settings
  TIMER0_TAILR_R = Period-1;    // 4) reload value
  TIMER0_TAPR_R = 0;            // 5) bus clock resolution
  TIMER0_ICR_R = 0x00000001;    // 6) clear TIMER0A timeout flag
  TIMER0_IMR_R = 0x00000001;    // 7) arm timeout interrupt
  //NVIC_PRI4_R = (NVIC_PRI4_R&0x00FFFFFF)|0x80000000; // 8) priority 4
	// interrupts enabled in the main program after all devices initialized
	// vector number 35, interrupt number 19
	//NVIC_EN0_R = 1<<19;           // 9) enable IRQ 19 in NVIC
  TIMER0_CTL_R |= 0x00000001;    // 10) Enable TIMER0A	
}

/**
 * @brief Timer0A Start.
 *
 * @return None
 */
void Timer0A_Start(){
	TIMER0_CTL_R |= 0x00000001;   // enable
}

/**
 * @brief Timer0A Stop.
 *
 * @return None
 */
void Timer0A_Stop(){
	TIMER0_CTL_R &= ~0x00000001; // disable
}

/**
 * @brief Enable All Interrupts.
 *
 * @return None
 */
void Enable_Interrupts(){

	Timer0A_Start();

}

/**
 * @brief Disable All Interrupts.
 *
 * @return None
 */
void Disable_Interrupts(){

	Timer0A_Stop();
}
