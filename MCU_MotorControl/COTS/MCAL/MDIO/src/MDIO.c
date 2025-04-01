#include "bit_math.h"
/* to include right reg .h file during testing */
#ifdef UNIT_TESTING_MODE	
	/* header files that are included only when in testing mode */
	#include <stdint.h>
	#include <stddef.h>
	#include <stdbool.h>
	#include "MDIO_MockReg.h"

	/* mock base addresses for ports using extern (only in testing mode) */ 
	extern MDIO_strPortRegElement_t MOCK_PORTA;
	extern MDIO_strPortRegElement_t MOCK_PORTB;
	extern MDIO_strPortRegElement_t MOCK_PORTC;
	extern MDIO_strPortRegElement_t MOCK_PORTD;
#else
	#include "std_types.h"
	#include "MDIO_Registers.h"
#endif
#include "MDIO_PBCFG.h"
#include "MDIO.h"

/* macros for checking function arguments */
#define IS_INVALID_PORT_NUM(X)			((((MDIO_enuPortNum_t)X) > MDIO_PORTD) || (((MDIO_enuPortNum_t)X) < MDIO_PORTA))
#define IS_INVALID_PIN_NUM(X)			((((MDIO_enuPinNum_t)X) > MDIO_PIN7) || (((MDIO_enuPinNum_t)X) < MDIO_PIN0))
#define IS_INVALID_PIN_CONFIG(X)		((((MDIO_enuPinConfig_t)X) != MDIO_PIN_OUTPUT) && (((MDIO_enuPinConfig_t)X) != MDIO_PIN_INPUT_PULLUP) && (((MDIO_enuPinConfig_t)X) != MDIO_PIN_INPUT))
#define IS_INVALID_PORT_CONFIG(X)		(((((MDIO_enuPortConfig_t)X)) != MDIO_PORT_OUTPUT) && (((MDIO_enuPortConfig_t)X) != MDIO_PORT_INPUT))
#define IS_INVALID_PIN_STATE(X)			((((MDIO_enuPinState_t)X) != MDIO_PIN_HIGH) && (((MDIO_enuPinState_t)X) != MDIO_PIN_LOW))
#define IS_INVALID_PORT_STATE(X)		((((MDIO_enuPortState_t)X) > MDIO_PORT_HIGH) || (((MDIO_enuPortState_t)X) < MDIO_PORT_LOW))
#define IS_INVALID_PTR(X)				((X) == NULL)

/*
 * @brief	Set logic applied to pin to HIGH or LOW                             
 *                                                                                       
 * @param (in) Copy_enuPortNum 	-> 	MDIO_PORTA, MDIO_PORTB, MDIO_PORTC, MDIO_PORTD  
 * @param (in) Copy_enuPinNum  	-> 	MDIO_PIN0, MDIO_PIN1, MDIO_PIN2, MDIO_PIN3		
 *									MDIO_PIN4, MDIO_PIN5, MDIO_PIN6, MDIO_PIN7		
 * @param (in) Copy_enuPinState	-> 	MDIO_PIN_HIGH, MDIO_PIN_LOW		

 * @return  MDIO_OK, MDIO_INVALID_PARAM, MDIO_INVALID_PIN, MDIO_INVALID_PORT    
 */
MDIO_enuErrorStatus_t MDIO_enuSetPinValue(MDIO_enuPortNum_t Copy_enuPortNum, MDIO_enuPinNum_t Copy_enuPinNum, MDIO_enuPinState_t Copy_enuPinState)
{
	/* defining a variable to store return status of function */
	MDIO_enuErrorStatus_t ret_enumStatus = MDIO_OK;

	if (IS_INVALID_PORT_NUM(Copy_enuPortNum))
	{
		/* do not continue if an invalid port number was passed */
		ret_enumStatus = MDIO_INVALID_PORT;
	}
	else if (IS_INVALID_PIN_NUM(Copy_enuPinNum)) 
	{
		/* do not continue if an invalid pin number was passed */
		ret_enumStatus = MDIO_INVALID_PIN;
	}
	else if (IS_INVALID_PIN_STATE(Copy_enuPinState))
	{	
		/* do not continue if an invalid pin state was passed */
		ret_enumStatus = MDIO_INVALID_PARAM;
	}
	else /* all arguments are valid */
	{
		/* get port base register based on passed port number */
		MDIO_strPortRegElement_t* Local_strPtrCurrPort = MDIO_GET_PORT_ADD(Copy_enuPortNum);

		if (Copy_enuPinState == MDIO_PIN_HIGH)
		{
			/* set bit corresponding to pin number if state is HIGH */
			SET_BIT(Local_strPtrCurrPort->PORT, Copy_enuPinNum);
		}
		else if (Copy_enuPinState == MDIO_PIN_LOW)
		{
			/* clear bit corresponding to pin number if state is LOW */
			CLR_BIT(Local_strPtrCurrPort->PORT, Copy_enuPinNum);
		}
		else {} /* written and left empty in compliance with MISRA C */
	}

	return ret_enumStatus;
}
             
                                                                   
/*
 * @brief	Set logic applied to all of port's pins to HIGH or LOW
 *                   
 * @param (in) Copy_enuPortNum 	-> 	MDIO_PORTA, MDIO_PORTB, MDIO_PORTC, MDIO_PORTD 
 * @param (in) Copy_enuPortState ->	MDIO_PORT_LOW, MDIO_PORT_HIGH
 *				
 * @return  MDIO_OK, MDIO_INVALID_PARAM, MDIO_INVALID_PIN, MDIO_INVALID_PORT   
 */
