/*-------------------------------------------------------------------------------------
 |         [FILE NAME]     <keypd.c>    		 									   |
 | 		   [AUTHOR]        <Abdolee>                                                   |
 |         [DATE CREATED]  <Jan 15, 2020>											   |
 |         [DESCRIPTION]   <the C driver file for the KEYPAD>						   |
 |-------------------------------------------------------------------------------------*/


/*************************************************
 *                   INCLUDES                    *
 *************************************************/
#include"keypad.h"

#if KEYPAD_ADC_MODE
#include "adc.h"
#endif

/*************************************************
 *               GLOABL VARIABLES                *
 *************************************************/
#if (!(KEYPAD_ADC_MODE))
/* PRIVET array that has the value of the pins NUMBER of the KEYPAD ROW declared in the \
 H file to give the ability of not wiring the keypad in the same port FLEXIBILITY in
 H.W connections */
static uint8 g_keypadPinNumRowArr[]={ROW1_PIN_NUMB,ROW2_PIN_NUMB,ROW3_PIN_NUMB,ROW4_PIN_NUMB};

/* PRIVET array that has the addresses of PIN REG of the KEYPAD ROW declared in the H file to give the ability of
 * not wiring the keypad in the same port FLEXIBILITY in H.W connections */
static volatile uint8* g_keypadRowArrPinPort[4];


#if KEYPAD4x3   /*  this global variables are fixed for 4x3KEYPAD*/

/* PRIVET array that has the value of the pins NUMBER of the KEYPAD COL declared in the \
 H file to give the ability of not wiring the keypad in the same port FLEXIBILITY in
 H.W connections */
static uint8 g_keypadPinNumColArr[]={COL1_PIN_NUMB,COL2_PIN_NUMB,COL3_PIN_NUMB};

/* PRIVET array that has the addresses of DDR REG of the KEYPAD COL declared in the H file to give the ability of
 * not wiring the keypad in the same port FLEXIBILITY in H.W connections */
static volatile uint8* g_keypadColArrDDR[3];

#endif


#if KEYPAD4x4   /*  this global variables are fixed for 4x4KEYPAD*/


/* PRIVET array that has the value of the pins NUMBER of the KEYPAD COL declared in the \
 H file to give the ability of not wiring the keypad in the same port FLEXIBILITY in
 H.W connections */
static uint8 g_keypadPinNumColArr[]={COL1_PIN_NUMB,COL2_PIN_NUMB,COL3_PIN_NUMB,COL4_PIN_NUMB};

/* PRIVET array that has the addresses of DDR REG of the KEYPAD COL declared in the H file to give the ability of
 * not wiring the keypad in the same port FLEXIBILITY in H.W connections */
static volatile uint8* g_keypadColArrDDR[4];

#endif

#endif

#if KEYPAD4x3

/*PRIVET multidimensional ARR that has the fixed value of KEYPAD buttons, declared in the H file, fixed for 3x4 KEYPAD*/
static uint8 g_keypadValArr[4][3]={{ROW1_COL1_VAL,ROW1_COL2_VAL,ROW1_COL3_VAL},
									{ROW2_COL1_VAL,ROW2_COL2_VAL,ROW2_COL3_VAL},
									{ROW3_COL1_VAL,ROW3_COL2_VAL,ROW3_COL3_VAL},
									{ROW4_COL1_VAL,ROW4_COL2_VAL,ROW4_COL3_VAL} };
#endif

#if KEYPAD4x4
/* multidimensional ARR that has the fixed value of KEYPAD buttons, declared in the H file, fixed for 4x4 KEYPAD*/
static uint8 g_keypadValArr[4][4]={{ROW1_COL1_VAL,ROW1_COL2_VAL,ROW1_COL3_VAL,ROW1_COL4_VAL},
								   {ROW2_COL1_VAL,ROW2_COL2_VAL,ROW2_COL3_VAL,ROW2_COL4_VAL},
								   {ROW3_COL1_VAL,ROW3_COL2_VAL,ROW3_COL3_VAL,ROW3_COL4_VAL},
								   {ROW4_COL1_VAL,ROW4_COL2_VAL,ROW4_COL3_VAL,ROW4_COL4_VAL} };
#endif

#if KEYPAD_ADC_MODE
/* at this ARR will be saved the voltage value of the buttons*/
static uint16 g_keypadAdcValArr[4][4]={0};
#endif

