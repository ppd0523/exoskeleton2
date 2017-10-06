#include "stm32f10x_usart.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x.h"
#include "misc.h"
#include "usart_servo.h"

void NVIC_Configuration_USART3(void);
void GPIO_Configuration_USART3(void);
void USART3_Configuration(void);
void UART3Send(const unsigned char *pucBuffer, unsigned long ulCount);
void USART3_IRQHandler(void);

void usart_servo_init(void) {
	/* Enable USART3 and GPIOA clock */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

	/* NVIC Configuration */
	NVIC_Configuration_USART3();

	/* Configure the GPIOs */
	GPIO_Configuration_USART3();

	/* Configure the USART3 */
	USART3_Configuration();

	/* Enable the USART3 Receive interrupt: this interrupt is generated when the
	 USART3 receive data register is not empty */
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
}

void GPIO_Configuration_USART3(void) {
	GPIO_InitTypeDef GPIO_InitStructure;

	/* Configure USART3 Tx (PB.10) as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	/* Configure USART3 Rx (PB.11) as input floating */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	/* Configure GPIO Direction for HALF DUPLEX (PB.5) as push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	//GPIO_ResetBits(GPIOB, GPIO_Pin_6);
	// ax-12a 와 통신하기 위한 핀. 모터에게 데이터를 보낼때 set,모터한테 데이터를 받을때 reset
}

void USART3_Configuration(void) {
	USART_InitTypeDef USART3_InitStructure;

	/* USART3 configuration ------------------------------------------------------*/
	/* USART3 configured as follow:
	 - BaudRate = 115200 baud
	 - Word Length = 8 Bits
	 - One Stop Bit
	 - No parity
	 - Hardware flow control disabled (RTS and CTS signals)
	 - Receive and transmit enabled
	 - USART Clock disabled
	 - USART CPOL: Clock is active low
	 - USART CPHA: Data is captured on the middle
	 - USART LastBit: The clock pulse of the last data bit is not output to
	 the SCLK pin
	 */
	USART3_InitStructure.USART_BaudRate = 115200;
	USART3_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART3_InitStructure.USART_StopBits = USART_StopBits_1;
	USART3_InitStructure.USART_Parity = USART_Parity_No;
	USART3_InitStructure.USART_HardwareFlowControl =
			USART_HardwareFlowControl_None;
	USART3_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_Init(USART3, &USART3_InitStructure);

	/* Enable USART */
	USART_Cmd(USART3, ENABLE);
}

void NVIC_Configuration_USART3(void) {
	NVIC_InitTypeDef NVIC_InitStructure;

	/* Enable the USARTx Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void USART3Send(const unsigned char *pucBuffer, unsigned long ulCount) {
	//
	// Loop while there are more characters to send.
	//
	GPIO_SetBits(GPIOA, GPIO_Pin_11); //RECEIVER OUT enable
	while (ulCount--) {
		/* Loop until the end of transmission */
		while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET)
			;
		USART_SendData(USART3, *pucBuffer++);
	}
	while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET)
		;
	GPIO_ResetBits(GPIOA, GPIO_Pin_11); //DRIVER IN enable
}

#define DEBUG

#define INST_WRITE 0x03
#define INST_READ 0x02
#define INST_ bREAD 0x92
#define INST_ bWRITE 0x93
#define RESERVE 0xAA

u8 GetPosition(){
	u8 packet[] =
		{ 0xFF, 0xFF, 0xFD, 0x00, 0x01/*ID*/, 0x07, 0x00,
				INST_READ, 0x63, 0x02, 0x04, 0x00, 0, 0};
	u8 packetSize = sizeof(packet)/sizeof(u8);
	u16 crc = Update_crc(packet, packet[5]+5);
	packet[packetSize-2] = (char) crc & 0x00FF;
	packet[packetSize-1] = (char) (crc>>8) & 0x00FF;
#ifdef DEBUG
		UARTSend(packet, sizeof(packet));
#endif
	USART3Send(packet, sizeof(packet)); // packet size = 13

	return packetSize;
}

