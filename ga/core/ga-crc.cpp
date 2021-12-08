/**
 * @file
 * CRC function implementations
 */

#include "ga-crc.h"

/**
 * CRC5-CCITT table
 * Generate with the commands:
 * - pycrc.py --width 5 --poly 0xB --reflect-in True --xor-in 0x1f --reflect-out True --xor-out 0x1f --algorithm table-driven --generate h -o crc.h 
 * - pycrc.py --width 5 --poly 0xB --reflect-in True --xor-in 0x1f --reflect-out True --xor-out 0x1f --algorithm table-driven --generate c -o crc.c
 */
static const crc5_t crc5_table_ccitt[256] = {
    0x00, 0x58, 0xb0, 0xe8, 0xc8, 0x90, 0x78, 0x20, 0x38, 0x60, 0x88, 0xd0, 0xf0, 0xa8, 0x40, 0x18,
    0x70, 0x28, 0xc0, 0x98, 0xb8, 0xe0, 0x08, 0x50, 0x48, 0x10, 0xf8, 0xa0, 0x80, 0xd8, 0x30, 0x68,
    0xe0, 0xb8, 0x50, 0x08, 0x28, 0x70, 0x98, 0xc0, 0xd8, 0x80, 0x68, 0x30, 0x10, 0x48, 0xa0, 0xf8,
    0x90, 0xc8, 0x20, 0x78, 0x58, 0x00, 0xe8, 0xb0, 0xa8, 0xf0, 0x18, 0x40, 0x60, 0x38, 0xd0, 0x88,
    0x68, 0x30, 0xd8, 0x80, 0xa0, 0xf8, 0x10, 0x48, 0x50, 0x08, 0xe0, 0xb8, 0x98, 0xc0, 0x28, 0x70,
    0x18, 0x40, 0xa8, 0xf0, 0xd0, 0x88, 0x60, 0x38, 0x20, 0x78, 0x90, 0xc8, 0xe8, 0xb0, 0x58, 0x00,
    0x88, 0xd0, 0x38, 0x60, 0x40, 0x18, 0xf0, 0xa8, 0xb0, 0xe8, 0x00, 0x58, 0x78, 0x20, 0xc8, 0x90,
    0xf8, 0xa0, 0x48, 0x10, 0x30, 0x68, 0x80, 0xd8, 0xc0, 0x98, 0x70, 0x28, 0x08, 0x50, 0xb8, 0xe0,
    0xd0, 0x88, 0x60, 0x38, 0x18, 0x40, 0xa8, 0xf0, 0xe8, 0xb0, 0x58, 0x00, 0x20, 0x78, 0x90, 0xc8,
    0xa0, 0xf8, 0x10, 0x48, 0x68, 0x30, 0xd8, 0x80, 0x98, 0xc0, 0x28, 0x70, 0x50, 0x08, 0xe0, 0xb8,
    0x30, 0x68, 0x80, 0xd8, 0xf8, 0xa0, 0x48, 0x10, 0x08, 0x50, 0xb8, 0xe0, 0xc0, 0x98, 0x70, 0x28,
    0x40, 0x18, 0xf0, 0xa8, 0x88, 0xd0, 0x38, 0x60, 0x78, 0x20, 0xc8, 0x90, 0xb0, 0xe8, 0x00, 0x58,
    0xb8, 0xe0, 0x08, 0x50, 0x70, 0x28, 0xc0, 0x98, 0x80, 0xd8, 0x30, 0x68, 0x48, 0x10, 0xf8, 0xa0,
    0xc8, 0x90, 0x78, 0x20, 0x00, 0x58, 0xb0, 0xe8, 0xf0, 0xa8, 0x40, 0x18, 0x38, 0x60, 0x88, 0xd0,
    0x58, 0x00, 0xe8, 0xb0, 0x90, 0xc8, 0x20, 0x78, 0x60, 0x38, 0xd0, 0x88, 0xa8, 0xf0, 0x18, 0x40,
    0x28, 0x70, 0x98, 0xc0, 0xe0, 0xb8, 0x50, 0x08, 0x10, 0x48, 0xa0, 0xf8, 0xd8, 0x80, 0x68, 0x30
};

/**
 * CRC5-USB table
 * Generate with the commands:
 * - pycrc.py --width 5 --poly 0x5 --reflect-in True --xor-in 0x1f --reflect-out True --xor-out 0x1f --algorithm table-driven --generate h -o crc.h 
 * - pycrc.py --width 5 --poly 0x5 --reflect-in True --xor-in 0x1f --reflect-out True --xor-out 0x1f --algorithm table-driven --generate c -o crc.c 
 */
