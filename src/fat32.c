//FAT32 Driver

#include "fat32.h"
#include <string.h>

void fat32_readInfo(const void *firstSector, struct fat32_Info *info)
{
    const u8 *sector = firstSector;

    info->bytesPerSector = *(u16*)&sector[FAT32_INFO_BYTESPERSECTOR];
    info->sectorsPerCluster = sector[FAT32_INFO_SECTORSPERCLUSTER];
    info->numberOfReservedSectors = *(u16*)&sector[FAT32_INFO_NUMBEROFRESERVEDSECTORS];
    info->numberOfFATs = sector[FAT32_INFO_NUMBEROFFATS];
    info->sectorsPerFAT = *(u32*)&sector[FAT32_INFO_SECTORSPERFAT];
    info->numberOfSectors = *(u32*)&sector[FAT32_INFO_NUMBEROFSECTORS];
}

void fat32_readFileEntry(const void *data, struct fat32_FileEntry *entry)
{
    const u8 *ptr = data;

    memcpy(entry->name, &ptr[0x00], 8);         //Name
    memcpy(entry->extension, &ptr[0x08], 3);    //Extension
    entry->attributs = ptr[0x0B];               //Fileattributs
    memcpy(entry->creationDate, &ptr[0x0D], 5); //CreationDate
    memcpy(entry->lastAccessDate, &ptr[0x12], 2); //LastAccessDate

    //Firstcluster
    const u16 *ptr0 = &ptr[0x1A]; //LowestBits
    entry->firstCluster = *ptr0;
    ptr0 = &ptr[0x12]; //HighestBits
    entry->firstCluster = (*ptr0<<16) | entry->firstCluster;

    memcpy(entry->lastChangeDate, &ptr[0x16], 4); //LastChangeDate
    memcpy(entry->sizeInBytes, &ptr[0x1C], 4);
}
