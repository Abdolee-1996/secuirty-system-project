/*************************************************************************************
 [FILE NAME]  		<timer1.h>
 [MODULE]           TIMER1
 [AUTHOR]     		<Abdulrahman Mohamed , abdulrahman.mohd1996@gmail.com>
 [DATE CREATED]     <15/12/2019>
 [DISCRIPATION]     <Header file for the AVR Timer1 driver>
 **************************************************************************h***********/

#ifndef TIMER1_H_
#define TIMER1_H_

/**************************************************************
 *                          INCLUDES                          *
 **************************************************************/

#include "avr/io.h"
#include "../std_types.h"
#include"../macros.h"
#include "avr/interrupt.h"
/**************************************************************
 *                          Definitions                       *
 **************************************************************/


/**************************************************************
 *                    Static Configurations                   *
 **************************************************************/
/* this #defines can delete code segments before compilation if not needed in the application */

#define CTC_MODE                               		ENABLE

#if (CTC_MODE)
	#define T1_CTC_INTERRUPT_A                        	DISABLE
	#define T1_CTC_INTERRUPT_B                        	DISABLE
	#define OC1A_PIN									DISABLE
	#define OC1B_PIN									DISABLE
#endif

#define T1_OVF_INTERRUPT                            DISABLE

/**************************************************************
 *                           ENUMS                            *
 **************************************************************/
/* this ENUM choose the mode of the timer running in the application*/
typedef enum
{
	OVF=0,
	CTC=1,
}Mode;

/* this ENUM select the CK of the timer*/
typedef enum
{
	NO_CLOCK=0,
	CK_PRESCALLED_BY_0=1,
	CK_PRESCALLED_BY_8=2,
	CK_PRESCALLED_BY_64=3,
	CK_PRESCALLED_BY_256=4,
	CK_PRESCALLED_BY_1024=5,
	EXTERNAL_CK_DETECT_AT_FALLING=6,
	EXTERNAL_CK_DETECT_AT_RAISING=7,
}Clock;


#if(OC1A_PIN || OC1B_PIN)
typedef enum
{

	OC_PIN_DISCONNECTED=0,
	TOGGLE_PIN=1,
	CLEAR_PIN_AT_MATCH=2,
	SET_PIN_AT_MATCH=3,

}OCPinMode;
#endif
/**************************************************************
 *                    Structures and Unions                   *
 **************************************************************/

/**************************************************************************************
 [Structure Name] : Timer1Settings
 [Structure Description] : this STRUCT is describe the timer functionality in the application
 	 	 	 	 	 	   , it will be passed by address to the TIMER1_Init Function
 ***************************************************************************************/

typedef struct
{

	Mode 				e_timer1mode;
	Clock     			e_timer1Clock;

	#if (CTC_MODE)
		uint16          CompareValRegA;
		uint16          CompareValRegB;

		#if (OC1A_PIN)
		OCPinMode   	e_Oc1PinA;
		#endif

		#if (OC1B_PIN)
		OCPinMode   	e_Oc1PinB;
		#endif

	#endif

}Timer1Settings;

/**************************************************************
 *                    FUNCTION-LIKE MACROS                    *
 **************************************************************/

/* macros that read the interrupt flag*/
#define READ_TIMER1_CTCA_INTERRUPT_FLAG()                   							READ_BIT(TIFR,OCF1A)
#define READ_TIMER1_CTCB_INTERRUPT_FLAG()                   							READ_BIT(TIFR,OCF1B)
#define READ_TIMER1_OVF_INTERRUPT_FLAG()												READ_BIT(TIDR,TOV1)
/*macros that cler the interrupt flags after its set*/
#define CLEAR_TIMER1_CTCA_INTERRUPT_FLAG()                  								SET_BIT(TIFR,OCF1A)
#define CLEAR_TIMER1_CTCB_INTERRUPT_FLAG()                  							SET_BIT(TIFR,OCF1B)
#define CLEAR_TIMER1_OVF_INTERRUPT_FLAG()												SET_BIT(TIFR,TOV1)
/* rest the timer1 conter*/
#define RESET_TIMER1_CNTR()                                 							TCNT1=0

#define READ_TIMER1_CNTR()                                  							TCNT1

/* enable and disable the interrupt macros*/
#define ENABLE_TIMER1_CTCA_INTERRUPT()                      							SET_BIT(TIMSK,OCIE1A)
#define DISABLE_TIMER1_CTCA_INTERRUPT()                     							CLEAR_BIT(TIMSK,OCIE1A)

