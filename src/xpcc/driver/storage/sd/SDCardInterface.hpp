/*
 * SDCardInterface.hpp
 *
 *  Created on: May 16, 2015
 *      Author: walmis
 */

#ifndef SRC_XPCC_DRIVER_STORAGE_SD_SDCARDINTERFACE_HPP_
#define SRC_XPCC_DRIVER_STORAGE_SD_SDCARDINTERFACE_HPP_

#include <stdint.h>
#include <xpcc/architecture.hpp>

#define R1_IDLE_STATE           (1 << 0)
#define R1_ERASE_RESET          (1 << 1)
#define R1_ILLEGAL_COMMAND      (1 << 2)
#define R1_COM_CRC_ERROR        (1 << 3)
#define R1_ERASE_SEQUENCE_ERROR (1 << 4)
#define R1_ADDRESS_ERROR        (1 << 5)
#define R1_PARAMETER_ERROR      (1 << 6)

#define SD_COMMAND_TIMEOUT 50

#define SD_CMD_GO_IDLE_STATE                       ((uint8_t)0)
#define SD_CMD_SEND_OP_COND                        ((uint8_t)1)
#define SD_CMD_ALL_SEND_CID                        ((uint8_t)2)
#define SD_CMD_SET_REL_ADDR                        ((uint8_t)3) /*!< SDIO_SEND_REL_ADDR for SD Card */
#define SD_CMD_SET_DSR                             ((uint8_t)4)
#define SD_CMD_SDIO_SEN_OP_COND                    ((uint8_t)5)
#define SD_CMD_HS_SWITCH                           ((uint8_t)6)
#define SD_CMD_SEL_DESEL_CARD                      ((uint8_t)7)
#define SD_CMD_HS_SEND_EXT_CSD                     ((uint8_t)8)
#define SD_CMD_SEND_CSD                            ((uint8_t)9)
#define SD_CMD_SEND_CID                            ((uint8_t)10)
#define SD_CMD_READ_DAT_UNTIL_STOP                 ((uint8_t)11) /*!< SD Card doesn't support it */
#define SD_CMD_STOP_TRANSMISSION                   ((uint8_t)12)
#define SD_CMD_SEND_STATUS                         ((uint8_t)13)
#define SD_CMD_HS_BUSTEST_READ                     ((uint8_t)14)
#define SD_CMD_GO_INACTIVE_STATE                   ((uint8_t)15)
#define SD_CMD_SET_BLOCKLEN                        ((uint8_t)16)
#define SD_CMD_READ_SINGLE_BLOCK                   ((uint8_t)17)
#define SD_CMD_READ_MULT_BLOCK                     ((uint8_t)18)
#define SD_CMD_HS_BUSTEST_WRITE                    ((uint8_t)19)
#define SD_CMD_WRITE_DAT_UNTIL_STOP                ((uint8_t)20) /*!< SD Card doesn't support it */
#define SD_CMD_SET_BLOCK_COUNT                     ((uint8_t)23) /*!< SD Card doesn't support it */
#define SD_CMD_WRITE_SINGLE_BLOCK                  ((uint8_t)24)
#define SD_CMD_WRITE_MULT_BLOCK                    ((uint8_t)25)
#define SD_CMD_PROG_CID                            ((uint8_t)26) /*!< reserved for manufacturers */
#define SD_CMD_PROG_CSD                            ((uint8_t)27)
#define SD_CMD_SET_WRITE_PROT                      ((uint8_t)28)
#define SD_CMD_CLR_WRITE_PROT                      ((uint8_t)29)
#define SD_CMD_SEND_WRITE_PROT                     ((uint8_t)30)
#define SD_CMD_SD_ERASE_GRP_START                  ((uint8_t)32) /*!< To set the address of the first write
                                                                  block to be erased. (For SD card only) */
#define SD_CMD_SD_ERASE_GRP_END                    ((uint8_t)33) /*!< To set the address of the last write block of the
                                                                  continuous range to be erased. (For SD card only) */
