#include<stdio.h>
#include<stdint.h>

#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

#define EI_NIDENT 16

#define ElfN_Addr       uint64_t
#define ElfN_Off        uint64_t
#define ElfN_Section    uint16_t
#define ElfN_Versym     uint16_t
#define Elf_Byte        unsigned char
#define ElfN_Half       uint16_t
#define ElfN_Sword      int32_t
#define ElfN_Word       uint32_t
#define ElfN_Sxword     int64_t
#define ElfN_Xword      uint64_t

typedef struct {
    unsigned char e_ident[EI_NIDENT];
    uint16_t      e_type;
    uint16_t      e_machine;
    uint32_t      e_version;
    ElfN_Addr     e_entry;
    ElfN_Off      e_phoff;
    ElfN_Off      e_shoff;
    uint32_t      e_flags;
    uint16_t      e_ehsize;
    uint16_t      e_phentsize;
    uint16_t      e_phnum;
    uint16_t      e_shentsize;
    uint16_t      e_shnum;
    uint16_t      e_shstrndx;
} ElfN_Ehdr;

int main()
{
    FILE* fileptr;
    if ((fileptr = fopen("code-oss", "rb+")) == NULL)
    {
        printf("Fail to open file!");
        exit(1);
    }

    fseek(fileptr, 0, SEEK_END);          // Jump to the end of the file
    long filelen = ftell(fileptr);        // Get the current byte offset in the file
    rewind(fileptr);                      // Jump back to the beginning of the file

    char *buffer = (char *)malloc(filelen * sizeof(char)); // Enough memory for the file
    fread(buffer, filelen, 1, fileptr);   // Read in the entire file
    fclose(fileptr);                      // Close the file

    ElfN_Ehdr* elf = (ElfN_Ehdr*)buffer;
    printf("hello %d\n", elf->e_type);
    return 0;
}
