/* @owner ICosta
 *
 * Copyright of this program is the property of Ivo Costa, without
 * whose written permission reproduction in whole or in part is prohibited.
 */
/**
 * @file Nixe.c
 *
 * @brief Services for Nixie Clock
 *
 * @author I. Costa
 * @date 2015-17-10
 */
 
 /* -- Includes */

/* Associated header */
#include "Nixie.h"

/* Standard headers */
#include <time.h>

/* Functional headers */
#include "Delay.h"
#include "driverlib/rom.h"

#include "inc/hw_memmap.h"

#include "main.h"

#include "Random.h"

#include "ShiftRegister.h"

#include "tm4c123gh6pm.h"

#include "Utils.h"

/* -- Constants */

/* -- Types */

/* -- Variables */
extern unsigned int bitOrder;
extern struct tm psTime;

/* -- Macros */

/**
 * @brief Toggle Dots.
 *
 * @param[in]				Dots 
 * @param[in]				Action
 *
 * @return None
 */
void NixieDots_toggle(uint8_t dots, uint8_t action){
	
	if(action == BlinkDots){
		GPIO_PORTB_DATA_R = (GPIO_PORTB_DATA_R^dots);		/* Toggle  Dots */
		return;
	}else if (action == DotsOn){
		GPIO_PORTB_DATA_R = dots;
		return;
	}else{
		GPIO_PORTB_DATA_R = DotsOff;
	}
}

/**
 * @brief Initializes the Nixie Power.
 *
 * @return None
 */
static void NixiePower_Init(void){ 
	
	volatile unsigned long delay;
  SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOB;         		// 1) activate Port B
  delay = SYSCTL_RCGC2_R;         		// allow time for clock to stabilize

	GPIO_PORTB_AMSEL_R &= ~BoothDots;     		// 3) disable analog functionality on PB5,PB3
	GPIO_PORTB_PCTL_R =~0x00FFFFFF; 		// 4) configure PB5,PB3 as GPIO
	GPIO_PORTB_DIR_R |= BoothDots;        		// 5) PB5,PB3 out
	GPIO_PORTB_AFSEL_R = ~BoothDots;      		// 6) disable alt funct on PB5,PB3
	GPIO_PORTB_DEN_R |= BoothDots;        		// 7) enable digital I/O on PB5,PB3
}
 
/**
 * @brief Initializes the Nixie Clock.
 *
 * @return None
 */
void NixieClock_Init(void){ 
	
	NixiePower_Init();
	ShiftRegister_Init(LSBFIRST);
}
 
/**
 * @brief Display Numbers on Nixie Tubes
 *
 * @param[in]				First2digits to Display  
 * @param[in]				Middle2digits to Display    
 * @param[in]				Last2digits to Display  
 *
 * @return None
 */
static void NixieDisplay(uint16_t First2digits, uint16_t Middle2digits, uint16_t Last2digits){
		int i;
		unsigned int reg[24];
	
		for (i = 0; i < 24; i++) {
			if (bitOrder == LSBFIRST){
				if ((dec2bcd(First2digits)<<16 	& (1<<i)) || 
						(dec2bcd(Middle2digits)<<8 	& (1<<i)) || 
						(dec2bcd(Last2digits) 			& (1<<i)) ) {
					reg[i]=1;
				}else {
					reg[i]=0;
				}
			}else{
				if ((dec2bcd(First2digits)<<16 	& (1<<(23 - i))) || 
						(dec2bcd(Middle2digits)<<8 	& (1<<(23 - i)))  || 
						(dec2bcd(Last2digits) 			& (1<<(23 - i)))) {
					reg[i]=1;
				}else {
					reg[i]=0;
				}	
			}		
		}

		ShiftRegister_Output(reg,24);
}

	/**
 * @brief Display Numbers on Nixie Tubes Digit By Digit
 *
 * @param[in]				FirstDigit to Display  
 * @param[in]				SecondDigit to Display    
 * @param[in]				ThirdDigit to Display
 * @param[in]				FourthDigit to Display  
 * @param[in]				FifthDigit to Display    
 * @param[in]				SixthDigit to Display    
 *
 * @return None
 */
