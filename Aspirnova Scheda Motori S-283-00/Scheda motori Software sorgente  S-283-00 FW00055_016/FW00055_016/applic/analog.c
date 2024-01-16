#include "analog.h"
#include "application.h"
#include "board.h"
#include "dati.h"
#include "adc.h"
#include "temperature_sensor.h"

#include <string.h>


/** NOTE
 *
 * Esempio ciclo main:
 *
 * 		...
 *		// Gestione valori AD convertiti al ciclo precedente
 * 		Analog_Management();
 *
 *		// Lettura AD
 *		Analog_Measure();
 *		...
 *
 *		// Carico in a l'ultimo valore letto su AIN_CH1
 *		a = Analog_GetVolt(AIN_CH1, NORMAL_VALUE)
 *
 *		// Carico in b l'ultimo valore con oversampling calcolato per AIN_CH1
 *		b = Analog_GetVolt(AIN_CH1, OVERSAMPLING_VALUE)
 *
 *		// Carico i c la media degli ultimi FILTER_COUNT valori con oversampling calcolati per AIN_CH1
 *		c = Analog_GetVolt(AIN_CH1, OVERSAMPLING_FILTERED_VALUE)
 *
 *
 *
 *
 *	In ogni ciclo di main elaboro i valori letti nel ciclo precedente e lancio una nuova conversione
 *
 *	Misure nell'applicazione corrente:
 *		- Durata conversione dei 4 canali configurati: 250us
 *		- Ogni 10 ms (durata ciclo di main) elaboro i valori e lancio nuova conversione
 *		- Ogni OVERSAMPLING_SAMPLES cicli di main (160ms) calcolo il valore con oversampling
 *		- Ogni OVERSAMPLING_SAMPLES cicli di main (160ms) calcolo la media tra gli ultimi FILTER_COUNT valori con oversampling.
 *		(media mobile sugli ultimi FILTER_COUNT*160ms)
 *
*/



#define FILTER_COUNT				30

#define OVERSAMPLING_SAMPLES		16	// N (4) elevato al numero di bit che si vogliono aggiungere alla risoluzione
#define OVERSAMPLING_DIV			4	// M (2) elevato al numero di bit che si vogliono aggiungere alla risoluzione


uint8_t analogConfigured = FALSE;

uint16_t adc_value[ADC_CHANNELS];

uint16_t adc_filtered_buffer[ADC_CHANNELS][FILTER_COUNT];
uint16_t indexFilter = 0;
uint16_t adc_filtered_value[ADC_CHANNELS];
uint8_t adcFilteredPresent = FALSE;

uint32_t adc_oversampling_value[ADC_CHANNELS];
uint32_t adc_sum_oversampling_value[ADC_CHANNELS];
uint16_t indexOversampling = 0;



void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* h)
{

}

int Analog_Init(void)
{
	uint8_t w;

	indexFilter = 0;
	indexOversampling = 0;
	for (w=0; w<ADC_CHANNELS; w++)
	{
		for (int j=0; j<FILTER_COUNT; j++)
			adc_filtered_buffer[w][j] = 0;
		adc_oversampling_value[w] = 0;
		adc_sum_oversampling_value[w] = 0;
		adc_value[w] = 0;
		adc_filtered_value[w] = 0;
	}

	// Init ADC1-DMA
	HAL_ADC_Start(&hadc1);

	analogConfigured = TRUE;

	// Start measure
	Analog_Measure();

	return 0;
}



uint8_t Analog_FilteredValuePresent(void)
{
	return adcFilteredPresent;
}



uint16_t Analog_Get(uint8_t channel, uint8_t type_value)
{
	if (channel >= ADC_CHANNELS)
		return 0;

	if (type_value == OVERSAMPLING_VALUE)
		return adc_oversampling_value[channel];
	else if (type_value == OVERSAMPLING_FILTERED_VALUE)
		return adc_filtered_value[channel];
	else
		return adc_value[channel];
}


float Analog_GetVolt(uint8_t channel, uint8_t type_value)
{
	float tmpFloat;
	uint32_t fullScale;

	fullScale = ADC_FULLSCALE;
	if (type_value != NORMAL_VALUE)
		fullScale = (fullScale << 2);

	tmpFloat = ((float)Analog_Get(channel, type_value) * VREF) / fullScale;

	return tmpFloat;
}





int Analog_Management(void)
{
	uint8_t w;
	uint32_t tmpLong;

	// Canali configurati?
	if (analogConfigured == FALSE)
		return 1;

	// Somma valori letti al ciclo di main precedente
	for (w=0; w<ADC_CHANNELS; w++)
	{
		adc_sum_oversampling_value[w] += adc_value[w];
	}

	// Raggiunto il numero di campioni per il conteggio dell'oversampling?
	if (++indexOversampling >= OVERSAMPLING_SAMPLES)
	{
		indexOversampling = 0;

		for (w=0; w<ADC_CHANNELS; w++)
		{
			adc_oversampling_value[w] = adc_sum_oversampling_value[w] / OVERSAMPLING_DIV;
			adc_sum_oversampling_value[w] = 0;

			// Memorizza ultimo valore calcolato nel buffer circolare
			adc_filtered_buffer[w][indexFilter] = adc_oversampling_value[w];
		}

		// Aggiorna indice buffer circolare
		if (++indexFilter >= FILTER_COUNT)
		{
			indexFilter = 0;
			adcFilteredPresent = TRUE;
		}

		// Scorre i canali
		for (w=0; w<ADC_CHANNELS; w++)
		{
			// Somma tutti i valori presenti nel buffer
			tmpLong = 0;
			for (int j=0; j<FILTER_COUNT; j++)
			{
				tmpLong += adc_filtered_buffer[w][j];
			}

			// Per ogni canale ottengo la media tra gli ultimi N valori generati dall'oversampling
			adc_filtered_value[w] = tmpLong / FILTER_COUNT;
		}

	}

	return 0;
}


int Analog_Measure(void)
{
	uint8_t w;

	// Canali configurati?
	if (analogConfigured == FALSE)
		return 1;

	// Azzera ultimi valori letti
	for (w=0; w<ADC_CHANNELS; w++)
	{
		adc_value[w] = 0;
	}

	// Avvia conversione. (non bloccante. Al termine della conversione di tutti i canali verrà richiamata HAL_ADC_ConvCpltCallback)
	HAL_ADC_Start_DMA(&hadc1, (uint32_t *)adc_value, ADC_CHANNELS);

	return 0;
}




