/**	\file   display.h
 *
 *	\brief  Modulo gestione display. Interfaccia con le funzioni GLCD.
 *
 * 	\author Walter Giusiano - lab3841 \n
 *  	walter.giusiano@lab3841.com 
 *
 *
 *  \version    v1.0 - 05/05/2015
 * 	-   Versione iniziale

 *
 */
#ifndef _DISPLAY_H_
#define _DISPLAY_H_

#include "main.h"
// Direzione dati (input / output)
#define DISPLAY_DATA_DIR_IN		(0)
#define DISPLAY_DATA_DIR_OUT	(1)



#define PORT_DISPLAY_DATA		LCD_D0_GPIO_Port

#define CS_On()					HAL_GPIO_WritePin(LCD_CS_GPIO_Port, 	LCD_CS_Pin, 	GPIO_PIN_SET)
#define CS_Off()				HAL_GPIO_WritePin(LCD_CS_GPIO_Port, 	LCD_CS_Pin, 	GPIO_PIN_RESET)
#define RESET_On()				HAL_GPIO_WritePin(LCD_CLR_GPIO_Port, 	LCD_CLR_Pin, 	GPIO_PIN_SET)
#define RESET_Off()				HAL_GPIO_WritePin(LCD_CLR_GPIO_Port, 	LCD_CLR_Pin, 	GPIO_PIN_RESET)
#define RS_On()					HAL_GPIO_WritePin(LCD_RS_GPIO_Port, 	LCD_RS_Pin, 	GPIO_PIN_SET)
#define RS_Off()				HAL_GPIO_WritePin(LCD_RS_GPIO_Port, 	LCD_RS_Pin, 	GPIO_PIN_RESET)
#define RW_On()					HAL_GPIO_WritePin(LCD_RW_GPIO_Port, 	LCD_RW_Pin, 	GPIO_PIN_SET)
#define RW_Off()				HAL_GPIO_WritePin(LCD_RW_GPIO_Port, 	LCD_RW_Pin, 	GPIO_PIN_RESET)
#define EN_On()					HAL_GPIO_WritePin(LCD_EN_GPIO_Port, 	LCD_EN_Pin, 	GPIO_PIN_SET)
#define EN_Off()				HAL_GPIO_WritePin(LCD_EN_GPIO_Port, 	LCD_EN_Pin, 	GPIO_PIN_RESET)




/** Inizializza display */
unsigned char Display_Init(void);
	

void LCD_DATA_WRITE(unsigned char data);
void LCD_DATA_DIRECTION(unsigned char dir);  
char LCD_DATA_READ(void);  



#endif 
