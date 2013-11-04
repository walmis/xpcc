/*
 * USBMSDHandler.cpp
 *
 *  Created on: Oct 26, 2013
 *      Author: walmis
 */

#include "USBMSDHandler.h"

namespace xpcc {

//#define _MSD_DEBUG

#ifndef _MSD_DEBUG
#undef XPCC_LOG_LEVEL
#define XPCC_LOG_LEVEL xpcc::log::DISABLED
#endif


#define SCSI_SET_SENSE(Key, Acode, Aqual)  { senseData.SenseKey = (Key);   \
                                             senseData.ASC      = (Acode); \
                                             senseData.ASCQ     = (Aqual); }

#define CBW_Signature   0x43425355
#define CSW_Signature   0x53425355

// SCSI Commands
#define TEST_UNIT_READY            0x00
#define REQUEST_SENSE              0x03
#define FORMAT_UNIT                0x04
#define INQUIRY                    0x12
#define MODE_SELECT6               0x15
#define MODE_SENSE6                0x1A
#define START_STOP_UNIT            0x1B
#define MEDIA_REMOVAL              0x1E
#define READ_FORMAT_CAPACITIES     0x23
#define READ_CAPACITY              0x25
#define READ10                     0x28
#define WRITE10                    0x2A
#define VERIFY10                   0x2F
#define READ12                     0xA8
#define WRITE12                    0xAA
#define MODE_SELECT10              0x55
#define MODE_SENSE10               0x5A


/** \name SCSI Sense Key Values */
//@{
/** SCSI Sense Code to indicate no error has occurred. */
#define SCSI_SENSE_KEY_GOOD                            0x00

/** SCSI Sense Code to indicate that the device has recovered from an error. */
#define SCSI_SENSE_KEY_RECOVERED_ERROR                 0x01

/** SCSI Sense Code to indicate that the device is not ready for a new command. */
#define SCSI_SENSE_KEY_NOT_READY                       0x02

/** SCSI Sense Code to indicate an error whilst accessing the medium. */
#define SCSI_SENSE_KEY_MEDIUM_ERROR                    0x03

/** SCSI Sense Code to indicate a hardware error has occurred. */
#define SCSI_SENSE_KEY_HARDWARE_ERROR                  0x04

/** SCSI Sense Code to indicate that an illegal request has been issued. */
#define SCSI_SENSE_KEY_ILLEGAL_REQUEST                 0x05

/** SCSI Sense Code to indicate that the unit requires attention from the host to indicate
 *  a reset event, medium removal or other condition.
 */
#define SCSI_SENSE_KEY_UNIT_ATTENTION                  0x06

/** SCSI Sense Code to indicate that a write attempt on a protected block has been made. */
#define SCSI_SENSE_KEY_DATA_PROTECT                    0x07

/** SCSI Sense Code to indicate an error while trying to write to a write-once medium. */
#define SCSI_SENSE_KEY_BLANK_CHECK                     0x08

/** SCSI Sense Code to indicate a vendor specific error has occurred. */
#define SCSI_SENSE_KEY_VENDOR_SPECIFIC                 0x09

/** SCSI Sense Code to indicate that an EXTENDED COPY command has aborted due to an error. */
#define SCSI_SENSE_KEY_COPY_ABORTED                    0x0A

/** SCSI Sense Code to indicate that the device has aborted the issued command. */
#define SCSI_SENSE_KEY_ABORTED_COMMAND                 0x0B

/** SCSI Sense Code to indicate an attempt to write past the end of a partition has been made. */
#define SCSI_SENSE_KEY_VOLUME_OVERFLOW                 0x0D

/** SCSI Sense Code to indicate that the source data did not match the data read from the medium. */
#define SCSI_SENSE_KEY_MISCOMPARE                      0x0E
//@}

/** \name SCSI Additional Sense Codes */
//@{
/** SCSI Additional Sense Code to indicate no additional sense information is available. */

#define SCSI_ASENSE_UNRECOVERED_READ_ERROR             0x11

#define SCSI_ASENSE_WRITE_FAULT				           0x03

#define SCSI_ASENSE_NO_ADDITIONAL_INFORMATION          0x00

/** SCSI Additional Sense Code to indicate that the logical unit (LUN) addressed is not ready. */
#define SCSI_ASENSE_LOGICAL_UNIT_NOT_READY             0x04

