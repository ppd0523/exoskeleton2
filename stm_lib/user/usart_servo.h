void USART3_IRQHandler(void);
void usart_servo_init(void);
void GPIO_Configuration_USART3(void);
void USART3_Configuration(void);
void NVIC_Configuration_USART3(void);
void USART3Send(const unsigned char *pucBuffer, unsigned long ulCount);
