/* @owner ICosta
 *
 * Copyright of this program is the property of Ivo Costa, without
 * whose written permission reproduction in whole or in part is prohibited.
 */
/**
 * @file Clock.c
 *
 * @brief Services for Time Handling
 *
 * @author I. Costa
 * @date 2016-20-02
 */
 
 /* -- Includes */
/* Associated header */
#include "Clock.h"

/* Standard headers */
#include <time.h>

/* Functional headers */
#include "Delay.h"
#include "main.h"
#include "Nixie.h"

#include "Random.h"
#include "Rtc.h"
#include "Interface.h"
#include "Timer.h"
#include "driverlib/uart.h"
#include "utils/uartstdio.h"
#include "driverlib/timer.h"
#include "inc/hw_memmap.h"

/* -- Constants */

/* -- Types */
struct tm psTime;

uint8_t lastSec;
uint8_t lastHour;
	
/**
 * @brief Initializes the Clock_Init.
 *
 * @return None
 */
void	Clock_Init(void){ 
	
	  NixieClock_Init(); 	/** NixieClock Init */
		RTCInit();				 	/** RTC Init */
		Timer0A_Init(80000000/(1/20.0));	/** Init Timer 0A for Button */	
	
		/** Initialize Random Seed */
		Random_Init(TimerValueGet(TIMER0_BASE, TIMER_A));
	
}

/**
 * @brief Clock Show Hour.
 *
 * @return None
 */
void	Clock_Show_Hour(void){ 
	
		/** Acknowledge Action **/
		Sw_Ack();
							
		lastSec= psTime.tm_sec;		/** Save Second for toggle Dots*/
		lastHour= psTime.tm_hour;	/** Save Hour for Nixie Effects*/
		
		/** Read Time from RTC*/
		GetRtcTimeDate(&psTime);

		/** Display Time */
		NixieDisplay_Time(psTime.tm_hour,psTime.tm_min,psTime.tm_sec);
	
		/** Check last second and if diferent Toggle NixieDots. */
		if(difftime(psTime.tm_sec,lastSec) != 0){
			NixieDots_toggle(BoothDots,BlinkDots);
			lastSec= psTime.tm_sec;
		}
		
		/** Check last Hour and if diferent Random Display Nixie Effects. */
		if(difftime(psTime.tm_hour,lastHour) != 0){
			NixieDisplay_Effects(RandomX(NbEffects));
			lastHour= psTime.tm_hour;
		}
	
}	
	
/**
 * @brief Clock Show Date.
 *
 * @return None
 */
void	Clock_Show_Date(void){ 
	
		/** Acknowledge Action **/
		Sw_Ack();
		
		/** Read Time from RTC*/
		GetRtcTimeDate(&psTime);

		/** Display Date */
		NixieDisplay_Date(psTime.tm_mday,psTime.tm_mon,psTime.tm_year);

}	

/**
 * @brief Hour Blink Dots Left
 *
 * @return None
 */
void	Clock_Hour_Blink_Dots_Left(void){ 
	
		/** Acknowledge Action **/
		Sw_Ack();
		
		/** Blink Left Dots */
		NixieDots_toggle(LeftDots,BlinkDots);

}	

/**
 * @brief Hour Blink Dots Right
 *
 * @return None
 */
void	Clock_Hour_Blink_Dots_Right(void){ 
	
		/** Acknowledge Action **/
		Sw_Ack();
		
	  /** Blink Right Dots */
		NixieDots_toggle(RightDots,BlinkDots);

}	

/**
 * @brief Date Blink Dots Left
 *
 * @return None
 */
void	Clock_Date_Blink_Dots_Left(void){
	
		/** Acknowledge Action **/
		Sw_Ack();
		
		/** Display Date */
		NixieDisplay_Date(psTime.tm_mday,psTime.tm_mon,psTime.tm_year);
	
	  /** Blink Left Dots */
		NixieDots_toggle(LeftDots,BlinkDots);
	
}

/**
 * @brief Date Blink Dots Right
 *
 * @return None
 */
void	Clock_Date_Blink_Dots_Right(void){
	
		/** Acknowledge Action **/
		Sw_Ack();
		
		/** Blink Right Dots */
		NixieDots_toggle(RightDots,BlinkDots);
	
}

/**
 * @brief Date No Blink Dots
 *
 * @return None
 */
void	Clock_Date_No_Blink_Dots(void){
	
		/** Acknowledge Action **/
		Sw_Ack();
		
		/** Dots ON */
		NixieDots_toggle(BoothDots,DotsOn);
	
}

/**
 * @brief Clock Set
 *
 * @return None
 */
void	Clock_SetTime(void){
	
		/** Acknowledge Action **/
		Sw_Ack();
		
		/** Dots Off */
		NixieDots_toggle(BoothDots,DotsOff);
		/** Set Seconds to Zero */
		psTime.tm_sec = 0;
		/** Set RTC */
		SetRtcTime(psTime);
	
}

