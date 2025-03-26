#ifndef MDIO_H
#define MDIO_H

/* enum for error status */
typedef enum
{
	MDIO_OK = 0,
	MDIO_INVALID_PARAM,
	MDIO_INVALID_PIN,
	MDIO_INVALID_PORT,
	MDIO_NULL_PTR,
	MDIO_NOK,
} MDIO_enuErrorStatus_t;

/* enum for pin numbers */
typedef enum
{
	MDIO_PIN0 = 0x00,
	MDIO_PIN1,
	MDIO_PIN2,
	MDIO_PIN3,
	MDIO_PIN4,
	MDIO_PIN5,
	MDIO_PIN6,
	MDIO_PIN7,
} MDIO_enuPinNum_t;

/* enum for port numbers */
typedef enum
{
	MDIO_PORTA = 0x00,
	MDIO_PORTB,
	MDIO_PORTC,
	MDIO_PORTD,
} MDIO_enuPortNum_t;

/* enum for pin configuration */
typedef enum
{
	MDIO_PIN_OUTPUT = 0x00,
	MDIO_PIN_INPUT_PULLUP,
	MDIO_PIN_INPUT,
} MDIO_enuPinConfig_t;

/* enum for port configuration */
typedef enum
{
	MDIO_PORT_OUTPUT = 0xFF,
	MDIO_PORT_INPUT = 0x00,
} MDIO_enuPortConfig_t;

/* enum for logic levels (pins) */
typedef enum
{
	MDIO_PIN_HIGH = 0x01,
	MDIO_PIN_LOW = 0x00,
} MDIO_enuPinState_t;

/* enum for logic levels (ports) */
typedef enum
{
	MDIO_PORT_LOW = 0x00,
	MDIO_PORT_HIGH = 0xFF,
} MDIO_enuPortState_t;


/*
 * @brief	Set logic applied to pin to HIGH or LOW                             
 *                                                                                       
 * @param (in) Copy_enuPortNum 	-> 	MDIO_PORTA, MDIO_PORTB, MDIO_PORTC, MDIO_PORTD  
 * @param (in) Copy_enuPinNum  	-> 	MDIO_PIN0, MDIO_PIN1, MDIO_PIN2, MDIO_PIN3		
 *									MDIO_PIN4, MDIO_PIN5, MDIO_PIN6, MDIO_PIN7		
 * @param (in) Copy_enuPinState	-> 	MDIO_PIN_HIGH, MDIO_PIN_LOW		

 * @return  MDIO_OK, MDIO_INVALID_PARAM, MDIO_INVALID_PIN, MDIO_INVALID_PORT    
 */
MDIO_enuErrorStatus_t MDIO_enuSetPinValue(MDIO_enuPortNum_t Copy_enuPortNum, MDIO_enuPinNum_t Copy_enuPinNum, MDIO_enuPinState_t Copy_enuPinState);


/*
 * @brief	Set logic applied to all of port's pins to HIGH or LOW
 *                   
 * @param (in) Copy_enuPortNum 	-> 	MDIO_PORTA, MDIO_PORTB, MDIO_PORTC, MDIO_PORTD 
 * @param (in) Copy_enuPortState ->	MDIO_PORT_LOW, MDIO_PORT_HIGH
 *				
 * @return  MDIO_OK, MDIO_INVALID_PARAM, MDIO_INVALID_PIN, MDIO_INVALID_PORT   
 */
MDIO_enuErrorStatus_t MDIO_enuSetPortValue(MDIO_enuPortNum_t Copy_enuPortNum, MDIO_enuPortState_t Copy_enuPortState);


/*
 * @brief	Reads logic state applied to a pin                             
 *                                                                                       
 * @param (in) Copy_enuPortNum -> 	MDIO_PORTA, MDIO_PORTB, MDIO_PORTC, MDIO_PORTD  
 * @param (in) Copy_enuPinNum -> 	MDIO_PIN0, MDIO_PIN1, MDIO_PIN2, MDIO_PIN3		
 *										MDIO_PIN4, MDIO_PIN5, MDIO_PIN6, MDIO_PIN7		
 * @param (out) Add_uint8PtrPinValue -> 	Ptr to return logic state				
 *										MDIO_PIN_HIGH, MDIO_PIN_LOW		
 *		 		
 * @return  MDIO_OK, MDIO_INVALID_PARAM, MDIO_INVALID_PIN, MDIO_INVALID_PORT, MDIO_NULL_PTR    
 */
MDIO_enuErrorStatus_t MDIO_enuGetPinValue(MDIO_enuPortNum_t Copy_enuPortNum, MDIO_enuPinNum_t Copy_enuPinNum, uint8_t* Add_uint8PtrPinValue);


#endif