/************************************************************************************
 [FILE NAME] :  	  <external_eeprom.c>
 [MODULE] :           <TIMER1>
 [AUTHOR] :     	  <Abdulrahman Mohamed , abdulrahman.mohd1996@gmail.com>
 [DATE CREATED] :     <15/12/2019>
 [DISCRIPATION] :     <Source file for the AVR EXT EEPROM driver>
 **************************************************************************************/

#include "external_eeprom.h"


/******************************************************************************************
*[Function Name] :   <EXTERNAL_EEPROM_Init>													  	  *
*																					      *
*[Description] :     initialize the I2C module only as it use it communicate 	          *
*																				          *
*[Args] :            VOID	  															  *
*																						  *
*[Returns]:          no returns															  *
 ******************************************************************************************/
void EXTERNAL_EEPROM_Init(void)
{
	TWI_Init();
}

/******************************************************************************************
*[Function Name] :   <EXTERNAL_EEPROM_WriteByte>									      *
*																					      *
*[Description] :     this function write a byte to EXT EEPROM, using I2C   				  *
*																				          *
*[Args] :            1.[a_address]:the address that user to write in the EXT EEPROM		  *
*					 2.[a_data] : the data the user want to save in the EXT EEPROM 		  *
*																						  *
*[Returns]:          it return SUCCESS if the action is done or ERROR if not			  *
 ******************************************************************************************/

uint8 EXTERNAL_EEPROM_WriteByte(uint16 a_address, uint8 a_data)
{
	/*as we communicate with the EXT EEPROM using I2C need to INIT it frame*/
	SEND_START_BIT();
	WAIT_TILL_INT_FLAG_SET();
	if(READ_TWI_STATUS()!=START_BIT_HAS_BEEN_TRANSMITTED)
	{
		/* if the statues REG doesn't  has this VAL it return FAIL*/
		return FAIL;
	}
	/*at this EEPROM its address is 4 Bits (0xA0) it give as the ability to send part of the place that need to write or read from in the first
	 * address calling , this EEPROM has 2^10 address so it need 10 bits for addressing, we send the 3 MSB BITS  in the first address calling
	 * FRAM SHAPE IS LIKE : (H.W_EEPROM_ADDRESS+ THE_THREE_MSB_OF THE WANTED_ADDRESS_INSIDE_EEPROM+ R/W) */
	TWI_SendByte(EXTERNAL_EEPROM_SLAVE_ADDRESS| ( ( a_address & 0X0700 ) >> 7 ) );
	if(READ_TWI_STATUS()!=SLA_W_HAS_BEEN_TRANSMITTED_AND_ACK_RECEIVED)
	{
		/* if the statues REG doesn't  has this VAL it return FAIL*/
		return FAIL;
	}
	/* them send the rest of 8bit address of the section inside the EEPROM*/
	TWI_SendByte((uint8)a_address);
	if(READ_TWI_STATUS()!=DATA_BYTE_HAS_BEEN_TRANSMITTED_AND_ACK_RECIEVED)
	{
		/* if the statues REG doesn't  has this VAL it return FAIL*/
		return FAIL;
	}
	/* send the data wanted to be write or saved inside the EEPROM*/
	TWI_SendByte(a_data);
	if(READ_TWI_STATUS()!=DATA_BYTE_HAS_BEEN_TRANSMITTED_AND_ACK_RECIEVED)
	{
		/* if the statues REG doesn't  has this VAL it return FAIL*/
		return FAIL;
	}

/* send stop bit to indicate the end of the FRAME*/
	SEND_STOP_BIT();

/*  return SUCCESS to indicate that action is done*/
	return SUCCESS;

}


/******************************************************************************************
*[Function Name] :   <EXERNAL_EEPROM_ReadByte>									      	  *
*																					      *
*[Description] :     this function read a byte from EXT EEPROM, using I2C   			  *
*																				          *
*[Args] :            1.[a_address]:the address that user want to from in the EXT EEPROM	  *
*					 2.[a_byte_Ptr] : a PTR that the user want to save the read VAL in    *
*																						  *
*[Returns]:          it return SUCCESS if the action is done or ERROR if not			  *
 ******************************************************************************************/

uint8 EXERNAL_EEPROM_ReadByte(uint16 a_address, uint8* a_byte_Ptr)
{
	/*as we communicate with the EXT EEPROM using I2C need to INIT it frame*/

	SEND_START_BIT();
	WAIT_TILL_INT_FLAG_SET();
	if(READ_TWI_STATUS()!=START_BIT_HAS_BEEN_TRANSMITTED)
	{
		/* if the statues REG doesn't  has this VAL it return FAIL*/
		return FAIL;
	}
	/*at this EEPROM its address is 4 Bits (0xA0) it give as the ability to send part of the place that need to write or read from in the first
	 * address calling , this EEPROM has 2^10 address so it need 10 bits for addressing, we send the 3 MSB BITS  in the first address calling
	 * FRAM SHAPE IS LIKE : (H.W_EEPROM_ADDRESS+ THE_THREE_MSB_OF THE WANTED_ADDRESS_INSIDE_EEPROM+ R/W) */
	TWI_SendByte((EXTERNAL_EEPROM_SLAVE_ADDRESS| ( ( a_address & 0X0700 ) >> 7 ) ) );
	if(READ_TWI_STATUS()!=SLA_W_HAS_BEEN_TRANSMITTED_AND_ACK_RECEIVED)
	{
		/* if the statues REG doesn't  has this VAL it return FAIL*/
		return FAIL;
	}
	/* them send the rest of 8bit address of the section inside the EEPROM*/
	TWI_SendByte((uint8)a_address);
	if(READ_TWI_STATUS()!=DATA_BYTE_HAS_BEEN_TRANSMITTED_AND_ACK_RECIEVED)
	{

		return FAIL;
	}

	/*at this point we need to read so sending start bit gain to and the address to make the EEPROM send to me*/
	SEND_START_BIT();
	WAIT_TILL_INT_FLAG_SET();
	if(READ_TWI_STATUS()!=REAPETED_START_BIT_HAS_BEEN_TRANSMITTED)
	{
		/* if the statues REG doesn't  has this VAL it return FAIL*/
		return FAIL;
	}
	TWI_SendByte((EXTERNAL_EEPROM_SLAVE_ADDRESS| ( ( a_address & 0X0700 ) >> 7 ) ) | 1 );
	if(READ_TWI_STATUS()!=SLA_R_HAS_BEEN_TRANSMITTED_AND_ACK_RECEIVED)
	{
		/* if the statues REG doesn't  has this VAL it return FAIL*/
		return FAIL;
	}
	/* this FUNC return the VAL of the register after receiving the data and send NACK to make the EEPROM stop sending */
	*a_byte_Ptr=TWI_ReadByteWithNack();
	if(READ_TWI_STATUS()!=DATA_BYTE_HAS_BEEN_RECIEVED_AND_NACK_TRANSMITTED)
	{
		/* if the statues REG doesn't  has this VAL it return FAIL*/
		return FAIL;
	}
	SEND_STOP_BIT();

	return SUCCESS;
}
