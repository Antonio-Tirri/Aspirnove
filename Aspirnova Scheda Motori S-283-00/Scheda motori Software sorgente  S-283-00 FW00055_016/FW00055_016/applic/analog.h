/**	\file   analog.h
 *
 * 	\brief  Gestione canali AD
 *
 * 	\author Walter Giusiano - lab3841 srl \n
 *  walter.giusiano@lab3841.it
 *
 *
 *  \version    v1.0 - 22/03/2019
 * 	-   Versione iniziale
 *
 * 	\section req Requisiti per il corretto funzionamento
 * 	-   Nessuno
 *
 */
#ifndef _analog_h_
#define _analog_h_

#include <board.h>

// C++ Support
#ifdef __cplusplus
	extern "C" {
#endif




#define ADC_FULLSCALE				4095.0

#define ADC_DIGITAL_THRESHOLD		3722	// 3V

#define VREF						3.3



// ADC
enum
{
	AIN_CO2	= 0,
	AIN_AUX1,
	AIN_AUX2,
	AIN_TEMP_EXT,
	AIN_TEMP_INT,

	ADC_CHANNELS
};

#define NORMAL_VALUE					0
#define OVERSAMPLING_VALUE				1
#define OVERSAMPLING_FILTERED_VALUE		2

		
/** Inizializza modulo */
int Analog_Init(void);


/** Da richiamare per elaborare i dati convertiti. Da richiamare nel main prima di eseguire una nuova misura. */
int Analog_Management(void);

/** Lancia la conversione su tutti canali AD */
int Analog_Measure(void);

/** Restituisce TRUE se i valori filtrati sono disponibili (ci vogliono alcuni secondi dopo il reset) */
uint8_t Analog_FilteredValuePresent(void);

/** Restituisce valore canale AD convertito
	\param channel: numero canale AD
	\param type_value: NORMAL_VALUE - OVERSAMPLING_VALUE - OVERSAMPLING_FILTERED_VALUE
*/
uint16_t Analog_Get(uint8_t channel, uint8_t type_value);

		
/** Restituisce tensione canale, in V
	\param channel: numero canale AD
	\param type_value: NORMAL_VALUE - OVERSAMPLING_VALUE - OVERSAMPLING_FILTERED_VALUE
*/
float Analog_GetVolt(uint8_t channel, uint8_t type_value);


/** Gestione calibrazione canali */
void Analog_Calibration(void);



// C++ Support
#ifdef __cplusplus
	}
#endif

#endif 

