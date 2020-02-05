
/*************************************************************************************
 [FILE NAME]  		<control.c>
 [AUTHOR]     		<Abdulrahman Mohamed , abdulrahman.mohd1996@gmail.com>
 [DATE CREATED]     <29/1/2020>
 [DISCRIPATION]     <Source file for the MAIN CONTROL M.C of security system project >
 **************************************************************************************/

/**************************************************************
 *                          INCLUDES                          *
 **************************************************************/
#include "control_module.h"

/**************************************************************
 *                  	GLOBAL VARIABLES			          *
 **************************************************************/
/* this is flag to indicate if this is the first time using this system or
 * there is an previous password saved in the EEPROM*/
uint8 g_firstTimeFlag=0;

/**************************************************************
 *                      Function Definitions                  *
 **************************************************************/

int main(void)
{
	/*at first need to INIT the EXTERNAL EEPROM AND DC MOTOR */
	EXTERNAL_EEPROM_Init();
	DC_MOTOR_Init(M1);
	/*setting the buzzer pin as O/P*/
	SET_BIT(BUZZER_DDR,BUZZER_PIN);

	/*INIT the UART to send and receive data from the and to the HMI M.C*/
	uartSettings uart={DISABLE_PARITY,EIGHT_BITS,RATE_9600_BPS};
	UART_Init(&uart);
	/* make a delay to make sure that other HMI M.C  is finished from it's INIT and ready to receive from me  */
	_delay_ms(250);

	UART_TransmitChar(MAX_PASSWORD_CNT);
	UART_ReceiveChar();
	_delay_ms(100);
	UART_TransmitChar(MIN_PASSWORD_CNT);
	UART_ReceiveChar();
	_delay_ms(100);
	/*check if by calling this FUNC that return if its the first time or not, the user
	 * used this system*/
	g_firstTimeFlag=CONTROL_CheckIfFirstTime();
	/* and send the flag to HMI to tell him which mode it should enter the main mode
	 * or enter new password mode*/
	UART_TransmitChar(g_firstTimeFlag);

	/* the main mode is inside this while loop*/
	while(1)
	{
		/*after sending to the HMI wait till it send a response that it receive my data
		 * and i enter the wanted mode */
		switch(UART_ReceiveChar())
		{
		/* if the case is enter a new password*/
		case ENTER_NEW_PASSWORD :
/* it first check the flg if its the first time or not*/
			if(g_firstTimeFlag)
			{
				/* if its true it enter the save new password mode by calling this FUNC, without check for any authority*/
				CONTROL_SaveNewPassword();
			}
			else
			{
				/*if not its first time it checks for authority first by calling this FUNC*/
				if(CONTROL_CheckAuth())
				{
					/* IF its TRUE it enter the wanted mode*/
					CONTROL_SaveNewPassword();
				}
			}
			break;
/* if the wanted mode is to open the door*/
		case OPEN_THE_DOOR :
			/* it checks for authority*/
			if(CONTROL_CheckAuth())
			{
				/* if true it enters the mode*/
				/* as it start opening the door as requested*/
				DC_MOTOR_MoveCkWise(M1);
				TIMER1_MyDelay(15000);
				/*then send to the HMI to display he door is closing*/
				UART_TransmitChar(DOOR_IS_CLOSSING);
				DC_MOTOR_MoveAntiCkWise(M1);
				TIMER1_MyDelay(15000);
				DC_MOTOR_Stop(M1);
				/* and send to the HMI to return the to the main mode*/
				UART_TransmitChar(UNBLOCK);
			}
		}
	}
	return 0;
}



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
uint8 CONTROL_CheckIfFirstTime(void)
{
	/*this local variable will save the value read from the EEPROM*/
	uint8 check=0;
	if(EXERNAL_EEPROM_ReadByte(0x0001,&check))
	{
		/*if return success */
		if(check==FIRST_TIME_CHECK_VAL)
		{
	/* and the value is the same as the known value return not first time*/
		return !(FIRST_TIME);
		}
		else
		{
	/* if not the same return*/
		return FIRST_TIME;
		}

	}
	else
	{
		/* if the action on the EEPROM is return FAIL */
		return ERROR;
	}
}


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