/**
 * @brief Clock Set Date
 *
 * @return None
 */
void	Clock_SetDate(void){
	
		/** Acknowledge Action **/
		Sw_Ack();
		/** Dots Off */
		NixieDots_toggle(BoothDots,DotsOff);
		/** Set RTC */
		SetRtcDate(psTime);
	
}

/**
 * @brief Clock Inc Hour
 *
 * @return None
 */
void	Clock_Inc_Hour(void){
	
		/** Acknowledge Action **/
		Sw_Ack();
		
		/** Increment Hour */
		psTime.tm_hour++;
	if(psTime.tm_hour>23 ){
			psTime.tm_hour = 0;
	}
	/** Display Time */
	NixieDisplay_Time(psTime.tm_hour,psTime.tm_min,0);
	
}

/**
 * @brief Clock Dec Hour
 *
 * @return None
 */
void	Clock_Dec_Hour(void){
	
		/** Acknowledge Action **/
		Sw_Ack();
	
		/** Decrement Hour */
		psTime.tm_hour--;
	if(psTime.tm_hour<0 ){
			psTime.tm_hour = 23;
	}
	/** Display Time */
	NixieDisplay_Time(psTime.tm_hour,psTime.tm_min,0);	
}

/**
 * @brief Clock Inc Minutes
 *
 * @return None
 */
void	Clock_Inc_Min(void){
	
		/** Acknowledge Action **/
		Sw_Ack();
		
		/** Increment Minutes */
		psTime.tm_min++;
	if(psTime.tm_min>59 ){
			psTime.tm_min = 0;
	}
	/** Display Time */
	NixieDisplay_Time(psTime.tm_hour,psTime.tm_min,0);
	
}

/**
 * @brief Clock Dec Minutes
 *
 * @return None
 */
void	Clock_Dec_Min(void){
	
		/** Acknowledge Action **/
		Sw_Ack();
		
		/** Decrement Minutes */
		psTime.tm_min--;
	if(psTime.tm_min<0 ){
			psTime.tm_min = 59;
	}
	/** Display Time */
	NixieDisplay_Time(psTime.tm_hour,psTime.tm_min,0);	
}

/**
 * @brief Clock Inc Day
 *
 * @return None
 */
void	Clock_Inc_Day(void){
	
		/** Acknowledge Action **/
		Sw_Ack();
		
		/** Increment Day */
		psTime.tm_mday++;
	if(psTime.tm_mday>31 ){
			psTime.tm_mday = 1;
	}
	/** Display Date */
	NixieDisplay_Date(psTime.tm_mday,psTime.tm_mon,psTime.tm_year);
	
}

/**
 * @brief Clock Dec Day
 *
 * @return None
 */
void	Clock_Dec_Day(void){
	
		/** Acknowledge Action **/
		Sw_Ack();
		
		/** Decrement Day */
		psTime.tm_mday--;
	if(psTime.tm_mday<1 ){
			psTime.tm_mday = 31;
	}
	/** Display Date */
	NixieDisplay_Date(psTime.tm_mday,psTime.tm_mon,psTime.tm_year);	
}

/**
 * @brief Clock Inc Month
 *
 * @return None
 */
void	Clock_Inc_Month(void){
	
		/** Acknowledge Action **/
		Sw_Ack();
		
		/** Increment Month */
		psTime.tm_mon++;
	if(psTime.tm_mon>12 ){
			psTime.tm_mon = 1;
	}
	/** Display Date */
	NixieDisplay_Date(psTime.tm_mday,psTime.tm_mon,psTime.tm_year);
	
}

/**
 * @brief Clock Dec Day
 *
 * @return None
 */
void	Clock_Dec_Month(void){
	
		/** Acknowledge Action **/
		Sw_Ack();
		
		/** Decrement Month */
		psTime.tm_mon--;
	if(psTime.tm_mon<1 ){
			psTime.tm_mon = 12;
	}
	/** Display Date */
	NixieDisplay_Date(psTime.tm_mday,psTime.tm_mon,psTime.tm_year);	
}

/**
 * @brief Clock Inc Year
 *
 * @return None
 */
void	Clock_Inc_Year(void){
	
		/** Acknowledge Action **/
		Sw_Ack();
		
		/** Increment Year */
		psTime.tm_year++;
	if(psTime.tm_year>99 ){
			psTime.tm_year = 0;
	}
	/** Display Date */
	NixieDisplay_Date(psTime.tm_mday,psTime.tm_mon,psTime.tm_year);
	
}

/**
 * @brief Clock Dec Year
 *
 * @return None
 */
void	Clock_Dec_Year(void){
	
		/** Acknowledge Action **/
		Sw_Ack();
		
		/** Decrement Year */
		psTime.tm_year--;
	if(psTime.tm_year<0 ){
			psTime.tm_year = 99;
	}
	/** Display Date */
	NixieDisplay_Date(psTime.tm_mday,psTime.tm_mon,psTime.tm_year);	
}