u8 GetVelocity(){
	u8 packet[] =
		{ 0xFF, 0xFF, 0xFD, 0x00, 0x01/*ID*/, 0x07, 0x00,
				INST_READ, 0x63, 0x02, 0x04, 0x00, 0, 0};
	u8 packetSize = sizeof(packet)/sizeof(u8);
	u16 crc = Update_crc(packet, packet[5]+5);
	packet[packetSize-2] = (char) crc & 0x00FF;
	packet[packetSize-1] = (char) (crc>>8) & 0x00FF;
#ifdef DEBUG
		UARTSend(packet, sizeof(packet));
#endif
	USART3Send(packet, sizeof(packet)); // packet size = 13

	return 15;
}

/**
 * 수정중
 */
u8 SetVelocity(u32 value){
	union union_u32{
		u8 raw[4];
		u32 value;
	} Data;
	Data.value = value;

	u8 packet[] = { 0xFF, 0xFF, 0xFD, 0x00, 0x01/*ID*/, 0x09, 0x00,
			INST_WRITE, 0x58, 0x02, Data.raw[0], Data.raw[1], Data.raw[2], Data.raw[3], 0, 0};	//16

	u16 crc = Update_crc(packet, 14);
	packet[14] = (char) crc & 0x00FF;
	packet[15] = (char) (crc>>8) & 0x00FF;
#ifdef DEBUG
		UARTSend(packet, sizeof(packet));
#endif
	USART3Send(packet, sizeof(packet)); // packet size = 13

	return 16;
}

u8 SetPosition(u32 value){
	union union_u32{
		u8 raw[4];
		u32 value;
	} Data;
	Data.value = value;

	u8 packet[] = { 0xFF, 0xFF, 0xFD, 0x00, 0x01/*ID*/, 0x09, 0x00, INST_WRITE, 0x54, 0x02, Data.raw[0], Data.raw[1], Data.raw[2], Data.raw[3], 0, 0};	//16

	u16 crc = Update_crc(packet, 14);
	packet[14] = (char) crc & 0x00FF;
	packet[15] = (char) (crc>>8) & 0x00FF;
#ifdef DEBUG
		UARTSend(packet, sizeof(packet));
#endif
	USART3Send(packet, sizeof(packet)); // packet size = 13

	return 11;
}

u8 TorqueMode(u8 value){
	u8 packet[] = { 0xFF, 0xFF, 0xFD, 0x00, 0x01/*ID*/, 0x06, 0x00, INST_WRITE, 0x32, 0x02, value, 0, 0};	//13개

	u16 crc = Update_crc(packet, 11);
	packet[11] = (char) crc & 0x00FF;
	packet[12] = (char) (crc>>8) & 0x00FF;
#ifdef DEBUG
		UARTSend(packet, sizeof(packet));
#endif
	USART3Send(packet, sizeof(packet)); // packet size = 13

	return 11;
}

u8 CheckError(){
	u8 packet[] = { 0xFF, 0xFF, 0xFD, 0x00, 0x01/*ID*/, 0x06, 0x00, INST_READ, 0x7C, 0x03, 0x01, 0, 0};	//13개

	u16 crc = Update_crc(packet, 11);
	packet[11] = (char) crc & 0x00FF;
	packet[12] = (char) (crc>>8) & 0x00FF;
#ifdef DEBUG
		UARTSend(packet, sizeof(packet)-1);
#endif
	USART3Send(packet, sizeof(packet)); // packet size = 13

	return 11;
}

u8 ServoLED(u8 value) {
	u8 packet[] = { 0xFF, 0xFF, 0xFD, 0x00, 0x01/*ID*/, 0x06, 0x00, INST_WRITE, 0x35, 0x02, value, 0, 0};	//13개

	u16 crc = Update_crc(packet, 11);
	packet[11] = (char) crc & 0x00FF;
	packet[12] = (char) (crc>>8) & 0x00FF;
#ifdef DEBUG
		UARTSend(packet, sizeof(packet));
#endif
	USART3Send(packet, sizeof(packet)); // packet size = 13

	return 11;
}

