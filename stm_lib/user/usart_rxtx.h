#ifndef __USART_RXTX_H
#define __USART_RXTX_H

void USART1_IRQHandler(void);
void UARTSend(const unsigned char *pucBuffer, unsigned long ulCount);

void usart_init(void);
void int2str(unsigned char* buf, uint16_t n);
void int2str2(unsigned char* buf, uint16_t n);
void adc2str(unsigned char buf[], uint16_t adc[]);

#endif /* __USART_RXTX_H */
