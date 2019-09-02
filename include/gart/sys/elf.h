#pragma once
#include_next <sys/elf.h>

#define EF_MIPS_ARCH_32R6 0x90000000
#define EF_MIPS_ARCH_64R6 0xa0000000
#define PT_MIPS_ABIFLAGS 0x70000003

struct ElfTypes32 {
  typedef Elf32_Addr Addr;
  typedef Elf32_Off Off;
  typedef Elf32_Half Half;
  typedef Elf32_Word Word;
  typedef Elf32_Sword Sword;
  typedef Elf32_Ehdr Ehdr;
  typedef Elf32_Shdr Shdr;
  typedef Elf32_Sym Sym;
  typedef Elf32_Rel Rel;
  typedef Elf32_Rela Rela;
  typedef Elf32_Phdr Phdr;
  typedef Elf32_Dyn Dyn;
};

struct ElfTypes64 {
  typedef Elf64_Addr Addr;
  typedef Elf64_Off Off;
  typedef Elf64_Half Half;
  typedef Elf64_Word Word;
  typedef Elf64_Sword Sword;
  typedef Elf64_Xword Xword;
  typedef Elf64_Sxword Sxword;
  typedef Elf64_Ehdr Ehdr;
  typedef Elf64_Shdr Shdr;
  typedef Elf64_Sym Sym;
  typedef Elf64_Rel Rel;
  typedef Elf64_Rela Rela;
  typedef Elf64_Phdr Phdr;
  typedef Elf64_Dyn Dyn;
};
