/*-------------------------------------------------------------------------------------
 |         [FILE NAME]     <lcd.c>                                                     |
 | 		   [AUTHOR]        <Abdolee>                                                   |
 |         [DATE CREATED]  <Jan 15, 2020>											   |
 |         [DESCRIPTION]   <the C driver file for the LCD>						       |
 |-------------------------------------------------------------------------------------*/

/*************************************************
 *                   INCLUDES                    *
 *************************************************/
#include"lcd.h"

/*************************************************
 *              FUNCTION DEFINITIONS             *
 *************************************************/
/*-------------------------------------------------------------------------------------
 |     [FUNCTION NAME]     <LCD_iNIT>         									   |
 | 	   [DESCRIPTION]       < this function is used to InitIalize the LCD from       |
 |	 	 	 	 	 	 	  the static CONFIG in the H file>                         |
 |     [ARGS]  			   <VOID>
 |	   [RETURNS]           <VOID>										           |
 |-------------------------------------------------------------------------------------*/

void LCD_Init(void)
{
	/*INIT GPIO selected in the header file*/
	SET_BIT(LCD_RW_DDR,LCD_RW_PIN_NUMB);
	SET_BIT(LCD_RS_DDR,LCD_RS_PIN_NUMB);
	SET_BIT(LCD_E_DDR,LCD_E_PIN_NUMB);

	CLEAR_BIT(LCD_RW_PORT,LCD_RW_PIN_NUMB);
	CLEAR_BIT(LCD_RS_PORT,LCD_RS_PIN_NUMB);
	CLEAR_BIT(LCD_E_PORT,LCD_E_PIN_NUMB);

#if INTERFACE_4BIT
	#if UPPER_PINS
	LCD_DATA_DDR|=0X0F;
	LCD_DATA_PORT&=0xF0;
	#elif LOWER_PINS
	LCD_DATA_DDR|=0XF0;
	LCD_DATA_PORT&=0x0F;
	#endif
	/*sending the LCD the command of 4bit mode and 2 lines*/
	LCD_SendCommand(SET_4BIT);
	LCD_SendCommand(FUNCTION_SET_4BIT_2LINE);

#elif INTERFACE_8BIT
	LCD_DATA_DDR=0XFF;
	LCD_DATA_PORT=0;
	/*sending the LCD the command of 8bit mode and 2 lines*/
	LCD_SendCommand(FUNCTION_SET_8BIT_2LINE);
#endif
	/*enable display and CURSER off command, and clear the display*/
	LCD_SendCommand(DESPLAY_ON_CURSER_OFF);
	LCD_SendCommand(CLEAR_DISPLAY);

}


/*-------------------------------------------------------------------------------------
 |     [FUNCTION NAME]     <LCD_SendCommand>         									   |
 | 	   [DESCRIPTION]       < this function is used to send a command to the LCD>                         |
 |     [ARGS]  			   <the command wanted to send>
 |	   [RETURNS]           <VOID>										           |
 |-------------------------------------------------------------------------------------*/


void LCD_SendCommand(uint8 a_command)
{
#if INTERFACE_8BIT
	CLEAR_BIT(LCD_RS_PORT,LCD_RS_PIN_NUMB);
	_delay_ms(1);
	SET_BIT(LCD_E_PORT,LCD_E_PIN_NUMB);
	_delay_ms(1);
	LCD_DATA_PORT=a_command;
	_delay_ms(1);
	CLEAR_BIT(LCD_E_PORT,LCD_E_PIN_NUMB);
	_delay_ms(1);
	CLEAR_BIT(LCD_RS_PORT,LCD_RS_PIN_NUMB);
	LCD_DATA_PORT=0;

#elif INTERFACE_4BIT
	CLEAR_BIT(LCD_RS_PORT,LCD_RS_PIN_NUMB);
	_delay_ms(1);
	SET_BIT(LCD_E_PORT,LCD_E_PIN_NUMB);
	_delay_ms(1);

	#if LOWER_PINS
	LCD_DATA_PORT= (LCD_DATA_PORT&0X0F)|(a_command & 0XF0);
	#elif UPPER_PINS
	LCD_DATA_PORT= (LCD_DATA_PORT&0XF0)|((a_command & 0XF0)>>4);
	#endif

	_delay_ms(1);
	CLEAR_BIT(LCD_E_PORT,LCD_E_PIN_NUMB);
	_delay_ms(1);
	SET_BIT(LCD_E_PORT,LCD_E_PIN_NUMB);
	_delay_ms(1);

	#if LOWER_PINS
	LCD_DATA_PORT=(LCD_DATA_PORT&0X0F)|((a_command & 0X0F)<<4);
	#elif UPPER_PINS
	LCD_DATA_PORT= (LCD_DATA_PORT&0XF0)|(a_command & 0X0F);
	#endif

	_delay_ms(1);
	CLEAR_BIT(LCD_E_PORT,LCD_E_PIN_NUMB);
	_delay_ms(1);
	CLEAR_BIT(LCD_RS_PORT,LCD_RS_PIN_NUMB);

	#if LOWER_PINS
	LCD_DATA_PORT&=0X0F;
	#elif UPPER_PINS
	LCD_DATA_PORT&=0XF0;
	#endif

#endif

}

