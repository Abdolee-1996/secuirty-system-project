
/*************************************************************************************
 [FILE NAME]  		<hmi.c>
 [AUTHOR]     		<Abdulrahman Mohamed , abdulrahman.mohd1996@gmail.com>
 [DATE CREATED]     <15/12/2019>
 [DISCRIPATION]     <Source file for the HMI of security system project >
 **************************************************************************************/

/**************************************************************
 *                          INCLUDES                          *
 **************************************************************/

#include "hmi_module.h"

/**************************************************************
 *                  	GLOBAL VARIABLES			          *
 **************************************************************/
/*this global variables indicates the max and the MIN password CNTS*/
volatile uint8 g_maxPassCnts=0;
volatile uint8 g_minPassCnts=0;


/**************************************************************
 *                      Function Definitions                  *
 **************************************************************/
int main(void)
{
	/* at first need to INIT the UART  as quick as possible to be ready to receive any
	 * data the HEAD M.C SEND and don't occur any miss synchronization*/
	uartSettings uart={DISABLE_PARITY,EIGHT_BITS,RATE_9600_BPS};
	UART_Init(&uart);

	/*as well need to INIT the KEYPAD and the LCD to handle the HMI process*/
	LCD_Init();
	KEYPAD_Init();
/*display a LOADING at the LCD screen till the receive a response of the HEAD M.C
 * to start processing */
	LCD_SendString("LOADING...");

/* at these stage the HEAD M.C sending the max and MIN password CNTS */
	g_maxPassCnts=UART_ReceiveChar();
	UART_TransmitChar(TRUE);
	g_minPassCnts=UART_ReceiveChar();
	UART_TransmitChar(TRUE);
	/* depend on the first MSG sent from the HEAD M.C, it will switch as if its the first time
	 * this user use this security system or the there is a password saved before*/
	switch(UART_ReceiveChar())
	{
	/*if first time send to the HEAD M.C i received your MSG and need to enter my
	 * new first password, so it call the HMI_EnterNewPassword() FUNC to enter to
	 * enter new password mode*/
	case FIRST_TIME:
		UART_TransmitChar(ENTER_NEW_PASSWORD);
		HMI_EnterNewPassword();
		break;
		/*if the received data is an error MSG as there is an H.W error so the LCD
		 * display an ERROR MSG and enter an infinite loop till restart */
	case ERROR:
		LCD_SendString("ERROR");
		while(1);
	}

/* if not enter the main screen inside the while 1*/
	while(1)
	{
		/*the main screen contain of these two options enter new password or open the door
		 * selecting one of these options by the keypad*/
		LCD_SendCommand(CLEAR_DISPLAY);
		LCD_SendString("'=' OPENS DOOR");
		LCD_SendCommand(FORCE_TO_NEXT_LINE);
		LCD_SendString("'+' CHANGES PASS");
/* wait till the user enter an defined I/P in the keypad is not defined won't make any action*/
		switch(KEYPAD_ReadVal())
		{
		/* case 1 or first mode is entering new new password mode but this time not
		 * for the first time means that i need to change my pssword  */
		case '+':
			/*send that i need to enter this mode by UART*/
			UART_TransmitChar(ENTER_NEW_PASSWORD);
			/* NOW calls the HMI_Check_Auth() that return the password the user is correct or not
			 * depending on the response received from the Head M.C */
			if(HMI_Check_Auth())
			{
				/* if true enter the wanted mode*/
				HMI_EnterNewPassword();
			}
			break;
			/*case 2 is the second mode open the door mode*/
		case '=':
			/*send using the UART need to enter this mode*/
			UART_TransmitChar(OPEN_THE_DOOR);
			/*as well need to check authority as well , and check and return if the entered  password is TRUE */
			if(HMI_Check_Auth())
			{
				/*if TRUE enter the wanted mode, as at these mode no action need from the HMI, so it only
				 * DISPLAY a MSG and halt till the the HEAD M.C send a MSG to end the HALT so no need to write
				 * a specific Function for this mode*/
				LCD_SendCommand(CLEAR_DISPLAY);
				LCD_SendString("DOOR IS OPENING");
				UART_ReceiveChar();
				LCD_SendCommand(CLEAR_DISPLAY);
				LCD_SendString("DOOR IS CLOSSING");
				UART_ReceiveChar();
			}
			break;
		}
	}

	return 0;
}



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

