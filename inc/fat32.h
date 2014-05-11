#ifndef FAT32_H
#define FAT32_H

#include <stdint.h>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;

typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;

#define FILEENTRY_SIZE 32

struct fat32_Info
{
    u16 bytesPerSector;
    u8 sectorsPerCluster;
    u16 numberOfReservedSectors;
    u8 numberOfFATs;
    u32 sectorsPerFAT;
    u32 numberOfSectors;
};

struct fat32_FileEntry
{
    char name[8];
    char extension[3];
    u8 attributs;
    u8 creationDate[5];
    u16 lastAccessDate;
    u32 lastChangeDate;
    u32 firstCluster;
    u32 sizeInBytes;
};

enum FAT32_INFO
{
    FAT32_INFO_BYTESPERSECTOR = 0x0B, FAT32_INFO_SECTORSPERCLUSTER = 0x0D, FAT32_INFO_NUMBEROFRESERVEDSECTORS = 0x0E,
    FAT32_INFO_NUMBEROFFATS = 0x10, FAT32_INFO_SECTORSPERFAT = 0x24, FAT32_INFO_NUMBEROFSECTORS = 0x20
};

enum FAT32_FILE
{
    FAT32_FILE_FILENAME = 0x00, FAT32_FILE_FILEEXT = 0x08, FAT32_FILE_FILEATTRIBUTS = 0x0B,
    FAT32_FILE_FILECREATIONDATE = 0x0D, FAT32_FILE_FILELASTACCESSDATE = 0x12, FAT32_FILE_FILEFIRSTCLUSTER1 = 0x14,
    FAT32_FILE_FILELASTCHANGEDATE = 0x16, FAT32_FILE_FILEFIRSTCLUSTER0 = 0x1A, FAT32_FILE_FILESIZE = 0x1C
};

enum FAT32_FILE_ATTRIB
{
    FAT32_ATTRIB_READONLY = 0x01, FAT32_ATTRIB_HIDDEN = 0x02, FAT32_ATTRIB_SYSTEM = 0x04,
    FAT32_ATTRIB_VOLUMEID = 0x08, FAT32_ATTRIB_DIRECTORY = 0x10, FAT32_ATTRIB_ARCHIVE = 0x20,
    FAT32_ATTRIB_LONGNAME = 0x0F
};

void fat32_readInfo(const void *firstSector, struct fat32_Info *info);
void fat32_readFileEntry(const void *data, struct fat32_FileEntry *entry);

#endif
