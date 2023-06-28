#include <asm-generic/errno-base.h>
#include <elf.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "lib/lib.h"

#define SAMPLE_ELF_PATH "./sample"

extern int shell_start(elf_ctx *elf);

int main(int argc, char *argv[]) {
    // Declare a variable to store the index of a symbol
    uint64_t sym_index;
    // Declare an instance of the elf_ctx struct and initialize it to 0
    elf_ctx ctx = {0};

    // Open the sample ELF file for reading
    FILE *fp = fopen(SAMPLE_ELF_PATH, "r");

    parse_elf(fp, &ctx);

    shell_start(&ctx);

    return 0;
}