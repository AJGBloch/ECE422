/*
 * Filename: main.c
 * Author: Anthony Bloch
 * 
 * Class: ECE422 - lab8 part 2 (UART)
 * Date: 11/5/2018
 * 
 * Brief: The following program drives an LED.
 * With the push of a button on the board, the LED will toggle on/off.
 * When 't' is entered on the computer's keyboard on the serial terminal, LED2 will toggle on/off.
 * When 's' is entered, the status (on/off) is reported on the terminal.
 * When 'b' is entered, the number of times the button or 't' have been pressed is reported on the terminal.
 * When 'c' is entered, the counter tracking the number of times the button or 't' has been pressed is reset to 0.
 * There is also a reset push button on the board that resets the system when pushed.
 * When there is UART activity, LED1 will turn on. Otherwise it is off.
 * /
/*
 * Training - Fundamentals of C Programming - by Microchip Developer Help
 * http://microchipdeveloper.com/tls2101:start
 * 
 * Tour of MPLAB X User Interface
 * http://microchipdeveloper.com/tls0101:parts-of-the-ide
 * 
 * Microchip Tutorial on creating a project
 * http://microchipdeveloper.com/tls0101:lab1
 * 
 */

#include <xc.h>
#include "config.h"
#include "gpio.h"
#include "uart.h"
#include "app.h"
#include "timer.h"

int main(void)
{
    gpio_init();
    uart_init();
    run_app();
}

/**********************************************************************
* © 2013 Microchip Technology Inc.
* SOFTWARE LICENSE AGREEMENT:
* Microchip Technology Inc. (Microchip) licenses this software to you
* solely for use with Microchip dsPIC digital signal controller
* products. The software is owned by Microchip and is protected under
* applicable copyright laws.  All rights reserved.
*
* SOFTWARE IS PROVIDED AS IS.  MICROCHIP EXPRESSLY DISCLAIMS ANY
* WARRANTY OF ANY KIND, WHETHER EXPRESS OR IMPLIED, INCLUDING BUT NOT
* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
* PARTICULAR PURPOSE, OR NON-INFRINGEMENT. IN NO EVENT SHALL MICROCHIP
* BE LIABLE FOR ANY INCIDENTAL, SPECIAL, INDIRECT OR CONSEQUENTIAL
* DAMAGES, LOST PROFITS OR LOST DATA, HARM TO YOUR EQUIPMENT, COST OF
* PROCUREMENT OF SUBSTITUTE GOODS, TECHNOLOGY OR SERVICES, ANY CLAIMS
* BY THIRD PARTIES (INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF),
* ANY CLAIMS FOR INDEMNITY OR CONTRIBUTION, OR OTHER SIMILAR COSTS.
**********************************************************************/