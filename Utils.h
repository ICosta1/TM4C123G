/* @owner ICosta
 *
 * Copyright of this program is the property of Ivo Costa, without
 * whose written permission reproduction in whole or in part is prohibited.
 */
/**
 * @file Utils.h
 *
 * @brief Miscellaneous Tools 
 *
 * @author I. Costa
 * @date 2015-21-11
 */

#ifndef __UTILS_H__
#define __UTILS_H__

/* -- Includes */

/* Standard headers */
         
/* Functional headers */

/* -- Constants */

/**
 * @brief Convert Decimal Number to BCD.
 *
 * @return BCD Number
 */
#define dec2bcd(dec) (((dec / 10) << 4) | (dec % 10))

 /**
 * @brief Convert BCD to Decimal.
 *
 * @return Decimal Number
 */
 #define bcd2dec(BCD) (((BCD & 0xF0) >> 4) * 10) + (BCD & 0x0F)


/* -- Types */


 /**
 * @brief Convert BCD to Decimal.
 *
 * @return Decimal Number
 */
 #define bcd2dec(BCD) (((BCD & 0xF0) >> 4) * 10) + (BCD & 0x0F)

#endif /* __UTILS_H__ */
