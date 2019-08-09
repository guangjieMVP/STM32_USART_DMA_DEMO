#ifndef _USART_IDLE_DMA_H_
#define _USART_IDLE_DMA_H_

#include "stm32f10x.h"

#define USING_USART_DMA_RX     1    

#define USING_USART_DMA_TX     1

#define USING_USART             USART1

#define USART_BAUDRATE          115200                 /* 波特率 */
#define USART1_CLK              RCC_APB2Periph_USART1  /* USART1时钟 */
#define USART1_PORT_CLK         RCC_APB2Periph_GPIOA   /* USART1 GPIO时钟 */
#define USART1_PORT             GPIOA                  /* USART1 PORT */
#define USART1_TX_PIN           GPIO_Pin_9             /* USART1 Tx PIN */
#define USART1_RX_PIN           GPIO_Pin_10            /* USART1 Rx PIN */

#define USART1_DR_ADDR           USART1->DR            /* USART1 Data reg */

#define USART1_RX_DMA_CHANNEL    DMA1_Channel5         /* USART1 Rx DMA channel */
#define USART1_TX_DMA_CHANNEL    DMA1_Channel4         /* USART1 Tx DMA channel */

#define USART_DMA_TX_BUFF_LEN    250                   /* you are 250 */

#define USART_RX_BUFF_LEN    250                   /* recvice buffer size */
typedef struct 
{
	uint8_t  rx_buff[USART_RX_BUFF_LEN];
	uint32_t rx_data_len;
}stUSART_RX_BUFF;

void USARTx_Config(void);


#if USING_USART_DMA_TX

void USART1_DMA_SendData(uint16_t DataNumber);
#endif

extern stUSART_RX_BUFF  g_USART_RxBuff_t;   /* 接收缓冲区 */


#endif