#define SD_CMD_ERASE_GRP_START                     ((uint8_t)35) /*!< To set the address of the first write block to be erased.
                                                                  (For MMC card only spec 3.31) */

#define SD_CMD_ERASE_GRP_END                       ((uint8_t)36) /*!< To set the address of the last write block of the
                                                                  continuous range to be erased. (For MMC card only spec 3.31) */

#define SD_CMD_ERASE                               ((uint8_t)38)
#define SD_CMD_FAST_IO                             ((uint8_t)39) /*!< SD Card doesn't support it */
#define SD_CMD_GO_IRQ_STATE                        ((uint8_t)40) /*!< SD Card doesn't support it */
#define SD_CMD_LOCK_UNLOCK                         ((uint8_t)42)
#define SD_CMD_APP_CMD                             ((uint8_t)55)
#define SD_CMD_GEN_CMD                             ((uint8_t)56)
#define SD_CMD_NO_CMD                              ((uint8_t)64)

/**
  * @brief Following commands are SD Card Specific commands.
  *        SDIO_APP_CMD should be sent before sending these commands.
  */
#define SD_CMD_APP_SD_SET_BUSWIDTH                 ((uint8_t)6)  /*!< For SD Card only */
#define SD_CMD_SD_APP_STAUS                        ((uint8_t)13) /*!< For SD Card only */
#define SD_CMD_SD_APP_SEND_NUM_WRITE_BLOCKS        ((uint8_t)22) /*!< For SD Card only */
#define SD_CMD_SD_APP_OP_COND                      ((uint8_t)41) /*!< For SD Card only */
#define SD_CMD_SD_APP_SET_CLR_CARD_DETECT          ((uint8_t)42) /*!< For SD Card only */
#define SD_CMD_SD_APP_SEND_SCR                     ((uint8_t)51) /*!< For SD Card only */
#define SD_CMD_SDIO_RW_DIRECT                      ((uint8_t)52) /*!< For SD I/O Card only */
#define SD_CMD_SDIO_RW_EXTENDED                    ((uint8_t)53) /*!< For SD I/O Card only */

/**
  * @brief Following commands are SD Card Specific security commands.
  *        SDIO_APP_CMD should be sent before sending these commands.
  */
#define SD_CMD_SD_APP_GET_MKB                      ((uint8_t)43) /*!< For SD Card only */
#define SD_CMD_SD_APP_GET_MID                      ((uint8_t)44) /*!< For SD Card only */
#define SD_CMD_SD_APP_SET_CER_RN1                  ((uint8_t)45) /*!< For SD Card only */
#define SD_CMD_SD_APP_GET_CER_RN2                  ((uint8_t)46) /*!< For SD Card only */
#define SD_CMD_SD_APP_SET_CER_RES2                 ((uint8_t)47) /*!< For SD Card only */
#define SD_CMD_SD_APP_GET_CER_RES1                 ((uint8_t)48) /*!< For SD Card only */
#define SD_CMD_SD_APP_SECURE_READ_MULTIPLE_BLOCK   ((uint8_t)18) /*!< For SD Card only */
#define SD_CMD_SD_APP_SECURE_WRITE_MULTIPLE_BLOCK  ((uint8_t)25) /*!< For SD Card only */
#define SD_CMD_SD_APP_SECURE_ERASE                 ((uint8_t)38) /*!< For SD Card only */
#define SD_CMD_SD_APP_CHANGE_SECURE_AREA           ((uint8_t)49) /*!< For SD Card only */
#define SD_CMD_SD_APP_SECURE_WRITE_MKB             ((uint8_t)48) /*!< For SD Card only */

#define SDIO_SEND_IF_COND               ((uint32_t)0x00000008)

