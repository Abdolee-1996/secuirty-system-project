/*-------- -----------------------------------------------------------------------------
 |         [FILE NAME]     <lcd.h>     												   |
 | 		   [AUTHOR]        <Abdolee>                                                   |
 |         [DATE CREATED]  <Jan 15, 2020>											   |
 |         [DESCRIPTION]   <the H driver file for the LCD>						       |
  -------------------------------------------------------------------------------------*/

#ifndef LCD_H_
#define LCD_H_

/*************************************************
 *                   INCLUDES                    *
 *************************************************/

#include"../std_types.h"
#include"../macros.h"
#include<avr/io.h>
#include<util/delay.h>

/*************************************************
 *      DEFINITIONS AND STATIC CONFIGRUATION     *
 *************************************************/
/* 4-bit mode or 8-bit mode*/
#define INTERFACE_8BIT    								ENABLE
#define INTERFACE_4BIT    								DISABLE

/* switch between sending command(0) or data(1) bit*/
#define LCD_RS_DDR   									(DDRC)
#define LCD_RS_PORT   									(PORTC)
#define LCD_RS_PIN_NUMB   								(0)

#define LCD_RW_DDR   									(DDRC)
#define LCD_RW_PORT   									(PORTC)
#define LCD_RW_PIN_NUMB   								 (1)

/*Enable bit*/
#define LCD_E_DDR   									(DDRC)
#define LCD_E_PORT   									(PORTC)
#define LCD_E_PIN_NUMB   								 (2)


#define LCD_DATA_DDR   									(DDRA)
#define LCD_DATA_PORT   								(PORTA)

#if INTERFACE_4BIT
#define UPPER_PINS                                      DISABLE  //0-3
#define LOWER_PINS                                      ENABLE  //4-7
#endif


/* command list for LCD */
#define FUNCTION_SET_8BIT_2LINE                         (0X38)
#define SET_4BIT                                        (0X02)   //NEED TO CONNECT THE D4-D7 AT THE LCD
#define FUNCTION_SET_4BIT_2LINE                         (0X28)
#define CLEAR_DISPLAY         			 				(0X01)
#define RETURN_HOME           			 				(0X02)
#define DISABLE_AUTO_SHIFTING            				(0X04)
#define ENABLE_AUTO_SHIFTING            				(0X06)
#define ENABLE_AUTO_SHIFTING_LEFT            			(0X04)
#define DESPLAY_ON_CURSER_ON  			 				(0X0E)
#define DESPLAY_ON_CURSER_OFF            				(0X0C)
#define DESPLAY_ON_CURSER_ON_CURSER_BLINKING            (0X0F)
#define SHIFT_DESPLAY_RIGHT_SHIFT						(0X1C)
#define SHIFT_DESPLAY_LEFT_SHIFT						(0X18)
#define SHIFT_CURSOR_RIGHT_SHIFT						(0X14)
#define SHIFT_CURSOR_LEFT_SHIFT						    (0X10)
#define FORCE_TO_NEXT_LINE                              (0XC0)
#define SET_DDRAM_ADDRESS_TO(ADDRESS)                   ((1<<7)|ADDRESS)
#define FORCE_ROW_COL(ROW,COL)                          ( ( 1<<7 ) |( ( ROW==1 ) ? ( 0X40 + COL ) : ( COL ) ) )

/*************************************************
 *          FUNCTION DECLARATION/PROTOTYPE       *
 *************************************************/
/*-------------------------------------------------------------------------------------
 |     [FUNCTION NAME]     <LCD_iNIT>         									   |
 | 	   [DESCRIPTION]       < this function is used to InitIalize the LCD from       |
 |	 	 	 	 	 	 	  the static CONFIG in the H file>                         |
 |     [ARGS]  			   <VOID>
 |	   [RETURNS]           <VOID>										           |
 |-------------------------------------------------------------------------------------*/
void LCD_Init(void);

/*-------------------------------------------------------------------------------------
 |     [FUNCTION NAME]     <LCD_SendCommand>         									   |
 | 	   [DESCRIPTION]       < this function is used to send a command to the LCD>                         |
 |     [ARGS]  			   <the command wanted to send>
 |	   [RETURNS]           <VOID>										           |
 |-------------------------------------------------------------------------------------*/
void LCD_SendCommand(uint8 a_command);

/*-------------------------------------------------------------------------------------
 |     [FUNCTION NAME]     <LCD_SendChar>         									   |
 | 	   [DESCRIPTION]       < this function is used to send a char to the LCD>                         |
 |     [ARGS]  			   <the char wanted to send>
 |	   [RETURNS]           <VOID>										           |
 |-------------------------------------------------------------------------------------*/
void LCD_SendChar(uint8 a_char);

/*-------------------------------------------------------------------------------------
 |     [FUNCTION NAME]     <LCD_SendString>         									   |
 | 	   [DESCRIPTION]       < this function is used to send a STRING to the LCD>                         |
 |     [ARGS]  			   <the STRING wanted to send>
 |	   [RETURNS]           <VOID>										           |
 |-------------------------------------------------------------------------------------*/
void LCD_SendString(char* a_string_Ptr);

/*-------------------------------------------------------------------------------------
 |     [FUNCTION NAME]     <LCD_Sendint>         									   |
 | 	   [DESCRIPTION]       < this function is used to send an INT to the lcd>                         |
 |     [ARGS]  			   <32 bit INT >
 |	   [RETURNS]           <VOID>										           |
 |-------------------------------------------------------------------------------------*/
void LCD_SendInt(uint32 a_int);

/*-------------------------------------------------------------------------------------
 |     [FUNCTION NAME]     <LCD_SendFloat>         									   |
 | 	   [DESCRIPTION]       < this function is used to send a float to the LCD>                         |
 |     [ARGS]  			   <the float wanted to send>
 |	   [RETURNS]           <VOID>										           |
 |-------------------------------------------------------------------------------------*/
void LCD_SendFloat(float32 a_float);
#endif /* LCD_H_ */