/*************************************************
 *            PRIVATE FUNCTION PROTOTYPE         *
 *************************************************/



/*************************************************
 *              FUNCTION DEFINITIONS             *
 *************************************************/



/*-------------------------------------------------------------------------------------
 |     [FUNCTION NAME]     <KEYPAD_iNIT>         									   |
 | 	   [DESCRIPTION]       < this function is used to InitIalize the KEYPAD from       |
 |	 	 	 	 	 	 	  the static CONFIG in the H file>                         |
 |     [ARGS]  			   <VOID>
 |	   [RETURNS]           <VOID>										           |
 |-------------------------------------------------------------------------------------*/

void KEYPAD_Init(void)
{
#if (!(KEYPAD_ADC_MODE))
/*  putting the PIN AND DDR REG addresses in the global ARR, these #defines are declared in the H file  */
	g_keypadRowArrPinPort[0]=&ROW1_PIN;
	g_keypadRowArrPinPort[1]=&ROW2_PIN;
	g_keypadRowArrPinPort[2]=&ROW3_PIN;
	g_keypadRowArrPinPort[3]=&ROW4_PIN;

	g_keypadColArrDDR[0]=&COL1_DDR;
	g_keypadColArrDDR[1]=&COL2_DDR;
	g_keypadColArrDDR[2]=&COL3_DDR;
	#if KEYPAD4x4
	g_keypadColArrDDR[3]=&COL4_DDR;
	#endif

/* InitIalize the row pins to be I/P pins   */
	CLEAR_BIT(ROW1_DDR,ROW1_PIN_NUMB);
	CLEAR_BIT(ROW2_DDR,ROW2_PIN_NUMB);
	CLEAR_BIT(ROW3_DDR,ROW3_PIN_NUMB);
	CLEAR_BIT(ROW4_DDR,ROW4_PIN_NUMB);

	#if INTERNAL_PULLUP  /* this instructions are fixed if the INTERNAL PULLUP is ENABLED*/
	SET_BIT(ROW1_PORT,ROW1_PIN_NUMB);
	SET_BIT(ROW2_PORT,ROW2_PIN_NUMB);
	SET_BIT(ROW3_PORT,ROW3_PIN_NUMB);
	SET_BIT(ROW4_PORT,ROW4_PIN_NUMB);
	#endif

/* InitIalize the row pins to be I/P pins, as an initial value  */
	CLEAR_BIT(COL1_DDR,COL1_PIN_NUMB);
	CLEAR_BIT(COL2_DDR,COL2_PIN_NUMB);
	CLEAR_BIT(COL3_DDR,COL3_PIN_NUMB);

	#if KEYPAD4x4
	CLEAR_BIT(COL4_DDR,COL4_PIN_NUMB);
	#endif

	/* DISBLING THE PULLUP RES */
	CLEAR_BIT(COL1_PORT,COL1_PIN_NUMB);
	CLEAR_BIT(COL2_PORT,COL2_PIN_NUMB);
	CLEAR_BIT(COL3_PORT,COL3_PIN_NUMB);

	#if KEYPAD4x4
	CLEAR_BIT(COL4_PORT,COL4_PIN_NUMB);
	#endif
#endif

#if (KEYPAD_ADC_MODE)

/* by using the voltage divider to calculate the ADC value of the buttons and save it in an array
 * to compare with it during the run time*/

	for(uint8 i=0;i<ROW;i++)
		{
			for(uint8 j=0;j<COL;j++)
			{
				g_keypadAdcValArr[i][j]= ( 1024*( PULLDOWN_RES_VAL / ( PULLDOWN_RES_VAL + ( ROW_RES_VAL*i ) + ( COL_RES_VAL*j ) ) ) );
			}
		}
	if(ADCSRA==0)  /* this condition checks if the ADC is already enabled
	 	 	 	 	 as is it not enabled only start calling the INIT FUNC*/
	{
		ADC_Settings keypadAdcSettings={AVCC,DIV_BY_64};
		ADC_Init(&keypadAdcSettings);
	}

#endif

}

/*------------------------------------------------------------------------------------------
 |     [FUNCTION NAME]     <KEYPAD_ReadVal>         									   |
 | 	   [DESCRIPTION]       < this function is used to get the value presses in the keypad  |
 |                            by using the polling technique, that wait till it pressed>   |
 |     [ARGS]  			   <VOID> 														   |
 |	   [RETURNS]		   <it return the value that pressed, is represented as a char     |
 |                           that is as well a static value declared in the H file> 	   |							               |
 |-----------------------------------------------------------------------------------------*/