/** SCSI Additional Sense Code to indicate an invalid field was encountered while processing the issued command. */
#define SCSI_ASENSE_INVALID_FIELD_IN_CDB               0x24

/** SCSI Additional Sense Code to indicate that a medium that was previously indicated as not ready has now
 *  become ready for use.
 */
#define SCSI_ASENSE_NOT_READY_TO_READY_CHANGE          0x28

/** SCSI Additional Sense Code to indicate that an attempt to write to a protected area was made. */
#define SCSI_ASENSE_WRITE_PROTECTED                    0x27

/** SCSI Additional Sense Code to indicate an error whilst formatting the device medium. */
#define SCSI_ASENSE_FORMAT_ERROR                       0x31

/** SCSI Additional Sense Code to indicate an invalid command was issued. */
#define SCSI_ASENSE_INVALID_COMMAND                    0x20

/** SCSI Additional Sense Code to indicate a write to a block out outside of the medium's range was issued. */
#define SCSI_ASENSE_LOGICAL_BLOCK_ADDRESS_OUT_OF_RANGE 0x21

/** SCSI Additional Sense Code to indicate that no removable medium is inserted into the device. */
#define SCSI_ASENSE_MEDIUM_NOT_PRESENT                 0x3A
//@}

/** \name SCSI Additional Sense Key Code Qualifiers */
//@{
/** SCSI Additional Sense Qualifier Code to indicate no additional sense qualifier information is available. */
#define SCSI_ASENSEQ_NO_QUALIFIER                      0x00

/** SCSI Additional Sense Qualifier Code to indicate that a medium format command failed to complete. */
#define SCSI_ASENSEQ_FORMAT_COMMAND_FAILED             0x01

/** SCSI Additional Sense Qualifier Code to indicate that an initializing command must be issued before the issued
 *  command can be executed.
 */
#define SCSI_ASENSEQ_INITIALIZING_COMMAND_REQUIRED     0x02

/** SCSI Additional Sense Qualifier Code to indicate that an operation is currently in progress. */
#define SCSI_ASENSEQ_OPERATION_IN_PROGRESS             0x07
//@}


// MSC class specific requests
#define MSC_REQUEST_RESET          0xFF
#define MSC_REQUEST_GET_MAX_LUN    0xFE

#define DEFAULT_CONFIGURATION (1)

// max packet size
#define MAX_PACKET  MAX_PACKET_SIZE_EPBULK

// CSW Status
enum Status {
    CSW_PASSED,
    CSW_FAILED,
    CSW_ERROR,
};


USBMSDHandler::USBMSDHandler(uint8_t bulkIn, uint8_t bulkOut) :
		bulkIn(bulkIn), bulkOut(bulkOut) {
	stage = READ_CBW;
	page = 0;
	writeBusy = false;

	memset((uint8_t*)&senseData, 0, sizeof(RequestSense));

	senseData.AddSenseLen = 0x0A;
	senseData.ResponseCode = 0x70;

	memset((void *) &cbw, 0, sizeof(CBW));
	memset((void *) &csw, 0, sizeof(CSW));
}



// Called in ISR context to process a class specific request
bool USBMSDHandler::USBCallback_request(void) {

    bool success = false;
    CONTROL_TRANSFER * transfer = device->getTransferPtr();
    static uint8_t maxLUN[1] = {0};

    if (transfer->setup.bmRequestType.Type == CLASS_TYPE) {
        switch (transfer->setup.bRequest) {
            case MSC_REQUEST_RESET:
                reset();
                success = true;
                break;
            case MSC_REQUEST_GET_MAX_LUN:
                transfer->remaining = 1;
                transfer->ptr = maxLUN;
                transfer->direction = DEVICE_TO_HOST;
                success = true;
                break;
            default:
                break;
        }
    }

    return success;
}


bool USBMSDHandler::initialize() {

    //disk initialization
    if (disk_status() & NO_INIT) {
        if (disk_initialize()) {
            return false;
        }
    }

    // get number of blocks
    BlockCount = disk_sectors();
    //MemorySize = BlockCount * disk_sector_size();

    if (BlockCount > 0) {
        BlockSize = disk_sector_size();
        if (BlockSize != 0) {
            if(page)
            	free(page);

        	page = (uint8_t *)malloc(BlockSize * sizeof(uint8_t));
            if (page == NULL)
                return false;
        }
    } else {
        return false;
    }

    return true;
}


