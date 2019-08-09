#ifndef _DMA_H_
#define _DMA_H_
#include "stm32f10x.h"

void My_DMA_Config(DMA_Channel_TypeDef* DMA_CHx,u32 peri_base_addr,
	               u32 mem_base_addr,u16 buff_size,uint32_t DMA_DIR,uint32_t DMA_Mode);

void USARTx_DMA_Enable_Send(USART_TypeDef* USARTx,DMA_Channel_TypeDef*DMA_CHx,uint16_t DataNumber);

#endif

