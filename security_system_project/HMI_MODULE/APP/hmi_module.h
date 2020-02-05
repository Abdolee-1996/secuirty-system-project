
/*************************************************************************************
 [FILE NAME]  		<hmi.h>
 [AUTHOR]     		<Abdulrahman Mohamed , abdulrahman.mohd1996@gmail.com>
 [DATE CREATED]     <15/12/2019>
 [DISCRIPATION]     <Header file for the HMI of security system project >
 **************************************************************************************/
#ifndef APP_HMI_MODULE_H_DULE_H_
#define APP_HMI_MODULE_H_DULE_H_
/**************************************************************
 *                          INCLUDES                          *
 **************************************************************/

#include "../HAL/lcd.h"
#include "../HAL/keypad.h"
#include "../MCAL/uart.h"


/**************************************************************
 *                  Private Function Declaration              *
 **************************************************************/

/*****************************************************************************************
*[Function Name] :   <HMI_EnterNewPassword>												 *
*																						 *
*[Description] :     this function calls the control the displaying when the HMI enter   *
*						the ENTER NEW PASSWORD mode as it's in main option			     *						     *
*																						 *
*[Args] :            none																 *
*																						 *
*[Returns]:          no returns															 *
 *****************************************************************************************/
void HMI_EnterNewPassword(void);

/*****************************************************************************************
*[Function Name] :   <HMI_SendPassword>												 	 *
*																						 *
*[Description] :     as the HMI should not suppose to handle any action to the password  *
*					so it send the password entered from the user using UART, and wait	 *
*					the HEAD controller for a response, so this function send the entered*
*					numbers from the user using UART 									 *
*									    	  						     				 *
*																						 *
*[Args] :            none																 *
*																						 *
*[Returns]:          no returns															 *
 *****************************************************************************************/
void HMI_SendPassword(void);

/*****************************************************************************************
*[Function Name] :   <HMI_Check_Auth>												     *
*																						 *
*[Description] :     as when the HEAD controller ENTER the check authority mode, the     *
*					the user has 3 tries to enter it right so this function handle the   *
*					displaying of the massage till the Head Controller send a response   *
*					 																	 *
*																						 *
*									    	 											 *						     				*
*																						 *
*[Args] :            none																 *
*																						 *
*[Returns]:          RETURNS TRUE OR FALSE depends on the user entered password,		 *													 *
 *****************************************************************************************/
uint8 HMI_Check_Auth(void);

/**************************************************************
 *                          Definitions                       *
 **************************************************************/

/* this #DEFINES represents the data that will be sent to and from the two controllers*/
/*TRANSMITTED DATA*/
#define ENTER_NEW_PASSWORD                 ('N')
#define OPEN_THE_DOOR                      ('O')
#define DELETE                              ('-')
#define ENTER                              ('K')

/* RECIVED DATA*/
#define ERROR                              ('E')
#define FIRST_TIME                         (1)
#define DONE                               ('D')
#define DOOR_IS_CLOSSING 				   ('C')
#define WRONG_PASSWORD                     ('W')
#define UNBLOCK                            ('U')
#define MISSMATCHING                       ('G')
#define MATCH                              ('M')

#endif /* APP_HMI_MODULE_H_ */
