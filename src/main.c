#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include "fat32.h"

u8 buffer[512];
//u8 fileBuffer[512];

void printInfo(struct fat32_Info *info)
{
    printf("Bytes per sector: %u\n", info->bytesPerSector);
    printf("Sectors per cluster: %u\n", info->sectorsPerCluster);
    printf("Reserved sectors: %u\n", info->numberOfReservedSectors);
    printf("FATs: %u\n", info->numberOfFATs);
    printf("Sectors per FAT: %u\n", info->sectorsPerFAT);
    printf("Total sectors: %u\n", info->numberOfSectors);
}

void printFileEntry(const struct fat32_FileEntry *entry)
{
    printf("Name: ");
    fwrite(entry->name, 1, 8, stdout);
    printf(" Ext: ");
    fwrite(entry->extension, 1, 3, stdout);

    if (entry->attributs & 0x10) //Directory
        printf(" Directory");
    else
        printf(" File"); //File

    printf(" Attrib: %x", entry->attributs);
    printf(" cluster: %u", entry->firstCluster);
    printf(" size: %u\n", entry->sizeInBytes);
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

    puts("Files in root directory:");

    const u8 *ptr = buffer;
    struct fat32_FileEntry entry;

    while (1)
    {
        fat32_readFileEntry(ptr, &entry);
        ptr += FILEENTRY_SIZE;

        //check if end
        if (entry.name[0] == 0x00)
            break;

        //check if empty entry
        if (entry.name[0] == 0xE5)
            continue;

        printFileEntry(&entry);
    }

    close(device);

    return 0;
}
