/*************************************************************************************
 [FILE NAME]  		<twi.h>
 [MODULE]           TWI
 [AUTHOR]     		<Abdulrahman Mohamed , abdulrahman.mohd1996@gmail.com>
 [DATE CREATED]     <15/12/2019>
 [DISCRIPATION]     <Header file for the AVR TWI driver>
 **************************************************************************h***********/

#ifndef TWI_H_
#define TWI_H_

/**************************************************************
 *                          INCLUDES                          *
 **************************************************************/
#include "avr/io.h"
#include "../macros.h"
#include "../std_types.h"
#include <avr/interrupt.h>


/**************************************************************
 *                    Static Configurations                   *
 **************************************************************/
#define NORMAL_MODE_RATE_100KB             DISABLE
#define FAST_MODE_RATE_400KB               ENABLE


#define MY_DEVICE_ADDRESS                  (0x01U)   //7-BIT ADDRESS
#define GENERAL_CALL_RECOGNITION           DISABLE

#define TWI_INTERRUPT                      DISABLE




/*STATUES REGISTER VALUE MACROS*/
/* at master mode*/
#define START_BIT_HAS_BEEN_TRANSMITTED       	          			 (0X08)
#define REAPETED_START_BIT_HAS_BEEN_TRANSMITTED                 	 (0X10)
#define SLA_W_HAS_BEEN_TRANSMITTED_AND_ACK_RECEIVED        			 (0X18)
#define SLA_W_HAS_BEEN_TRANSMITTED_AND_NACK_RECEIVED             	 (0X20)
#define DATA_BYTE_HAS_BEEN_TRANSMITTED_AND_ACK_RECIEVED              (0X28)
#define DATA_BYTE_HAS_BEEN_TRANSMITTED_AND_NACK_RECIEVED             (0X30)
#define SLA_R_HAS_BEEN_TRANSMITTED_AND_ACK_RECEIVED     			 (0X40)
#define SLA_R_HAS_BEEN_TRANSMITTED_AND_NACK_RECEIVED     			 (0X48)
#define DATA_BYTE_HAS_BEEN_RECIEVED_AND_ACK_TRANSMITTED              (0X50)
#define DATA_BYTE_HAS_BEEN_RECIEVED_AND_NACK_TRANSMITTED             (0X58)

/*at slave mode*/
#define OWN_SLA_W_RECEIVED_ACK_TRANSMITTED                   		 (0X60)
#define SLA_W_DATA_RECIEVED_AND_ACK_TRANSMITTED                      (0X80)
#define SLA_W_DATA_RECIEVED_AND_NACK_TRANSMITTED                     (0X88)
#define STOP_BIT_RECIVED											 (0XA0)
#define OWN_SLA_R_RECEIVED_ACK_TRANSMITTED                   	     (0XA8)
#define SLA_R_DATA_TRANSMITTED_AND_ACK_RECEIVED                      (0XB8)
#define SLA_R_DATA_TRANSMITTED_AND_NACK_RECEIVED                     (0XC0)
#define LAST_DATA_BYTE_HAS_BEEN_TRANMITTED_TWEA_ZERO                 (0XC8)

/*GENERAL STATES*/
#define NO_STATE_AVAILABLE_TWINT_ZERO                      			 (0XF8)
#define BUS_ERROR                             						 (0X00)



/**************************************************************
 *                    FUNCTION-LIKE MACROS                    *
 **************************************************************/
/* set the bit to send the start bit*/
#define SEND_START_BIT()                         TWCR=(1<<TWINT)|(1<<TWSTA)|(1<<TWEN)
/* set the bit to send the stop bit*/
#define SEND_STOP_BIT()                          TWCR=(1<<TWINT)|(1<<TWEN)|(1<<TWSTO)
/*this macro wait till the interrupt flag is set*/
#define WAIT_TILL_INT_FLAG_SET()                 while(!( READ_BIT(TWCR,TWINT) ) )
/*this read the statues REG*/
#define READ_TWI_STATUS()                        (TWSR)
/*this macro enable the ACK bit that be used at the slave mode, after losing the bus */
#define ENABLE_ACK_BIT()                         TWCR|=(1<<TWEA)
/*this macro enable the ACK bit that be used at the master mode, after controlling the bus */
#define DISABLE_ACK_BIT()                        TWCR&=~(1<<TWEA)
/*this enable the interrupt to handle the I2C operation using INTERRUPT*/
#define ENABLE_INTERRUPT()                       SET_BIT(TWCR,TWIE)
/* disable the interrupt*/
#define DISABLE_INTERRUPT()                      CLEAR_BIT(TWCR,TWIE)
/*macro that clear the interrupt flag*/
#define CLEAR_INT_FLAG()						 TWCR|=(1<<TWINT)


/**************************************************************
 *                   FUNCTION DECLARATIONS                     *
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
void TWI_Init(void);

/******************************************************************************************
*[Function Name] :   <TWI_SendByte>													  	  *
*																					      *
*[Description] :     it send a byte and wait till the flag is set						  *
*																				          *
*[Args] :            the byte that needed to be send	  								  *
*																						  *
*[Returns]:          no returns															  *
 ******************************************************************************************/
void TWI_SendByte(uint8 a_data);

/******************************************************************************************
*[Function Name] :   <TWI_ReadByteWithAck>												  *
*																					      *
*[Description] :     read byte and send ACK bit to the sender						      *
*																				          *
*[Args] :            void	  								  							  *
*																						  *
*[Returns]:          it returns the byte received										  *
 ******************************************************************************************/
uint8 TWI_ReadByteWithAck(void);

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
uint8 TWI_ReadByteWithNack(void);

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
void TWI_SetCallBackFunc(void(*a_Func_Ptr)(void));



#endif /* TWI_H_ */
