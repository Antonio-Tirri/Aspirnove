/**	\file   usb.h
 *
 * 	\brief  Gestione  USB
 *
 * 	\author Walter Giusiano - lab3841 srl \n
 *  walter.giusiano@lab3841.it
 *
 *
 *  \version    v1.0 - 08/02/2017
 * 	-   Versione iniziale
 *
 * 	\section req Requisiti per il corretto funzionamento
 * 	-   Nessuno
 *
 */
#ifndef _USB_H_
#define _USB_H_

#include "stdint.h"

#define USB_RX_DATA_SIZE		4096
#define USB_TX_DATA_SIZE		256



/** Inizializza modulo USB CDC */
int USB_Init(void);

/** Invia pacchetto su USB CDC 
	\param *data = puntatore al buffer di dati
	\param len = numero di byte da inviare
	\return Byte accodati nel buffer di trasmissione
*/
unsigned short USB_Send(uint8_t *data, uint8_t len);


/** Pacchetto ricevuto su USB CDC. Da richiamare nell'endpoint di ricezione USB
	\param *data = puntatore al buffer di dati ricevuto
	\param len = numero di byte ricevuti
	\return Byte accodati nel buffer di ricezione
*/
unsigned short USB_OnRx(uint8_t *data, uint8_t len);

/** Legge primo byte ancora da leggere dal buffer di ricezione */
int USB_RecvChar(unsigned char *c);
	

/** Scarica dati ricevuti prelevandoli dal buffer di ricezione dell'USB  
	\param *data = puntatore al buffer dove caricare i dati
	\param len = numero massimo di byte da scaricare
	\return Byte scaricati
*/
unsigned short USB_Recv(uint8_t *data, uint8_t len);


/** Disabilita definitivamente l'USB.
*/
void USB_Disable(void);

#endif 











