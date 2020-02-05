/*************************************************************************************
 [FILE NAME]  		<dc_motor.h>
 [AUTHOR]     		<Abdulrahman Mohamed , abdulrahman.mohd1996@gmail.com>
 [DATE CREATED]     <15/12/2019>
 [DISCRIPATION]     <Header file for the AVR dc_motor driver>
 **************************************************************************************/


#ifndef DC_MOTOR_H_
#define DC_MOTOR_H_

/*******************************************
 *               #INCLUDES                 *
 *******************************************/
#include"../macros.h"
#include"../std_types.h"
#include<avr/io.h>

/*******************************************
 *              DIFENATIONS                *
 *******************************************/
/* this indicate the type of H-BRIDGE H.W and it might  in the future will use another H.W that needs a DIFFRENT code*/
#define L293D_HBRIDGE  ENABLE
/* this #defines indicate how many motor it will be used in the application*/
#define MOTOR1    ENABLE
#define MOTOR2    DISABLE
#define MOTOR3    DISABLE
#define MOTOR4    DISABLE

/*******************************************
 *              STATIC CONFIG              *
 *******************************************/
/* static configuration for the pins connected to H-BRIDGE*/
	#if L293D_HBRIDGE

		#if MOTOR1

		#define M1      	              	    1

		#define _MOTOR1_OP_DDR1          	   DDRB
		#define _MOTOR1_OP_PORT1          	   PORTB
		#define MOTOR1_OP_PIN1_NUMB         	0

		#define _MOTOR1_OP_DDR2          	   DDRB
		#define _MOTOR1_OP_PORT2           	   PORTB
		#define MOTOR1_OP_PIN2_NUMB         	1

		#endif


		#if MOTOR2

		#define M2      	              	   2

		#define _MOTOR2_OP_DDR1          	   DDRC
		#define _MOTOR2_OP_PORT1          	   PORTC
		#define MOTOR2_OP_PIN1_NUMB         	2

		#define _MOTOR2_OP_DDR2          	   DDRC
		#define _MOTOR2_OP_PORT2           	   PORTC
		#define MOTOR2_OP_PIN2_NUMB         	3

		#endif


		#if MOTOR3

		#define M3      	              	3

		#define _MOTOR3_OP_DDR1          	   DDRC
		#define _MOTOR3_OP_PORT1          	   PORTC
		#define MOTOR3_OP_PIN1_NUMB         	0

		#define _MOTOR3_OP_DDR2          	   DDRC
		#define _MOTOR3_OP_PORT2           	   PORTC
		#define MOTOR3_OP_PIN2_NUMB         	1

		#endif


		#if MOTOR4

		#define M4      	              	4

		#define _MOTOR4_OP_DDR1          	   DDRC
		#define _MOTOR4_OP_PORT1          	   PORTC
		#define MOTOR4_OP_PIN1_NUMB         	0

		#define _MOTOR4_OP_DDR2          	   DDRC
		#define _MOTOR4_OP_PORT2           	   PORTC
		#define MOTOR4_OP_PIN2_NUMB         	1

		#endif

	#endif

/*******************************************
 *           FUNCTIONS PROTOTYPES          *
 *******************************************/

/******************************************************************************************
*[Function Name] :   <DC_MOTOR_Init>													  	  *
*																					      *
*[Description] :     it set bits to O/P pins, that connect to H-BRIDGE,					  *
*																				          *
*[Args] :            it takes the motor number as it INIT each one by itself	          *
*																						  *
*[Returns]:          no returns															  *
 ******************************************************************************************/
void DC_MOTOR_Init(uint8 a_motorNumb);

/******************************************************************************************
*[Function Name] :   <DC_MOTOR_MoveCkWise>												  *
*																					      *
*[Description] :     it set the motor direction to CK WISE, by the H-BRODGE				  *
*																				          *
*[Args] :            it takes the motor number as it control each one by itself	          *
*																						  *
*[Returns]:          no returns															  *
 ******************************************************************************************/
void DC_MOTOR_MoveCkWise(uint8 a_motorNumb);

/******************************************************************************************
*[Function Name] :   <DC_MOTOR_MoveCkWise>												  *
*																					      *
*[Description] :     it set the motor direction to antiCK WISE, by the H-BRODGE			  *
*																				          *
*[Args] :            it takes the motor number as it control each one by itself	          *
*																						  *
*[Returns]:          no returns															  *
 ******************************************************************************************/
void DC_MOTOR_MoveAntiCkWise(uint8 a_motorNumb);

/******************************************************************************************
*[Function Name] :   <DC_MOTOR_MoveCkWise>												  *
*																					      *
*[Description] :     it stop the motor				  									  *
*																				          *
*[Args] :            it takes the motor number as it control each one by itself	          *
*																						  *
*[Returns]:          no returns															  *
 ******************************************************************************************/
void DC_MOTOR_Stop(uint8 a_motorNumb);

#endif /* DC_MOTOR_H_ */
