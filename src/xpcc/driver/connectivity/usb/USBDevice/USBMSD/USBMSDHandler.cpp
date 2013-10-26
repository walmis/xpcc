/*
 * USBMSDHandler.cpp
 *
 *  Created on: Oct 26, 2013
 *      Author: walmis
 */

#include "USBMSDHandler.h"

namespace xpcc {


#define DISK_OK         0x00
#define NO_INIT         0x01
#define NO_DISK         0x02
#define WRITE_PROTECT   0x04

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


USBMSDHandler::USBMSDHandler(USBDevice* device, uint8_t bulkIn, uint8_t bulkOut) :
		bulkIn(bulkIn), bulkOut(bulkOut), USBInterfaceHandler(device) {
	stage = READ_CBW;
	page = 0;
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

    // get memory size
    MemorySize = disk_size();

    if (BlockCount > 0) {
        BlockSize = MemorySize / BlockCount;
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
	            device->stallEndpoint(EPBULK_IN);
	            sendCSW();
	            break;

	            // the host has received the CSW -> we wait a CBW
	        case WAIT_CSW:
	            stage = READ_CBW;
	            break;
	    }
	    return true;

	} else

	if(ep == bulkOut) {

	    uint32_t size = 0;
	    uint8_t buf[MAX_PACKET_SIZE_EPBULK];
	    device->readEP(EPBULK_OUT, buf, &size, MAX_PACKET_SIZE_EPBULK);
	    switch (stage) {
	            // the device has to decode the CBW received
	        case READ_CBW:
	            CBWDecode(buf, size);
	            break;

	            // the device has to receive data from the host
	        case PROCESS_CBW:
	            switch (cbw.CB[0]) {
	                case WRITE10:
	                case WRITE12:
	                    memoryWrite(buf, size);
	                    break;
	                case VERIFY10:
	                    memoryVerify(buf, size);
	                    break;
	            }
	            break;

	            // an error has occured: stall endpoint and send CSW
	        default:
	            device->stallEndpoint(EPBULK_OUT);
	            csw.Status = CSW_ERROR;
	            sendCSW();
	            break;
	    }

	    //reactivate readings on the OUT bulk endpoint
	    device->readStart(EPBULK_OUT, MAX_PACKET_SIZE_EPBULK);
	    return true;
	}
	return false;
}


void USBMSDHandler::memoryWrite (uint8_t * buf, uint16_t size) {

    if ((addr + size) > MemorySize) {
        size = MemorySize - addr;
        stage = ERROR;
        device->stallEndpoint(EPBULK_OUT);
    }

    // we fill an array in RAM of 1 block before writing it in memory
    for (int i = 0; i < size; i++)
        page[addr%BlockSize + i] = buf[i];

    // if the array is filled, write it in memory
    if (!((addr + size)%BlockSize)) {
        if (!(disk_status() & WRITE_PROTECT)) {
            disk_write(page, addr/BlockSize);
        }
    }

    addr += size;
    length -= size;
    csw.DataResidue -= size;

    if ((!length) || (stage != PROCESS_CBW)) {
        csw.Status = (stage == ERROR) ? CSW_FAILED : CSW_PASSED;
        sendCSW();
    }
}

void USBMSDHandler::memoryVerify (uint8_t * buf, uint16_t size) {
    uint32_t n;

    if ((addr + size) > MemorySize) {
        size = MemorySize - addr;
        stage = ERROR;
        device->stallEndpoint(EPBULK_OUT);
    }

    // beginning of a new block -> load a whole block in RAM
    if (!(addr%BlockSize))
        disk_read(page, addr/BlockSize);

    // info are in RAM -> no need to re-read memory
    for (n = 0; n < size; n++) {
        if (page[addr%BlockSize + n] != buf[n]) {
            memOK = false;
            break;
        }
    }

    addr += size;
    length -= size;
    csw.DataResidue -= size;

    if ( !length || (stage != PROCESS_CBW)) {
        csw.Status = (memOK && (stage == PROCESS_CBW)) ? CSW_PASSED : CSW_FAILED;
        sendCSW();
    }
}


