#include "dma.h"

void My_DMA_Config(DMA_Channel_TypeDef* DMA_CHx,u32 peri_base_addr,
	               u32 mem_base_addr,u16 buff_size,uint32_t DMA_DIR,uint32_t DMA_Mode)
{
	DMA_InitTypeDef DMA_InitStructure;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	//使能DMA传输

	DMA_DeInit(DMA_CHx);       //将DMA的通道1寄存器重设为缺省值
//	DMA1_MEM_LEN                             = cndtr;
	DMA_InitStructure.DMA_PeripheralBaseAddr = peri_base_addr; //DMA外设基地址
	DMA_InitStructure.DMA_MemoryBaseAddr     = mem_base_addr;  //DMA内存基地址
	DMA_InitStructure.DMA_DIR                = DMA_DIR;  //数据传输方向，从内存读取发送到外设
	DMA_InitStructure.DMA_BufferSize         = buff_size;                  //DMA通道的DMA缓存的大小
	DMA_InitStructure.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;  //外设地址寄存器不递增
	DMA_InitStructure.DMA_MemoryInc          = DMA_MemoryInc_Enable;       //内存地址寄存器递增
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;  //数据宽度为8位
	DMA_InitStructure.DMA_MemoryDataSize     = DMA_MemoryDataSize_Byte;    //数据宽度为8位
	DMA_InitStructure.DMA_Mode               = DMA_Mode;            //工作在正常模式
	DMA_InitStructure.DMA_Priority           = DMA_Priority_Medium;        //DMA通道 x拥有中优先级 
	DMA_InitStructure.DMA_M2M                = DMA_M2M_Disable;             //DMA通道x没有设置为内存到内存传输
	DMA_Init(DMA_CHx, &DMA_InitStructure);   //根据DMA_InitStruct中指定的参数初始化DMA的通道USART1_Tx_DMA_Channel所标识的寄存器
}

void USARTx_DMA_Enable_Send(USART_TypeDef* USARTx,DMA_Channel_TypeDef*DMA_CHx,uint16_t DataNumber)
{ 
	DMA_Cmd(DMA_CHx, DISABLE);                //关闭USART1 TX DMA1 所指示的通道      
 	DMA_SetCurrDataCounter(DMA_CHx,DataNumber);//DMA通道的DMA缓存的大小
 	DMA_Cmd(DMA_CHx, ENABLE);                    //使能USART1 TX DMA1 所指示的通道 
	USART_DMACmd(USARTx,USART_DMAReq_Tx,ENABLE); //使能DMA传输
}	  