void NixieDisplayDigitbyDigit(uint8_t FirstDigit, 
																		 uint8_t SecondDigit, 
																		 uint8_t ThirdDigit,
																		 uint8_t FourthDigit,
																		 uint8_t FifthDigit,
																		 uint8_t SixthDigit){																	
		int i;
		unsigned int reg[24];
		
		for (i = 0; i < 24; i++) {
			if (bitOrder == LSBFIRST){
				if ((dec2bcd(FirstDigit)<<20 	& (1<<i)) || 
						(dec2bcd(SecondDigit)<<16 & (1<<i)) || 
						(dec2bcd(ThirdDigit)<<12 & (1<<i)) || 
						(dec2bcd(FourthDigit)<<8 	& (1<<i)) ||
						(dec2bcd(FifthDigit)<<4 	& (1<<i)) || 			
						(dec2bcd(SixthDigit) & (1<<i)) ) {
					reg[i]=1;
				}else {
					reg[i]=0;
				}
			}else{
				if ((dec2bcd(FirstDigit)<<20 & (1<<(23 - i))) || 
						(dec2bcd(SecondDigit)<<16 & (1<<(23 - i)))  ||
						(dec2bcd(ThirdDigit)<<12 & (1<<(23 - i)))  || 
						(dec2bcd(FourthDigit)<<8 & (1<<(23 - i)))  || 
						(dec2bcd(FifthDigit)<<4 & (1<<(23 - i)))  || 			
						(dec2bcd(SixthDigit) & (1<<(23 - i)))) {
					reg[i]=1;
				}else {
					reg[i]=0;
				}	
			}	
		}

		ShiftRegister_Output(reg,24);
}

/**
 * @brief Display Time on Nixie Tubes
 *	
 * @param[in]				Hour to Display  
 * @param[in]				Minutes to Display    
 * @param[in]				Seconds to Display  
 *
 * @return None
 */
void NixieDisplay_Time(uint16_t Hours, uint16_t Minuts, uint16_t Seconds){
	
	NixieDisplay(Hours,Minuts,Seconds);	
}

/**
 * @brief Display Date on Nixie Tubes
 *	
 * @param[in]				Day to Display  
 * @param[in]				Month to Display    
 * @param[in]				Year to Display  
 *
 * @return None
 */
void NixieDisplay_Date(uint16_t Day, uint16_t Month, uint16_t Year){
	
	NixieDisplay(Day,Month,Year);	
}

/**
 * @brief Effects on Nixie Tubes Nb1
 *				Goes from 9 to 0 and back to 9 (Par of digits)
 *
 * @return None
 */
static void NixieEffects_1(void){
	
	int i,z;
	
	for (i = 0; i < 3; i++) {					/** Loop on Par of  Digits */
		for (z = 0; z <= 99; z+=11) { 	/** Increments Digits by 10 */
			switch (i){
				case 0:
					NixieDisplay(0,0,z);
				break;
				case 1:
					NixieDisplay(0,z,0);
				break;
				case 2:
					NixieDisplay(z,0,0);
				break;
			}
			NixieDots_toggle(BoothDots,BlinkDots);				
			Delay_1ms(5);
		}		
		for (z = 99; z >= 0; z-=11) {		/** Decrement Digits by 10 */
			switch (i){
				case 0:
					NixieDisplay(0,0,z);
				break;
				case 1:
					NixieDisplay(0,z,0);
				break;
				case 2:
					NixieDisplay(z,0,0);
				break;
			}
			NixieDots_toggle(BoothDots,BlinkDots);				
			Delay_1ms(5);
		}
	}
}

/**
 * @brief Effects on Nixie Tubes Nb2
 *				Goes from 9 to 0 and back to 9 (Digit by digit)
 *
 * @return None
 */
