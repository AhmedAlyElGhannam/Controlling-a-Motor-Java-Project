#include "control.h"

#define ACK_BYTE    0xFF
#define ID_OFFSET   5
#define DIR_OFFSET  4
#define CLOCKWISE   Forward
#define ANTICLOCKWISE   Backward

typedef enum 
{   
    FULL_DATA_BYTE = 0,
    BYTE_ID,
    SPEED_LVL,
    SPEED_DIR,
    NUM_OF_DATA_FIELDS,
} APP_enuDataFields_t;

volatile static uint8_t global_uint8IsFirstReceivedByte = true;
volatile static uint8_t global_uint8HasReceivedDataWithinWindow = false;
volatile static uint8_t global_uint8HasTimedOut = false;

volatile static uint8_t arr_uint8CurrDataFields[NUM_OF_DATA_FIELDS];
volatile static uint8_t arr_uint8PrevDataFields[NUM_OF_DATA_FIELDS];

void echoReceivedByte(u8 data)
{
    /* transfer received data into a variable */
    arr_uint8CurrDataFields[FULL_DATA_BYTE] =  UART_ReceiveByte();
    UART_SendByte(ACK_BYTE);    
    global_uint8HasReceivedDataWithinWindow = true;  
    HLED_uint8SetLEDValue(HLED_RECEPTION_SUCCESSFUL, HLED_ON);
}


void APP_voidExtractDataFromReceivedByte(void)
{
    arr_uint8CurrDataFields[BYTE_ID] = (arr_uint8CurrDataFields[FULL_DATA_BYTE] & 0xE0) >> ID_OFFSET;
    arr_uint8CurrDataFields[SPEED_DIR] = (arr_uint8CurrDataFields[FULL_DATA_BYTE] & 0x10) >> DIR_OFFSET;
    arr_uint8CurrDataFields[SPEED_LVL] = (arr_uint8CurrDataFields[FULL_DATA_BYTE] & 0x0F);
}

void APP_voidControlSystemInit(void)
{
    cli();

    // MPORT_voidInit();

    HLED_voidInit();

    Motor_Init();

    UART_Config_t uartConfig = 
    {
        .baudRate = UART_BAUD_9600,     /**< Standard 9600 baud rate */
        .parity = UART_PARITY_NONE,     /**< No parity checking */
        .stopBits = UART_STOP_BITS_1,   /**< 1 stop bit */
        .dataBits = UART_DATA_BITS_8,   /**< 8 data bits */
        .mode = UART_MODE_ASYNC,        /**< Asynchronous operation */
        .speed = UART_SPEED_NORMAL,     /**< Normal speed mode */
        .rxCallback = echoReceivedByte   /**< Receive callback function */
    };

    /* Initialize UART with specified configuration */
    UART_Init(&uartConfig);

	HSCHEDULER_voidInit();
}

void APP_voidCpyDataHistory(void)
{
    volatile uint8_t local_uint8Iter;

    for (local_uint8Iter = FULL_DATA_BYTE; local_uint8Iter < NUM_OF_DATA_FIELDS; local_uint8Iter++)
    {
        arr_uint8PrevDataFields[local_uint8Iter] = arr_uint8CurrDataFields[local_uint8Iter];
    }
}

void APP_voidScheduledControlFunc(void)
{
    if (global_uint8HasReceivedDataWithinWindow)
    {
        global_uint8HasReceivedDataWithinWindow = false;

        if (global_uint8HasTimedOut)
        {
            HLED_uint8SetLEDValue(HLED_TIMEOUT, HLED_OFF);
            global_uint8HasTimedOut = false;
        } 
        else {}

        APP_voidExtractDataFromReceivedByte();

        if (global_uint8IsFirstReceivedByte) // first received byte does not require ID check
        {
            global_uint8IsFirstReceivedByte = false;

            APP_voidCpyDataHistory();

            /* set dir */
            Motor_SetDirection(arr_uint8CurrDataFields[SPEED_DIR]);
        }
        else  
        {
            /* check if newly received byte ID is different */
            if (arr_uint8CurrDataFields[BYTE_ID] == arr_uint8PrevDataFields[BYTE_ID])
            {
                /* light up an LED to indicate an invalid ID event */
                HLED_uint8SetLEDValue(HLED_INVALID_ID, HLED_ON);

            }
            else /* ID is unique */
            {
                APP_voidCpyDataHistory();
                HLED_uint8SetLEDValue(HLED_INVALID_ID, HLED_OFF);
            }
        }

        /* check for speed reversal for graceful stop then start in opposite dir */
        if (arr_uint8CurrDataFields[SPEED_DIR] == !arr_uint8PrevDataFields[SPEED_DIR])
        {
            /* stop motor fully */
            Motor_Brake();

            /* set dir */
            Motor_SetDirection(arr_uint8CurrDataFields[SPEED_DIR]);

            /* light up LED for this action */
            HLED_uint8SetLEDValue(HLED_SPEED_REVERSE, HLED_ON);
        }
        else 
        {
            /* no need to reset speed */
            HLED_uint8SetLEDValue(HLED_SPEED_REVERSE, HLED_OFF);
        } 

        /* set motor speed */
        Motor_SetSpeed(arr_uint8CurrDataFields[SPEED_LVL]);

        /* light up LED for this action to indicate success */
        HLED_uint8SetLEDValue(HLED_SUCCESSFUL_TRANSACTION, HLED_ON);
    }
    else 
    {
        /* light up an LED to indicate timeout event */
        HLED_uint8SetLEDValue(HLED_TIMEOUT, HLED_ON);

        global_uint8HasTimedOut = true;
    }
}