enum class SDCardState
{
  SD_CARD_READY                  = ((uint32_t)0x00000001),
  SD_CARD_IDENTIFICATION         = ((uint32_t)0x00000002),
  SD_CARD_STANDBY                = ((uint32_t)0x00000003),
  SD_CARD_TRANSFER               = ((uint32_t)0x00000004),
  SD_CARD_SENDING                = ((uint32_t)0x00000005),
  SD_CARD_RECEIVING              = ((uint32_t)0x00000006),
  SD_CARD_PROGRAMMING            = ((uint32_t)0x00000007),
  SD_CARD_DISCONNECTED           = ((uint32_t)0x00000008),
  SD_CARD_ERROR                  = ((uint32_t)0x000000FF)
};

 //------------------------------------------------------------------------------
 struct CID {
   // byte 0
   uint8_t mid;  // Manufacturer ID
   // byte 1-2
   char oid[2];  // OEM/Application ID
   // byte 3-7
   char pnm[5];  // Product name
   // byte 8
   uint16_t prv_m : 4;  // Product revision n.m
   uint16_t prv_n : 4;
   // byte 9-12
   uint32_t psn;  // Product serial number
   // byte 13
   uint16_t mdt_year_high : 4;  // Manufacturing date
   uint16_t reserved : 4;
   // byte 14
   uint16_t mdt_month : 4;
   uint16_t mdt_year_low :4;
   // byte 15
   uint16_t always1 : 1;
   uint16_t crc : 7;
 }__attribute__ ((packed));
 typedef struct CID cid_t;
 //------------------------------------------------------------------------------
 // CSD for version 1.00 cards
 struct CSDV1 {
   // byte 0
   uint16_t reserved1 : 6;
   uint16_t csd_ver : 2;
   // byte 1
   uint8_t taac;
   // byte 2
   uint8_t nsac;
   // byte 3
   uint8_t tran_speed;
   // byte 4
   uint8_t ccc_high;
   // byte 5
   uint16_t read_bl_len : 4;
   uint16_t ccc_low : 4;
   // byte 6
   uint16_t c_size_high : 2;
   uint16_t reserved2 : 2;
   uint16_t dsr_imp : 1;
   uint16_t read_blk_misalign :1;
   uint16_t write_blk_misalign : 1;
   uint16_t read_bl_partial : 1;
   // byte 7
   uint8_t c_size_mid;
   // byte 8
   uint16_t vdd_r_curr_max : 3;
   uint16_t vdd_r_curr_min : 3;
   uint16_t c_size_low :2;
   // byte 9
   uint16_t c_size_mult_high : 2;
   uint16_t vdd_w_cur_max : 3;
   uint16_t vdd_w_curr_min : 3;
   // byte 10
   uint16_t sector_size_high : 6;
   uint16_t erase_blk_en : 1;
   uint16_t c_size_mult_low : 1;
   // byte 11
   uint16_t wp_grp_size : 7;
   uint16_t sector_size_low : 1;
   // byte 12
   uint16_t write_bl_len_high : 2;
   uint16_t r2w_factor : 3;
   uint16_t reserved3 : 2;
   uint16_t wp_grp_enable : 1;
   // byte 13
   uint16_t reserved4 : 5;
   uint16_t write_partial : 1;
   uint16_t write_bl_len_low : 2;
   // byte 14
   uint16_t reserved5: 2;
   uint16_t file_format : 2;
   uint16_t tmp_write_protect : 1;
   uint16_t perm_write_protect : 1;
   uint16_t copy : 1;
   uint16_t file_format_grp : 1;
   // byte 15
   uint16_t always1 : 1;
   uint16_t crc : 7;
 }__attribute__ ((packed));
 typedef CSDV1 csd1_t;
 //------------------------------------------------------------------------------
 // CSD for version 2.00 cards
 struct CSDV2 {
   // byte 0
   uint16_t reserved1 : 6;
   uint16_t csd_ver : 2;
   // byte 1
   uint8_t taac;
   // byte 2
   uint8_t nsac;
   // byte 3
   uint8_t tran_speed;
   // byte 4
   uint8_t ccc_high;
   // byte 5
   uint16_t read_bl_len : 4;
   uint16_t ccc_low : 4;
   // byte 6
   uint16_t reserved2 : 4;
   uint16_t dsr_imp : 1;
   uint16_t read_blk_misalign :1;
   uint16_t write_blk_misalign : 1;
   uint16_t read_bl_partial : 1;
   // byte 7
   uint16_t reserved3 : 2;
   uint16_t c_size_high : 6;
   // byte 8
   uint8_t c_size_mid;
   // byte 9
   uint8_t c_size_low;
   // byte 10
   uint16_t sector_size_high : 6;
   uint16_t erase_blk_en : 1;
   uint16_t reserved4 : 1;
   // byte 11
   uint16_t wp_grp_size : 7;
   uint16_t sector_size_low : 1;
   // byte 12
   uint16_t write_bl_len_high : 2;
   uint16_t r2w_factor : 3;
   uint16_t reserved5 : 2;
   uint16_t wp_grp_enable : 1;
   // byte 13
   uint16_t reserved6 : 5;
   uint16_t write_partial : 1;
   uint16_t write_bl_len_low : 2;
   // byte 14
   uint16_t reserved7: 2;
   uint16_t file_format : 2;
   uint16_t tmp_write_protect : 1;
   uint16_t perm_write_protect : 1;
   uint16_t copy : 1;
   uint16_t file_format_grp : 1;
   // byte 15
   uint16_t always1 : 1;
   uint16_t crc : 7;
 }__attribute__ ((packed));
 typedef CSDV2 csd2_t;
 //------------------------------------------------------------------------------
 // union of old and new style CSD register
 union csd_t {
   csd1_t v1;
   csd2_t v2;
 };