void USBMSDHandler::reset() {
    stage = READ_CBW;
}

bool USBMSDHandler::EP_handler(uint8_t ep) {
	//XPCC_LOG_DEBUG .printf("USBMSDHandler::EP_handler(%d)\n", ep);
	if(ep == bulkIn) {

	    switch (stage) {

	        // the device has to send data to the host
	        case PROCESS_CBW:
	            switch (cbw.CB[0]) {
	                case READ10:
	                case READ12:
	                    memoryRead();
	                    break;
	            }
	            break;

	            //the device has to send a CSW
	        case SEND_CSW:
	            sendCSW();
	            break;

	            // an error has occured
	        case ERROR:
	            XPCC_LOG_DEBUG .printf("USBMSDHandler::EP_handler stage:ERROR\n");
	        	device->stallEndpoint(bulkIn);
	            sendCSW();
	            break;

	            // the host has received the CSW -> we wait a CBW
	        case WAIT_CSW:
	        	XPCC_LOG_DEBUG .printf("WAIT_CSW -> READ_CBW\n");
	            stage = READ_CBW;
	            break;
	    }
	    return true;

	} else

	if(ep == bulkOut) {

	    //uint32_t size = 0;
	    //uint8_t buf[MAX_PACKET_SIZE_EPBULK];

	    switch (stage) {
	            // the device has to decode the CBW received
	        case READ_CBW:
	            CBWDecode();
	            break;

	            // the device has to receive data from the host
	        case PROCESS_CBW:
	            switch (cbw.CB[0]) {
	                case WRITE10:
	                case WRITE12:
	                    return memoryWrite();
	                    break;
	            }
	            break;

	            // an error has occured: stall endpoint and send CSW
	        default:
	        	XPCC_LOG_DEBUG .printf("CSW error stage\n");
	        	device->stallEndpoint(bulkOut);
	            csw.Status = CSW_ERROR;
	            sendCSW();
	            break;
	    }

	    return true;
	}
	return false;
}


void USBMSDHandler::disk_write_finalize(bool success) {
	//XPCC_LOG_DEBUG.printf("finalize writeRequest:%d %d\n", writeRequest, writeBusy);

    if(!success) {
    	xpcc::atomic::Lock lock;

    	XPCC_LOG_DEBUG .printf("USBMSDHandler::disk_write_finalize(%d)\n", success);

    	SCSI_SET_SENSE(SCSI_SENSE_KEY_MEDIUM_ERROR,
	                   SCSI_ASENSE_WRITE_FAULT,
	                   SCSI_ASENSEQ_NO_QUALIFIER);

    	csw.Status = CSW_FAILED;

    	sendCSW();

    	writeBusy = false;
        return;
    }

	if(writeBusy) {
		writeBusy = false;
		device->readStart(bulkOut, MAX_PACKET_SIZE_EPBULK);

		{
			xpcc::atomic::Lock lock;

			if(writeRequest) {
				memoryWrite();
			}
		}

		if ((!length) || (stage != PROCESS_CBW)) {
			csw.Status = (stage == ERROR) ? CSW_FAILED : CSW_PASSED;
			//XPCC_LOG_DEBUG .printf("send csw %d\n", cbw.CB[0]);

			sendCSW();
		}
	}
}

bool USBMSDHandler::memoryWrite () {

	writeRequest = true;
	//XPCC_LOG_DEBUG .printf("writeRequest busy:%d\n", writeBusy);

	if(!writeBusy) {
		uint8_t buf[MAX_PACKET_SIZE_EPBULK];
		uint32_t size = 0;

		//XPCC_LOG_DEBUG .printf("start read * ");
		bool res = device->readEP_NB(bulkOut, buf, &size, MAX_PACKET_SIZE_EPBULK);

		//XPCC_LOG_DEBUG .printf("<readRes %d>\n", res);

		if(!res) {
			return false;
		}

		if (blockAddr + ((dataPos+size) / (BlockSize+1)) > BlockCount) {
			size = BlockSize - (dataPos & (BlockSize-1));
			XPCC_LOG_DEBUG .printf("USBMSDHandler::memoryWrite error\n");
			stage = ERROR;
			device->stallEndpoint(bulkOut);
		}

		memcpy(&page[dataPos & (BlockSize-1)], buf, size);

		dataPos += size;
		length -= size;
		csw.DataResidue -= size;

		//XPCC_LOG_DEBUG .printf("wr block:%d pos:%d left:%d\n", blockAddr, dataPos, length);
		// if the array is filled, write it in memory
		if (dataPos >= BlockSize) {
			writeBusy = true;
			disk_write_start(page, blockAddr, length/BlockSize);

			blockAddr++;
			dataPos = 0;
		} else {
			device->readStart(bulkOut, MAX_PACKET_SIZE_EPBULK);
		}
		return true;
	}
	return false;
}


