#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include "fat32.h"

u8 buffer[512];
u8 fileBuffer[512];

void printInfo(struct fat32_Info *info)
{
    printf("Bytes per sector: %u\n", info->bytesPerSector);
    printf("Sectors per cluster: %u\n", info->sectorsPerCluster);
    printf("Reserved sectors: %u\n", info->numberOfReservedSectors);
    printf("FATs: %u\n", info->numberOfFATs);
    printf("Sectors per FAT: %u\n", info->sectorsPerFAT);
    printf("Total sectors: %u\n", info->numberOfSectors);
}

void printFileEntry(const u8 entry[32])
{
    printf("Name: ");
    fwrite(entry, 1, 8, stdout);
    printf(" Ext: ");
    fwrite(&entry[0x08], 1, 3, stdout);
    printf(" Attrib: %x", entry[0x0B]);
    printf(" cluster: %u\n", *((u16*)&entry[FAT32_FILE_FILEFIRSTCLUSTER0]) | ((*(u16*)&entry[FAT32_FILE_FILEFIRSTCLUSTER1])<<8));
}

int main(int argc, char **argv)
{
    int device;
    struct fat32_Info info;

    if (argc < 2)
    {
        puts("Too less arguments");
        return 1;
    }

    device = open(argv[1], O_RDONLY);
    if (device == -1)
    {
        puts("open() failed!");
        return 1;
    }

    if (read(device, buffer, 512) != 512)
    {
        puts("read() failed!");
        return 1;
    }

    fat32_readInfo(buffer, &info);
    printInfo(&info);

    //read FAT sector
    if ( pread(device, buffer, 512, (info.numberOfReservedSectors + info.numberOfFATs * info.sectorsPerFAT) * info.bytesPerSector) != 512 )
    {
        puts("pread() failed!");
        return 1;
    }

    for (size_t i=0;i<16;i++)
    {
        const u8 *ptr = &buffer[i*32];

        //empty entry
        if (ptr[0] == 0x00 || ptr[0] == 0xE5)
            continue;

        //only dirs
        if (!(ptr[FAT32_FILE_FILEATTRIBUTS] & FAT32_ATTRIB_DIRECTORY))
            continue;

        printFileEntry(ptr);

        puts("Enter directory...");
        u32 offset = info.sectorsPerCluster * ((*(u16*)&ptr[FAT32_FILE_FILEFIRSTCLUSTER0]) | ((*(u16*)&ptr[FAT32_FILE_FILEFIRSTCLUSTER1])<<8)) * info.bytesPerSector;
        pread(device, fileBuffer, 512, offset);

        for (size_t u=0;u<16;u++)
        {
            const u8 *ptrDir = &fileBuffer[u*32];

            //empty entry
            if (ptrDir[0] == 0x00 || ptrDir[0] == 0xE5)
                continue;

            printFileEntry(ptrDir);
        }
    }

    close(device);

    return 0;
}