static void NixieEffects_2(void){
	
	int i,z;
	
	NixieDisplay(99,99,99);
	
	for (i = 0; i < 6; i++) {			/** Loop on Digits */
		for (z = 9; z >= 0; z--) {	/** Decrement Digit By one*/
			switch (i){
				case 0:
					NixieDisplayDigitbyDigit(9,9,9,9,9,z);
				break;
				case 1:
					NixieDisplayDigitbyDigit(9,9,9,9,z,0);
				break;
				case 2:
					NixieDisplayDigitbyDigit(9,9,9,z,0,0);
				break;
				case 3:
					NixieDisplayDigitbyDigit(9,9,z,0,0,0);
				break;
				case 4:
					NixieDisplayDigitbyDigit(9,z,0,0,0,0);
				break;
				case 5:
					NixieDisplayDigitbyDigit(z,0,0,0,0,0);
				break;
			}
			NixieDots_toggle(BoothDots,BlinkDots);			
			Delay_1ms(5);
		}
	}
	Delay_1ms(5);
	for (i = 5; i >= 0; i--) {		/** Loop on Digits */
		for (z = 1; z <= 9; z++) {	/** Increment Digit By one*/
			switch (i){
				case 5:
					NixieDisplayDigitbyDigit(z,0,0,0,0,0);
				break;
				case 4:
					NixieDisplayDigitbyDigit(9,z,0,0,0,0);
				break;
				case 3:
					NixieDisplayDigitbyDigit(9,9,z,0,0,0);
				break;
				case 2:
					NixieDisplayDigitbyDigit(9,9,9,z,0,0);
				break;
				case 1:
					NixieDisplayDigitbyDigit(9,9,9,9,z,0);
				break;
				case 0:
					NixieDisplayDigitbyDigit(9,9,9,9,9,z);
				break;
			}
			NixieDots_toggle(BoothDots,BlinkDots);			
			Delay_1ms(5);
		}
	}		
}

/**
 * @brief Effects on Nixie Tubes Nb3
 *				Show random number on each digit until mathc current time
 *
 * @return None
 */
static void NixieEffects_3(void){
	
	int i;
	int rand = 99 ;
	unsigned int Hour1Digit			= psTime.tm_hour/10;
	unsigned int Hour2Digit			= psTime.tm_hour%10;
	unsigned int Minutes1Digit	= psTime.tm_min/10;
	unsigned int Minutes2Digit	= psTime.tm_min%10;
	unsigned int Seconds1Digit	= psTime.tm_sec/10;
	unsigned int Seconds2Digit	= psTime.tm_sec%10;
	NixieDisplay(0,0,0);
	
	for (i = 0; i < 6; i++) {					/** Loop on all Digits */
		switch (i){
			case 0:
				do{
					rand = RandomX(10);	
					NixieDisplayDigitbyDigit(rand,9,9,9,9,9);
					NixieDots_toggle(BoothDots,BlinkDots);				
					Delay_1ms(10);						
				}while (rand != Hour1Digit);
			break;
			case 1:
				rand = 9 ;
				do{
					rand = RandomX(10);	
					NixieDisplayDigitbyDigit(Hour1Digit,rand,9,9,9,9);
					NixieDots_toggle(BoothDots,BlinkDots);				
					Delay_1ms(10);						
				}while (rand != Hour2Digit);
			break;
			case 2:
				rand = 9 ;
				do{
					rand = RandomX(10);	
					NixieDisplayDigitbyDigit(Hour1Digit,Hour2Digit,rand,9,9,9);
					NixieDots_toggle(BoothDots,BlinkDots);				
					Delay_1ms(10);						
				}while (rand != Minutes1Digit);
			break;
			case 3:
				rand = 9 ;
				do{
					rand = RandomX(10);	
					NixieDisplayDigitbyDigit(Hour1Digit,Hour2Digit,Minutes1Digit,rand,9,9);
					NixieDots_toggle(BoothDots,BlinkDots);				
					Delay_1ms(10);						
				}while (rand != Minutes2Digit);
			break;
			case 4:
				rand = 9 ;
				do{
					rand = RandomX(10);	
					NixieDisplayDigitbyDigit(Hour1Digit,Hour2Digit,Minutes1Digit,Minutes2Digit,rand,9);
					NixieDots_toggle(BoothDots,BlinkDots);				
					Delay_1ms(10);						
				}while (rand != Seconds1Digit);
			break;
			case 5:
				rand = 9 ;
				do{
					rand = RandomX(10);	
					NixieDisplayDigitbyDigit(Hour1Digit,Hour2Digit,Minutes1Digit,Minutes2Digit,Seconds1Digit,rand);
					NixieDots_toggle(BoothDots,BlinkDots);				
					Delay_1ms(10);						
				}while (rand != Seconds2Digit);
			break;
		}
	}		
}
/**
 * @brief Display Effects on Nixie Tubes 
 *
 * @param[in]				Effect Number to Display 
 *
 * @return None
 */
void NixieDisplay_Effects(uint8_t effects){
	
	switch (effects){
		case 0:
			NixieEffects_1();
		break;
		case 1:
			NixieEffects_2();
		break;
		case 2:
			NixieEffects_3();
		break;	
	}	
}

