/************************************************************************************
 [FILE NAME] :  	  <timer1.c>
 [MODULE] :           <TIMER1>
 [AUTHOR] :     	  <Abdulrahman Mohamed , abdulrahman.mohd1996@gmail.com>
 [DATE CREATED] :     <15/12/2019>
 [DISCRIPATION] :     <Source file for the AVR Timer1 driver>
 **************************************************************************************/


/**************************************************************
 *                          INCLUDES                          *
 **************************************************************/
#include "timer1.h"

/**************************************************************
 *                       Global Variables                     *
 **************************************************************/
/* Global variables to hold the address of the call back function in the application */
#if (T1_CTC_INTERRUPT_A)
volatile static void (*g_ctcAIsr_Ptr)(void)=NULL_PTR;
#endif

#if (T1_CTC_INTERRUPT_B)
volatile static void (*g_ctcBIsr_Ptr)(void)=NULL_PTR;
#endif

#if (T1_OVF_INTERRUPT)
volatile static void (*g_ovfIsr_Ptr)(void)=NULL_PTR;
#endif


/**************************************************************
 *                  Private Function Declaration              *
 **************************************************************/


/**************************************************************
 *                           ISR                              *
 **************************************************************/

#if (T1_CTC_INTERRUPT_A)

ISR(TIMER1_COMPA_vect)
{
	if(g_ctcAIsr_Ptr!=NULL_PTR)
	{
		/* Call the Call Back function in the application after the edge is detected */
		(*g_ctcAIsr_Ptr)();
	}
}

#endif


#if (T1_CTC_INTERRUPT_B)

ISR(TIMER1_COMPB_vect)
{
	if(g_ctcAIsr_Ptr!=NULL_PTR)
	{
		/* Call the Call Back function in the application after the edge is detected */
		(*g_ctcBIsr_Ptr)();
	}
}

#endif

#if (T1_OVF_INTERRUPT)

ISR(TIMER1_OVF_vect)
{
	if(g_ovfIsr_Ptr!=NULL_PTR)
	{
		/* Call the Call Back function in the application after the edge is detected */
		(*g_ovfIsr_Ptr)();
	}
}

#endif

/**************************************************************
 *                      Function Definitions                  *
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
void TIMER1_Init(Timer1Settings* a_settings_Ptr)
{

	/* as this FUNC is a critical section, check if is the I-BIT is enabled or not
	 * ,and Clear it if it is by setting a flag to true if  it's enabled before entering this
	 *FUNC to enable it before exiting*/
	bool globalInterruptflag=TRUE;
	(READ_BIT(SREG,7))?(CLEAR_BIT(SREG,7)):(globalInterruptflag=FALSE);

	/* clearing CNTR register*/
	TCNT1=0;

	switch(a_settings_Ptr->e_timer1mode)
	{
	case CTC:

		/* setting the compare value in the registers*/
		OCR1A=a_settings_Ptr->CompareValRegA;
		OCR1B=a_settings_Ptr->CompareValRegB;

		/* CTC mode non PWM mode so set FOC1B and FOC1A*/
		TCCR1A=(3<<FOC1B);
		/* Setting the clock and selecting the CTC mode*/
		TCCR1B=(1<<WGM12)|a_settings_Ptr->e_timer1Clock;

		#if (OC1A_PIN) /*set an output at the OC PINS */
		SET_BIT(DDRD,PD5);
		TCCR1A|=(a_settings_Ptr->e_Oc1PinA<<COM1A0);
		#endif

		#if (OC1B_PIN)
		SET_BIT(DDRD,PD4);
		TCCR1A|=(a_settings_Ptr->e_Oc1PinB<<COM1B0);
		#endif
		break;

	default:
		/* NORMAL mode is the default mode as select the clock and select a NON PWM mode*/
		TCCR1A=(3<<FOC1B);
		TCCR1B=a_settings_Ptr->e_timer1Clock;
	}

	/* setting the interrupt enable if needed */
	#if (T1_CTC_INTERRUPT_A)
	ENABLE_TIMER1_CTCA_INTERRUPT();
	#endif

	#if (T1_CTC_INTERRUPT_B)
	ENABLE_TIMER1_CTCB_INTERRUPT();
	#endif

	#if (T1_OVF_INTERRUPT)
	ENABLE_TIMER1_OVF_INTERRUPT();
	#endif

	/* checking the flag and enable the I-BIT is it's was*/
	(globalInterruptflag)?(SET_BIT(SREG,7)):(FALSE);

}


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
void TIMER1_DeInit(void)
{
	/*clearing the control registers*/
	TCCR1A=0;
	TCCR1B=0;
	#if(CTC_MODE)
	/*clearing the compare value*/
	OCR1A=0;
	OCR1B=0;
	#endif
	/* clearing CNTR register*/
	TCNT1=0;

	/* disabling the interrupts*/
	#if (T1_CTC_INTERRUPT_A)
	DISABLE_TIMER1_CTCA_INTERRUPT();
	#endif

	#if (T1_CTC_INTERRUPT_B)
	DISABLE_TIMER1_CTCB_INTERRUPT();
	#endif

	#if (T1_OVF_INTERRUPT)
	DISABLE_TIMER1_OVF_INTERRUPT();
	#endif
}


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
void TIMER1_SetCallBackFuncForCtcAIsr( void (*a_func_Ptr)(void))
{
	g_ctcAIsr_Ptr=a_func_Ptr;
}
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
void TIMER1_SetCallBackFuncForCtcBIsr( void (*a_func_Ptr)(void))
{
	g_ctcBIsr_Ptr=a_func_Ptr;
}
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
void TIMER1_SetCallBackFuncForOvfIsr( void (*a_func_Ptr)(void))
{
	g_ovfIsr_Ptr=a_func_Ptr;
}
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

void TIMER1_MyDelay(float64 a_timeInMiliSec)
{
	/* this macro is used calculate the number of counts needed to
	 * to occur the wanted delay */
	uint32 counts=CALCULATE_COUNTS(F_CPU,1024.0,a_timeInMiliSec);
	/* the timer setting making the prescaler in 1024 to use the max
	 * possible range */
	Timer1Settings MyDelay={CTC,CK_PRESCALLED_BY_1024};
	while(counts!=0) /* if counts is ended */
	{
		if(counts<=(0XFFFFu))
		{
			/* if its smaller the the timer OVF, then but the counts directly
			 * to the Compare Register*/
			MyDelay.CompareValRegA=counts;
			counts=0;
		}
		else
		{
			/* if counts is bigger than the OVF then need to make multiple OVF
			 * and subtract the OVF counts till the counts is smaller than the OVF
			 * Range then it will be possible to reach the count without OVF*/
			MyDelay.CompareValRegA=(0xFFFFu);
			counts=counts-(0xFFFFu);
		}
		/* Clling the INIT Func every time change the Compare Register*/
		TIMER1_Init(&MyDelay);
		/* reading the Interrupt flag till it's set*/
		while(!(READ_TIMER1_CTCA_INTERRUPT_FLAG()));
		/* then Clear it and start again*/
		CLEAR_TIMER1_CTCA_INTERRUPT_FLAG();
	}
	/* at the end DEINIT the Timer*/
	TIMER1_DeInit();
}