unsigned short Update_crc(unsigned char *data_blk_ptr, unsigned short data_blk_size) {
	unsigned short crc_accum = 0;
	unsigned short i, j;
	unsigned short crc_table[256] = { 0x0000, 0x8005, 0x800F, 0x000A, 0x801B,
			0x001E, 0x0014, 0x8011, 0x8033, 0x0036, 0x003C, 0x8039, 0x0028,
			0x802D, 0x8027, 0x0022, 0x8063, 0x0066, 0x006C, 0x8069, 0x0078,
			0x807D, 0x8077, 0x0072, 0x0050, 0x8055, 0x805F, 0x005A, 0x804B,
			0x004E, 0x0044, 0x8041, 0x80C3, 0x00C6, 0x00CC, 0x80C9, 0x00D8,
			0x80DD, 0x80D7, 0x00D2, 0x00F0, 0x80F5, 0x80FF, 0x00FA, 0x80EB,
			0x00EE, 0x00E4, 0x80E1, 0x00A0, 0x80A5, 0x80AF, 0x00AA, 0x80BB,
			0x00BE, 0x00B4, 0x80B1, 0x8093, 0x0096, 0x009C, 0x8099, 0x0088,
			0x808D, 0x8087, 0x0082, 0x8183, 0x0186, 0x018C, 0x8189, 0x0198,
			0x819D, 0x8197, 0x0192, 0x01B0, 0x81B5, 0x81BF, 0x01BA, 0x81AB,
			0x01AE, 0x01A4, 0x81A1, 0x01E0, 0x81E5, 0x81EF, 0x01EA, 0x81FB,
			0x01FE, 0x01F4, 0x81F1, 0x81D3, 0x01D6, 0x01DC, 0x81D9, 0x01C8,
			0x81CD, 0x81C7, 0x01C2, 0x0140, 0x8145, 0x814F, 0x014A, 0x815B,
			0x015E, 0x0154, 0x8151, 0x8173, 0x0176, 0x017C, 0x8179, 0x0168,
			0x816D, 0x8167, 0x0162, 0x8123, 0x0126, 0x012C, 0x8129, 0x0138,
			0x813D, 0x8137, 0x0132, 0x0110, 0x8115, 0x811F, 0x011A, 0x810B,
			0x010E, 0x0104, 0x8101, 0x8303, 0x0306, 0x030C, 0x8309, 0x0318,
			0x831D, 0x8317, 0x0312, 0x0330, 0x8335, 0x833F, 0x033A, 0x832B,
			0x032E, 0x0324, 0x8321, 0x0360, 0x8365, 0x836F, 0x036A, 0x837B,
			0x037E, 0x0374, 0x8371, 0x8353, 0x0356, 0x035C, 0x8359, 0x0348,
			0x834D, 0x8347, 0x0342, 0x03C0, 0x83C5, 0x83CF, 0x03CA, 0x83DB,
			0x03DE, 0x03D4, 0x83D1, 0x83F3, 0x03F6, 0x03FC, 0x83F9, 0x03E8,
			0x83ED, 0x83E7, 0x03E2, 0x83A3, 0x03A6, 0x03AC, 0x83A9, 0x03B8,
			0x83BD, 0x83B7, 0x03B2, 0x0390, 0x8395, 0x839F, 0x039A, 0x838B,
			0x038E, 0x0384, 0x8381, 0x0280, 0x8285, 0x828F, 0x028A, 0x829B,
			0x029E, 0x0294, 0x8291, 0x82B3, 0x02B6, 0x02BC, 0x82B9, 0x02A8,
			0x82AD, 0x82A7, 0x02A2, 0x82E3, 0x02E6, 0x02EC, 0x82E9, 0x02F8,
			0x82FD, 0x82F7, 0x02F2, 0x02D0, 0x82D5, 0x82DF, 0x02DA, 0x82CB,
			0x02CE, 0x02C4, 0x82C1, 0x8243, 0x0246, 0x024C, 0x8249, 0x0258,
			0x825D, 0x8257, 0x0252, 0x0270, 0x8275, 0x827F, 0x027A, 0x826B,
			0x026E, 0x0264, 0x8261, 0x0220, 0x8225, 0x822F, 0x022A, 0x823B,
			0x023E, 0x0234, 0x8231, 0x8213, 0x0216, 0x021C, 0x8219, 0x0208,
			0x820D, 0x8207, 0x0202 };

	for (j = 0; j < data_blk_size; j++) {
		i = ((unsigned short) (crc_accum >> 8) ^ data_blk_ptr[j]) & 0xFF;
		crc_accum = (crc_accum << 8) ^ crc_table[i];
	}

	return crc_accum;
}
