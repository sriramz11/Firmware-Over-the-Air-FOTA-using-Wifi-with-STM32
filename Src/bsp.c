/*
 * File : bsp.c
 * Author : Prudhvi Raj Belide
 * Description :This file configures GPIOA pin PA5 as an output for controlling an LED and GPIOC pin PC13 as an input
 * for reading the state of a push button, implementing basic LED control and button state detection.
 */

#include "bsp.h"
#include "stm32f4xx.h"


#define GPIOAEN		(1U<<0)
#define GPIOCEN		(1U<<2)

#define PIN5		(1U<<5)
#define LED_PIN		PIN5
#define PIN13		(1U<<13)
#define BTN_PIN		PIN13

void led_init(void)
{
	/*Enable clock access to GPIOA*/
	RCC->AHB1ENR |= GPIOAEN;

	/*Set PA5 mode to output mode*/
	GPIOA->MODER |=(1U<<10);
	GPIOA->MODER &=~(1U<<11);
}


void led_on(void)
{
	/*Set PA5 high*/
	GPIOA->ODR |=LED_PIN;
}

void led_off(void)
{
	/*Set PA5 low*/
	GPIOA->ODR &=~LED_PIN;

}

void button_init(void)
{
	/*Enable clock access to PORTC*/
	RCC->AHB1ENR |=GPIOCEN;

	/*Set PC13 as an input pin*/
	GPIOC->MODER &=~(1U<<26);
	GPIOC->MODER &=~(1U<<27);

}


bool get_btn_state(void)
{

	/*Note : BTN is active low*/

	/*Check if button is pressed*/
	if(GPIOC->IDR & BTN_PIN)
	{
		return false;
	}
	else
	{
		return true;
	}

}