bool USBMSDHandler::inquiryRequest (void) {
	struct
	{
		uint8_t Peripheral;
		uint8_t Removble;
		uint8_t Version;
		uint8_t Response_Data_Format;
		uint8_t AdditionalLength;
		uint8_t Sccstp;
		uint8_t bqueetc;
		uint8_t CmdQue;
		char vendorID[8];
		char productID[16];
		char productRev[4];
	} inquiry = {
			0x00, 0x80, 0x00, 0x01,
			36 - 4, 0x00, 0x00, 0x00,
			"xpcc",
			"Mass Storage",
			"1.0"
	};

    if (!write((uint8_t*)&inquiry, sizeof(inquiry))) {
        return false;
    }
    return true;
}


bool USBMSDHandler::readFormatCapacity() {
    uint8_t capacity[] = { 0x00, 0x00, 0x00, 0x08,
                           (BlockCount >> 24) & 0xff,
                           (BlockCount >> 16) & 0xff,
                           (BlockCount >> 8) & 0xff,
                           (BlockCount >> 0) & 0xff,

                           0x02,
                           (BlockSize >> 16) & 0xff,
                           (BlockSize >> 8) & 0xff,
                           (BlockSize >> 0) & 0xff,
                         };
    if (!write(capacity, sizeof(capacity))) {
        return false;
    }
    return true;
}


bool USBMSDHandler::readCapacity (void) {
    uint8_t capacity[] = {
        ((BlockCount - 1) >> 24) & 0xff,
        ((BlockCount - 1) >> 16) & 0xff,
        ((BlockCount - 1) >> 8) & 0xff,
        ((BlockCount - 1) >> 0) & 0xff,

        (BlockSize >> 24) & 0xff,
        (BlockSize >> 16) & 0xff,
        (BlockSize >> 8) & 0xff,
        (BlockSize >> 0) & 0xff,
    };

    if (!write(capacity, sizeof(capacity))) {
        return false;
    }

    return true;
}

bool USBMSDHandler::write (uint8_t * buf, uint16_t size) {

    if (size >= cbw.DataLength) {
        size = cbw.DataLength;
    }
    stage = SEND_CSW;

    device->writeNB(bulkIn, buf, size, MAX_PACKET_SIZE_EPBULK);

    csw.DataResidue -= size;
    csw.Status = CSW_PASSED;
    return true;
}


bool USBMSDHandler::modeSense6 (void) {
	uint8_t sense6[] = { 0x03, 0x00,
			(disk_status() & WRITE_PROTECT) ? 0x80 : 0x00, 0x00 };

    if (!write(sense6, sizeof(sense6))) {
        return false;
    }
    return true;
}

void USBMSDHandler::sendCSW() {

	if (csw.Status == CSW_PASSED) {
		SCSI_SET_SENSE(SCSI_SENSE_KEY_GOOD,
				SCSI_ASENSE_NO_ADDITIONAL_INFORMATION,
				SCSI_ASENSEQ_NO_QUALIFIER);
	}

    csw.Signature = CSW_Signature;

    XPCC_LOG_DEBUG .printf("sendCSW %d\n", csw.Status);
    //XPCC_LOG_DEBUG .dump_buffer((uint8_t *)&csw, sizeof(CSW));
    device->writeNB(bulkIn, (uint8_t *)&csw, sizeof(CSW), MAX_PACKET_SIZE_EPBULK);
    stage = WAIT_CSW;
}



bool USBMSDHandler::requestSense (void) {
    XPCC_LOG_DEBUG .printf("requestSense\n");

	//XPCC_LOG_DEBUG .dump_buffer((uint8_t*)&senseData, sizeof(RequestSense));

    if (!write((uint8_t*)&senseData, sizeof(RequestSense))) {
        return false;
    }

    return true;
}

void USBMSDHandler::fail() {
	//XPCC_LOG_DEBUG .printf("USBMSDHandler::fail()\n");
    csw.Status = CSW_FAILED;
    sendCSW();
}


