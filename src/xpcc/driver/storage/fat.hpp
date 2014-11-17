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
#ifndef XPCC__FAT_HPP
#define XPCC__FAT_HPP

#include <stdint.h>
#include <fatfs/ff.h>
#include <fatfs/diskio.h>
#include <xpcc/io.hpp>
#include <xpcc/debug.hpp>

namespace xpcc {
namespace fat {
typedef DRESULT Result;
typedef DSTATUS Status;

/**
 * \brief	Interface to a SD Card, Dataflash, etc.
 */
class PhysicalVolume {
public:
	virtual ~PhysicalVolume() {
	}

	/**
	 * \brief	Initialize Volume
	 */
	virtual Status
	doInitialize() = 0;

	/**
	 * \brief
	 * \return
	 */
	virtual Status
	doGetStatus() = 0;

	/**
	 * \brief	Read sectors
	 *
	 * \param buffer		Pointer to the data buffer to store read data
	 * \param sectorNumber	Start sector number (LBA)
	 * \param sectorCount	Sector count (1..255)
	 *
	 * \return	TODO
	 */
	virtual Result
	doRead(uint8_t *buffer, int32_t sectorNumber, uint32_t sectorCount) = 0;

	/**
	 * \brief	Write Sectors
	 *
	 * \param buffer		Pointer to the data to be written
	 * \param sectorNumber	Start sector number (LBA)
	 * \param sectorCount	Sector count (1..255)
	 *
	 * \return	TODO
	 */
	virtual Result
	doWrite(const uint8_t *buffer, int32_t sectorNumber,
			uint32_t sectorCount) = 0;

	/**
	 * \brief	Execute a command
	 *
	 * \param command			Command
	 * \param[in,out] buffer	Contains the parameters for the command
	 * 							and will be overwritten with the result.
	 */
	virtual Result
	doIoctl(uint8_t command, uint32_t *buffer) = 0;
};

class FileSystem {
public:
	FileSystem(PhysicalVolume *volume, uint8_t drive = 0);

	~FileSystem();

	FRESULT mount() {
		return f_mount(&this->fileSystem, "", 0);
	}

	static FRESULT mkdir(const char* dir) {
		return f_mkdir(dir);
	}
	PhysicalVolume* volume() {
		return _volume;
	}
protected:
	PhysicalVolume* _volume;
	FATFS fileSystem;
};

class FileInfo {
public:
	FileInfo() {
#if _USE_LFN
		info.lfname = (TCHAR*) lfn;
		info.lfsize = _MAX_LFN;
#endif
	}

	inline bool stat(const char* path) {
		return f_stat(path, &info) == FR_OK;
	}

	inline uint32_t getSize() const {
		return info.fsize;
	}

	inline int16_t getModifiedDate() const {
		return info.fdate;
	}

	inline int16_t getModifiedTime() const {
		return info.ftime;
	}

	inline char*
	getName() {
#if _USE_LFN
		if (info.lfname[0] == 0)
			return getShortName();
		return info.lfname;
#else
		return info.fname;
#endif
	}

	/// Name in 8.3 format
	inline char*
	getShortName() {
		return info.fname;
	}

	inline bool eod() const {
		return info.fname[0] == 0;
	}

	friend class Directory;

protected:
#if _USE_LFN
	static uint8_t lfn[_MAX_LFN];
#endif
	FILINFO info;
};

class Directory {
public:
	FRESULT open(char* path) {
		return f_opendir(&directory, path);
	}

	FRESULT readDir(FileInfo &fileinfo) {
		return f_readdir(&directory, &fileinfo.info);
	}

	FRESULT close() {
		return f_closedir(&directory);
	}

	~Directory() {
		close();
	}

protected:
	DIR directory;
};

class File: public IOStream, IODevice {
public:
	File() :
			IOStream(*static_cast<IODevice*>(this)) {
		opened = false;
	}

	~File() {
		close();
	}

	FRESULT open(const char* path, const char* mode);

	FRESULT close() {
		opened = false;
		return f_close(&file);
	}

	size_t write(char c) override {
		f_putc(c, &file);
		return 1;
	}

	void flush() override {
		f_sync(&file);
	}

	int16_t read() override {
		UINT read = -1;
		char c;
		if (f_read(&file, (uint8_t*) &c, 1, &read) != FR_OK)
			return -1;
		return c;
	}

	size_t write(uint8_t* buffer, size_t len) {
		uint32_t written = 0;
		f_write(&file, (void*) buffer, len, (unsigned int*) &written);
		return written;
	}

	bool feof() {
		return f_eof(&file);
	}

	uint32_t fsize() {
		return f_size(&file);
	}

	uint32_t ftell() {
		return f_tell(&file);
	}

	//return true if line fit the buffer
	//false if not
	bool readLine(uint8_t* line, size_t maxSize) {
		char tmp;
		while (1) {
			if ((tmp = read()) != -1) {
				if (maxSize) {
					*line = tmp;
					if (tmp == '\r') {
						*line = '\0';
						tmp = read();
					}
					if (tmp == '\n') {
						*line = '\0';
						return true;
					}

					maxSize--;
					line++;
				} else {
					return false;
				}

			} else {
				return false;
			}
		}
	}

	int32_t read(uint8_t* buffer, uint32_t len) {
		UINT read = -1;
		FRESULT res = f_read(&file, buffer, len, &read);
		if (res != FR_OK) {
			return -res;
		}
		return read;
	}

	void lseek(size_t pos) {
		f_lseek(&file, pos);
	}

	bool isOpened() {
		return opened;
	}

	void
	truncate();

	size_t size() {
		return f_size(&file);
	}

protected:
	bool opened;
	FIL file;
};

}
}

#endif // XPCC__FAT_HPP
