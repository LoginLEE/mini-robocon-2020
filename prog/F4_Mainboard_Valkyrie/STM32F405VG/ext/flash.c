#include "flash.h"

// static const u8 PROGRAM_JUST_FLASHED = 0xFF;

#define FLASH_SECTOR_DEFS(A, B, C, D) {B, C, D},
static const struct {
	u32 start_addr; u32 end_addr; u16 sector_addr;
} FlashSectorDefs[] = { FLASH_SECTOR_TABLE(FLASH_SECTOR_DEFS) };

#define FLASH_SECTOR_STARTADDR(A, B, C, D) B,
static u32 sector_head_pointers[] = { FLASH_SECTOR_TABLE(FLASH_SECTOR_STARTADDR) };

#define GET_SECTOR_HEAD() sector_head_pointers[sector]
#define GET_SECTOR_DEF() FlashSectorDefs[sector]
#define GET_SECTOR_START() GET_SECTOR_DEF().start_addr
#define GET_SECTOR_END() GET_SECTOR_DEF().end_addr

#define deref(P) (*((u8*)(P)))
#define index(P, I) (*((u8*)(P + I)))
#define derefT(P, T) (*(((T)*)(P)))
#define indexT(P, T, I) (*((((T)*)(P)) + I))
#define flash_byte_not_empty(P) ((~(deref(P))) & 0xFF)
#define flash_not_erased(_) (flash_byte_not_empty(GET_SECTOR_START()))

#define tag_get_len(T) (((T) & 0xE0) >> 5)
#define tag_get_tag(T) ((T) & 0x1F)
#define tag_set_len(T, L) (((T) & 0x1F) | ((L) << 5))
#define TAG_LEN_BYTE 0x00
#define TAG_LEN_HWORD 0x01
#define TAG_LEN_WORD 0x03
#define TAG_LEN_DWORD 0x07
#define TAG_LEN_ARR 0x02

void flash_init() {
	// if (PROGRAM_JUST_FLASHED) {
		// FLASH_Unlock();
		// FLASH_ClearFlag(FLASH_ALL_FLAGS);
		// FLASH_ProgramByte((u32) &PROGRAM_JUST_FLASHED, 0);
		// FLASH_Lock();
	// }

	for (u8 sector=0; sector<NUM_OF_FLASH_SECTORS; sector++) {
		while (flash_byte_not_empty(GET_SECTOR_HEAD()) && (GET_SECTOR_HEAD() < GET_SECTOR_END())) {
			if (tag_get_len(GET_SECTOR_HEAD()) == TAG_LEN_ARR) 
				GET_SECTOR_HEAD() += 2 + index(GET_SECTOR_HEAD(), 1);
			else 
				GET_SECTOR_HEAD() += 1 + tag_get_len(GET_SECTOR_HEAD());
		}
	}
}
u32 flash_head_ptr(FlashSector sector) {
	return GET_SECTOR_HEAD();
}

void flash_lock() {
	// FLASH_Lock();
}

void erase_flash(FlashSector sector) {
	if (flash_not_erased(sector)) {
		FLASH_Unlock();
		FLASH_ClearFlag(FLASH_ALL_FLAGS);
		
		FLASH_EraseSector(FlashSectorDefs[sector].sector_addr, FLASH_VOLTAGE_RANGE);
		
		FLASH_Lock();

		GET_SECTOR_HEAD() = GET_SECTOR_START();
	}
}

u32 read_flash(FlashSector sector, u8 tag) {
	u32 addr = GET_SECTOR_START();
	u32 found_addr = 0;
	while (flash_byte_not_empty(addr) && (addr < GET_SECTOR_END())) {
		if ((tag_get_tag(deref(addr)) == tag)) found_addr = addr;

		if (tag_get_len(addr) == TAG_LEN_ARR) 
			addr += 2 + index(addr, 1);
		else 
			addr += 1 + tag_get_len(addr);
	}
	return found_addr;
}

u8 read_flash_byte(FlashSector sector, u8 tag, u8* error) {
	u32 addr = read_flash(sector, tag);
	if (addr == 0) {
		*error |= FLASH_READ_NOT_FOUND_ERROR;
		return 0;
	}
	return deref(addr+1);
}
u32 read_flash_word(FlashSector sector, u8 tag, u8* error) {
	u32 addr = read_flash(sector, tag);
	if (addr == 0) {
		*error |= FLASH_READ_NOT_FOUND_ERROR;
		return 0;
	}
	u32 temp = 0;
	temp |= deref(addr+1) << 24;
	temp |= deref(addr+2) << 16;
	temp |= deref(addr+3) << 8;
	temp |= deref(addr+4);
	return temp;
}
u16 read_flash_hword(FlashSector sector, u8 tag, u8* error) {
	u32 addr = read_flash(sector, tag);
	if (addr == 0) {
		*error |= FLASH_READ_NOT_FOUND_ERROR;
		return 0;
	}
	u16 temp = 0;
	temp |= deref(addr+1) << 8;
	temp |= deref(addr+2);
	return temp;
}
uint64_t read_flash_dword(FlashSector sector, u8 tag, u8* error) {
	u32 addr = read_flash(sector, tag);
	if (addr == 0) {
		*error |= FLASH_READ_NOT_FOUND_ERROR;
		return 0;
	}
	uint64_t temp = 0;
	temp |= deref(addr+1) << 24;
	temp |= deref(addr+2) << 16;
	temp |= deref(addr+3) << 8;
	temp |= deref(addr+4);
	temp <<= 32;
	temp |= deref(addr+5) << 24;
	temp |= deref(addr+6) << 16;
	temp |= deref(addr+7) << 8;
	temp |= deref(addr+8);
	return temp;
}
void read_flash_arr(FlashSector sector, u8 tag, u8* arr, u8 arrlen, u8* error) {
	u32 addr = read_flash(sector, tag);
	if (addr == 0) {
		*error |= FLASH_READ_NOT_FOUND_ERROR;
		return;
	}
	addr++;
	u8 written_len = deref(addr);
	if (written_len != arrlen) {
		*error |= FLASH_READ_ARR_WRONG_LEN;
		return; //TODO: IDK
	}
	addr++;
	for (u8 i=0; i<arrlen; i++) {
		*(arr+i) = deref(addr+i);
	}
}

