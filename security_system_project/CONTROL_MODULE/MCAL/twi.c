/************************************************************************************
 [FILE NAME] :  	  <twi.c>
 [MODULE] :           <TWI>
 [AUTHOR] :     	  <Abdulrahman Mohamed , abdulrahman.mohd1996@gmail.com>
 [DATE CREATED] :     <15/12/2019>
 [DISCRIPATION] :     <Source file for the AVR TWI driver>
 **************************************************************************************/

/**************************************************************
 *                          INCLUDES                          *
 **************************************************************/
#include "twi.h"

/**************************************************************
 *                       Global Variables                     *
 **************************************************************/
/* Global variables to hold the address of the call back function in the application */

#if TWI_INTERRUPT
volatile static void (*g_callBck_Ptr)(void)=NULL_PTR;



/**************************************************************
 *                           ISR                              *
 **************************************************************/
ISR(TWI_vect)
{

	if(g_callBck_Ptr!=NULL_PTR)
	{
		/* Call the Call Back function in the application after the edge is detected */
		(*g_callBck_Ptr)();
	}
	CLEAR_INT_FLAG();
}

#endif



/**************************************************************
 *                      Function Definitions                  *
 **************************************************************/

/******************************************************************************************
*[Function Name] :   <TWI_Init>													  	  	  *
*																					      *
*[Description] :     1.select the RATE										      		  *
* 	 	 	 	 	 2.INIT the slave address											  *
* 	 	 	 	 	 3. enable the interrupt if needed									  *
* 	 	 	 	 	 4. enable general call Recognition if needed  					      *
*																				          *
*[Args] :            void	  															  *
*																						  *
*[Returns]:          no returns															  *
 ******************************************************************************************/
void TWI_Init(void)
{
/* but inside the register a value depends on the rate*/
	#if NORMAL_MODE_RATE_100KB
	TWBR=32;
	#elif FAST_MODE_RATE_400KB
	TWBR=2;
	#endif
	/* setting the address you write in the static CONF in the H file */
	TWAR=(MY_DEVICE_ADDRESS<<1);

	TWCR=(1<<TWEN)|(1<<TWEA);

	#if TWI_INTERRUPT
	TWCR|=(1<<TWIE);
	#endif


#if GENERAL_CALL_RECOGNITION
	TWAR|=1;
#endif

}

/******************************************************************************************
*[Function Name] :   <TWI_SendByte>													  	  *
*																					      *
*[Description] :     it send a byte and wait till the flag is set						  *
*																				          *
*[Args] :            the byte that needed to be send	  								  *
*																						  *
*[Returns]:          no returns															  *
 ******************************************************************************************/
void TWI_SendByte(uint8 a_data)
{
	TWDR=a_data;
	TWCR=(1<<TWINT)| (1<<TWEN)|(1<<TWEA);
	WAIT_TILL_INT_FLAG_SET();
}

/******************************************************************************************
*[Function Name] :   <TWI_ReadByteWithAck>												  *
*																					      *
*[Description] :     read byte and send ACK bit to the sender						      *
*																				          *
*[Args] :            void	  								  							  *
*																						  *
*[Returns]:          it returns the byte received										  *
 ******************************************************************************************/
uint8 TWI_ReadByteWithAck(void)
{
	(READ_BIT(TWCR,TWEA))?(TWCR|=(1<<TWINT)):(TWCR=(1<<TWEN)|(1<<TWINT)|(1<<TWEA));

	WAIT_TILL_INT_FLAG_SET();
	return TWDR;
}

/******************************************************************************************
*[Function Name] :   <TWI_ReadByteWithNack>												  *
*																					      *
*[Description] :     read byte and send NACK bit to the sender,as an indication of		  *
*					 stop the sending						      						  *
*																				          *
*[Args] :            void	  								  							  *
*																						  *
*[Returns]:          it returns the byte received										  *
 ******************************************************************************************/
uint8 TWI_ReadByteWithNack(void)
{
	TWCR=(1<<TWEN)|(1<<TWINT);
	WAIT_TILL_INT_FLAG_SET();
	return TWDR;
}


#if TWI_INTERRUPT
/******************************************************************************************
*[Function Name] :  <TWI_SetCallBackFunc>						              *
*																					      *
*[Description] :    set the call back function for the TWI interrupt 					  *
*																				          *
*[Args] :           it takes a PTR to Function that will be executed when the interrupt   *
*					occur		 											              *
*																						  *
*[Returns]:          no returns															  *
 ******************************************************************************************/
void TWI_SetCallBackFunc(void(*a_Func_Ptr)(void))
{
	g_callBck_Ptr=a_Func_Ptr;
}
#endif
