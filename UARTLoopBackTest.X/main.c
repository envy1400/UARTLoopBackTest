/**
  Generated main.c file from MPLAB Code Configurator

  @Company
    Microchip Technology Inc.

  @File Name
    main.c

  @Summary
    This is the generated main.c using PIC24 / dsPIC33 / PIC32MM MCUs.

  @Description
    This source file provides main entry point for system initialization and application code development.
    Generation Information :
        Product Revision  :  PIC24 / dsPIC33 / PIC32MM MCUs - 1.166.0
        Device            :  PIC24FJ256GA705
    The generated drivers are tested against the following:
        Compiler          :  XC16 v1.41
        MPLAB 	          :  MPLAB X v5.30
*/

/*
    (c) 2019 Microchip Technology Inc. and its subsidiaries. You may use this
    software and any derivatives exclusively with Microchip products.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION
    WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

    MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
    TERMS.
*/

/**
  Section: Included Files
*/
#include <string.h>
#include "mcc_generated_files/system.h"
#include "mcc_generated_files/pin_manager.h"
#include "mcc_generated_files/uart1.h"
#include "mcc_generated_files/uart2.h"

#define UART_1_RX_BUFF_SIZE 512
#define UART_2_RX_BUFF_SIZE 512

typedef enum{
    APP_INIT,
    APP_SEND_HELLO,
    APP_SEND_BYE,
    APP_IDLE,
} eAppState;

static eAppState appState = APP_INIT;

static uint8_t uart1RxBuffer[UART_1_RX_BUFF_SIZE], uart2RxBuffer[UART_2_RX_BUFF_SIZE];
static uint16_t iUart1RxBuffer, iUart2RxBuffer;

static const uint8_t helloString[] = {'h','e','l','l','o','!', '\0'};
static const uint8_t byeString[] = {'b','y','e','!', '\0'};
static uint8_t helloStringLength, byeStringLength, iHelloString, iByeString;

// Switch Debounce variables
static uint16_t switch1Debounce, switch2Debounce;

// Local Function Declarations 
static void Switch1Callback(void);
static void Switch2Callback(void);

/*
                         Main application
 */
int main(void)
{
    // initialize the device
    SYSTEM_Initialize();

    while (1)
    {
        // Add your application code
        if(UART1_IsRxReady())
        {
            uart1RxBuffer[iUart1RxBuffer++] = UART1_Read();                                        
            if(iUart1RxBuffer >= UART_1_RX_BUFF_SIZE)
                iUart1RxBuffer = 0;
        }
        
        if(UART2_IsRxReady())
        {
            uart2RxBuffer[iUart2RxBuffer++] = UART2_Read();                                        
            if(iUart2RxBuffer >= UART_2_RX_BUFF_SIZE)
                iUart2RxBuffer = 0;
        }
        
        switch(appState)
        {
            case APP_INIT:
            {
                // Initialize values
                helloStringLength = strlen((const char *)helloString);
                byeStringLength = strlen((const char *)byeString);
                iHelloString = iByeString = iUart1RxBuffer = iUart2RxBuffer = 0;
                switch1Debounce = switch2Debounce = 0;
                
                
                // Register button callback functions
                IO_RA11_SetInterruptHandler(Switch1Callback);                
                IO_RA12_SetInterruptHandler(Switch2Callback);
                
                appState = APP_IDLE;
                break;
            }
            case APP_SEND_HELLO:
            {
                // Send the hello string
                if(iHelloString < helloStringLength)
                {
                   if(UART1_IsTxReady())
                  {
                      UART1_Write(helloString[iHelloString]);
                      iHelloString++;
                  }
                }
                else
                {
                    // Done transmitting. Go to app idle.
                    iHelloString = 0;
                    appState = APP_IDLE;
                }
                
                break;
            }
            case APP_SEND_BYE :
            {
                // Send the bye string
                if(iByeString < byeStringLength)
                {
                   if(UART2_IsTxReady())
                  {
                      UART2_Write(byeString[iByeString]);
                      iByeString++;
                  }
                }
                else
                {
                    // Done transmitting. Go to app idle.
                    iByeString = 0;
                    appState = APP_IDLE;
                }
                
                break;
            }
            case APP_IDLE:
            default:
                break;
        }
    }

    return 1;
}

static void Switch1Callback(void)
{
//    if(switch1Debounce == 0xFFFF )
//    {
//        switch1Debounce = 0;
        if(appState == APP_IDLE)
            appState = APP_SEND_HELLO;    
//    }    
//    else
//    {
//        switch1Debounce <<= 1;
//        switch1Debounce |= 0x01;
//    }
}

static void Switch2Callback(void)
{
//    if(switch2Debounce == 0xFFFF )
//    {
//        switch2Debounce = 0;
        if(appState == APP_IDLE)
            appState = APP_SEND_BYE;
//    }    
//    else
//    {
//        switch2Debounce <<= 1;
//        switch2Debounce |= 0x01;
//    }    
}

/**
 End of File
*/

