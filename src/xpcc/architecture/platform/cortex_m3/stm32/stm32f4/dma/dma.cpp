/*
 * dma.cpp
 *
 *  Created on: Apr 22, 2015
 *      Author: walmis
 */


#include "dma.hpp"

namespace xpcc {
namespace stm32 {
namespace dma {

DMAStream* DMAStream::streams[16];

extern "C" void DMA1_Stream0_IRQHandler(void) {
	DMAStream* stream = DMAStream::streams[0];
	if(stream) {
		stream->handleIRQ();
	}
}
extern "C" void DMA1_Stream1_IRQHandler(void) {
	DMAStream* stream = DMAStream::streams[1];
	if(stream) {
		stream->handleIRQ();
	}
}
extern "C" void DMA1_Stream2_IRQHandler(void) {
	DMAStream* stream = DMAStream::streams[2];
	if(stream) {
		stream->handleIRQ();
	}
}
extern "C" void DMA1_Stream3_IRQHandler(void) {
	DMAStream* stream = DMAStream::streams[3];
	if(stream) {
		stream->handleIRQ();
	}
}
extern "C" void DMA1_Stream4_IRQHandler(void) {
	DMAStream* stream = DMAStream::streams[4];
	if(stream) {
		stream->handleIRQ();
	}
}
extern "C" void DMA1_Stream5_IRQHandler(void) {
	DMAStream* stream = DMAStream::streams[5];
	if(stream) {
		stream->handleIRQ();
	}
}
extern "C" void DMA1_Stream6_IRQHandler(void) {
	DMAStream* stream = DMAStream::streams[6];
	if(stream) {
		stream->handleIRQ();
	}
}
extern "C" void DMA1_Stream7_IRQHandler(void) {
	DMAStream* stream = DMAStream::streams[7];
	if(stream) {
		stream->handleIRQ();
	}
}

extern "C" void DMA2_Stream0_IRQHandler(void) {
	DMAStream* stream = DMAStream::streams[8];
	if(stream) {
		stream->handleIRQ();
	}
}
extern "C" void DMA2_Stream1_IRQHandler(void) {
	DMAStream* stream = DMAStream::streams[9];
	if(stream) {
		stream->handleIRQ();
	}
}
extern "C" void DMA2_Stream2_IRQHandler(void) {
	DMAStream* stream = DMAStream::streams[10];
	if(stream) {
		stream->handleIRQ();
	}
}
extern "C" void DMA2_Stream3_IRQHandler(void) {
	DMAStream* stream = DMAStream::streams[11];
	if(stream) {
		stream->handleIRQ();
	}
}
extern "C" void DMA2_Stream4_IRQHandler(void) {
	DMAStream* stream = DMAStream::streams[12];
	if(stream) {
		stream->handleIRQ();
	}
}
extern "C" void DMA2_Stream5_IRQHandler(void) {
	DMAStream* stream = DMAStream::streams[13];
	if(stream) {
		stream->handleIRQ();
	}
}
extern "C" void DMA2_Stream6_IRQHandler(void) {
	DMAStream* stream = DMAStream::streams[14];
	if(stream) {
		stream->handleIRQ();
	}
}
extern "C" void DMA2_Stream7_IRQHandler(void) {
	DMAStream* stream = DMAStream::streams[15];
	if(stream) {
		stream->handleIRQ();
	}
}

}
}
}
