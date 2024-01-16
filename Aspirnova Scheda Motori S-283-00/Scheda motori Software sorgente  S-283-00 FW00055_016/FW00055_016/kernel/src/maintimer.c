#include "maintimer.h"


// Contatore ms
unsigned int maintimerCont = 0;

void MainTimer_Callback(void);



void MainTimer_Init(void)
{}

void MainTimer_OnTick(void)
{
    // Incrementa contatore ms
    maintimerCont++;	
	
	MainTimer_Callback();
}



unsigned int MainTimer_GetValue(void)
{
    return maintimerCont;
}


void MainTimer_Wait(unsigned int value)
{
    unsigned int start = MainTimer_GetValue();

    // Attesa per i ms richiesti...
    while((MainTimer_GetValue() - start) < value)
    {}
}

