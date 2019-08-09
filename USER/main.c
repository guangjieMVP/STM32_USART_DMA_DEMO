#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"
#include "usart_idle_dma.h"
 
extern char g_sendBuff[USART_DMA_TX_BUFF_LEN];
 int main(void)
 {		
	u16 i;
	delay_init();	    	 //延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
//	uart_init(115200);	 //串口初始化为115200
	
 	LED_Init();			     //LED端口初始化
	KEY_Init();          //初始化与按键连接的硬件接口
	USARTx_Config(); 
 	while(1)
	{
		if(g_USART_RxBuff_t.rx_data_len > 0)
		{			
			printf(">>>data len is：%d \r\n",g_USART_RxBuff_t.rx_data_len);
			for(i = 0;i < g_USART_RxBuff_t.rx_data_len;i++)
			{
				g_sendBuff[i] = g_USART_RxBuff_t.rx_buff[i];
			}				
			USART1_DMA_SendData(g_USART_RxBuff_t.rx_data_len);
			g_USART_RxBuff_t.rx_data_len = 0;
		}
//		printf("aaa\r\n");
	}	 
}

