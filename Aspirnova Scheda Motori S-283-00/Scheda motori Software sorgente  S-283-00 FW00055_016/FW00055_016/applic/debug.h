/** \file   debug.h
 * 
 * 	\brief  Supporto messaggi di debug 
 * 
 * 	\author Walter Giusiano - lab3841 srl \n
 *  walter.giusiano@lab3841.it
 *
 *  \version    v1.0 - 16/06/2015
 * 	-   Versione iniziale
 *  
 */
#ifndef _DEBUG_H_
#define _DEBUG_H_

#include "stdint.h"

/** Elenco di livelli di debug
*/
enum
{
	DBG_NONE,	 /**< Nessun prefisso */
    DBG_ERROR,   /**< Condizione di errore */
    DBG_WARNING, /**< Condizione di warning */
    DBG_INFO,    /**< Messaggio di informazione */
    DBG_DEBUG,   /**< Messaggio di debug */

    DBG_COUNT   /**< Numero massimo di livelli di debug implementati */
};

// Verifica costanti di compilazione



#include "stdio.h"

// Supporto C++
#ifdef __cplusplus
	extern "C" {
#endif

		
/** Stampa le informazioni di debug sull'output predefinito. 
	La stampa delle informazioni può essere ridotta in base al livello di severità
    impostato attraverso il metodo debug_setlevel().
    \param level
    Livello di severità del messaggio di debug. Le informazioni sono stampate se il livello
    di severità è minore o uguale al livello impostato con il metodo debug_setlevel().
    \param *data
    Stringa di debug da stampare
	\param wait_tx
	Se TRUE attende invio stringa
*/
void Debug(int level, char *data, uint8_t wait_tx);


/** Imposta il livello di severità utilizzato per la stampa delle informazioni di debug.
    Le informazioni sono stampate se il livello di severità è minore o uguale al livello impostato
    con il presente metodo. Ad es., se si imposta:
    \code
    debug_setlevel(DBG_INFO);
    Debug(DBG_INFO, "Info");
    Debug(DBG_DEBUG, "Debug");
    \endcode
    verrà stampata solo la stringa "Info".
    \param level
    Livello di severità prevista per i messaggi di debug.
*/
void Debug_SetLevel(int level);

/** Restituisce numero di byte ancora da trasmettere */
int Debug_GetTxLen(void);

// Supporto C++
#ifdef __cplusplus
	}
#endif


#endif // _DEBUG_H_