void write_flash_byte(FlashSector sector, u8 tag, u8 byte) {
	if (GET_SECTOR_HEAD() + TAG_LEN_BYTE < GET_SECTOR_END()) return;

	FLASH_Unlock();
	FLASH_ClearFlag(FLASH_ALL_FLAGS);

	FLASH_ProgramByte(GET_SECTOR_HEAD()++, tag_set_len(tag, TAG_LEN_BYTE));
	FLASH_ProgramByte(GET_SECTOR_HEAD()++, byte);

	FLASH_Lock();
}
void write_flash_word(FlashSector sector, u8 tag, u32 word) {
	if (GET_SECTOR_HEAD() + TAG_LEN_WORD < GET_SECTOR_END()) return;

	FLASH_Unlock();
	FLASH_ClearFlag(FLASH_ALL_FLAGS);

	FLASH_ProgramByte(GET_SECTOR_HEAD()++, tag_set_len(tag, TAG_LEN_WORD));
	FLASH_ProgramByte(GET_SECTOR_HEAD()++, (u8) word>>24);
	FLASH_ProgramByte(GET_SECTOR_HEAD()++, (u8) word>>16);
	FLASH_ProgramByte(GET_SECTOR_HEAD()++, (u8) word>>8);
	FLASH_ProgramByte(GET_SECTOR_HEAD()++, (u8) word);

	FLASH_Lock();
}
void write_flash_hword(FlashSector sector, u8 tag, u16 hword) {
	if (GET_SECTOR_HEAD() + TAG_LEN_HWORD < GET_SECTOR_END()) return;

	FLASH_Unlock();
	FLASH_ClearFlag(FLASH_ALL_FLAGS);

	FLASH_ProgramByte(GET_SECTOR_HEAD()++, tag_set_len(tag, TAG_LEN_HWORD));
	FLASH_ProgramByte(GET_SECTOR_HEAD()++, (u8) hword>>8);
	FLASH_ProgramByte(GET_SECTOR_HEAD()++, (u8) hword);

	FLASH_Lock();
}
void write_flash_dword(FlashSector sector, u8 tag, uint64_t dword) {
	if (GET_SECTOR_HEAD() + TAG_LEN_DWORD < GET_SECTOR_END()) return;

	FLASH_Unlock();
	FLASH_ClearFlag(FLASH_ALL_FLAGS);

	FLASH_ProgramByte(GET_SECTOR_HEAD()++, tag_set_len(tag, TAG_LEN_DWORD));
	u32 tword = dword >> 32;
	FLASH_ProgramByte(GET_SECTOR_HEAD()++, (u8) tword>>24);
	FLASH_ProgramByte(GET_SECTOR_HEAD()++, (u8) tword>>16);
	FLASH_ProgramByte(GET_SECTOR_HEAD()++, (u8) tword>>8);
	FLASH_ProgramByte(GET_SECTOR_HEAD()++, (u8) tword);
	FLASH_ProgramByte(GET_SECTOR_HEAD()++, (u8) dword>>24);
	FLASH_ProgramByte(GET_SECTOR_HEAD()++, (u8) dword>>16);
	FLASH_ProgramByte(GET_SECTOR_HEAD()++, (u8) dword>>8);
	FLASH_ProgramByte(GET_SECTOR_HEAD()++, (u8) dword);

	FLASH_Lock();
}
void write_flash_arr(FlashSector sector, u8 tag, u8* arr, u8 arrlen) {
	if (GET_SECTOR_HEAD() + 2 + arrlen < GET_SECTOR_END()) return;

	FLASH_Unlock();
	FLASH_ClearFlag(FLASH_ALL_FLAGS);

	FLASH_ProgramByte(GET_SECTOR_HEAD()++, tag_set_len(tag, TAG_LEN_ARR));
	FLASH_ProgramByte(GET_SECTOR_HEAD()++, (u8) arrlen);
	while (arrlen--) {
		FLASH_ProgramByte(GET_SECTOR_HEAD()++, (u8) *(arr++));
	}

	FLASH_Lock();
}