void HMI_SendPassword(void)
{
	/*as from the main function that calls this FUNC is responsible for Displaying the MSG
	 * to the user this is only responsible to send the password entered and display stars in
	 * the next line, so send the LCD start writing in the Second line  */
	LCD_SendCommand(FORCE_TO_NEXT_LINE);
	/* this local variable will hold the value of the value entered from the user, using keypad*/
	uint8 val=0;
	/*this is a CNTR for that ondecate the number of valid I/P t be used as a password,
	 * and to as well control the max password I/P i need for this system , in this system
	 * i designed it to be 4 numbers I/P*/
	uint8 passwordCntr=0;
	while(val!=ENTER||passwordCntr<g_minPassCnts )
	{
		/* wont leave this loop till the entered value will  be an enter key and enter
		 * the MIN password numbers is 4 as well*/
		val=KEYPAD_ReadInt();
		/*reads the keypad VAL */
		if(val==DELETE && passwordCntr>0 )
		{
			/*this section is responsible of the deleting feature as if i entered an wrong number
			 * and need to to delete it , i wont enter this section till the value is '-'  and the number
			 * of entered numbers is bigger than 0*/
			LCD_SendCommand(FORCE_ROW_COL(1,(passwordCntr-1)));
			/*i need the decrement the number of stars displayed on the screen by this
			 * block of code , by changing the cursor to the last place it write and delete the stare*/
			LCD_SendChar(' ');
			/*and write start writing from this place*/
			LCD_SendCommand(FORCE_ROW_COL(1,(passwordCntr-1)));
			/*don't forget to decrement the CNTR as well*/
			passwordCntr--;
			/*send the delete command by UART*/
			UART_TransmitChar(DELETE);
		}
		/* if the entered is valid value as and the CNTR is less than the max value*/
		else if(val>=0 && val<=9 && passwordCntr<g_maxPassCnts)
		{
			/*display a star*/
			LCD_SendChar('*');
			/*increment the CNTR*/
			passwordCntr++;
			/*send the entered number by UART*/
			UART_TransmitChar(val);
		}
	}
	/*send the enter command by UART*/
	UART_TransmitChar(ENTER);
}

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


void HMI_EnterNewPassword(void)
{
	/*its one of the main modes , this task wont get out till its finish from the process, either
	 * by saving new password or not enter a one */
	while(1)
	{
		/*at this mode i need to make the user enter the wanted to be saved password to times
		 * and check the matching , so i display and calling the sendPassword FUNC twice*/
		LCD_SendCommand(CLEAR_DISPLAY);
		LCD_SendString("ENTER NEW PASS:");
		LCD_SendCommand(FORCE_TO_NEXT_LINE);
		HMI_SendPassword();

		LCD_SendCommand(CLEAR_DISPLAY);
		LCD_SendString("ENTER AGAIN:");
		HMI_SendPassword();

		/* after sending the two password wait till the MAEN M.C send the response */
		switch(UART_ReceiveChar())
		{
		/*is the response is done and the password is saved it will display the following MSG
		 * return to the MAIN DISPLAY*/
		case DONE:
			LCD_SendCommand(CLEAR_DISPLAY);
			LCD_SendString("SUCCESS");
			_delay_ms(1000);
			return;
			/*if the response is error , it display an error MSG and return , this is error
			 * in other side at the HEAD M.C , it mainly hardware error*/
		case ERROR:
			LCD_SendCommand(CLEAR_DISPLAY);
			LCD_SendString("ERROR");
			_delay_ms(1000);
			return;
			/* if the entered password not matched it will display this MSG and make the same process
			 * again till the the two entered password is right*/
		case MISSMATCHING:
			LCD_SendCommand(CLEAR_DISPLAY);
			LCD_SendString("PASS MISSMATCH");
			_delay_ms(1000);
		}

	}
}

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
uint8 HMI_Check_Auth(void)
{
	/*as a mention before i need to count the tries , this local variable is saving the
	 * number of tries  */
	uint8 tryCntr=0;
	while(1)
	{
		/*need to enter the saved password so calling send password FUNC*/
		LCD_SendCommand(CLEAR_DISPLAY);
		LCD_SendString("ENTER YOUR PASS:");
		HMI_SendPassword();


		/*wait for the MAIN M.C to response*/
		switch(UART_ReceiveChar())
		{
		/*if match return TRUE*/
		case (MATCH):
			return TRUE;
		/*if the response is error , it display an error MSG and return FALSE , this is error
			 * in other side at the HEAD M.C , it mainly hardware error*/
		case ERROR:
			LCD_SendCommand(CLEAR_DISPLAY);
			LCD_SendString("ERROR");
			_delay_ms(1000);
			return FALSE;
			/*if the PASSWORD is not the saved password */
		case (WRONG_PASSWORD):
		/*it check the number of tries if its more than 3*/
			if(tryCntr>=2)
			{
				/*it display this MSG and and enter an halt mode till the MAIN M.C
				 * send to return FALSE */
				LCD_SendCommand(CLEAR_DISPLAY);
				LCD_SendString("Y7RAMI YABNEL***");
				UART_ReceiveChar();
				return FALSE;
			}
		/*if number of tries less than 3 is display this MSG and INC the CNTR*/
			LCD_SendCommand(CLEAR_DISPLAY);
			LCD_SendString("WRONG PASS!");
			_delay_ms(1000);
			tryCntr++;
		}
	}
}

