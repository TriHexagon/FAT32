//FAT32 Driver

#include "fat32.h"
#include <string.h>

#define FAT32_HIGHLEVEL

#ifdef FAT32_HIGHLEVEL
#define BUFFER_SIZE 512
u8 buffer[BUFFER_SIZE];

fat32_ReadSectorFunc readSectorFunc = NULL;
fat32_WriteSectorFunc writeSectorFunc = NULL;
#endif

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

    memcpy(entry->name, &ptr[FAT32_FILE_NAME], 8);                          //Name
    memcpy(entry->extension, &ptr[FAT32_FILE_EXT], 3);                      //Extension
    entry->attributs = ptr[FAT32_FILE_ATTRIBUTS];                           //Fileattributs
    memcpy(entry->creationDate, &ptr[FAT32_FILE_CREATIONDATE], 5);          //CreationDate
    memcpy(&entry->lastAccessDate, &ptr[FAT32_FILE_LASTACCESSDATE], 2);     //LastAccessDate

    //Firstcluster
    const u16 *ptr0 = (u16*)&ptr[FAT32_FILE_FIRSTCLUSTER0]; //LowestBits
    entry->firstCluster = *ptr0;
    ptr0 = (u16*)&ptr[FAT32_FILE_FIRSTCLUSTER1]; //HighestBits
    entry->firstCluster = (((u32)*ptr0)<<16) | entry->firstCluster;

    memcpy(&entry->lastChangeDate, &ptr[FAT32_FILE_LASTCHANGEDATE], 4); //LastChangeDate
    memcpy(&entry->sizeInBytes, &ptr[FAT32_FILE_SIZE], 4);
}

#ifdef FAT32_HIGHLEVEL

bool fat32_init(fat32_ReadSectorFunc reader, fat32_WriteSectorFunc writer, fat32_Info* info)
{
    if (reader == NULL)
        return false;

    readSectorFunc = reader;
    writeSectorFunc = writer;

    if (!readSectorFunc(buffer, 0, BUFFER_SIZE))
        return false;

    fat32_readInfo(buffer, info);
    return true;
}

u32 fat32_openFile(struct fat32_Info *info, const char *name)
{
    //check arguments
    if (info == NULL || name == NULL)
        return 0;

    //check read callback
    if (readSector == NULL)
        return 0;

    //read rootdir sector
    if (!readSectorFunc(buffer, (info.numberOfReservedSectors + info.numberOfFATs * info.sectorsPerFAT) * info.bytesPerSector, BUFFER_SIZE))
        return 0;

    //parse name
    while (true)
    {

    }
}

#endif
