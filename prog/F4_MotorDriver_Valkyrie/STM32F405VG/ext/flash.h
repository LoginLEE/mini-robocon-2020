#ifndef __FLASH_LIB_H
#define __FLASH_LIB_H

#include "stm32f4xx.h"
#include "stm32f4xx_flash.h"

#define FLASH_VOLTAGE_RANGE VoltageRange_3

#define FLASH_SECTOR_TABLE(X) \
	X(FLASH_SECTOR_0,  0x08000000, 0x08003FFF, FLASH_Sector_0) \
	X(FLASH_SECTOR_1,  0x08004000, 0x08007FFF, FLASH_Sector_1) \
	X(FLASH_SECTOR_2,  0x08008000, 0x0800BFFF, FLASH_Sector_2) \
	X(FLASH_SECTOR_3,  0x0800C000, 0x0800FFFF, FLASH_Sector_3) \
	X(FLASH_SECTOR_4,  0x08010000, 0x0801FFFF, FLASH_Sector_4) \
	X(FLASH_SECTOR_5,  0x08020000, 0x0803FFFF, FLASH_Sector_5) \
	X(FLASH_SECTOR_6,  0x08040000, 0x0805FFFF, FLASH_Sector_6) \
	X(FLASH_SECTOR_7,  0x08060000, 0x0807FFFF, FLASH_Sector_7) \
	X(FLASH_SECTOR_8,  0x08080000, 0x0809FFFF, FLASH_Sector_8) \
	X(FLASH_SECTOR_9,  0x080A0000, 0x080BFFFF, FLASH_Sector_9) \
	X(FLASH_SECTOR_10, 0x080C0000, 0x080DFFFF, FLASH_Sector_10) \
	X(FLASH_SECTOR_11, 0x080E0000, 0x080FFFFF, FLASH_Sector_11) \

#define FLASH_READ_NOT_FOUND_ERROR 0x01
#define FLASH_READ_ARR_WRONG_LEN   0x02

#define FLASH_ALL_FLAGS (FLASH_FLAG_EOP|FLASH_FLAG_OPERR|FLASH_FLAG_WRPERR|FLASH_FLAG_PGAERR|FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR|FLASH_FLAG_RDERR|FLASH_FLAG_BSY)

#define FLASH_SECTOR_NAME_ENUM(A, B, C, D) A,
typedef enum {
	FLASH_SECTOR_TABLE(FLASH_SECTOR_NAME_ENUM)
} FlashSector;

#define FLASH_SECTOR_COUNTER(A, B, C, D) 1+
static const u8 NUM_OF_FLASH_SECTORS = FLASH_SECTOR_TABLE(FLASH_SECTOR_COUNTER) 0;

void flash_init(void);
void flash_lock(void);

u32 flash_head_ptr(FlashSector sector);

void erase_flash(FlashSector sector);

u32 read_flash(FlashSector sector, u8 tag);
u8 read_flash_byte(FlashSector sector, u8 tag, u8* error);
u32 read_flash_word(FlashSector sector, u8 tag, u8* error);
u16 read_flash_hword(FlashSector sector, u8 tag, u8* error);
uint64_t read_flash_dword(FlashSector sector, u8 tag, u8* error);
void read_flash_arr(FlashSector sector, u8 tag, u8* arr, u8 len, u8* error);

void write_flash_byte(FlashSector sector, u8 tag, u8 byte);
void write_flash_word(FlashSector sector, u8 tag, u32 word);
void write_flash_hword(FlashSector sector, u8 tag, u16 hword);
void write_flash_dword(FlashSector sector, u8 tag, uint64_t dword);
void write_flash_arr(FlashSector sector, u8 tag, u8* arr, u8 len);

#endif