class SDCardInterface {
public:

	bool init();

	bool isInitialized();

	//------------------------------------------------------------------------------
	/** Begin a read multiple blocks sequence.
	 *
	 * \return The value one, true, is returned for success and
	 * the value zero, false, is returned for failure.
	 */
	bool readStart(uint32_t blockNumber);

	//------------------------------------------------------------------------------
	/** End a read multiple blocks sequence.
	 *
	 * \return The value one, true, is returned for success and
	 * the value zero, false, is returned for failure.
	 */
	bool readStop();

	bool readData(uint8_t* buffer, size_t length);

	bool readSingleBlock(uint8_t* buffer, size_t block_number);

	//------------------------------------------------------------------------------
	/** Start a write multiple blocks sequence.
	 *
	 * \param[in] blockNumber Address of first block in sequence.
	 * \param[in] eraseCount The number of blocks to be pre-erased.
	 *
	 * \note This function is used with writeData() and writeStop()
	 * for optimized multiple block writes.
	 *
	 * \return The value one, true, is returned for success and
	 * the value zero, false, is returned for failure.
	 */
	bool writeStart(uint32_t blockNumber, uint32_t eraseCount);
	//------------------------------------------------------------------------------
	/** End a write multiple blocks sequence.
	 *
	 * \return The value one, true, is returned for success and
	 * the value zero, false, is returned for failure.
	 */
	bool writeStop();

	//------------------------------------------------------------------------------
	/** Write one data block in a multiple block write sequence
	 * \param[in] src Pointer to the location of the data to be written.
	 * \return The value one, true, is returned for success and
	 * the value zero, false, is returned for failure.
	 */
	bool writeData(const uint8_t* src);

	//------------------------------------------------------------------------------
	// send one block of data for write block or write multiple blocks
	bool writeData(uint8_t token, const uint8_t* src);

	//write a single block to SD card
	bool writeBlock(uint32_t blockNumber, const uint8_t* src);

	uint32_t getSectorCount();

	xpcc::Semaphore* semaphore();

protected:
	bool initialized;

};


#endif /* SRC_XPCC_DRIVER_STORAGE_SD_SDCARDINTERFACE_HPP_ */