MDIO_enuErrorStatus_t MDIO_enuSetPortValue(MDIO_enuPortNum_t Copy_enuPortNum, MDIO_enuPortState_t Copy_enuPortState)
{
	/* defining a variable to store return status of function */
	MDIO_enuErrorStatus_t ret_enumStatus = MDIO_OK;

	
	if (IS_INVALID_PORT_NUM(Copy_enuPortNum))
	{
		/* do not continue if an invalid port number was passed */
		ret_enumStatus = MDIO_INVALID_PORT;
	}
	else if (IS_INVALID_PORT_STATE(Copy_enuPortState))
	{
		/* do not continue if an invalid port state was passed */
		ret_enumStatus = MDIO_INVALID_PARAM;
	}
	else /* all arguments are valid */
	{
		/* get port base register based on passed port number */
		MDIO_strPortRegElement_t* Local_strPtrCurrPort = MDIO_GET_PORT_ADD(Copy_enuPortNum);

		/* set all port pins by the passed value */
		(Local_strPtrCurrPort->PORT) = Copy_enuPortState;
	}

	return ret_enumStatus;
}

													
/*
 * @brief	Reads logic state applied to a pin                             
 *                                                                                       
 * @param (in) Copy_enuPortNum -> 	MDIO_PORTA, MDIO_PORTB, MDIO_PORTC, MDIO_PORTD  
 * @param (in) Copy_enuPinNum -> 	MDIO_PIN0, MDIO_PIN1, MDIO_PIN2, MDIO_PIN3		
 *									MDIO_PIN4, MDIO_PIN5, MDIO_PIN6, MDIO_PIN7		
 * @param (out) Add_uint8PtrPinValue -> 	Ptr to return logic state				
 *											MDIO_PIN_HIGH, MDIO_PIN_LOW		
 *		 		
 * @return  MDIO_OK, MDIO_INVALID_PARAM, MDIO_INVALID_PIN, MDIO_INVALID_PORT, MDIO_NULL_PTR    
 */
MDIO_enuErrorStatus_t MDIO_enuGetPinValue(MDIO_enuPortNum_t Copy_enuPortNum, MDIO_enuPinNum_t Copy_enuPinNum, uint8_t* Add_uint8PtrPinValue)
{
	/* defining a variable to store return status of function */
	MDIO_enuErrorStatus_t ret_enumStatus = MDIO_OK;

	if (IS_INVALID_PORT_NUM(Copy_enuPortNum))
	{
		/* do not continue if an invalid port number was passed */
		ret_enumStatus = MDIO_INVALID_PORT;
	}
	else if (IS_INVALID_PIN_NUM(Copy_enuPinNum))
	{
		/* do not continue if an invalid pin number was passed */
		ret_enumStatus = MDIO_INVALID_PIN;
	}
	else if (IS_INVALID_PTR(Add_uint8PtrPinValue))
	{
		/* do not continue if a null pointer was passed was passed */
		ret_enumStatus = MDIO_NULL_PTR;
	}
	else /* all arguments are valid */
	{
		/* get port base register based on passed port number */
		MDIO_strPortRegElement_t* Local_strPtrCurrPort = MDIO_GET_PORT_ADD(Copy_enuPortNum);

		/* get the value of bit corresponding to passed pin number and store it in the passed address */
		(*Add_uint8PtrPinValue) = GET_BIT(Local_strPtrCurrPort->PIN, Copy_enuPinNum);
	}

	return ret_enumStatus;
}
