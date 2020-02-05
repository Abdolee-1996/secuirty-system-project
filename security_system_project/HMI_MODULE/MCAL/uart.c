
/*************************************************************************************
 [FILE NAME]  		<uart.C>
 [MODULE]           UART
 [AUTHOR]     		<Abdulrahman Mohamed , abdulrahman.mohd1996@gmail.com>
 [DATE CREATED]     <21/1/2020>
 [DISCRIPATION]     <Source file for the AVR UART driver>
 **************************************************************************************/

/**************************************************************
 *                          INCLUDES                          *
 **************************************************************/
#include "uart.h"



/**************************************************************
 *                       Global Variables                     *
 **************************************************************/
/* Global variables to hold the address of the call back function in the application */

#if UART_RX_INTERRUPT
volatile static void (*g_rxCallBack_Ptr)(void)=NULL_PTR;
#endif

#if UART_TX_INTERRUPT
volatile static void (*g_txCallBack_Ptr)(void)=NULL_PTR;
#endif

#if UART_DATA_REG_EMPTY
volatile static void (*g_registerEmptyCallBack_Ptr)(void)=NULL_PTR;
#endif


/**************************************************************
 *                           ISR                              *
 **************************************************************/

#if UART_RX_INTERRUPT

ISR(USART_RXC_vect)
{
	if(g_rxCallBack_Ptr!=NULL_PTR)
	{
		/* Call the Call Back function in the application after the edge is detected */
		(*g_rxCallBack_Ptr)();
	}

}

#endif


#if UART_DATA_REG_EMPTY

ISR(USART_UDRE_vect)
{
	if(g_txCallBack_Ptr!=NULL_PTR)
	{
		/* Call the Call Back function in the application after the edge is detected */
		g_rxCallBack_Ptr();
	}
}

#endif


#if UASRT_TX_INTERRUPT
ISR(USART_TXC_vect)
{
	if(g_registerEmptyCallBack_Ptr!=NULL_PTR)
	{
		/* Call the Call Back function in the application after the edge is detected */
		g_rxCallBack_Ptr();
	}
}

#endif


/**************************************************************
 *                      Function Definitions                  *
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
void UART_Init(uartSettings* a_setings_Ptr)
{

	UCSRA=(1<<U2X);
	UCSRB=(1<<TXEN)|(1<<RXEN);
	UCSRC=(1<<URSEL)|(a_setings_Ptr->parityMode<<UPM0)|(a_setings_Ptr->frameSize<<UCSZ0);

	/*****************************************************************
	 * BAUD=( ( Fosc ) / ( 8 * ( UBRR+1 ) ) ) (DOUBLE SPEED MODE)    *
	 * SO UBRR = (((Fosc)/(8*BAUD))-1)      						 *
	 *****************************************************************/
	UBRRL=( (uint16) ( ( ( F_CPU ) / ( 8.0 * (a_setings_Ptr->boadRate) ) ) - 1 ) & 0X00FF);
	UBRRH=( (uint16)( ( ( F_CPU ) / ( 8.0 * (a_setings_Ptr->boadRate ) ) ) - 1 ) & 0X0F00);

	/*ENABLING INTERRUPTS IF NEEDED*/
#if UART_TX_INTERRUPT
	UCSRB|=(1<<TXCIE);
#endif

#if UART_RX_INTERRUPT
	UCSRB|=(1<<RXCIE);
#endif

#if UART_DATA_REG_EMPTY
	UCSRB|=(1<<UDRIE);
#endif

}


/******************************************************************************************
*[Function Name] :   <UART_TransmitChar>													  	  *
*																					      *
*[Description] :     it sends using UART module an 8BIT										      *
*																				          *
*[Args] :            it takes a CHAR that wanted to be sent								  *
*																						  *
*[Returns]:          no returns															  *
 ******************************************************************************************/
void UART_TransmitChar(uint8 a_data)
{
	/*wait till register is empty*/
	while(!(READ_BIT(UCSRA,UDRE)));
	UDR=a_data;
}


/******************************************************************************************
*[Function Name] :   <UART_ReceiveChar>													  	  *
*																					      *
*[Description] :     it receive using UART module an 8BIT										      *
*																				          *
*[Args] :            void								  *
*																						  *
*[Returns]:          return the received char															  *
 ******************************************************************************************/
uint8 UART_ReceiveChar(void)
{
	/*wait till the rx interrupt flag set*/
	while(!(READ_BIT(UCSRA,RXC)));
	return UDR;
}


/******************************************************************************************
*[Function Name] :   <UART_TransmitStrig>													  	  *
*																					      *
*[Description] :     it sends using UART module a STRING										      *
*																				          *
*[Args] :            it takes the String that wanted to be sent								  *
*																						  *
*[Returns]:          no returns															  *
 ******************************************************************************************/
void UART_TransmitStrig(const uint8* a_String_Ptr)
{
	/*transmit CHAR till reach NULL*/
	while(*(a_String_Ptr)!='\0')
	{
		UART_TransmitChar(*a_String_Ptr);
		a_String_Ptr++;
	}
	/*send the char that indicate the ending of thee string*/
	UART_TransmitChar(STRING_END_CHAR);
}


/******************************************************************************************
*[Function Name] :   <UART_RecieveString>													  	  *
*																					      *
*[Description] :     it sends using UART module a STING										      *
*																				          *
*[Args] :            it takes an empty STRING to save the received one in it 								  *
*																						  *
*[Returns]:          return the same ARG string but after filling it															  *
 ******************************************************************************************/
uint8* UART_RecieveString(uint8* a_emptyString_Ptr)
{
	uint8 i=0;
/* receive till the end of the string by the END_CHAR */
	while(a_emptyString_Ptr[(i-1)]!=STRING_END_CHAR)
	{
		a_emptyString_Ptr[i]=UART_ReceiveChar();
		i++;
	}
	/*add null to be normal char*/
	a_emptyString_Ptr[i-1]='\0';
	/* return same PTR after adding to it*/
	return &(a_emptyString_Ptr[0]);
}


#if UART_RX_INTERRUPT
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
void UART_SetRxCallBackFunc(void(*a_func_Ptr)(void))
{
	g_rxCallBack_Ptr=a_func_Ptr;

}
#endif

#if UART_TX_INTERRUPT
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
void UART_SetTxCallBackFunc(void(*a_func_Ptr)(void))
{
	g_txCallBack_Ptr=a_func_Ptr;

}
#endif

#if UART_DATA_REG_EMPTY
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
void UART_SetRegisterEmptyCallBackFunc(void(*a_func_Ptr)(void))
{
	g_registerEmptyCallBack_Ptr=a_func_Ptr;

}
#endif