#define ENABLE_TIMER1_CTCB_INTERRUPT()                      							SET_BIT(TIMSK,OCIE1B)
#define DISABLE_TIMER1_CTCB_INTERRUPT()                     							CLEAR_BIT(TIMSK,OCIE1B)

#define ENABLE_TIMER1_OVF_INTERRUPT()													SET_BIT(TIMSK,TOIE1)
#define DISABLE_TIMER1_OVF_INTERRUPT()													CLEAR_BIT(TIMSK,TOIE1)

/* pause timer by disconnecting the clock*/
#define PAUSE_TIMER1()                                      							TCCR1B&=~(0X07)

/*resume the timer by setting clock again this macro ARG is an enum*/
#define RESUME_TIMER1(CLOCK_ENUM)                           							TCCR1B|=CLOCK_ENUM;

/* macro that calculate the counts for a certain time*/
#define CALCULATE_COUNTS(CPU_F,PRE_SCALLER,TIME_IN_MILI_SECOND)    					((TIME_IN_MILI_SECOND/1000.0)*(CPU_F/PRE_SCALLER))

/**************************************************************
 *                    Shared Global Variables                 *
 **************************************************************/


/**************************************************************
 *                   FUNCTION DECLARATIONS                     *
 **************************************************************/

/******************************************************************************************
*[Function Name] :   <TIMER1_Init>													  	  *
*																					      *
*[Description] :     1.select the timer mode										      *
* 	 	 	 	 	 2.select the timer clock										      *
* 	 	 	 	 	 3.enable interrupt if wanted								          *
* 	 	 	 	 	 4.set the OC Pin mode if wanted								      *
* 	 	 	 	 	 5.set the compare value at CTC mode              			          *
*																				          *
*[Args] :            it takes a PTR to TimerSeettings Structure that has the INIT Value	  *
*																						  *
*[Returns]:          no returns															  *
 ******************************************************************************************/
void TIMER1_Init(Timer1Settings* a_settings_Ptr);

/*****************************************************************************************
*[Function Name] :   <TIMER1_DeInit>												 	 *
*																						 *
*[Description] :     1. disconnect the clock from the timer module						 *
* 	 	 	 	 	 2. reset the timer value										     *
* 	 	 	 	 	 3.reset all the timer register to zero							     *
*																						 *
*[Args] :            none																 *
*																						 *
*[Returns]:          no returns															 *
 *****************************************************************************************/
void TIMER1_DeInit(void);

#if (T1_CTC_INTERRUPT_A)
/******************************************************************************************
*[Function Name] :  <TIMER1_SetCallBackFuncForCtcAIsr>						              *
*																					      *
*[Description] :    set the call back function for the CTCA interrupt 					  *
*																				          *
*[Args] :           it takes a PTR to Function that will be executed when the interrupt   *
*					occur		 											              *
*																						  *
*[Returns]:          no returns															  *
 ******************************************************************************************/
void TIMER1_SetCallBackFuncForCtcAIsr( void (*a_func_Ptr)(void));
#endif


#if (T1_CTC_INTERRUPT_B)
/******************************************************************************************
*[Function Name] :  <TIMER1_SetCallBackFuncForCtcBIsr>						              *
*																					      *
*[Description] :    set the call back function for the CTCB interrupt 					  *
*																				          *
*[Args] :           it takes a PTR to Function that will be executed when the interrupt   *
*					occur		 											              *
*																						  *
*[Returns]:          no returns															  *
 ******************************************************************************************/
void TIMER1_SetCallBackFuncForCtcBIsr( void (*a_func_Ptr)(void));
#endif


#if (T1_OVF_INTERRUPT)
/******************************************************************************************
*[Function Name] :  <TIMER1_SetCallBackFuncForOvfIsr>						              *
*																					      *
*[Description] :    set the call back function for the OVF interrupt 					  *
*																				          *
*[Args] :           it takes a PTR to Function that will be executed when the interrupt   *
*					occur		 											              *
*																						  *
*[Returns]:          no returns															  *
 ******************************************************************************************/
void TIMER1_SetCallBackFuncForOvfIsr( void (*a_func_Ptr)(void));
#endif

/******************************************************************************************
*[Function Name] :   <TIMER1_MyDelay>													  *
*																					      *
*[Description] :     this functions generate a delay using Timer1						  *
*																				          *
*[Args] :            it takes the time in MILI seconds	  								  *
*																						  *
*[Returns]:          no returns															  *
 ******************************************************************************************/
void TIMER1_MyDelay(float64 a_timeInMiliSec);

#endif /* TIMER1_H_ */
