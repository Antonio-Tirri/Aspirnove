#include "usb.h"
#include "usb_device.h"
#include "usbd_cdc_if.h"
#include "board.h"


extern USBD_HandleTypeDef hUsbDeviceFS;


// RX
uint8_t  USB_Rx [USB_RX_DATA_SIZE]; 
uint16_t USB_Rx_in;
uint16_t USB_Rx_out = 0;
uint16_t USB_Rx_length  = 0;




int USB_Init(void)
{
	

	return 0;
}


unsigned short USB_OnRx(uint8_t *data, uint8_t len)
{
	unsigned short i = len;
	
	while (i)
	{
		if (USB_Rx_length < USB_RX_DATA_SIZE)
		{
			USB_Rx_length++;
			USB_Rx[USB_Rx_in] = *data++;

			if (++USB_Rx_in >= USB_RX_DATA_SIZE)
				USB_Rx_in = 0;
		}
		
		--i;
	}

	return len;
}


/** Legge primo byte ancora da leggere dal buffer di ricezione */
int USB_RecvChar(uint8_t *c)
{
    if(USB_Rx_length)
    {
		__disable_irq();
        USB_Rx_length--;
		__enable_irq();
        *c = USB_Rx[USB_Rx_out];
        if (++USB_Rx_out >= USB_RX_DATA_SIZE)
            USB_Rx_out = 0;

        return 1;
    }
    else
        return 0;
}



unsigned short USB_Recv(uint8_t *data, uint8_t len)
{
    unsigned char *src = (unsigned char *)data;
    int recv = 0;

    for(recv=0;recv<len;recv++)
    {
        if(USB_RecvChar(src++) == 0)
            return recv;
    }
	
	return len;	
	
}

unsigned short USB_Send(uint8_t *data, uint8_t len)
{
	if (len > 0)
	{
		CDC_Transmit_FS((uint8_t *)data, len);
	}
	
	return len;
}

void USB_Disable(void)
{
	USBD_Stop(&hUsbDeviceFS);
}






