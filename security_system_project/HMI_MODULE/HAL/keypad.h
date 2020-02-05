/*-------- -----------------------------------------------------------------------------
 |         [FILE NAME]     <keypad.h>     											   |
 | 		   [AUTHOR]        <Abdolee>                                                   |
 |         [DATE CREATED]  <Jan 15, 2020>											   |
 |         [DESCRIPTION]   <the H driver file for the KEYPAD>						   |
  -------------------------------------------------------------------------------------*/


#ifndef KEYPAD_H_
#define KEYPAD_H_


/*************************************************
 *                   INCLUDES                    *
 *************************************************/
#include<avr/io.h>
#include"../macros.h"
#include"../std_types.h"
#include<util/delay.h>
/*************************************************
 *      DEFINITIONS AND STATIC CONFIGRUATION     *
 *************************************************/
/* Keypad configurations for number of rows and columns */
#define KEYPAD4x4              ENABLE
#define KEYPAD4x3              DISABLE

#define KEYPAD_ADC_MODE        DISABLE
/* at this mode we connect the keypad with 1 ADC channel Pin only
 * but need to connect RES for every COL and ROW and use a
 * voltage divider method to generate different volt for every button */


#define ROW                      (4)
#if KEYPAD4x4
#define COL                 	 (4)
#elif KEYPAD4x3
#define COL        		         (3)
#endif

#if  KEYPAD_ADC_MODE

#define ADC_CHANNEL_NUMB                ADC0

#define ROW_RES_VAL                     (4.7e3)
#define COL_RES_VAL                     (1e3)
#define PULLDOWN_RES_VAL                (1e3)

#endif

/* Keypad configurations for the fixed value of  ROW and COL */
#define ROW1_COL1_VAL          ('7')
#define ROW2_COL1_VAL          ('4')
#define ROW3_COL1_VAL          ('1')
#define ROW4_COL1_VAL          ('K')
#define ROW1_COL2_VAL          ('8')
#define ROW2_COL2_VAL          ('5')
#define ROW3_COL2_VAL          ('2')
#define ROW4_COL2_VAL          ('0')
#define ROW1_COL3_VAL          ('9')
#define ROW2_COL3_VAL          ('6')
#define ROW3_COL3_VAL          ('3')
#define ROW4_COL3_VAL          ('=')

#if KEYPAD4x4
#define ROW1_COL4_VAL          ('/')
#define ROW2_COL4_VAL          ('*')
#define ROW3_COL4_VAL          ('-')
#define ROW4_COL4_VAL          ('+')
#endif

/* Keypad configurations for the fixed value of DDR, PORT, and PIN NUMBER */
#if (!(KEYPAD_ADC_MODE))
#define ROW1_DDR               (DDRB)
#define ROW1_PORT              (PORTB)
#define ROW1_PIN               (PINB)
#define ROW1_PIN_NUMB           (PB0)

#define ROW2_DDR               (DDRB)
#define ROW2_PORT              (PORTB)
#define ROW2_PIN               (PINB)
#define ROW2_PIN_NUMB           (PB1)

#define ROW3_DDR               (DDRB)
#define ROW3_PORT              (PORTB)
#define ROW3_PIN               (PINB)
#define ROW3_PIN_NUMB           (PB2)

#define ROW4_DDR               (DDRB)
#define ROW4_PORT              (PORTB)
#define ROW4_PIN               (PINB)
#define ROW4_PIN_NUMB           (PB3)

#define COL1_DDR               (DDRB)
#define COL1_PORT              (PORTB)
#define COL1_PIN_NUMB           (PB4)

#define COL2_DDR               (DDRB)
#define COL2_PORT              (PORTB)
#define COL2_PIN_NUMB           (PB5)

#define COL3_DDR               (DDRB)
#define COL3_PORT              (PORTB)
#define COL3_PIN_NUMB           (PB6)

#if KEYPAD4x4
#define COL4_DDR               (DDRB)
#define COL4_PORT              (PORTB)
#define COL4_PIN_NUMB           (PB7)
#endif


/* Keypad configurations for INTERNAL PULLUP */
#define INTERNAL_PULLUP        ENABLE
#endif

/*************************************************
 *          FUNCTION DECLARATION/PROTOTYPE       *
 *************************************************/

/*-------------------------------------------------------------------------------------
 |     [FUNCTION NAME]     <KEYPAD_iNIT>         									   |
 | 	   [DESCRIPTION]       < this function is used to InitIalize the KEYPAD from       |
 |	 	 	 	 	 	 	  the static CONFIG in the H file>                         |
 |     [ARGS]  			   <VOID>
 |	   [RETURNS]           <VOID>										           |
 |-------------------------------------------------------------------------------------*/
void KEYPAD_Init(void);

/*------------------------------------------------------------------------------------------
 |     [FUNCTION NAME]     <KEYPAD_ReadVal>         									   |
 | 	   [DESCRIPTION]       < this function is used to get the value presses in the keypad  |
 |                            by using the polling technique, that wait till it pressed>   |
 |     [ARGS]  			   <VOID> 														   |
 |	   [RETURNS]		   <it return the value that pressed, is represented as a char     |
 |                           that is as well a static value declared in the H file> 	   |							               |
 |-----------------------------------------------------------------------------------------*/
uint8 KEYPAD_ReadVal(void);

/*------------------------------------------------------------------------------------------
 |     [FUNCTION NAME]     <KEYPAD_ReadVal>         									   |
 | 	   [DESCRIPTION]       < this function is used to get the value pressed in the keypad  |
 |                            but in an INT form, by using the switch case and getting     |
 | 	 	 	 	 	 	 	  and calling the KEYPAD_ReadVal() that declared in the same   |
 |	 	 	 	 	 	 	  file >												       |
 |     [ARGS]  			   <VOID> 														   |
 |	   [RETURNS]		   <it return the value that pressed, is represented as an INT>    |
 |-----------------------------------------------------------------------------------------*/
uint8 KEYPAD_ReadInt(void);

#endif /* KEYPAD_H_ */
