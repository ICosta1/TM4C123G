/* @owner ICosta
 *
 * Copyright of this program is the property of Ivo Costa, without
 * whose written permission reproduction in whole or in part is prohibited.
 */
/**
 * @file Delay.c
 *
 * @brief Delay Services
 *
 * @author I. Costa
 * @date 2015-10-10
 */


/**
 * @brief Delay 12ns (1 clock Cycle at 80MHZ Sysclk)
 * 
 * @param[in]				NbDelay	Number of peiod to delay (12ns)      
 *
 * @return None
 */
void Delay_12ns(unsigned long NbDelay){
    while(NbDelay){
	  	NbDelay--;
    }
}

/**
 * @brief Delay 1ms
 * 
 * @param[in]				NbDelay	Number of peiod to delay (1ms)      
 *
 * @return None
 */
void Delay_1ms(unsigned long NbDelay){
	unsigned long volatile time;
  while(NbDelay>0){
    time = 80000;  /** 0.001sec at 80 MHz */
    while(time){
	  	time--;
    }
    NbDelay--;
  }
}


/**
 * @brief Delay 100ms
 * 
 * @param[in]				NbDelay	Number of peiod to delay (100ms)      
 *
 * @return None
 */
void Delay_100ms(unsigned long NbDelay){
	unsigned long volatile time;
  while(NbDelay>0){
    time = 727240;  /** 0.1sec at 80 MHz */
    while(time){
	  	time--;
    }
    NbDelay--;
  }
}
