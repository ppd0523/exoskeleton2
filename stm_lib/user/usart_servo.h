void USART3_IRQHandler(void);
void usart_servo_init(void);
void GPIO_Configuration_USART3(void);
void USART3_Configuration(void);
void NVIC_Configuration_USART3(void);
void USART3Send(const unsigned char *pucBuffer, unsigned long ulCount);
unsigned short Update_crc(unsigned char *data_blk_ptr, unsigned short data_blk_size);
u8 ServoLED(u8 value);
u8 TorqueMode(u8 value);
u8 SetPosition(u32 value);
u8 SetVelocity(u32 value);
u8 CheckError();
u8 GetPosition();
u8 GetVelocity();
