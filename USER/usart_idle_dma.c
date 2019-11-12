/*******************************************************************************
* @File     usart_idle_dma.c
* @Brief    使用DMA的USART配置文件
* @Date     2019-11-12
* @Version  V1.0
* @Note     使用DMA进行USART接收和发送
* @Author   EmbeddedXGJ
*******************************************************************************/

#include "usart_idle_dma.h"
#include "dma.h"

char g_sendBuff[USART_DMA_TX_BUFF_LEN];
stUSART_RX_BUFF  g_USART_RxBuff_t;   /* 接收缓冲区 */


#if USING_USART_DMA_RX
/* 串口1接收DMA通道初始化 */
void USARTx_RX_DMA_Config(void)
{
	My_DMA_Config(USART1_RX_DMA_CHANNEL,         //USART1 DMA RX 通道
	              (u32)&USART1_DR_ADDR,           //外设内存地址
	              (u32)g_USART_RxBuff_t.rx_buff,  //内存地址
	              USART_RX_BUFF_LEN,           //DMA通道的DMA缓存的大小
	              DMA_DIR_PeripheralSRC,          //方向从外设到内存
	              DMA_Mode_Circular);             //循环模式
	
	DMA_ClearFlag(DMA1_FLAG_TC5);                            //清楚通道标志位                
	DMA_ITConfig(USART1_RX_DMA_CHANNEL,DMA_IT_TC, ENABLE);   //开启串口1的DMA接收完成中断
	DMA_Cmd(USART1_RX_DMA_CHANNEL,ENABLE);                 //使能DMA
}
#endif

#if USING_USART_DMA_TX
/* 串口1发送DMA通道初始化 */
void USARTx_TX_DMA_Config(void)
{
	My_DMA_Config(USART1_TX_DMA_CHANNEL,         //USART1 DMA RX 通道
	              (u32)&USART1_DR_ADDR,           //外设内存地址
	              (u32)g_sendBuff,                //内存地址
	              USART_DMA_TX_BUFF_LEN,          //DMA通道的DMA缓存的大小
	              DMA_DIR_PeripheralDST,          //方向从内存到外设
	              DMA_Mode_Normal);               //正常模式
}

/* 开启一次DMA传输 */
void USART1_DMA_SendData(uint16_t DataNumber)
{
	USARTx_DMA_Enable_Send(USART1,USART1_TX_DMA_CHANNEL,DataNumber);
}
#endif


void USART1_NVIC_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
		 //Usart1 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel                   = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
}

void USARTx_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	g_USART_RxBuff_t.rx_data_len = 0;
	
	RCC_APB2PeriphClockCmd(USART1_CLK, ENABLE);	     /* 使能USART1时钟 */
	RCC_APB2PeriphClockCmd(USART1_PORT_CLK, ENABLE);  /* 使能GPIOA时钟 */
	//USART1_TX   GPIOA.9
	GPIO_InitStructure.GPIO_Pin   = USART1_TX_PIN;      //PA.9
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;	//复用推挽输出
	GPIO_Init(USART1_PORT, &GPIO_InitStructure);       //初始化GPIOA.9

	//USART1_RX	  GPIOA.10初始化
	GPIO_InitStructure.GPIO_Pin  = USART1_RX_PIN;         //PA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
	GPIO_Init(USART1_PORT, &GPIO_InitStructure);         //初始化GPIOA.10  

	/* 串口1 NVIC配置*/
	USART1_NVIC_Config();
	
	/* 串口1初始化 */
	USART_InitStructure.USART_BaudRate            = USART_BAUDRATE;         //串口波特率
	USART_InitStructure.USART_WordLength          = USART_WordLength_8b;    //字长为8位数据格式
	USART_InitStructure.USART_StopBits            = USART_StopBits_1;       //一个停止位
	USART_InitStructure.USART_Parity              = USART_Parity_No;       //无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;	//收发模式	
	USART_Init(USART1, &USART_InitStructure);                                       //初始化串口1
				
#if USING_USART_DMA_RX  /* 使用DMA + 空闲中断接收 */
	USART_ITConfig(USING_USART, USART_IT_IDLE, ENABLE);//配置串口空闲中断
	USART_DMACmd(USING_USART,USART_DMAReq_Rx, ENABLE);	//开启DMA接收
	USARTx_RX_DMA_Config();                            //USART1的DMA初始化 
#else  /* 开启串口接受中断  普通中断接收 */
	USART_ITConfig(USING_USART, USART_IT_RXNE, ENABLE); //开启串口接受中断  普通中断接收
#endif

#if USING_USART_DMA_TX
//	USART_DMACmd(USING_USART,USART_DMAReq_Tx, ENABLE);	//开启DMA发送
	USARTx_TX_DMA_Config();
#endif

	USART_Cmd(USING_USART, ENABLE);                    //使能串口1 
}	

#if USING_USART_DMA_RX
void USART1_DMA_Recvice_Data(void)
{
	uint32_t buff_len;
	
	/* 关闭DMA ，防止干扰 */
	DMA_Cmd(USART1_RX_DMA_CHANNEL, DISABLE);  /* 暂时关闭dma，数据未处理 */ 
	
	buff_len = USART_RX_BUFF_LEN - DMA_GetCurrDataCounter(USART1_RX_DMA_CHANNEL);
	g_USART_RxBuff_t.rx_data_len = buff_len;   //保存接收的数据长度
	
	DMA_ClearFlag(DMA1_FLAG_TC5);               //清楚标志位
	
	//重新设置DMA通道的DMA缓存的大小
	DMA_SetCurrDataCounter(USART1_RX_DMA_CHANNEL, USART_RX_BUFF_LEN); 
	
	/* 使能DMA */ 	
	DMA_Cmd(USART1_RX_DMA_CHANNEL, ENABLE);  
}
#else
void USART_RecvData(void)
{
	u8 res;
	static uint32_t cnt = 0;
	
	res = USART_ReceiveData(USART1);	                   //读取接收到的数据	
	if(cnt < USART_RX_BUFF_LEN){   
		g_USART_RxBuff_t.rx_buff[cnt] = res;
		g_USART_RxBuff_t.rx_data_len = cnt;
		cnt++;	
	}else {
		g_USART_RxBuff_t.rx_buff[0] = res;   //把数据搬到第一个
		cnt = 1;
	}		
}
#endif

void USART1_IRQHandler(void)                	//串口1中断服务程序
{	
#if USING_USART_DMA_RX
	if(USART_GetITStatus(USART1,USART_IT_IDLE) != RESET)   /* 空闲中断 */
	{	
		/* 接收数据 */
		USART1_DMA_Recvice_Data();	
		/* 清除空闲中断标志位 */ 
		USART_ReceiveData(USART1);
	}
#else	
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //接收中断
	{
		USART_RecvData();
	} 
#endif											
} 