uint8 KEYPAD_ReadVal(void)
{
	uint8 i=0;  /*  this local variable is a counter for the COL loop*/
	uint8 j=0;  /*  this local variable is a counter for the ROW loop*/

#if (!(KEYPAD_ADC_MODE))
	while(1)
	{
		for(i=0;i<COL;i++)
		{
			/*sets the COL[i] pin to be  as O/P pin with, ZERO as O/P Value*/
			SET_BIT(*(g_keypadColArrDDR[i]),g_keypadPinNumColArr[i]);

			for(j=0;j<ROW;j++)
			{
				/*CHECK if the button is pressed by reading the ROW pins by LOOP
				 * , if the reading is LOW it means that the BUTTON[ ROW [ J ] ][ COL [ I ] ]
				 * is pressed and return the value from the ARR  */
				if( ( ! (  READ_BIT( ( * ( g_keypadRowArrPinPort[j] ) ), g_keypadPinNumRowArr[j] ) ) ) )
				{
					_delay_ms(30);
					if( ( ! (  READ_BIT( ( * ( g_keypadRowArrPinPort[j] ) ), g_keypadPinNumRowArr[j] ) ) ) )
					{
						while(( ( ! (  READ_BIT( ( * ( g_keypadRowArrPinPort[j] ) ), g_keypadPinNumRowArr[j] ) ) ) )){}

					CLEAR_BIT(*(g_keypadColArrDDR[i]),g_keypadPinNumColArr[i]);
					return g_keypadValArr[j][i];
					}
				}

			}
			/*sets the COL[i] pin to be  as I/P pin with, the pin in the FLOATING mode*/
			CLEAR_BIT(*(g_keypadColArrDDR[i]),g_keypadPinNumColArr[i]);
		}
	}
#endif

#if KEYPAD_ADC_MODE
	uint16 keyVAL=0;
	while(1)
	{
		/*by reading the ADC channel and comparing from the volt value ARR
		 * by looping throw this ARR  */
		keyVAL=ADC_ReadChannel(ADC_CHANNEL_NUMB);
		for(i=0;i<4;i++)
		{
			for(j=0;j<4;j++)
			{
				/* as this condition accepting an error, if the value is bigger or less by 1*/
				if(( g_keypadAdcValArr[i][j]==keyVAL )|| ( g_keypadAdcValArr[i][j]==(keyVAL+1))||( g_keypadAdcValArr[i][j]==(keyVAL-1)) )
				{
					_delay_ms(30);
					/*check for bouncing before returning the value*/
					if(( g_keypadAdcValArr[i][j]==keyVAL )|| ( g_keypadAdcValArr[i][j]==(keyVAL+1))||( g_keypadAdcValArr[i][j]==(keyVAL-1)) )
					{
						/* wont return till the button is not pressed*/
						while(ADC_ReadChannel(ADC_CHANNEL_NUMB)!=0);
							return g_keypadValArr[i][j];
					}
				}
			}
		}
	}
#endif

}






/*------------------------------------------------------------------------------------------
 |     [FUNCTION NAME]     <KEYPAD_ReadVal>         									   |
 | 	   [DESCRIPTION]       < this function is used to get the value pressed in the keypad  |
 |                            but in an INT form, by using the switch case and getting     |
 | 	 	 	 	 	 	 	  and calling the KEYPAD_ReadVal() that declared in the same   |
 |	 	 	 	 	 	 	  file >												       |
 |     [ARGS]  			   <VOID> 														   |
 |	   [RETURNS]		   <it return the value that pressed, is represented as an INT>    |
 |-----------------------------------------------------------------------------------------*/

uint8 KEYPAD_ReadInt(void)
{
	uint8 val=KEYPAD_ReadVal();
	switch(val)
	{
	case '0':
		return 0;
	case '1':
		return 1;
	case '2':
		return 2;
	case '3':
		return 3;
	case '4':
		return 4;
	case '5':
		return 5;
	case '6':
		return 6;
	case '7':
		return 7;
	case '8':
		return 8;
	case '9':
		return 9;
	default:
		return val;
	}


}

