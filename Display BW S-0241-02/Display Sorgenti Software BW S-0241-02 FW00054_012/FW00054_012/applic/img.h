/**	\file   img.h
 *
 * 	\brief  Icone visualizzate nelle videate
 *
 * 	\author Walter Giusiano - lab3841 \n
 *  walter.giusiano@lab3841.com 
 *
 *
 *  \version    v1.0 - 19/02/2016
 * 	-   Versione iniziale
 *
 * 	\section req Requisiti per il corretto funzionamento
 * 	-   Nessuno
 *
 */

#ifndef _IMG_
#define _IMG_

#include "windows.h"


extern const struct
{
	GSYMHEAD sh;
	SGUCHAR  b[1024];
} Logo;



extern const struct
{
	GSYMHEAD sh;
	SGUCHAR  b[64];
} Ice;

extern const struct
{
	GSYMHEAD sh;
	SGUCHAR  b[64];
} Sun;

extern const struct
{
	GSYMHEAD sh;
	SGUCHAR  b[64];
} Vuota16x16;

extern const struct
{
	GSYMHEAD sh;
	SGUCHAR  b[64];
} Termostato;

extern const struct
{
	GSYMHEAD sh;
	SGUCHAR  b[40];
} Ext;

extern const struct
{
	GSYMHEAD sh;
	SGUCHAR  b[40];
} Int;

extern const struct
{
	GSYMHEAD sh;
	SGUCHAR  b[64];
} Co2;

extern const struct
{
	GSYMHEAD sh;
	SGUCHAR  b[64];
} Man;

extern const struct
{
	GSYMHEAD sh;
	SGUCHAR  b[64];
} Auto;

extern const struct
{
	GSYMHEAD sh;
	SGUCHAR  b[64];
} Schedule;

extern const struct
{
	GSYMHEAD sh;
	SGUCHAR  b[64];
} ButtonOn;

extern const struct
{
	GSYMHEAD sh;
	SGUCHAR  b[64];
} SterilizerOff;

extern const struct
{
	GSYMHEAD sh;
	SGUCHAR  b[64];
} SterilizerOn;

extern const struct
{
	GSYMHEAD sh;
	SGUCHAR  b[64];
} Humidity;

extern const struct
{
	GSYMHEAD sh;
	SGUCHAR  b[64];
} Serpentina;

extern const struct
{
	GSYMHEAD sh;
	SGUCHAR  b[64];
} BypassOff;

extern const struct
{
	GSYMHEAD sh;
	SGUCHAR  b[64];
} Bell;



#endif 
