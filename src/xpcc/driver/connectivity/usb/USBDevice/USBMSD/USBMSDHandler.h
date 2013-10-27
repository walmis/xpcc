/*
 * USBMSDHandler.h
 *
 *  Created on: Oct 26, 2013
 *      Author: walmis
 */

#ifndef USBMSDHANDLER_H_
#define USBMSDHANDLER_H_

#include "../USBDevice/USBDevice.h"
#include "../USBDevice/USBInterfaceHandler.h"


namespace xpcc {

class USBMSDHandler : public USBInterfaceHandler
{
public:

    /**
    * Constructor
    *
    * @param vendor_id Your vendor_id
    * @param product_id Your product_id
    * @param product_release Your preoduct_release
    */
	USBMSDHandler(uint8_t bulkIn = EPBULK_IN,
			uint8_t bulkOut = EPBULK_OUT);

	bool initialize();

protected:

	enum TransferType {
		READ,
		WRITE
	};

	virtual void transfer_begins(TransferType type, uint64_t startBlock, int numBlocks) = 0;

    /*
    * read a block on a storage chip
    *
    * @param data pointer where will be stored read data
    * @param block block number
    * @returns 0 if successful
    */
    virtual int disk_read_start(uint8_t * data, uint64_t block, uint32_t blocksLeft) = 0;

    void disk_read_finalize(bool success);

    /*
    * write a block on a storage chip
    *
    * @param data data to write
    * @param block block number
    * @returns 0 if successful
    */
    virtual int disk_write(const uint8_t * data, uint64_t block) = 0;

    /*
    * Disk initilization
    */
    virtual int disk_initialize() = 0;

    /*
    * Return the number of blocks
    *
    * @returns number of blocks
    */
    virtual uint64_t disk_sectors() = 0;

    /*
    * Return memory size
    *
    * @returns memory size
    */
    virtual uint64_t disk_size() = 0;

    /*
    * To check the status of the storage chip
    *
    * @returns status: 0: OK, 1: disk not initialized, 2: no medium in the drive, 4: write protected
    */
    virtual int disk_status() = 0;


private:
    /*
    * Set configuration of device. Add endpoints
    */
    virtual bool USBCallback_setConfiguration(uint8_t configuration);

    /*
    * Callback called to process class specific requests
    */
    virtual bool USBCallback_request();

    bool EP_handler(uint8_t ep) override;

    // MSC Bulk-only Stage
    enum Stage {
        READ_CBW,     // wait a CBW
        ERROR,        // error
        PROCESS_CBW,  // process a CBW request
        SEND_CSW,     // send a CSW
        WAIT_CSW,     // wait that a CSW has been effectively sent
    };

    // Bulk-only CBW
    struct CBW{
        uint32_t Signature;
        uint32_t Tag;
        uint32_t DataLength;
        uint8_t  Flags;
        uint8_t  LUN;
        uint8_t  CBLength;
        uint8_t  CB[16];
    } __attribute((packed));

    // Bulk-only CSW
    struct CSW {
        uint32_t Signature;
        uint32_t Tag;
        uint32_t DataResidue;
        uint8_t  Status;
    } __attribute((packed));

    //state of the bulk-only state machine
    Stage stage;

    // current CBW
    CBW cbw;

    // CSW which will be sent
    CSW csw;

    uint8_t bulkIn;
    uint8_t bulkOut;

    // addr where will be read or written data
    uint32_t addr;

    // length of a reading or writing
    uint32_t length;

    // memory OK (after a memoryVerify)
    bool memOK;

    // cache in RAM before writing in memory. Useful also to read a block.
    uint8_t * page;

    int BlockSize;
    uint64_t MemorySize;
    uint64_t BlockCount;

    void CBWDecode(uint8_t * buf, uint16_t size);
    void sendCSW (void);
    bool inquiryRequest (void);
    bool write (uint8_t * buf, uint16_t size);
    bool readFormatCapacity();
    bool readCapacity (void);
    bool infoTransfer (void);
    void memoryRead (void);
    bool modeSense6 (void);
    void testUnitReady (void);
    bool requestSense (void);
    void memoryVerify (uint8_t * buf, uint16_t size);
    void memoryWrite (uint8_t * buf, uint16_t size);
    void reset();
    void fail();

    volatile bool blockReady;
    volatile bool readRequest;
    volatile bool writeRequest;

    void sendBlock();

};

} /* namespace xpcc */

#endif /* USBMSDHANDLER_H_ */
