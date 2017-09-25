/**
 * @file Interface.c
 *
 * @brief Interface Services
 *
 * @author I. Costa
 * @date 2016-10-02
 */

/* -- Includes */

/* Associated header */
#include "interface.h"

/* Standard headers */       
#include <stdbool.h>
#include <stdio.h>
#include <math.h>

/* Functional headers */
#include "tm4c123gh6pm.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "inc/hw_memmap.h"
#include "driverlib/uart.h"
#include "utils/uartstdio.h"
#include "driverlib/timer.h"

/* -- Constants */
#define DEBOUNCE 			800000 				// 10ms = (0,01 /0,000000125)
#define RELOADVALUE 	1599999999.0 	// Timer Reload Value 
#define TIMERPERIOD		20						// Seconds
#define LONGPRESS			3							// Seconds

/* -- Types */
uint32_t volatile New_Count  = 0;
uint32_t volatile Last_Count = 0;
uint32_t volatile Diff_Count = 0;

typedef struct Switch{
	uint8_t 	volatile	Pressed;
	uint8_t 	volatile	Action;
	uint32_t 	volatile	TimePressed;
	uint32_t 	volatile 	TimeReleased;	
	uint32_t 	volatile	Time;
}Switch_t;

Switch_t Switch;

/* Private variables ---------------------------------------------------------*/

/**
 * @brief Initializes the Buttons interface
 *
 * @return None
 */
void Buttons_Init(void){ 
  
	unsigned long volatile delay;
  SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOA; 	/** activate port A */
  delay = SYSCTL_RCGC2_R;    							/** allow time to finish activating */
  GPIO_PORTA_AMSEL_R &= ~0x0C;      			/** no analog */
  GPIO_PORTA_PCTL_R &= 0x00000000; 				/** regular function */
  GPIO_PORTA_DIR_R &= ~0x0C;      				/** make PA2, PA3 In */
  GPIO_PORTA_AFSEL_R &= ~0x0C;   					/** disable alt funct on PA2, PA3 */
  GPIO_PORTA_DEN_R |= 0x0C;      					/** enable digital I/O on PA2, PA3 */
	GPIO_PORTA_PDR_R |= 0x0C;								/** Pull-Down on PA2, PA3 */
	
  GPIO_PORTA_IS_R &= ~0x0C;								/** PA2, PA3 is edge-sensitive */
  GPIO_PORTA_IBE_R |= 0x0C;    						/** PA2, PA3 is not both edges */
  //GPIO_PORTA_IEV_R &= ~0x0C;    				/** PA2, PA3 Falling edge event */
  GPIO_PORTA_ICR_R = 0x0C;      					/** clear flag */
  GPIO_PORTA_IM_R |= 0x0C;      					/**  arm interrupt on */
  NVIC_PRI0_R = (NVIC_PRI0_R&0xFFFF0FFF)|0x000000A0; /** priority 5 */
  NVIC_EN0_R = 0x00000001;      					/** enable interrupt 0 in NVIC */

}

 /**
 * @brief GPIOA Interrupt Handler
 *
 * @return None
 */
void GPIOA_intHandler(void){

	/**  Debounce Buttons */
	New_Count=TimerValueGet(TIMER0_BASE, TIMER_A);
	if(New_Count > Last_Count){
		Diff_Count = (RELOADVALUE - New_Count) + Last_Count;
	}else{
		Diff_Count = Last_Count - New_Count;
	}
	Last_Count = New_Count;
	
	if(GPIO_PORTA_RIS_R&0x04){  		/** poll PA2	*/
    GPIO_PORTA_ICR_R = 0x04;  		/** acknowledge flag2 */
		if(Diff_Count < DEBOUNCE){
			return;
		}
		if(Switch.Pressed != true)
		{
			Switch.Pressed = true ;
			Switch.TimePressed = TimerValueGet(TIMER0_BASE, TIMER_A);
			return ;
		}else{
			Switch.Pressed = false ; 
			Switch.TimeReleased = TimerValueGet(TIMER0_BASE, TIMER_A);
			if(Switch.TimeReleased>Switch.TimePressed){
				Switch.Time =(uint32_t) roundf(((RELOADVALUE - Switch.TimeReleased) + Switch.TimePressed) * (TIMERPERIOD/RELOADVALUE));			
			}else{
				Switch.Time =(uint32_t) roundf((Switch.TimePressed-Switch.TimeReleased)*(TIMERPERIOD/RELOADVALUE));		
			}
		}
		/** Determine SW Action */
		if (Switch.Time < LONGPRESS){
			Switch.Action = SW1SmallPress;
		}else{
			Switch.Action = SW1LongPress;
		}
  }
	
  if(GPIO_PORTA_RIS_R&0x08){  		/** poll PA3 */
    GPIO_PORTA_ICR_R = 0x08;  		/** acknowledge flag3 */
		if(Diff_Count < DEBOUNCE){
			return;
		}
		if(Switch.Pressed != true)
		{
			Switch.Pressed = true ;
			Switch.TimePressed = TimerValueGet(TIMER0_BASE, TIMER_A);
			return;
		}else{
			Switch.Pressed = false ;
			Switch.TimeReleased = TimerValueGet(TIMER0_BASE, TIMER_A);
			if(Switch.TimeReleased>Switch.TimePressed){
				Switch.Time = (uint32_t) roundf(((RELOADVALUE - Switch.TimeReleased) + Switch.TimePressed) * (TIMERPERIOD/RELOADVALUE));
			}else{
				Switch.Time = (uint32_t) roundf((Switch.TimePressed-Switch.TimeReleased)*(TIMERPERIOD/RELOADVALUE));
			}
		}
		/** Determine SW Action */
		if (Switch.Time < LONGPRESS){
			Switch.Action = SW2SmallPress;
		}else{
			Switch.Action = SW2LongPress;
		}
	}	

}

 /**
 * @brief Acknowledege SW1
 *
 * @return None
 */
void Sw_Ack(void){
	
	Switch.Action = NoAction; 
}

/**
 * @brief Get Switch Ation
 *
 * @return Switch Action
 */
uint8_t Get_Switch_Action(void){
	
	return Switch.Action; 
}