void CONTROL_SaveNewPassword(void)
{
/* as when saving new password we need from the user to enter the password two times, so create two arrays
 * to save the values and compare between them*/
	uint8 password1[MAX_PASSWORD_CNT];
	uint8 password2[MAX_PASSWORD_CNT];
	while(1)
	{
		/* at this stage the user enter the password two times*/
		CONTROL_RecivePassword(password1);
		CONTROL_RecivePassword(password2);

		/* after finishing we compare between them */
		if(CONTROL_CheckEquality(password1,password2))
		{
			/* if true and match*/
			for(uint8 i=0;i<MAX_PASSWORD_CNT;i++)
			{
				/* start saving in EEPROM from KNOWN address*/
				if(!(EXTERNAL_EEPROM_WriteByte((START_EEPROM_SAVE_ADDRESS+i),password1[i])))
				{
					/* if the action on the EEPROM is return FAIL */
					UART_TransmitChar(ERROR);
					return;
				}
				/* this delay as the EEPROM not that fast*/
				_delay_ms(20);
			}
			/* and if its the first time the user save a password*/
			if(g_firstTimeFlag)
			{
				/* it save in address 0x001 the KNOWN VAL */
				if(!(EXTERNAL_EEPROM_WriteByte(0x0001,FIRST_TIME_CHECK_VAL)))
				{
					/* if the action on the EEPROM is return FAIL */
					UART_TransmitChar(ERROR);
					return;
				}
			}
			/* if the saving is done it send to HMI that the action is done*/
			UART_TransmitChar(DONE);
			return;
		}
		/* if the two password is not in match*/
		else
		{
			/* it return miss match to HMI and repeat the code again*/
			UART_TransmitChar(MISSMATCHING);
		}


	}

}



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

uint8 CONTROL_CheckAuth(void)
{
	/*this local variable is used to save the value read from EEPROM, the correct password*/
	uint8 savedPassword=0;

	/* this array is used to save the  password the user enter*/
	uint8 userPassword[MAX_PASSWORD_CNT];
	/* as this CNTR is counting the tries the user  enter the password wrong */
	uint8 tryCntr=0;

	while(1)
	{
		/* this flag indicate if the user entered password is the same as the corrct one
		 * saved in the EEPROM is INIT set to TRUE and while comparing it will be set to FALS if wrong  */
		uint8 matchFlag=TRUE;
		/* calling the FUNC responsible of receiving the password from the user*/
		CONTROL_RecivePassword(userPassword);

		for(uint8 i=0; i<MAX_PASSWORD_CNT ;i++)
		{
			/*now start comparing between the user I/P and the password saved in EEPROM*/
			if(!(EXERNAL_EEPROM_ReadByte((START_EEPROM_SAVE_ADDRESS+i), &(savedPassword) )))
			{
				/* if there is an error send ERROR to the HMI and return*/
				UART_TransmitChar(ERROR);
				return FALSE;
			}
			/* this delay as the EEPROM not that fast*/
			_delay_ms(20);
			/* if there is any mismatch between the two password*/
			if(savedPassword!=userPassword[i])
			{
				/* if the counts is more than 2*/
				if(tryCntr>=2)
				{
					/* it transmit WRONG PASS to the HMI */
					UART_TransmitChar(WRONG_PASSWORD);
					/* enable the buzzer*/
					SET_BIT(BUZZER_PORT,BUZZER_PIN);
					/* delay for 60 SEC*/
					TIMER1_MyDelay(60000)
					/* disable buzzer*/;
					CLEAR_BIT(BUZZER_PORT,BUZZER_PIN);
					/* and then send to the HMI to return to the main mode*/
					UART_TransmitChar(UNBLOCK);
					/* as well return from the function with wrong password*/
					return FALSE;
				}
				/* if the CNTS is less than two */
				/* it make the flag to FALSE*/
				matchFlag=FALSE;
				/* send wrong password to the HMI*/
				UART_TransmitChar(WRONG_PASSWORD);
				/* INC the CNTR*/
				tryCntr++;
				break;
			}
		}
		/* if the flag is still TRUE */
		if(matchFlag)
		{
			/* send that this password is matched*/
			UART_TransmitChar(MATCH);
			/* and return true password*/
			return TRUE;
		}
	}
}


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
void CONTROL_RecivePassword(uint8* a_password_Ptr)
{
	/* the keypad received from HMI is saved in these variable*/
	uint8 button=0;
	/* this a CNTR that counts the passwords CNTS*/
	uint8 passCnts=0;

	while(button!=ENTER|| passCnts<MIN_PASSWORD_CNT )
	{/*while the entered k is not 'K' and not the MIN password CNTS*/
		button=UART_ReceiveChar();
		if(button==DELETE && passCnts>0)
		{
			/* if the button is '-'  decrease the CNTS*/
			passCnts--;
		}
		else if(button>=0 && button<=9 && passCnts<MAX_PASSWORD_CNT)
		{
			/* save the value in the argument array*/
			a_password_Ptr[passCnts]=button;
			passCnts++;
		}
	}
	/* if not reach the max password CNTS it zero the rest of the array to make no mistake
	 * at comparing from the EEPROM*/
	while(passCnts<MAX_PASSWORD_CNT)
	{
		a_password_Ptr[passCnts]=0;
		passCnts++;
	}
}


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

uint8 CONTROL_CheckEquality(uint8*  a_password1_Ptr,uint8*  a_password2_Ptr)
{
	for(uint8 i=0; i<MAX_PASSWORD_CNT ; i++)
	{
		if(a_password1_Ptr[i]!=a_password2_Ptr[i])
		{
			/* IF ANY ELEMT NOT EQ RETURN FALSE*/
			return FALSE;
		}
	}

	return TRUE;
}
