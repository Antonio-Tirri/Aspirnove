#include "uart3.h"
#include "usart.h"
#include <string.h>


#include "stm32f2xx_hal.h"


/* Buffer di ricezione */
char uart3_rx_buffer[UART3_RX_BUFFER_SIZE];

/* Indici buffer di ricezione */
int uart3_rx_len, uart3_rx_w, uart3_rx_r;


/* Buffer di trasmissione */
static uint8_t uart3_tx_buffer[UART3_TX_BUFFER_SIZE];

/* Indici buffer di trasmissione */
static uint16_t uart3_tx_r, uart3_tx_w, uart3_tx_len;

/* Flag inizializzazione interrupt TX */
static uint8_t uart3_tx_restart;




int Uart3_Init(void) 
{
	USART3->CR1 |= USART_CR1_TCIE | USART_CR1_RXNEIE;
	uart3_tx_restart = 1;

	// Inizializza variabili
	memset(uart3_tx_buffer, 0, UART3_TX_BUFFER_SIZE);
	uart3_tx_r = 0;
	uart3_tx_w = 0;
	uart3_tx_len = 0;
	memset(uart3_rx_buffer, 0, UART3_TX_BUFFER_SIZE);
	uart3_rx_r = 0;
	uart3_rx_w = 0;
	uart3_rx_len = 0;	
	
	
	Uart3_Send(" ", 1);
	return 0;
	
}

void Uart3_IRQHandler (void)
{
	uint8_t ch;
	unsigned long IIR;

	IIR = USART3->SR;

	
	/* Error */
	if (IIR & (USART_SR_PE | USART_SR_FE | USART_SR_NE | USART_SR_ORE)) 
	{
		USART3->SR &= ~(USART_SR_PE | USART_SR_FE | USART_SR_NE | USART_SR_ORE);	        /* Clear interrupt */
	}

	/* RX */
	if (IIR & USART_SR_RXNE) 
	{                  				
		USART3->SR &= ~USART_SR_RXNE;	        /* Clear interrupt */
		ch = (USART3->DR & 0xFF);
		
		if (uart3_rx_len < UART3_RX_BUFFER_SIZE)
		{
			uart3_rx_len++;
			uart3_rx_buffer[uart3_rx_w] = ch;

			if (++uart3_rx_w >= UART3_RX_BUFFER_SIZE)
				uart3_rx_w = 0;
		}	
	}
	

	/* TX */
	if (IIR & USART_SR_TXE) 
	{
		USART3->SR &= ~USART_SR_TXE;	          	/* Clear interrupt */

		if (uart3_tx_len > 0)
		{
			USART3->DR = uart3_tx_buffer[uart3_tx_r];
			
			if(++uart3_tx_r >= UART3_TX_BUFFER_SIZE)
				uart3_tx_r = 0;
			
			uart3_tx_len--;
			uart3_tx_restart = 0;
		}
		else 
		{
			uart3_tx_restart = 1;
			USART3->CR1 &= ~USART_CR1_TCIE;		/* Disable TX interrupt if nothing to send */
		}
	  
	}	

}



int Uart3_SendChar(uint8_t ch)
{
	
	USART3->CR1 &= ~(USART_CR1_TCIE | USART_CR1_RXNEIE); 	/* Disable TX/RX interrupt */ 	
	if (uart3_tx_len < UART3_TX_BUFFER_SIZE)
	{
		
		/* Buffer empty? */
		if ((uart3_tx_len == 0) && (uart3_tx_restart) )
		{
			USART3->DR = ch; 
			uart3_tx_restart = 0;
		}
		else
		{			
			uart3_tx_len++;
			uart3_tx_buffer[uart3_tx_w] = ch;

			if (++uart3_tx_w >= UART3_TX_BUFFER_SIZE)
				uart3_tx_w = 0;		
		}
		USART3->CR1 |= USART_CR1_TCIE | USART_CR1_RXNEIE; 	/* Enable TX/RX interrupt */ 	
		
		return 1;
	}
	
	USART3->CR1 |= USART_CR1_TCIE | USART_CR1_RXNEIE; 	/* Enable TX/RX interrupt */ 	
	return 0;
}


int Uart3_Send(void *block, uint8_t len)
{
	uint8_t *src = (uint8_t *)block;
	uint16_t sent = 0;

	for(sent=0; sent<len; sent++)
	{
		if (Uart3_SendChar(*src++) == 0)
			return sent;
	}

	return sent;
}


int Uart3_RecvChar(unsigned char *c)
{
	USART3->CR1 &= ~USART_CR1_RXNEIE; 	/* Disable RX interrupt */ 	
    if(uart3_rx_len)
    {
        uart3_rx_len--;
        *c = uart3_rx_buffer[uart3_rx_r];
        if (++uart3_rx_r >= UART3_RX_BUFFER_SIZE)
                uart3_rx_r = 0;

		USART3->CR1 |= USART_CR1_RXNEIE; 	/* Enable RX interrupt */ 	
        return 1;
    }
    else
	{
		USART3->CR1 |= USART_CR1_RXNEIE; 	/* Enable RX interrupt */ 	
        return 0;
	}
}

int Uart3_Recv(void *block, int len)
{
    unsigned char *src = (unsigned char *)block;
    int recv = 0;

    for(recv=0;recv<len;recv++)
    {
		if(Uart3_RecvChar(src++) == 0)
			return recv;
    }

    return recv;
}
