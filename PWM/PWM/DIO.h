#ifndef DIO_H
#define DIO_H


#define MDIO_HIGH 1
#define MDIO_LOW  0

#define PIN_INPUT  0
#define PIN_OUTPUT 1

/* Port and Pin Enums */
typedef enum {
    MDIO_PORTA = 0,
    MDIO_PORTB,
    MDIO_PORTC,
    MDIO_PORTD
} MDIO_enuPortNum_t;

typedef enum {
    MDIO_PIN0 = 0,
    MDIO_PIN1,
    MDIO_PIN2,
    MDIO_PIN3,
    MDIO_PIN4,
    MDIO_PIN5,
    MDIO_PIN6,
    MDIO_PIN7
} MDIO_enuPinNum_t;


typedef enum {
    MDIO_OK,              // Operation successful
    MDIO_NOK,             // Operation failed
    MDIO_INVALID_PIN,     // Invalid pin number
    MDIO_INVALID_PORT     // Invalid port number
} MDIO_enuErrorStatus_t;

/* Pin and Port Configuration Enums */
typedef enum
{
    MDIO_INPUT,
    MDIO_OUTPUT,
    MDIO_INPUT_PULLUP
} MDIO_enuPinConfiguration_t;

typedef enum
{
    MDIO_PORT_INPUT = 0x00,
    MDIO_PORT_OUTPUT = 0xFF,
    MDIO_PORT_HIGH_NIBBLE_INPUP_PULL_UP_LOW_NIBBLE_OUTPUT = 0x0F,
    MDIO_PORT_LOW_NIBBLE_INPUT_PULL_UP_HIGH_NIBBLE_OUTPUT = 0xF0
    
} MDIO_enuPortConfiguration_t;

// Function to set a pin value
MDIO_enuErrorStatus_t MDIO_enuSetPinValue(MDIO_enuPortNum_t Copy_enuPortNum, MDIO_enuPinNum_t Copy_enuPinNum, MDIO_enuPinConfiguration_t Copy_enuPinConfiguration);
// Function to configure a pin direction
MDIO_enuErrorStatus_t MDIO_enuSetPinConfigration(MDIO_enuPortNum_t Copy_enuPortNum, MDIO_enuPinNum_t Copy_enuPinNum, u8 Copy_u8PinDir);

// Function to set the value of a whole port
MDIO_enuErrorStatus_t MDIO_enuSetPortValue(MDIO_enuPortNum_t Copy_enuPortNum, u8 Copy_u8PortValue);

// Function to configure the direction of a whole port
MDIO_enuErrorStatus_t MDIO_enuSetPortDir(MDIO_enuPortNum_t Copy_enuPortNum, MDIO_enuPortConfiguration_t Copy_enuPortConfiguration);
// Function to get the value of a pin
MDIO_enuErrorStatus_t MDIO_enuGetPinValue(MDIO_enuPortNum_t Copy_enuPortNum, MDIO_enuPinNum_t Copy_enuPinNum, u8* Add_pu8PinValue);


#endif /* DIO_H */