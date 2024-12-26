/*
 * File : esp82xx_driver.c
 * Author : Sriramkumar Jayaraman
 * Description : This file configures STM32F4's UART1 and UART2 for ESP82XX communication and debugging, including GPIO setup,
 * baud rate calculation, and RS pin control.
 */

#include <esp82xx_driver.h>
#include <stdint.h>

#define GPIOAEN		(1U<<0)
#define UART2EN		(1U<<17)
#define UART1EN		(1U<<4)

#define DBG_UART_BAUDRATE		115200
#define SYS_FREQ				16000000
#define APB1_CLK				SYS_FREQ
#define APB2_CLK				SYS_FREQ

#define CR1_TE					(1U<<3)
#define CR1_RE					(1U<<2)

#define CR1_UE					(1U<<13)
#define SR_TXE					(1U<<7)


static uint16_t compute_uart_bd(uint32_t periph_clk,uint32_t baudrate);

static void uart_set_baudrate(uint32_t periph_clk,uint32_t baudrate);
static void uart_write(int ch);

int __io_putchar(int ch)
{
	uart_write(ch);
	return ch;
}

void debug_uart_init(void)
{
	/*Enable clock access to GPIOA*/
	RCC->AHB1ENR |= GPIOAEN;

	/*Set the mode of PA2 to alternate function mode*/
	GPIOA->MODER &=~(1U<<4);
	GPIOA->MODER |=(1U<<5);

	/*Set the mode of PA3 to alternate function mode*/
	GPIOA->MODER &=~(1U<<6);
	GPIOA->MODER |=(1U<<7);

	/*Set alternate function type to AF7(UART2_TX)*/
	GPIOA->AFR[0] |=(1U<<8);
	GPIOA->AFR[0] |=(1U<<9);
	GPIOA->AFR[0] |=(1U<<10);
	GPIOA->AFR[0] &=~(1U<<11);


	/*Set alternate function type to AF7(UART2_RX)*/
	GPIOA->AFR[0] |=(1U<<12);
	GPIOA->AFR[0] |=(1U<<13);
	GPIOA->AFR[0] |=(1U<<14);
	GPIOA->AFR[0] &=~(1U<<15);

	/*Enable clock access to UART2*/
     RCC->APB1ENR |=	UART2EN;


 	/*Disable UART Module*/
      USART2->CR1 &= ~CR1_UE;

	/*Configure uart baudrate*/
     uart_set_baudrate(APB1_CLK,DBG_UART_BAUDRATE);

	/*Configure transfer direction*/
     USART2->CR1 = CR1_TE | CR1_RE;

     //NOTE:  Enable Tranmit Interrupt and Receive interrupt
     //In a separate function

     /*Enable interrupt in NVIC*/
    NVIC_EnableIRQ(USART2_IRQn);

	/*Enable UART Module*/
     USART2->CR1 |= CR1_UE;
}




/*Pinout :
 * UART Module     :  	UART1
 * UART Pins   	   : 	PA9 = TX, PA10 = RX
 * esp pin		    	stm32f4 pin
 * --------------------------
 * ESP82XX RS  Pin : 	PA8
 * ESP82XX EN  Pin :	3.3V
 * ESP82XX IO1 Pin :	3.3V
 * ESP82XX IO2 Pin :	3.3V
 * ESP82XX VCC Pin :	3.3V
 * ESP82XX GND Pin :	GND
 * ESP82XX TX Pin  :	PA10(RX)
 * ESP82XX RX Pin  :	PA9(TX)
 * */


void esp_rs_pin_init(void)
{

	/*Enable clock access to gpioa*/
	RCC->AHB1ENR |= GPIOAEN;


	/*Set PA8 as output pin*/
	GPIOA->MODER |=(1U<<16);
	GPIOA->MODER &=~(1U<<17);

}

void esp_rs_pin_enable(void)
{
	/*Set Pa8 to high*/
	GPIOA->ODR |=(1U<<8);
}
void esp_uart_init(void)
{
	/*Enable clock access to GPIOA*/
	RCC->AHB1ENR |= GPIOAEN;

	/*Set the mode of PA9 to alternate function mode*/
	GPIOA->MODER &=~(1U<<18);
	GPIOA->MODER |=(1U<<19);

	/*Set the mode of PA10 to alternate function mode*/
	GPIOA->MODER &=~(1U<<20);
	GPIOA->MODER |=(1U<<21);

	/*Set alternate function type to AF7(UART2_TX)*/
	GPIOA->AFR[1] |=(1U<<4);
	GPIOA->AFR[1] |=(1U<<5);
	GPIOA->AFR[1] |=(1U<<6);
	GPIOA->AFR[1] &=~(1U<<7);


	/*Set alternate function type to AF7(UART2_RX)*/
	GPIOA->AFR[1] |=(1U<<8);
	GPIOA->AFR[1] |=(1U<<9);
	GPIOA->AFR[1] |=(1U<<10);
	GPIOA->AFR[1] &=~(1U<<11);

	/*Enable clock access to UART1*/
     RCC->APB2ENR |=	UART1EN;


 	/*Disable UART Module*/
     USART1->CR1 &= ~CR1_UE;

	/*Configure uart baudrate*/
     USART1->BRR = compute_uart_bd(APB2_CLK,DBG_UART_BAUDRATE);

	/*Configure transfer direction*/
     USART1->CR1 = CR1_TE | CR1_RE;

     //NOTE:  Enable Tranmit Interrupt and Receive interrupt
     //In a separate function

     /*Enable interrupt in NVIC*/
    NVIC_EnableIRQ(USART1_IRQn);

	/*Enable UART Module*/
     USART1->CR1 |= CR1_UE;
}


static void uart_write(int ch)
{
	/*Make sure transmit data register is empty*/
	while(!(USART1->SR & SR_TXE)){}

	/*Write to transmit data register*/
	USART1->DR =(ch & 0xFF);
}
static uint16_t compute_uart_bd(uint32_t periph_clk,uint32_t baudrate)
{
	return((periph_clk + (baudrate/2U))/baudrate);
}

static void uart_set_baudrate(uint32_t periph_clk,uint32_t baudrate)
{
	USART2->BRR = compute_uart_bd(periph_clk,baudrate);
}