static const crc5_t crc5_table_usb[256] = {
    0x00, 0x70, 0xe0, 0x90, 0x88, 0xf8, 0x68, 0x18, 0x58, 0x28, 0xb8, 0xc8, 0xd0, 0xa0, 0x30, 0x40,
    0xb0, 0xc0, 0x50, 0x20, 0x38, 0x48, 0xd8, 0xa8, 0xe8, 0x98, 0x08, 0x78, 0x60, 0x10, 0x80, 0xf0,
    0x28, 0x58, 0xc8, 0xb8, 0xa0, 0xd0, 0x40, 0x30, 0x70, 0x00, 0x90, 0xe0, 0xf8, 0x88, 0x18, 0x68,
    0x98, 0xe8, 0x78, 0x08, 0x10, 0x60, 0xf0, 0x80, 0xc0, 0xb0, 0x20, 0x50, 0x48, 0x38, 0xa8, 0xd8,
    0x50, 0x20, 0xb0, 0xc0, 0xd8, 0xa8, 0x38, 0x48, 0x08, 0x78, 0xe8, 0x98, 0x80, 0xf0, 0x60, 0x10,
    0xe0, 0x90, 0x00, 0x70, 0x68, 0x18, 0x88, 0xf8, 0xb8, 0xc8, 0x58, 0x28, 0x30, 0x40, 0xd0, 0xa0,
    0x78, 0x08, 0x98, 0xe8, 0xf0, 0x80, 0x10, 0x60, 0x20, 0x50, 0xc0, 0xb0, 0xa8, 0xd8, 0x48, 0x38,
    0xc8, 0xb8, 0x28, 0x58, 0x40, 0x30, 0xa0, 0xd0, 0x90, 0xe0, 0x70, 0x00, 0x18, 0x68, 0xf8, 0x88,
    0xa0, 0xd0, 0x40, 0x30, 0x28, 0x58, 0xc8, 0xb8, 0xf8, 0x88, 0x18, 0x68, 0x70, 0x00, 0x90, 0xe0,
    0x10, 0x60, 0xf0, 0x80, 0x98, 0xe8, 0x78, 0x08, 0x48, 0x38, 0xa8, 0xd8, 0xc0, 0xb0, 0x20, 0x50,
    0x88, 0xf8, 0x68, 0x18, 0x00, 0x70, 0xe0, 0x90, 0xd0, 0xa0, 0x30, 0x40, 0x58, 0x28, 0xb8, 0xc8,
    0x38, 0x48, 0xd8, 0xa8, 0xb0, 0xc0, 0x50, 0x20, 0x60, 0x10, 0x80, 0xf0, 0xe8, 0x98, 0x08, 0x78,
    0xf0, 0x80, 0x10, 0x60, 0x78, 0x08, 0x98, 0xe8, 0xa8, 0xd8, 0x48, 0x38, 0x20, 0x50, 0xc0, 0xb0,
    0x40, 0x30, 0xa0, 0xd0, 0xc8, 0xb8, 0x28, 0x58, 0x18, 0x68, 0xf8, 0x88, 0x90, 0xe0, 0x70, 0x00,
    0xd8, 0xa8, 0x38, 0x48, 0x50, 0x20, 0xb0, 0xc0, 0x80, 0xf0, 0x60, 0x10, 0x08, 0x78, 0xe8, 0x98,
    0x68, 0x18, 0x88, 0xf8, 0xe0, 0x90, 0x00, 0x70, 0x30, 0x40, 0xd0, 0xa0, 0xb8, 0xc8, 0x58, 0x28
};

crc5_t
crc5_reflect(crc5_t data, int data_len) {
	int i;
	crc5_t ret;
	ret = data & 0x01;
	for(i = 1; i < data_len; i++) {
		data >>= 1;
		ret = (ret << 1) | (data & 0x01);
	}
	return ret;
}

/**
 * Common CRC5 update function
 *
 * @param crc [in] Current CRC value.
 * @param data [in] Data to be checksumed.
 * @param data_len [in] Length of the data.
 * @param table [in] The CRC5 table
 * @return Updated CRC value.
 */
crc5_t
crc5_update(crc5_t crc, const unsigned char *data, int data_len, const crc5_t *table) {
	unsigned int tbl_idx;
	while (data_len--) {
		tbl_idx = ((crc >> 3) ^ *data) & 0xff;
		crc = (table[tbl_idx] ^ (crc >> 8)) & (0x1f << 3);
		data++;
	}
	return crc & (0x1f << 3);
}

/**
 * CRC5-USB update function
 *
 * @param crc [in] Current CRC value.
 * @param data [in] Data to be checksumed.
 * @param data_len [in] Length of the data.
 * @return Updated CRC value.
 */
crc5_t
crc5_update_usb(crc5_t crc, const unsigned char *data, int data_len) {
	return crc5_update(crc, data, data_len, crc5_table_usb);
}

/**
 * CRC5-CCITT update function
 *
 * @param crc [in] Current CRC value.
 * @param data [in] Data to be checksumed.
 * @param data_len [in] Length of the data.
 * @return Updated CRC value.
 */
crc5_t
crc5_update_ccitt(crc5_t crc, const unsigned char *data, int data_len) {
	return crc5_update(crc, data, data_len, crc5_table_ccitt);
}
