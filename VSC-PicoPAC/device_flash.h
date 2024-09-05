#ifndef __DEVICE_FLASH_H__
#define __DEVICE_FLASH_H__

//device available flash memmory
#define AVAILABLE_FLASH 8
#define HW_FLASH_STORAGE_BASE  (1024 * 1024)

#define SECTOR_SIZE 512
#define SECTOR_NUM (((AVAILABLE_FLASH - 1) * 1024 * 1024) / SECTOR_SIZE) - 4

// Implements 512 byte FAT sectors on 4096 byte flash sectors.
// Doesn't really implement wear levelling (e.g. the fs_map) so not for heavy use but should be
// fine for the intended use case.

#define NUM_FAT_SECTORS SECTOR_NUM //30716   // 15megs / 512bytes = 30720, but we used 4 records for the header (8 bytes)
#define NUM_FLASH_SECTORS (((AVAILABLE_FLASH - 1) * HW_FLASH_STORAGE_BASE) / 4096) //3840  // 15megs / 4096bytes = 3840

#define debugfat true

#endif