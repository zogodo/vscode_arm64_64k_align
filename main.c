#include<stdio.h>
#include<stdint.h>

#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

// #include <libelf.h>

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
    if ((fileptr = fopen("code-oss", "r")) == NULL)
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
    printf("e_phoff = %p\n", elf->e_phoff);
    printf("e_phnum = %d\n", elf->e_phnum);

    Elf64_Phdr* phdr = (Elf64_Phdr*)(buffer + elf->e_phoff);
    for (int i = 0; i < elf->e_phnum; i++)
    {
        printf("p_type[%10d] p_memsz[%010p] p_align[%08p]\n", phdr[i].p_type, phdr[i].p_memsz, phdr[i].p_align);
    }

    uint64_t old_p_filesz = phdr[2].p_filesz;
    phdr[2].p_filesz = phdr[5].p_offset + phdr[5].p_filesz;
    phdr[2].p_memsz += (phdr[2].p_filesz - old_p_filesz);
    phdr[2].p_flags |= phdr[3].p_flags | phdr[4].p_flags | phdr[5].p_flags;
    phdr[2].p_align = 0x10000;
    printf("%p %p %p\n", phdr[2].p_filesz, phdr[5].p_offset, phdr[5].p_filesz);

    phdr[3].p_type = 4;
    phdr[4].p_type = 4;
    phdr[5].p_type = 4;

    if ((fileptr = fopen("code-oss-64k", "w+")) == NULL)
    {
        printf("Fail to open file!");
        exit(1);
    }

    fwrite(buffer, filelen, 1, fileptr);
    fclose(fileptr);

    return 0;
}