bool USBMSDHandler::inquiryRequest (void) {
    uint8_t inquiry[] = { 0x00, 0x80, 0x00, 0x01,
                          36 - 4, 0x80, 0x00, 0x00,
                          'M', 'B', 'E', 'D', '.', 'O', 'R', 'G',
                          'M', 'B', 'E', 'D', ' ', 'U', 'S', 'B', ' ', 'D', 'I', 'S', 'K', ' ', ' ', ' ',
                          '1', '.', '0', ' ',
                        };
    if (!write(inquiry, sizeof(inquiry))) {
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

    if (!device->writeNB(EPBULK_IN, buf, size, MAX_PACKET_SIZE_EPBULK)) {
        return false;
    }

    csw.DataResidue -= size;
    csw.Status = CSW_PASSED;
    return true;
}


bool USBMSDHandler::modeSense6 (void) {
    uint8_t sense6[] = { 0x03, 0x00, 0x00, 0x00 };
    if (!write(sense6, sizeof(sense6))) {
        return false;
    }
    return true;
}

void USBMSDHandler::sendCSW() {
    csw.Signature = CSW_Signature;
    device->writeNB(EPBULK_IN, (uint8_t *)&csw, sizeof(CSW), MAX_PACKET_SIZE_EPBULK);
    stage = WAIT_CSW;
}

bool USBMSDHandler::requestSense (void) {
    uint8_t request_sense[] = {
        0x70,
        0x00,
        0x05,   // Sense Key: illegal request
        0x00,
        0x00,
        0x00,
        0x00,
        0x0A,
        0x00,
        0x00,
        0x00,
        0x00,
        0x30,
        0x01,
        0x00,
        0x00,
        0x00,
        0x00,
    };

    if (!write(request_sense, sizeof(request_sense))) {
        return false;
    }

    return true;
}

void USBMSDHandler::fail() {
    csw.Status = CSW_FAILED;
    sendCSW();
}


void USBMSDHandler::CBWDecode(uint8_t * buf, uint16_t size) {
    if (size == sizeof(cbw)) {
        memcpy((uint8_t *)&cbw, buf, size);
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
                                device->stallEndpoint(EPBULK_OUT);
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
                                device->stallEndpoint(EPBULK_IN);
                                csw.Status = CSW_ERROR;
                                sendCSW();
                            }
                        }
                        break;
                    case VERIFY10:
                        if (!(cbw.CB[1] & 0x02)) {
                            csw.Status = CSW_PASSED;
                            sendCSW();
                            break;
                        }
                        if (infoTransfer()) {
                            if (!(cbw.Flags & 0x80)) {
                                stage = PROCESS_CBW;
                                memOK = true;
                            } else {
                                device->stallEndpoint(EPBULK_IN);
                                csw.Status = CSW_ERROR;
                                sendCSW();
                            }
                        }
                        break;
                    case MEDIA_REMOVAL:
                        csw.Status = CSW_PASSED;
                        sendCSW();
                        break;
                    default:
                        fail();
                        break;
                }
            }
        }
    }
}

void USBMSDHandler::testUnitReady (void) {

    if (cbw.DataLength != 0) {
        if ((cbw.Flags & 0x80) != 0) {
            device->stallEndpoint(EPBULK_IN);
        } else {
            device->stallEndpoint(EPBULK_OUT);
        }
    }

    csw.Status = CSW_PASSED;
    sendCSW();
}


void USBMSDHandler::memoryRead (void) {
    uint32_t n;

    n = (length > MAX_PACKET) ? MAX_PACKET : length;

    if ((addr + n) > MemorySize) {
        n = MemorySize - addr;
        stage = ERROR;
    }

    // we read an entire block
    if (!(addr%BlockSize))
        disk_read(page, addr/BlockSize);

    // write data which are in RAM
    device->writeNB(EPBULK_IN, &page[addr%BlockSize], n, MAX_PACKET_SIZE_EPBULK);

    addr += n;
    length -= n;

    csw.DataResidue -= n;

    if ( !length || (stage != PROCESS_CBW)) {
        csw.Status = (stage == PROCESS_CBW) ? CSW_PASSED : CSW_FAILED;
        stage = (stage == PROCESS_CBW) ? SEND_CSW : stage;
    }
}


bool USBMSDHandler::infoTransfer (void) {
    uint32_t n;

    // Logical Block Address of First Block
    n = (cbw.CB[2] << 24) | (cbw.CB[3] << 16) | (cbw.CB[4] <<  8) | (cbw.CB[5] <<  0);

    addr = n * BlockSize;

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

    length = n * BlockSize;

    if (!cbw.DataLength) {              // host requests no data
        csw.Status = CSW_FAILED;
        sendCSW();
        return false;
    }

    if (cbw.DataLength != length) {
        if ((cbw.Flags & 0x80) != 0) {
            device->stallEndpoint(EPBULK_IN);
        } else {
            device->stallEndpoint(EPBULK_OUT);
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

    // Configure endpoints > 0
    device->addEndpoint(EPBULK_IN, MAX_PACKET_SIZE_EPBULK);
    device->addEndpoint(EPBULK_OUT, MAX_PACKET_SIZE_EPBULK);

    //activate readings
    device->readStart(EPBULK_OUT, MAX_PACKET_SIZE_EPBULK);
    return true;
}


} /* namespace xpcc */
