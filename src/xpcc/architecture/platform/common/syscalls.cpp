// coding: utf-8
// ----------------------------------------------------------------------------
/* Copyright (c) 2010, Roboterclub Aachen e.V.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the Roboterclub Aachen e.V. nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY ROBOTERCLUB AACHEN E.V. ''AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL ROBOTERCLUB AACHEN E.V. BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
// ----------------------------------------------------------------------------
/**
 * @file	syscalls.cpp
 * @brief	Stubs for newlib
 *
 * Except for _sbrk_r() (which is needed for malloc()) all these stubs are
 * only dummy functions which don't do anything useful.
 *
 * _exit() and exit() are defined in the file "startup.S". They are marked
 * as weak references, so they can be easily overwritten by user code.
 */
// ----------------------------------------------------------------------------

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include <reent.h>
#include <sys/stat.h>
#include <ctype.h>

#include <xpcc/architecture/driver/heap/block_allocator.hpp>
#include <xpcc/architecture/driver/atomic.hpp>
// ----------------------------------------------------------------------------
/*
 * @brief	Simple abort implementation
 *
 * Does nothing except starting a infinite loop
 */
extern "C"
void
abort(void)
{
	while(1) {
		// do nothing
	}
}

// ----------------------------------------------------------------------------
extern "C"
int
_open_r(struct _reent */*r*/, const char */*name*/, int /*flags*/, int /*mode*/)
{
	return -1;
}

extern "C"
_ssize_t
_read_r(struct _reent */*r*/, int /*file*/, void */*ptr*/, size_t /*len*/)
{
	return 0;
}

extern "C"
_ssize_t
_write_r(struct _reent */*r*/, int /*file*/, const void */*ptr*/, size_t /*len*/)
{
	return 0;
}

extern "C"
int
_close_r(struct _reent */*r*/, int /*file*/)
{
	return 0;
}

// ----------------------------------------------------------------------------
extern "C"
_off_t
_lseek_r(struct _reent */*r*/, int /*file*/, _off_t /*ptr*/, int /*dir*/)
{
	return (_off_t) 0;		// Always indicate we are at file beginning.
}

// ----------------------------------------------------------------------------
extern "C"
int
_fstat_r(struct _reent */*r*/, int /*file*/, struct stat *st)
{
	st->st_mode = S_IFCHR;	// Always set as character device.
	return 0;
}

// ----------------------------------------------------------------------------
extern "C"
int
_isatty(int /*file*/)
{
	return 1;
}

// ----------------------------------------------------------------------------
/* Support function. Adjusts end of heap to provide more memory to
 * memory allocator.
 *
 *  struct _reent *r -- re-entrancy structure, used by newlib to
 *                      support multiple threads of operation.
 *  ptrdiff_t size   -- number of bytes to add.
 *                      Returns pointer to start of new heap area.
 *
 *  Note:  This implementation is not thread safe (despite taking a
 *         _reent structure as a parameter).
 *         Since the _reent struct is not used in the current implementation,
 *         the following messages must be suppressed.
 */
extern uint8_t __heap_start;
extern uint8_t __heap_end;

extern "C"
void *
_sbrk_r(struct _reent *,  ptrdiff_t size)
{
	static uint8_t *heap_ptr = &__heap_start;

	if((heap_ptr + size) < &__heap_end) {
		void *base  = heap_ptr;
		heap_ptr += size;
		return base;
	}

	return  (void *) -1;
}

// ----------------------------------------------------------------------------
// __heap_start is set in the linker command file and is the end of
// statically allocated data (thus start of heap).
extern uint8_t __heap_start;
extern uint8_t __heap_end;

//static xpcc::BlockAllocator<uint16_t, 8> allocator;

extern "C" int memsys3Init();
extern "C" void *memsys3Realloc(void *pPrior, int nBytes);
extern "C" void memsys3Free(void *pPrior);
extern "C" void *memsys3Malloc(int nBytes);

extern "C"
void __xpcc_initialize_memory(void)
{
	//allocator.initialize(&__heap_start, &__heap_end);
	//memsys3Init();
}

extern "C" void *pvPortMalloc( size_t xWantedSize );
extern "C" void vPortFree( void *pv );

//extern "C"
//void *malloc(size_t size)
//{
//	xpcc::atomic::Lock lock;
//	//return pvPortMalloc(size);
//	return memsys3Malloc(size);
//	//return allocator.allocate(size);
//}
//
//extern "C"
//void free(void *p)
//{
//	xpcc::atomic::Lock lock;
//	memsys3Free(p);
//	//vPortFree(p);
//	//allocator.free(p);
//}

static uint32_t lfsr = 1;
extern "C"
void srand(unsigned int seed) {
	if(!seed) seed = 1;
	lfsr = seed;
}

extern "C"
int rand() {
	lfsr = (lfsr >> 1) ^ (-(lfsr & 1u) & 0xD0000001u);
	return lfsr;
}

static int chartoint(int c)
{
    char hex[] = "aAbBcCdDeEfF";
    int i;
    int result = 0;

    for(i = 0; result == 0 && hex[i] != '\0'; i++)
    {
        if(hex[i] == c)
        {
            result = 10 + (i / 2);
        }
    }

    return result;
}

static unsigned int htoi(const char s[])
{
    unsigned int result = 0;
    int i = 0;
    int proper = 1;
    int temp;

    //To take care of 0x and 0X added before the hex no.
    if(s[i] == '0')
    {
        ++i;
        if(s[i] == 'x' || s[i] == 'X')
        {
            ++i;
        }
    }

    while(proper && s[i] != '\0')
    {
        result = result * 16;
        if(s[i] >= '0' && s[i] <= '9')
        {
            result = result + (s[i] - '0');
        }
        else
        {
            temp = chartoint(s[i]);
            if(temp == 0)
            {
                proper = 0;
            }
            else
            {
                result = result + temp;
            }
        }

        ++i;
    }
    //If any character is not a proper hex no. ,  return 0
    if(!proper)
    {
        result = 0;
    }

    return result;
}


static inline unsigned int ipow(int x, int n) {
    int r = 1;
    while (n--)
    r *= x;

    return r;
}

extern "C"
long atol(const char* p) {
	if(p[0] == '0' && (p[1] == 'x' || p[1] == 'X')) {
		return htoi(p);
	}

	int k = 0;
	bool neg = false;
	if (*p == '-') {
		p++;
		neg = true;
	}

	while (*p && isdigit(*p)) {
		k = (k << 3) + (k << 1) + (*p) - '0';
		p++;
	}
	if (neg)
		return -k;
	return k;
}

extern "C"
int atoi(const char* str) {
	return atol(str);
}

extern "C"
double atof(const char* c) {
	char str[20];
	strncpy(str, c, 20);

	char* tok;
	tok = strtok(str, ".");

	int base = atol(tok);

	tok = strtok(NULL, ".");
	if(tok) {
		uint8_t len = strlen(tok);

		float l = (float)atol(tok) / ipow(10, len);
		l += base;
		return l;

	} else {
		return base;
	}
}