void USBMSDHandler::CBWDecode() {
	uint8_t buf[MAX_PACKET_SIZE_EPBULK];
	uint32_t size;

	device->readEP(bulkOut, buf, &size, MAX_PACKET_SIZE_EPBULK);

	if (size == sizeof(cbw)) {
        memcpy((uint8_t *)&cbw, buf, size);
        XPCC_LOG_DEBUG .printf("USBMSDHandler::CBWDecode() SCSI Command (0x%x)\n", cbw.CB[0]);
        if (cbw.Signature == CBW_Signature) {
            csw.Tag = cbw.Tag;
            csw.DataResidue = cbw.DataLength;
            if ((cbw.CBLength <  1) || (cbw.CBLength > 16) ) {
                fail();
            } else {
                switch (cbw.CB[0]) {
                    case TEST_UNIT_READY:
                        testUnitReady();
                        break;
                    case REQUEST_SENSE:
                        requestSense();
                        break;
                    case INQUIRY:
                        inquiryRequest();
                        break;
                    case MODE_SENSE6:
                        modeSense6();
                        break;
                    case READ_FORMAT_CAPACITIES:
                        readFormatCapacity();
                        break;
                    case READ_CAPACITY:
                        readCapacity();
                        break;
                    case READ10:
                    case READ12:
                    	if (infoTransfer()) {
                            if ((cbw.Flags & 0x80)) {
                                stage = PROCESS_CBW;
                                memoryRead();
                            } else {
                                device->stallEndpoint(bulkOut);
                                csw.Status = CSW_ERROR;
                                sendCSW();
                            }
                        }
                        break;
                    case WRITE10:
                    case WRITE12:
                        if (infoTransfer()) {
                            if (!(cbw.Flags & 0x80)) {
                                stage = PROCESS_CBW;
                            } else {
                                device->stallEndpoint(bulkIn);
                                csw.Status = CSW_ERROR;
                                sendCSW();
                            }
                        }
                        break;

                   // case VERIFY10:
                    //case 0x1b:
//                    case MEDIA_REMOVAL:
//						csw.Status = CSW_PASSED;
//						sendCSW();
//
//                        break;
                    default:
                    	//XPCC_LOG_DEBUG .printf("Unhandled command %x\n", cbw.CB[0]);

            			SCSI_SET_SENSE(SCSI_SENSE_KEY_ILLEGAL_REQUEST,
            		                   SCSI_ASENSE_INVALID_COMMAND,
            		                   SCSI_ASENSEQ_NO_QUALIFIER);
                        fail();
                        break;
                }
            }
        }
    }
	device->readStart(bulkOut, MAX_PACKET_SIZE_EPBULK);
}

void USBMSDHandler::testUnitReady (void) {
    if (cbw.DataLength != 0) {
        if ((cbw.Flags & 0x80) != 0) {
            device->stallEndpoint(bulkIn);
        } else {
            device->stallEndpoint(bulkOut);
        }
    }

    if(disk_status() & NO_DISK) {
		SCSI_SET_SENSE(SCSI_SENSE_KEY_NOT_READY,
					   SCSI_ASENSE_MEDIUM_NOT_PRESENT,
					   SCSI_ASENSEQ_NO_QUALIFIER);

		csw.Status = CSW_FAILED;

    } else {
    	csw.Status = CSW_PASSED;
    }

	sendCSW();
}




void USBMSDHandler::disk_read_finalize(bool success) {
	XPCC_LOG_DEBUG .printf("USBMSDHandler::disk_read_finalize(%d)\n", success);

	if(!success) {

		SCSI_SET_SENSE(SCSI_SENSE_KEY_MEDIUM_ERROR,
					   SCSI_ASENSE_UNRECOVERED_READ_ERROR,
					   SCSI_ASENSEQ_NO_QUALIFIER);

		//dont send any more data
		csw.DataResidue = 0;
		csw.Status = CSW_FAILED;
		stage = SEND_CSW;

		//write a zero length packet, this causes usb subsystem call bulkIn interrupt later
		//if we do a sendCSW here we will deadlock
		device->writeNB(bulkIn, 0, 0, MAX_PACKET_SIZE_EPBULK);

		return;
	}
	blockReady = true;

	sendBlock();
}

