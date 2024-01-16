#include "temperature_sensor.h"


typedef struct _tNTCCurve
{
	int16_t		Temperature; 	/**< Temperature in 0.1°C */
	uint16_t 	Count; 			/**< Count */

} tNTCCurve;

#define NTC_LENGTH		106

// TEMPERATURE_SENSOR_TYPE_NTC_10K_3435
static const tNTCCurve NTC_10K_3435_ntc[NTC_LENGTH] =
{
		{-250, 15547  },
		{-240, 15506 },
		{-230, 15464 },
		{-220, 15421 },
		{-210, 15376 },
		{-200, 15329 },
		{-190, 15280 },
		{-180, 15229 },
		{-170, 15177 },
		{-160, 15123 },
		{-150, 15067 },
		{-140, 15009 },
		{-130, 14949 },
		{-120, 14887 },
		{-110, 14823 },
		{-100, 14757 },
		{-90, 14689 },
		{-80, 14619 },
		{-70, 14547 },
		{-60, 14473 },
		{-50, 14396 },
		{-40, 14318 },
		{-30, 14237 },
		{-20, 14154 },
		{-10, 14069 },
		{0, 13981 },
		{10, 13892 },
		{20, 13800 },
		{30, 13706 },
		{40, 13610 },
		{50, 13512 },
		{60, 13411 },
		{70, 13309 },
		{80, 13204 },
		{90, 13097 },
		{100, 12989 },
		{110, 12878 },
		{120, 12765 },
		{130, 12651 },
		{140, 12534 },
		{150, 12416 },
		{160, 12296 },
		{170, 12174 },
		{180, 12050 },
		{190, 11925 },
		{200, 11799 },
		{210, 11671 },
		{220, 11541 },
		{230, 11411 },
		{240, 11279 },
		{250, 11146 },
		{260, 11011 },
		{270, 10876 },
		{280, 10740 },
		{290, 10603 },
		{300, 10465 },
		{310, 10327 },
		{320, 10188 },
		{330, 10049 },
		{340, 9909 },
		{350, 9769 },
		{360, 9629 },
		{370, 9488 },
		{380, 9348 },
		{390, 9208 },
		{400, 9068 },
		{410, 8928 },
		{420, 8788 },
		{430, 8649 },
		{440, 8510 },
		{450, 8372 },
		{460, 8234 },
		{470, 8097 },
		{480, 7961 },
		{490, 7825 },
		{500, 7691 },
		{510, 7558 },
		{520, 7425 },
		{530, 7294 },
		{540, 7163 },
		{550, 7034 },
		{560, 6907 },
		{570, 6780 },
		{580, 6655 },
		{590, 6531 },
		{600, 6409 },
		{610, 6288 },
		{620, 6168 },
		{630, 6050 },
		{640, 5934 },
		{650, 5819 },
		{660, 5706 },
		{670, 5594 },
		{680, 5484 },
		{690, 5376 },
		{700, 5269 },
		{710, 5164 },
		{720, 5060 },
		{730, 4959 },
		{740, 4858 },
		{750, 4760 },
		{760, 4663 },
		{770, 4568 },
		{780, 4474 },
		{790, 4383 },
		{800, 4292 },

};





int16_t TemperatureSensor_Convert(uint8_t type, int16_t value)
{
    uint16_t i;

    tNTCCurve *ntc;

    switch (type)
    {
    	default:
    	case (TEMPERATURE_SENSOR_TYPE_NTC_10K_3435):
			ntc = (tNTCCurve *)NTC_10K_3435_ntc;
    	break;

    }

    /*

	if (value <= ntc[0].Count)
	{
		// Temperatura minima
		return ntc[0].Temperature;
	}


    for (i = 0; i < NTC_LENGTH - 1; i++)
    {
        if (value < ntc[i+1].Count)
        {
            return ( ntc[i].Temperature + (int16_t)((int32_t)(ntc[i + 1].Temperature - ntc[i].Temperature) *
            		(int32_t)(value - ntc[i].Count)) / (int32_t)(ntc[i + 1].Count - ntc[i].Count));
        }
    }
*/

	if (value >= ntc[0].Count)
	{
		// Temperatura minima
		return ntc[0].Temperature;
	}


    for (i = 0; i < NTC_LENGTH - 1; i++)
    {
        if (value > ntc[i+1].Count)
        {
            return ( ntc[i+1].Temperature + (int16_t)((int32_t)(ntc[i].Temperature - ntc[i+1].Temperature) *
            		(int32_t)(value - ntc[i+1].Count)) / (int32_t)(ntc[i].Count - ntc[i+1].Count));
        }
    }

    // Temperatura massima
    return ntc[i].Temperature;
}



