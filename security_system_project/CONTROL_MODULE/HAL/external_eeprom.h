/*************************************************************************************
 [FILE NAME]  		<external_eeprom.c>
 [AUTHOR]     		<Abdulrahman Mohamed , abdulrahman.mohd1996@gmail.com>
 [DATE CREATED]     <28/1/2019>
 [DISCRIPATION]     <Header file for the AVR EXT EEPROM driver>
 **************************************************************************************/
#ifndef EXTERNAL_EEPROM_H_
#define EXTERNAL_EEPROM_H_
/**************************************************************
 *                          INCLUDES                          *
 **************************************************************/
#include "../MCAL/twi.h"

/**************************************************************
 *                          Definitions                       *
 **************************************************************/
#define SUCCESS 				(1u)
#define FAIL                    (0u)

/**************************************************************
 *                    Static Configurations                   *
 **************************************************************/
#define EXTERNAL_EEPROM_SLAVE_ADDRESS               (0XA0)




/**************************************************************
 *                   FUNCTION DECLARATIONS                     *
 **************************************************************/
/******************************************************************************************
*[Function Name] :   <EXTERNAL_EEPROM_Init>													  	  *
*																					      *
*[Description] :     initialize the I2C module only as it use it communicate 	          *
*																				          *
*[Args] :            VOID	  															  *
*																						  *
*[Returns]:          no returns															  *
 ******************************************************************************************/
void EXTERNAL_EEPROM_Init(void);

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
uint8 EXTERNAL_EEPROM_WriteByte(uint16 a_address, uint8 a_data);

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
uint8 EXERNAL_EEPROM_ReadByte(uint16 a_address, uint8* a_byte_Ptr);


#endif /* EXTERNAL_EEPROM_H_ */
