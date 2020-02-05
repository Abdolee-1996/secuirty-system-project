/*************************************************************************************
 [FILE NAME]  		<control_module.h>
 [AUTHOR]     		<Abdulrahman Mohamed , abdulrahman.mohd1996@gmail.com>
 [DATE CREATED]     <29/1/2020>
 [DISCRIPATION]     <header file for the MAIN CONTROL M.C of security system project >
 **************************************************************************************/

#ifndef APP_CONTROL_MODULE_H_DULE_H_
#define APP_CONTROL_MODULE_H_DULE_H_

/**************************************************************
 *                          INCLUDES                          *
 **************************************************************/
#include "../MCAL/uart.h"
#include "../HAL/external_eeprom.h"
#include "../HAL/dc_motor.h"
#include "util/delay.h"
#include "../MCAL/timer1.h"

/**************************************************************
 *                          Definitions                       *
 **************************************************************/
/* BE NOTICED IF CHANED THESE OPTION YOU NEED TO RESTART EEPROM, OR DELET THE OLD SAVING*/

/* this represent the max and MIN numbers i can enter as a password*/
#define MAX_PASSWORD_CNT                      7
#define MIN_PASSWORD_CNT                      4

#define BUZZER_DDR            				(DDRD)
#define BUZZER_PORT           				(PORTD)
#define BUZZER_PIN            				(PD5)

#define FIRST_TIME_CHECK_VAL                (22u)

#define START_EEPROM_SAVE_ADDRESS           (0X005)
/* this #DEFINES represents the data that will be sent to and from the two controllers*/
/* RECIVED DATA*/

#define ENTER_NEW_PASSWORD                 ('N')
#define OPEN_THE_DOOR                      ('O')
#define DELETE                             ('-') //can delete an wrong number i entered
#define ENTER                              ('K')

/*TRANSMITTED DATA*/

#define ERROR                              ('E')
#define FIRST_TIME                         (1)
#define DONE                               ('D')
#define DOOR_IS_CLOSSING 				   ('C')
#define WRONG_PASSWORD                     ('W')
#define UNBLOCK                            ('U')
#define MISSMATCHING                       ('G')
#define MATCH                              ('M')


/**************************************************************
 *                  Private Function Declaration              *
 **************************************************************/

/*****************************************************************************************
*[Function Name] :   <CONTROL_CheckIfFirstTime>										     *
*																						 *
*[Description] :     this function check if its the first time used this system or not   *
*					 by reading from the EEPROM from its first address , that it suppose *
*					 to have a default known VAL    					     			 *
*																						 *
*[Args] :            none																 *
*																						 *
*[Returns]:          if the KNOWN VAL not found it return its the first time, if found   *
*					 it returns its not the first time									 *
 *****************************************************************************************/
uint8 CONTROL_CheckIfFirstTime(void);

/*****************************************************************************************
*[Function Name] :   <CONTROL_SaveNewPassword>										     *
*																						 *
*[Description] :     this function is one of the main that this system do it saves a     *
*					 a new password to the EEPROM
*																						 *
*[Args] :            none																 *
*																						 *
*[Returns]:          no return														     *
 *****************************************************************************************/
void CONTROL_SaveNewPassword(void);

/*****************************************************************************************
*[Function Name] :   <CONTROL_CheckAuth>										     *
*																						 *
*[Description] :     this function is responsible of checking the user password is the
*					as the password saved in the EEPROM or not					         *
*																						 *
*[Args] :            none																 *
*																						 *
*[Returns]:          wrong or right password														     *
 *****************************************************************************************/
uint8 CONTROL_CheckAuth(void);

/*****************************************************************************************
*[Function Name] :   <CONTROL_RecivePassword>												 	 *
*																						 *
*[Description] :     this function is responsible of receiving the password from the HMI *
*									    	  						     				 *
*																						 *
*[Args] :            PTR to array the the received password will save in																 *
*																						 *
*[Returns]:          no returns															 *
 *****************************************************************************************/
void CONTROL_RecivePassword(uint8* a_password_Ptr);

/*****************************************************************************************
*[Function Name] :   <CONTROL_CheckEquality>												 	 *
*																						 *
*[Description] :     this function is responsible of checking the equality of
*				     two array passed to it 											 *
*									    	  						     				 *
*																						 *
*[Args] :            1.[a_password1_Ptr]: is a PTR to the first password ARR
*					 2.[a_password2_Ptr]: is a PTR to the second password ARR																 *
*																						 *
*[Returns]:          IF EQ OR NOT															 *
 *****************************************************************************************/
uint8 CONTROL_CheckEquality(uint8*  a_password1_Ptr,uint8*  a_password2_Ptr);

#endif
