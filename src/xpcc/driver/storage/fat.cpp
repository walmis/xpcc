// coding: utf-8
// ----------------------------------------------------------------------------
/* Copyright (c) 2011, Roboterclub Aachen e.V.
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

#include <fatfs/diskio.h>
#include <fatfs/ffconf.h>

#include "fat.hpp"
#include <sys/time.h>
#include <xpcc/driver/time/time.hpp>
// ----------------------------------------------------------------------------
// FatFs Backend
extern "C"
DWORD
get_fattime(void)
{
	timeval tv;
	if(gettimeofday(&tv, 0) == 0) {
		xpcc::UnixTime ut(tv.tv_sec);
		xpcc::Date d;
		ut.toDate(&d);

		return ((d.year - 80) << 25) |
				(d.month << 21) |
				(d.day << 16) |
				(d.hour << 11) |
				(d.minute << 5) |
				(d.second >> 1);

	} else {
		return 0;
	}
}


uint32_t xpcc::fat::FileInfo::getUnixTimestamp() {
	uint16_t date = getModifiedDate();
	uint16_t time = getModifiedTime();
	uint32_t second, day, leap_day, month, year;
	year = date >> 9;
	month = std::max(1, (date >> 5) & 0xf);
	day = std::max(1, date & 0x1f) - 1;
	leap_day = (year + 3) / 4;
	if (year > YEAR_2100) /* 2100 isn't leap year */
		leap_day--;

	if (IS_LEAP_YEAR(year) && month > 2)
		leap_day++;

	second = (time & 0x1f) << 1;
	second += ((time >> 5) & 0x3f) * 60;
	second += (time >> 11) * 3600;
	second += (year * 365 + leap_day + days_in_year[month] + day + DAYS_DELTA)
			* 86400;
	return second;
}

// ----------------------------------------------------------------------------
#if _VOLUMES == 1

static xpcc::fat::PhysicalVolume * globalVolume;

extern "C"
DSTATUS
disk_initialize(BYTE /*drive*/)
{
	return globalVolume->doInitialize();
}

extern "C"
DSTATUS
disk_status(BYTE /*drive*/)
{
	return globalVolume->doGetStatus();
}

extern "C"
DRESULT
disk_read(BYTE /*drive*/, BYTE* buffer, DWORD sectorNumber, UINT sectorCount)
{
	return globalVolume->doRead(buffer, sectorNumber, sectorCount);
}

extern "C"
DRESULT
disk_write(BYTE /*drive*/, const BYTE* buffer, DWORD sectorNumber, UINT sectorCount)
{
	return globalVolume->doWrite(buffer, sectorNumber, sectorCount);
}

extern "C"
DRESULT
disk_ioctl(BYTE /*drive*/, BYTE command, void* buffer)
{
	return globalVolume->doIoctl(command, reinterpret_cast<uint32_t *>(buffer));
	//return RES_ERROR;
}

#else
#	error "Only one FatFS drive supported at the moment!"
#endif

// ----------------------------------------------------------------------------
xpcc::fat::FileSystem::FileSystem(PhysicalVolume *volume,
		uint8_t drive)
{
	globalVolume = volume;
	_volume = volume;
	mount();
}

xpcc::fat::FileSystem::~FileSystem()
{
	f_mount(0, "", 0);
}


namespace xpcc {
namespace fat {

#if _USE_LFN
uint8_t xpcc::fat::FileInfo::lfn[_MAX_LFN];
#endif

FRESULT File::open(const char* path, const char* mode) {
	if(opened)
		close();

	uint8_t flags = 0;
	bool append = false;
	switch (mode[0]) {
	case 'r':
		flags |= FA_READ | FA_OPEN_EXISTING;
		if (mode[1] == '+') {
			flags |= FA_WRITE;
		}
		break;

	case 'a':
		append = true;
	case 'w':
		flags |= FA_CREATE_ALWAYS | FA_WRITE;
		if (mode[1] == '+') {
			flags |= FA_READ;
		}
		break;

	}
	FRESULT r = f_open(&file, path, flags);
	if(r == FR_OK)
		opened = true;
	if (r == FR_OK && append) {
		lseek(size());
	}
	return r;
}

}
}
