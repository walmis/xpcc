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
#include "USBMSD.h"

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

	enum {
		DISK_OK      =   0x01,
		NO_INIT      =   0x02,
		NO_DISK	     =    0x04,
		WRITE_PROTECT =   0x08
	};

	USBMSDHandler(uint8_t bulkIn = MSD_EPBULK_IN,
			uint8_t bulkOut = MSD_EPBULK_OUT);

	bool initialize();

	void set_device_strings(const char* vendorId, const char* productId, const char* productRev);

	void setPageSize(size_t size);

protected:

	enum TransferType {
		READ,
		WRITE
	};
	uint32_t a;
	/*
	 * Called when usb msd transfer request is received.
	 * @param Transfer type READ or WRITE
	 * @param first block to be read
	 * @param number of blocks to read or write
	 */
	virtual void transfer_begins(TransferType type, uint32_t startBlock, int numBlocks) = 0;

    /*
    * called when a new sector needs to be filled
    * call disk_read_finalize() when data is filled.
    * Useful for asynchronous operations
    *
    * @param data pointer where will be stored read data
    * @param block block number
    * @returns 0 if successful
    */
    virtual int disk_read_start(uint8_t * data, uint32_t block, uint32_t blocksLeft) = 0;

    /*
     * Finalize a disk read. Call when sector is filled with valid data
     */
    void disk_read_finalize(bool success);

    /*
    * write a block on a storage chip
    *
    * @param data data to write
    * @param block block number
    * @returns 0 if successful
    */
    virtual int disk_write_start(const uint8_t * data, uint32_t block, uint32_t blocksLeft) = 0;


    void disk_write_finalize(bool success);

    /*
    * Disk initilization
    */
    virtual int disk_initialize() = 0;

    /*
    * Return the number of blocks
    *
    * @returns number of blocks
    */
    virtual uint32_t disk_sectors() = 0;

    /*
    * Return memory size
    *
    * @returns memory size
    */
    virtual uint16_t disk_sector_size() = 0;

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

    virtual void SOF(int frameNumber) override;

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
        uint32_t Signature;/**< Status block signature */
        uint32_t Tag;/**< Unique command ID value, to associate a command block wrapper with its command status wrapper. */
        uint32_t DataResidue;/**< Number of bytes of data not processed in the SCSI command. */
        uint8_t  Status;
    } __attribute((packed));


	struct RequestSense {
		uint8_t ResponseCode :7;
		uint8_t VALID :1;

		uint8_t Obsolete;

		uint8_t SenseKey :4;
		uint8_t Resv :1;
		uint8_t ILI :1;
		uint8_t EOM :1;
		uint8_t FILEMARK :1;

		uint32_t Information;
		uint8_t AddSenseLen;
		uint32_t CmdSpecificInfo;
		uint8_t ASC;
		uint8_t ASCQ;
		uint8_t FRUC;
		uint8_t SenseKeySpecific[3];
	}__attribute__((packed));

	uint8_t buf[MAX_PACKET_SIZE_EPBULK];

    //state of the bulk-only state machine
    Stage stage;

    // current CBW
    CBW cbw;

    // CSW which will be sent
    CSW csw;

    RequestSense senseData;

    uint8_t bulkIn;
    uint8_t bulkOut;

    // addr where will be read or written data
    uint32_t blockAddr;
    uint32_t dataPos;

    // length of a reading or writing
    uint32_t length;

    // cache in RAM before writing in memory. Useful also to read a block.
    uint8_t * page;

    int BlockSize;
    //uint64_t MemorySize;

    uint32_t BlockCount;

    const char* vendorId = "xpcc";
    const char* productId = "MassStorage";
    const char* productRev = "1.0";

    void CBWDecode();
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
    bool memoryWrite ();
    void reset();
    void fail();

    volatile bool blockReady;
    volatile bool readRequest;
    volatile bool writeRequestPending;

    volatile bool blockSent;

    volatile bool writeBusy;

    void sendBlock();

};

} /* namespace xpcc */

#endif /* USBMSDHANDLER_H_ */
