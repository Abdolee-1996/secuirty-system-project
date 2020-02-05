/************************************************************************************
 [FILE NAME] :  	  <dc_motor.c>
 [AUTHOR] :     	  <Abdulrahman Mohamed , abdulrahman.mohd1996@gmail.com>
 [DATE CREATED] :     <15/12/2019>
 [DISCRIPATION] :     <Source file for the AVR DC MOTOR driver>
 **************************************************************************************/

/**************************************************************
 *                          INCLUDES                          *
 **************************************************************/
#include"dc_motor.h"


/**************************************************************
 *                      Function Definitions                  *
 **************************************************************/

/******************************************************************************************
*[Function Name] :   <DC_MOTOR_Init>													  	  *
*																					      *
*[Description] :     it set bits to O/P pins, that connect to H-BRIDGE,					  *
*																				          *
*[Args] :            it takes the motor number as it INIT each one by itself	          *
*																						  *
*[Returns]:          no returns															  *
 ******************************************************************************************/
void DC_MOTOR_Init(uint8 a_motorNumb)
{
	switch(a_motorNumb)
	{
	case M1:
		SET_BIT(_MOTOR1_OP_DDR1,MOTOR1_OP_PIN1_NUMB);
		SET_BIT(_MOTOR1_OP_DDR2,MOTOR1_OP_PIN2_NUMB);
		break;

#if MOTOR2
	case M2:
		SET_BIT(_MOTOR2_OP_DDR1,MOTOR2_OP_PIN1_NUMB);
		SET_BIT(_MOTOR2_OP_DDR2,MOTOR2_OP_PIN2_NUMB);
		break;
#endif

#if MOTOR3
	case M3:
		SET_BIT(_MOTOR3_OP_DDR1,MOTOR3_OP_PIN1_NUMB);
		SET_BIT(_MOTOR3_OP_DDR2,MOTOR3_OP_PIN2_NUMB);
		break;
#endif

#if MOTOR4
	case M4:
		SET_BIT(_MOTOR4_OP_DDR1,MOTOR4_OP_PIN1_NUMB);
		SET_BIT(_MOTOR4_OP_DDR2,MOTOR4_OP_PIN2_NUMB);
		break;
#endif

	}

}

/******************************************************************************************
*[Function Name] :   <DC_MOTOR_MoveCkWise>												  *
*																					      *
*[Description] :     it set the motor direction to CK WISE, by the H-BRODGE				  *
*																				          *
*[Args] :            it takes the motor number as it control each one by itself	          *
*																						  *
*[Returns]:          no returns															  *
 ******************************************************************************************/

void DC_MOTOR_MoveCkWise(uint8 a_motorNumb)
{
	switch(a_motorNumb)
	{
	case M1:
		CLEAR_BIT(_MOTOR1_OP_PORT1,MOTOR1_OP_PIN1_NUMB);
		SET_BIT(_MOTOR1_OP_PORT2,MOTOR1_OP_PIN2_NUMB);
		break;

#if MOTOR2
	case M2:
		CLEAR_BIT(_MOTOR2_OP_PORT1,MOTOR2_OP_PIN1_NUMB);
		SET_BIT(_MOTOR2_OP_PORT2,MOTOR2_OP_PIN2_NUMB);
		break;
#endif

#if MOTOR3
	case M3:
		CLEAR_BIT(_MOTOR3_OP_PORT1,MOTOR3_OP_PIN1_NUMB);
		SET_BIT(_MOTOR3_OP_PORT2,MOTOR3_OP_PIN2_NUMB);
		break;
#endif

#if MOTOR4
	case M4:
		CLEAR_BIT(_MOTOR4_OP_PORT1,MOTOR4_OP_PIN1_NUMB);
		SET_BIT(_MOTOR4_OP_PORT2,MOTOR4_OP_PIN2_NUMB);
		break;
#endif

	}

}

/******************************************************************************************
*[Function Name] :   <DC_MOTOR_MoveCkWise>												  *
*																					      *
*[Description] :     it set the motor direction to antiCK WISE, by the H-BRODGE			  *
*																				          *
*[Args] :            it takes the motor number as it control each one by itself	          *
*																						  *
*[Returns]:          no returns															  *
 ******************************************************************************************/
void DC_MOTOR_MoveAntiCkWise(uint8 a_motorNumb)
{
	switch(a_motorNumb)
	{
	case M1:
		SET_BIT(_MOTOR1_OP_PORT1,MOTOR1_OP_PIN1_NUMB);
		CLEAR_BIT(_MOTOR1_OP_PORT2,MOTOR1_OP_PIN2_NUMB);
		break;

#if MOTOR2
	case M2:
		SET_BIT(_MOTOR2_OP_PORT1,MOTOR2_OP_PIN1_NUMB);
		CLEAR_BIT(_MOTOR2_OP_PORT2,MOTOR2_OP_PIN2_NUMB);
		break;
#endif

#if MOTOR3
	case M3:
		SET_BIT(_MOTOR3_OP_PORT1,MOTOR3_OP_PIN1_NUMB);
		CLEAR_BIT(_MOTOR3_OP_PORT2,MOTOR3_OP_PIN2_NUMB);
		break;
#endif

#if MOTOR4
	case M4:
		SET_BIT(_MOTOR4_OP_PORT1,MOTOR4_OP_PIN1_NUMB);
		CLEAR_BIT(_MOTOR4_OP_PORT2,MOTOR4_OP_PIN2_NUMB);
		break;
#endif

	}

}

/******************************************************************************************
*[Function Name] :   <DC_MOTOR_MoveCkWise>												  *
*																					      *
*[Description] :     it stop the motor				  									  *
*																				          *
*[Args] :            it takes the motor number as it control each one by itself	          *
*																						  *
*[Returns]:          no returns															  *
 ******************************************************************************************/

void DC_MOTOR_Stop(uint8 a_motorNumb)
{
	switch(a_motorNumb)
	{
	case 1:
		CLEAR_BIT(_MOTOR1_OP_PORT1,MOTOR1_OP_PIN1_NUMB);
		CLEAR_BIT(_MOTOR1_OP_PORT2,MOTOR1_OP_PIN2_NUMB);
		break;

#if MOTOR2
	case M2:
		CLEAR_BIT(_MOTOR2_OP_PORT1,MOTOR2_OP_PIN1_NUMB);
		CLEAR_BIT(_MOTOR2_OP_PORT2,MOTOR2_OP_PIN2_NUMB);
		break;
#endif

#if MOTOR3
	case M3:
		CLEAR_BIT(_MOTOR3_OP_PORT1,MOTOR3_OP_PIN1_NUMB);
		CLEAR_BIT(_MOTOR3_OP_PORT2,MOTOR3_OP_PIN2_NUMB);
		break;
#endif

#if MOTOR4
	case M4:
		CLEAR_BIT(_MOTOR4_OP_PORT1,MOTOR4_OP_PIN1_NUMB);
		CLEAR_BIT(_MOTOR4_OP_PORT2,MOTOR4_OP_PIN2_NUMB);
		break;
#endif

	}

}