/*-------------------------------------------------------------------------------------
 |     [FUNCTION NAME]     <LCD_SendChar>         									   |
 | 	   [DESCRIPTION]       < this function is used to send a char to the LCD>                         |
 |     [ARGS]  			   <the char wanted to send>
 |	   [RETURNS]           <VOID>										           |
 |-------------------------------------------------------------------------------------*/

void LCD_SendChar(uint8 a_char)
{
#if INTERFACE_8BIT
	SET_BIT(LCD_RS_PORT,LCD_RS_PIN_NUMB);
	_delay_ms(1);
	SET_BIT(LCD_E_PORT,LCD_E_PIN_NUMB);
	_delay_ms(1);
	LCD_DATA_PORT=a_char;
	_delay_ms(1);
	CLEAR_BIT(LCD_E_PORT,LCD_E_PIN_NUMB);
	_delay_ms(1);
	CLEAR_BIT(LCD_RS_PORT,LCD_RS_PIN_NUMB);
	LCD_DATA_PORT=0;

#elif INTERFACE_4BIT
	SET_BIT(LCD_RS_PORT,LCD_RS_PIN_NUMB);
	_delay_ms(1);
	SET_BIT(LCD_E_PORT,LCD_E_PIN_NUMB);
	_delay_ms(1);

	#if LOWER_PINS
	LCD_DATA_PORT= (LCD_DATA_PORT&0X0F)|(a_char & 0XF0);
	#elif UPPER_PINS
	LCD_DATA_PORT= (LCD_DATA_PORT&0XF0)|((a_char & 0XF0)>>4);
	#endif

	_delay_ms(1);
	CLEAR_BIT(LCD_E_PORT,LCD_E_PIN_NUMB);
	_delay_ms(1);
	SET_BIT(LCD_E_PORT,LCD_E_PIN_NUMB);
	_delay_ms(1);

	#if LOWER_PINS
	LCD_DATA_PORT=(LCD_DATA_PORT&0X0F)|((a_char & 0X0F)<<4);
	#elif UPPER_PINS
	LCD_DATA_PORT=(LCD_DATA_PORT&0XF0)|(a_char & 0X0F);
	#endif
	_delay_ms(1);
	CLEAR_BIT(LCD_E_PORT,LCD_E_PIN_NUMB);
	_delay_ms(1);
	CLEAR_BIT(LCD_RS_PORT,LCD_RS_PIN_NUMB);

	#if LOWER_PINS
	LCD_DATA_PORT&=0X0F;
	#elif UPPER_PINS
	LCD_DATA_PORT&=0XF0;
	#endif

#endif


}

/*-------------------------------------------------------------------------------------
 |     [FUNCTION NAME]     <LCD_SendString>         									   |
 | 	   [DESCRIPTION]       < this function is used to send a STRING to the LCD>                         |
 |     [ARGS]  			   <the STRING wanted to send>
 |	   [RETURNS]           <VOID>										           |
 |-------------------------------------------------------------------------------------*/

void LCD_SendString(char* a_string_Ptr)
{
	while(*(a_string_Ptr)!='\0')
	{
		LCD_SendChar(*(a_string_Ptr));
		a_string_Ptr++;
	}
}

/*-------------------------------------------------------------------------------------
 |     [FUNCTION NAME]     <LCD_Sendint>         									   |
 | 	   [DESCRIPTION]       < this function is used to send an INT to the lcd>                         |
 |     [ARGS]  			   <32 bit INT >
 |	   [RETURNS]           <VOID>										           |
 |-------------------------------------------------------------------------------------*/
void LCD_SendInt(uint32 a_int)
{
	uint16 i=0;
	/*by dividing the integer into small integers and saving it into an ARR*/
	uint16 temp[16]={0};

	do{
		temp[i]=(a_int%10);
		a_int/=10;
		i++;
	   }while(a_int);

	/*then displaying it element by element, the ARR is displayed backword, to occur the
	 * wanted representation*/
	while(i!=0)
	{
		LCD_SendChar(48+temp[(i-1)]);
		--i;
	}

}

/*-------------------------------------------------------------------------------------
 |     [FUNCTION NAME]     <LCD_SendFloat>         									   |
 | 	   [DESCRIPTION]       < this function is used to send a float to the LCD>                         |
 |     [ARGS]  			   <the float wanted to send>
 |	   [RETURNS]           <VOID>										           |
 |-------------------------------------------------------------------------------------*/

void LCD_SendFloat(float32 a_float)
{
	/*this method seperate the fraction and the INT and display
	 * every one of them seperatly*/
	uint32 frac=(((uint32)((a_float)*1000))%1000);
	LCD_SendInt(a_float);
	LCD_SendChar('.');
	LCD_SendInt(frac);
}


