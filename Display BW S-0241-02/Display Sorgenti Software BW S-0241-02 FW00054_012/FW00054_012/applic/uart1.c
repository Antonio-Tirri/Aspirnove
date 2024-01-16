#include "uart1.h"
#include "usart.h"
#include "modbusMaster.h"
#include <string.h>


#include "stm32f2xx_hal.h"


// Considera pacchetto modbus ricevuto dopo MODBUS_TIMEOUT_RX ms dall'ultimo byte ricevuto
#define MODBUS_TIMEOUT_RX			5



/* Buffer di ricezione */
char uart1_rx_buffer[UART1_RX_BUFFER_SIZE];

/* Indici buffer di ricezione */
int uart1_rx_len, uart1_rx_w, uart1_rx_r;


/* Buffer di trasmissione */
static uint8_t uart1_tx_buffer[UART1_TX_BUFFER_SIZE];

/* Indici buffer di trasmissione */
static uint16_t uart1_tx_r, uart1_tx_w, uart1_tx_len;

/* Flag inizializzazione interrupt TX */
static uint8_t uart1_tx_restart;

static uint8_t modbusContTimeoutRx;


int Uart1_Init(void) 
{
	USART1->CR1 |= USART_CR1_TCIE | USART_CR1_RXNEIE;
	uart1_tx_restart = 1;

	// Inizializza variabili
	memset(uart1_tx_buffer, 0, UART1_TX_BUFFER_SIZE);
	uart1_tx_r = 0;
	uart1_tx_w = 0;
	uart1_tx_len = 0;
	memset(uart1_rx_buffer, 0, UART1_TX_BUFFER_SIZE);
	uart1_rx_r = 0;
	uart1_rx_w = 0;
	uart1_rx_len = 0;
	
	modbusContTimeoutRx = 0;
	
	Uart1_Send(" ", 1);
	return 0;
	

}

void Uart1_IRQHandler (void)
{
	uint8_t ch;
	unsigned long IIR;

	IIR = USART1->SR;

	

	/* Error */
	if (IIR & (USART_SR_PE | USART_SR_FE | USART_SR_NE | USART_SR_ORE)) 
	{
		USART1->SR &= ~(USART_SR_PE | USART_SR_FE | USART_SR_NE | USART_SR_ORE);	        /* Clear interrupt */
	}



	/* RX */
	if (IIR & USART_SR_RXNE)
	{                  				
		USART1->SR &= ~USART_SR_RXNE;	        /* Clear interrupt */
		ch = (USART1->DR & 0xFF);

		if (uart1_rx_len < UART1_RX_BUFFER_SIZE)
		{

			modbusContTimeoutRx = MODBUS_TIMEOUT_RX;
			uart1_rx_len++;
			uart1_rx_buffer[uart1_rx_w] = ch;

			if (++uart1_rx_w >= UART1_RX_BUFFER_SIZE)
				uart1_rx_w = 0;
		}	
	}


	/* TX */
	if (IIR & USART_SR_TXE)
	{
		USART1->SR &= ~USART_SR_TXE;	          	/* Clear interrupt */

		if (uart1_tx_len > 0)
		{
			USART1->DR = uart1_tx_buffer[uart1_tx_r];
			
			if(++uart1_tx_r >= UART1_TX_BUFFER_SIZE)
				uart1_tx_r = 0;
			
			uart1_tx_len--;
			uart1_tx_restart = 0;
		}
		else 
		{
			RS485_TXEN_Off();
			uart1_tx_restart = 1;
			USART1->CR1 &= ~USART_CR1_TCIE;		/* Disable TX interrupt if nothing to send */
		}
	  
	}	

}



int Uart1_SendChar(uint8_t ch)
{
	
	USART1->CR1 &= ~(USART_CR1_TCIE | USART_CR1_RXNEIE); 	/* Disable TX/RX interrupt */ 	
	if (uart1_tx_len < UART1_TX_BUFFER_SIZE)
	{
		
		/* Buffer empty? */
		if ((uart1_tx_len == 0) && (uart1_tx_restart) )
		{
			USART1->DR = ch;
			uart1_tx_restart = 0;
		}
		else
		{			
			uart1_tx_len++;
			uart1_tx_buffer[uart1_tx_w] = ch;

			if (++uart1_tx_w >= UART1_TX_BUFFER_SIZE)
				uart1_tx_w = 0;		
		}
		USART1->CR1 |= USART_CR1_TCIE | USART_CR1_RXNEIE; 	/* Enable TX/RX interrupt */ 	
		
		return 1;
	}
	
	USART1->CR1 |= USART_CR1_TCIE | USART_CR1_RXNEIE; 	/* Enable TX/RX interrupt */ 	
	return 0;
}


int Uart1_Send(void *block, uint8_t len)
{
	uint8_t *src = (uint8_t *)block;
	uint16_t sent = 0;

	for(sent=0; sent<len; sent++)
	{
		if (Uart1_SendChar(*src++) == 0)
			return sent;
	}

	return sent;
}


int Uart1_RecvChar(unsigned char *c)
{
	USART1->CR1 &= ~USART_CR1_RXNEIE; 	/* Disable RX interrupt */ 	
    if(uart1_rx_len)
    {
        uart1_rx_len--;
        *c = uart1_rx_buffer[uart1_rx_r];
        if (++uart1_rx_r >= UART1_RX_BUFFER_SIZE)
                uart1_rx_r = 0;

		USART1->CR1 |= USART_CR1_RXNEIE; 	/* Enable RX interrupt */ 	
        return 1;
    }
    else
	{
		USART1->CR1 |= USART_CR1_RXNEIE; 	/* Enable RX interrupt */ 	
        return 0;
	}
}

int Uart1_Recv(void *block, int len)
{
    unsigned char *src = (unsigned char *)block;
    int recv = 0;

    for(recv=0;recv<len;recv++)
    {
		if(Uart1_RecvChar(src++) == 0)
			return recv;
    }

    return recv;
}


void Uart1_RTOManagement(void)
{
	// Se sono passati MODBUS_TIMEOUT_RX ms dall'ultimo byte ricevuto
	if (modbusContTimeoutRx > 0)
	{
		if (--modbusContTimeoutRx == 0)
		{
			// Gestione pacchetto ricevuto
			ModbusMaster_OnPacketReceive(uart1_rx_buffer, uart1_rx_len);

			// Azzera buffer e indici di ricezione
			memset(uart1_rx_buffer, 0, UART1_TX_BUFFER_SIZE);
			uart1_rx_r = 0;
			uart1_rx_w = 0;
			uart1_rx_len = 0;
		}

	}
}
