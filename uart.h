
/*************************************************************************************
 [FILE NAME]  		<uart.h>
 [MODULE]           UART
 [AUTHOR]     		<Abdulrahman Mohamed , abdulrahman.mohd1996@gmail.com>
 [DATE CREATED]     <21/1/2020>
 [DISCRIPATION]     <Header file for the AVR UART driver>
 **************************************************************************************/

#ifndef UART_H_
#define UART_H_

/**************************************************************
 *                          INCLUDES                          *
 **************************************************************/
#include <avr/io.h>
#include "../macros.h"
#include "../std_types.h"
#include <avr/interrupt.h>

/**************************************************************
 *                    Static Configurations                   *
 **************************************************************/
/* this #defines can delete code segments before compilation if not needed in the application */

#define UART_TX_INTERRUPT  	    	    	   		DISABLE
#define UART_RX_INTERRUPT  	    	    	        DISABLE
#define UART_DATA_REG_EMPTY                         DISABLE

/*as this CHAR is used to define the the string that i send is ended at this char*/
#define STRING_END_CHAR                             '$'

/*************************************************************
 *                           ENUMS                            *
 **************************************************************/

typedef enum
{
	RATE_2400_BPS=2400,
	RATE_4800_BPS=4800,
	RATE_9600_BPS=9600,
	RATE_19200_BPS=19200,
	RATE_38400_BPS=38400,
	RATE_57600_BPS=57600,
	RATE_115200_BPS=115200,
}standerdBaudRate;


typedef enum
{
	DISABLE_PARITY=0,
	EVEN_PARITY=2,
	ODD_PARITY=3,
}parityModes;



typedef enum
{
	FIVE_BITS=0,
	SIX_BITS=1,
	SEVEN_BITS=2,
	EIGHT_BITS=3,
}charSize;

/**************************************************************
 *                    Structures and Unions                   *
 **************************************************************/

/**************************************************************************************
 [Structure Name] : uartSettings
 [Structure Description] : this STRUCT is describe the UART functionality in the application
 	 	 	 	 	 	   , it will be passed by address to the UART_Init Function
 ***************************************************************************************/

typedef struct
{
	parityModes parityMode;
	charSize frameSize;
	standerdBaudRate boadRate;
}uartSettings;


/**************************************************************
 *                    FUNCTION-LIKE MACROS                    *
 **************************************************************/

/*DISABLE AND ENABLE INTERRUPTS MACROS*/
#define ENABLE_RX_INTERRUPT()                			  UCSRB|=(1<<RXCIE)
#define DISABLE_RX_INTERRUPT()                			  UCSRB&=~(1<<RXCIE)

#define ENABLE_TX_INTERRUPT()          			          UCSRB|=(1<<TXCIE)
#define DISABLE_TX_INTERRUPT()          			      UCSRB&=~(1<<TXCIE)

#define ENABLE_DATA_REG_EMPTY_INTERRUPT()                 UCSRB|=(1<<UDRIE)
#define DISABLE_DATA_REG_EMPTY_INTERRUPT()                UCSRB&=~(1<<UDRIE)


/**************************************************************
 *                   FUNCTION DECLARATIONS                     *
 **************************************************************/
/******************************************************************************************
*[Function Name] :   <UART_Init>													  	  *
*																					      *
*[Description] :     1.select the Parity bit
*					 2.select the frame size
*					 3. setup the baud rate
*					 4.enable UART module									      *
*																				          *
*[Args] :            it takes a PTR to UARTSeettings Structure that has the INIT Value	  *
*																						  *
*[Returns]:          no returns															  *
 ******************************************************************************************/
void UART_Init(uartSettings* a_setings_Ptr);

/******************************************************************************************
*[Function Name] :   <UART_TransmitChar>													  	  *
*																					      *
*[Description] :     it sends using UART module an 8BIT										      *
*																				          *
*[Args] :            it takes a CHAR that wanted to be sent								  *
*																						  *
*[Returns]:          no returns															  *
 ******************************************************************************************/
void UART_TransmitChar(uint8 a_data);

/******************************************************************************************
*[Function Name] :   <UART_ReceiveChar>													  	  *
*																					      *
*[Description] :     it receive using UART module an 8BIT										      *
*																				          *
*[Args] :            void								  *
*																						  *
*[Returns]:          return the received char															  *
 ******************************************************************************************/
uint8 UART_ReceiveChar(void);

/******************************************************************************************
*[Function Name] :   <UART_TransmitStrig>													  	  *
*																					      *
*[Description] :     it sends using UART module a STRING										      *
*																				          *
*[Args] :            it takes the String that wanted to be sent								  *
*																						  *
*[Returns]:          no returns															  *
 ******************************************************************************************/
void UART_TransmitStrig(const uint8* a_String_Ptr);

/******************************************************************************************
*[Function Name] :   <UART_RecieveString>													  	  *
*																					      *
*[Description] :     it sends using UART module a STING										      *
*																				          *
*[Args] :            it takes an empty STRING to save the received one in it 								  *
*																						  *
*[Returns]:          return the same ARG string but after filling it															  *
 ******************************************************************************************/
uint8* UART_RecieveString(uint8* a_emptyString_Ptr);

#if (UART_RX_INTERRUPT)
/******************************************************************************************
*[Function Name] :  <UART_SetRxCallBackFunc>						              *
*																					      *
*[Description] :    set the call back function for the RX interrupt 					  *
*																				          *
*[Args] :           it takes a PTR to Function that will be executed when the interrupt   *
*					occur		 											              *
*																						  *
*[Returns]:          no returns															  *
 ******************************************************************************************/
void UART_SetRxCallBackFunc(void(*a_func_Ptr)(void));

#endif

#if (UART_TX_INTERRUPT)
/******************************************************************************************
*[Function Name] :  <UART_SetTxCallBackFunc>						              *
*																					      *
*[Description] :    set the call back function for the TX interrupt 					  *
*																				          *
*[Args] :           it takes a PTR to Function that will be executed when the interrupt   *
*					occur		 											              *
*																						  *
*[Returns]:          no returns															  *
 ******************************************************************************************/
void UART_SetTxCallBackFunc(void(*a_func_Ptr)(void));

#endif

#if (UART_DATA_REG_EMPTY)
/******************************************************************************************
*[Function Name] :  <UART_SetRegisterEmptyCallBackFunc>						              *
*																					      *
*[Description] :    set the call back function for the EMPTY REG interrupt 					  *
*																				          *
*[Args] :           it takes a PTR to Function that will be executed when the interrupt   *
*					occur		 											              *
*																						  *
*[Returns]:          no returns															  *
 ******************************************************************************************/

void UART_SetRegisterEmptyCallBackFunc(void(*a_func_Ptr)(void));

#endif



#endif /* UART_H_ */