void USBMSDHandler::sendBlock() {
    if(blockReady && readRequest) {
		uint32_t n;

		n = (length > MAX_PACKET) ? MAX_PACKET : length;

		if ((blockAddr + ((dataPos + n - 1) / BlockSize)) > BlockCount) {
			XPCC_LOG_DEBUG.printf("USBMSDHandler::sendBlock() error line:%d\n",
					__LINE__);

			stage = ERROR;
		}

		// write data which are in RAM
		device->writeNB(bulkIn, &page[dataPos & (BlockSize-1)], n, MAX_PACKET_SIZE_EPBULK);

		dataPos += n;
		length -= n;

		csw.DataResidue -= n;

		//if block is fully transfered increment blockAddress
		if((dataPos & (BlockSize-1)) == 0) {
			blockAddr++;
			dataPos = 0;
		}

		if ( !length || (stage != PROCESS_CBW)) {
			csw.Status = (stage == PROCESS_CBW) ? CSW_PASSED : CSW_FAILED;
			stage = (stage == PROCESS_CBW) ? SEND_CSW : stage;
		}

		readRequest = false;
    }
}

void USBMSDHandler::memoryRead (void) {
    //XPCC_LOG_DEBUG .printf("Read %d\n", n);

    readRequest = true;
    // we read an entire block
    if ((dataPos & (BlockSize-1)) == 0) {
    	blockReady = false;

        disk_read_start(page, blockAddr, length/BlockSize);
    }

    //if block is ready, send it
    sendBlock();
}


bool USBMSDHandler::infoTransfer (void) {
    uint32_t n;

    // Logical Block Address of First Block
    n = (cbw.CB[2] << 24) | (cbw.CB[3] << 16) | (cbw.CB[4] <<  8) | (cbw.CB[5] <<  0);

    //start block address
    blockAddr = n;

    // Number of Blocks to transfer
    switch (cbw.CB[0]) {
        case READ10:
        case WRITE10:
        case VERIFY10:
            n = (cbw.CB[7] <<  8) | (cbw.CB[8] <<  0);
            break;

        case READ12:
        case WRITE12:
            n = (cbw.CB[6] << 24) | (cbw.CB[7] << 16) | (cbw.CB[8] <<  8) | (cbw.CB[9] <<  0);
            break;
    }

    //length in bytes
    length = n * BlockSize;

    if (!cbw.DataLength) {              // host requests no data
        csw.Status = CSW_FAILED;
        sendCSW();
        return false;
    }

    TransferType type = READ;
    switch (cbw.CB[0]) {
        case READ10:
        case READ12:
        case VERIFY10:
        	type = READ;
        	break;
        case WRITE10:
        case WRITE12:
        	type = WRITE;
            break;
    }

    dataPos = 0;

    if(type == WRITE && (disk_status() & WRITE_PROTECT)) {
		SCSI_SET_SENSE(SCSI_SENSE_KEY_DATA_PROTECT,
	                   SCSI_ASENSE_WRITE_PROTECTED,
	                   SCSI_ASENSEQ_NO_QUALIFIER);

    	csw.Status = CSW_FAILED;
    	device->stallEndpoint(bulkOut);

    	sendCSW();

        return false;
    }

    transfer_begins(type, blockAddr, n);

    //XPCC_LOG_DEBUG .printf("infoTransfer %x blockAddr:%d blocks:%d\n", cbw.CB[0], blockAddr, n);

    if (cbw.DataLength != length) {
        if ((cbw.Flags & 0x80) != 0) {
            device->stallEndpoint(bulkIn);
        } else {
            device->stallEndpoint(bulkOut);
        }

        csw.Status = CSW_FAILED;
        sendCSW();
        return false;
    }

    return true;
}

// Called in ISR context
// Set configuration. Return false if the
// configuration is not supported.
bool USBMSDHandler::USBCallback_setConfiguration(uint8_t configuration) {
    if (configuration != DEFAULT_CONFIGURATION) {
        return false;
    }
    XPCC_LOG_DEBUG .printf("USBMSDHandler::USBCallback_setConfiguration()\n");
    initialize();

    // Configure endpoints > 0
    device->addEndpoint(bulkIn, MAX_PACKET_SIZE_EPBULK);
    device->addEndpoint(bulkOut, MAX_PACKET_SIZE_EPBULK);

    //activate readings
    device->readStart(bulkOut, MAX_PACKET_SIZE_EPBULK);
    return true;
}


} /* namespace xpcc */
