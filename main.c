#include<stdio.h>
#include<stdint.h>

#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

#define EI_NIDENT 16

#define Elf64_Addr      uint64_t
#define Elf64_Off       uint64_t
#define Elf64_Section   uint16_t
#define Elf64_Versym    uint16_t
#define Elf_Byte        unsigned char
#define Elf64_Half      uint16_t
#define Elf64_Sword     int32_t
#define Elf64_Word      uint32_t
#define Elf64_Sxword    int64_t
#define Elf64_Xword     uint64_t

typedef struct {
    unsigned char e_ident[EI_NIDENT];
    uint16_t      e_type;
    uint16_t      e_machine;
    uint32_t      e_version;
    Elf64_Addr    e_entry;
    Elf64_Off     e_phoff;
    Elf64_Off     e_shoff;
    uint32_t      e_flags;
    uint16_t      e_ehsize;
    uint16_t      e_phentsize;
    uint16_t      e_phnum;
    uint16_t      e_shentsize;
    uint16_t      e_shnum;
    uint16_t      e_shstrndx;
} ElfN_Ehdr;

typedef struct {
    uint32_t   p_type;
    uint32_t   p_flags;
    Elf64_Off  p_offset;
    Elf64_Addr p_vaddr;
    Elf64_Addr p_paddr;
    uint64_t   p_filesz;
    uint64_t   p_memsz;
    uint64_t   p_align;
} Elf64_Phdr;

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

    char* buffer = (char* )malloc(filelen * sizeof(char)); // Enough memory for the file
    fread(buffer, filelen, 1, fileptr);   // Read in the entire file
    fclose(fileptr);                      // Close the file

    ElfN_Ehdr* elf = (ElfN_Ehdr*)buffer;
    printf("hello %d\n", elf->e_type);
    return 0;
}
