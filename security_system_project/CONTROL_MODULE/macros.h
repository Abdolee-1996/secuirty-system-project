/*
 * common_macros.h
 *
 *  Created on: Jan 11, 2020
 *      Author: my laptop
 */

#ifndef MACROS_H_
#define MACROS_H_

#ifndef F_CPU
#define F_CPU           (1000000u)
#endif

#define SET_BIT(REG,PIN)     (REG|=(1<<PIN))
#define CLEAR_BIT(REG,PIN)   (REG&=~(1<<PIN))
#define TOOGLE_BIT(REG,PIN)  (REG^=(1<<PIN))
#define READ_BIT(REG,PIN)    ((REG&(1<<PIN))>>PIN)

#define SET_GLOBAL_INTERRUPT()   (SET_BIT(SREG,7))
#define CLEAR_GLOBAL_INTERRUPT()   (CLEAR_BIT(SREG,7))


#endif /* MACROS_H_ */
