#include "lib.h"

#include <elf.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int read_elf_header(FILE *fp, elf_ctx *ctx) {
    printf("Reading ELF header\n");
    if (fread(&ctx->elf_header, sizeof(Elf64_Ehdr), 1, fp) != 1) {
        perror("fread");
        return -1;
    }
    // reset the file pointer to the beginning of the file
    rewind(fp);
    return 0;
}

void print_elf_header(Elf64_Ehdr *elf_header) {
    printf("ELF header:\n");
    printf("  e_ident: %s\n", elf_header->e_ident);
    switch (elf_header->e_type) {
        case ET_NONE:
            printf("  e_type: ET_NONE\n");
            break;
        case ET_REL:
            printf("  e_type: ET_REL\n");
            break;
        case ET_EXEC:
            printf("  e_type: ET_EXEC\n");
            break;
        case ET_DYN:
            printf("  e_type: ET_DYN\n");
            break;
        case ET_CORE:
            printf("  e_type: ET_CORE\n");
            break;
        default:
            printf("  e_type: Unknown\n");
            break;
    }
    printf("  e_machine: %d\n", elf_header->e_machine);
    printf("  e_version: %d\n", elf_header->e_version);
    printf("  e_entry: %lu\n", elf_header->e_entry);
    printf("  e_phoff: %lu\n", elf_header->e_phoff);
    printf("  e_shoff: %lu\n", elf_header->e_shoff);
    printf("  e_flags: %d\n", elf_header->e_flags);
    printf("  e_ehsize: %d\n", elf_header->e_ehsize);
    printf("  e_phentsize: %d\n", elf_header->e_phentsize);
    printf("  e_phnum: %d\n", elf_header->e_phnum);
    printf("  e_shentsize: %d\n", elf_header->e_shentsize);
    printf("  e_shnum: %d\n", elf_header->e_shnum);
    printf("  e_shstrndx: %d\n", elf_header->e_shstrndx);
}

Elf64_Phdr *read_program_headers(FILE *fp, elf_ctx *ctx) {
    printf("Reading program headers\n");
    if (ctx->elf_header.e_phnum == 0) {
        return NULL;
    }
    printf("Allocating space for %d program headers\n",
           ctx->elf_header.e_phnum);
    ctx->program_headers = calloc(ctx->elf_header.e_phnum, sizeof(Elf64_Phdr));
    if (fseek(fp, ctx->elf_header.e_phoff, SEEK_SET) < 0) {
        perror("fseek");
        rewind(fp);
        return NULL;
    }
    if (fread(ctx->program_headers, sizeof(Elf64_Phdr), ctx->elf_header.e_phnum,
              fp) != ctx->elf_header.e_phnum) {
        perror("fread");
        rewind(fp);
        free(ctx->program_headers);
        return NULL;
    }
    ctx->n_prog_hdrs = ctx->elf_header.e_phnum;
    rewind(fp);
    return ctx->program_headers;
}

void print_program_headers(Elf64_Phdr *program_headers, uint64_t n) {
    for (uint64_t i = 0; i < n; i++) {
        printf("Program headers:\n");
        switch (program_headers[i].p_type) {
            case PT_NULL:
                printf("  p_type: PT_NULL\n");
                break;
            case PT_LOAD:
                printf("  p_type: PT_LOAD\n");
                break;
            case PT_DYNAMIC:
                printf("  p_type: PT_DYNAMIC\n");
                break;
            case PT_INTERP:
                printf("  p_type: PT_INTERP\n");
                break;
            case PT_NOTE:
                printf("  p_type: PT_NOTE\n");
                break;
            case PT_SHLIB:
                printf("  p_type: PT_SHLIB\n");
                break;
            case PT_PHDR:
                printf("  p_type: PT_PHDR\n");
                break;
            case PT_TLS:
                printf("  p_type: PT_TLS\n");
                break;
            case PT_NUM:
                printf("  p_type: PT_NUM\n");
                break;
            case PT_LOOS:
                printf("  p_type: PT_LOOS\n");
                break;
            case PT_GNU_EH_FRAME:
                printf("  p_type: PT_GNU_EH_FRAME\n");
                break;
            case PT_GNU_STACK:
                printf("  p_type: PT_GNU_STACK\n");
                break;
            case PT_GNU_RELRO:
                printf("  p_type: PT_GNU_RELRO\n");
                break;
            default:
                printf("  p_type: Unknown: (%d) \n", program_headers[i].p_type);
                break;
        }
        printf("  p_flags: %d\n", program_headers[i].p_flags);
        printf("  p_offset: 0x%lx\n", program_headers[i].p_offset);
        printf("  p_vaddr: 0x%lx\n", program_headers[i].p_vaddr);
        printf("  p_paddr: 0x%lx\n", program_headers[i].p_paddr);
        printf("  p_filesz: %lu\n", program_headers[i].p_filesz);
        printf("  p_memsz: %lu\n", program_headers[i].p_memsz);
        printf("  p_align: %lu\n", program_headers[i].p_align);
    }
}

Elf64_Shdr *read_section_headers(FILE *fp, elf_ctx *ctx) {
    printf("Reading section headers\n");
    if (ctx->elf_header.e_shnum == 0) {
        return NULL;
    }
    printf("Allocating space for %d section headers\n",
           ctx->elf_header.e_shnum);
    ctx->section_headers = calloc(ctx->elf_header.e_shnum, sizeof(Elf64_Shdr));
    if (fseek(fp, ctx->elf_header.e_shoff, SEEK_SET) < 0) {
        perror("fseek");
        rewind(fp);
        return NULL;
    }
    if (fread(ctx->section_headers, sizeof(Elf64_Shdr), ctx->elf_header.e_shnum,
              fp) != ctx->elf_header.e_shnum) {
        perror("fread");
        rewind(fp);
        free(ctx->section_headers);
        return NULL;
    }
    ctx->n_sections = ctx->elf_header.e_shnum;
    rewind(fp);
    return ctx->section_headers;
}

char *section_name(FILE *fp, Elf64_Shdr *strtbl, Elf64_Shdr *sec) {
    char *name = calloc(255, sizeof(char));
    fseek(fp, strtbl->sh_offset + sec->sh_name, SEEK_SET);
    if (fread(name, 255, 1, fp) != 1) {
        if (!feof(fp)) {
            fprintf(stderr, "Error reading file\n");
            exit(1);
        }
    }
    return name;
}

void print_section_headers(FILE *fp, Elf64_Ehdr *elf,
                           Elf64_Shdr *section_headers, uint64_t n) {
    if (n < elf->e_shstrndx) return;

    Elf64_Shdr *str_tbl = &section_headers[elf->e_shstrndx];

    for (int i = 0; i < n; i++) {
        printf("Section headers:\n");
        printf("  sh_name: %s\n",
               section_name(fp, str_tbl, &section_headers[i]));
        printf("  sh_type: %d\n", section_headers[i].sh_type);
        printf("  sh_flags: %lu\n", section_headers[i].sh_flags);
        printf("  sh_addr: %lu\n", section_headers[i].sh_addr);
        printf("  sh_offset: %lu\n", section_headers[i].sh_offset);
        printf("  sh_size: %lu\n", section_headers[i].sh_size);
        printf("  sh_link: %d\n", section_headers[i].sh_link);
        printf("  sh_info: %d\n", section_headers[i].sh_info);
        printf("  sh_addralign: %lu\n", section_headers[i].sh_addralign);
        printf("  sh_entsize: %lu\n", section_headers[i].sh_entsize);
    }
}

char *read_section(FILE *fp, Elf64_Shdr *sec) {
    if (sec->sh_size == 0) {
        return NULL;
    }
    char *sec_data = calloc(sec->sh_size, 1);
    if (fseek(fp, sec->sh_offset, SEEK_SET) < 0) {
        perror("fseek");
        return NULL;
    }
    if (fread(sec_data, sec->sh_size, 1, fp) != 1) {
        perror("fread");
        free(sec_data);
        return NULL;
    }
    return sec_data;
}

Elf64_Sym *read_sym_table(FILE *fp, elf_ctx *ctx) {
    Elf64_Shdr *sym_sec = NULL;
    for (int i = 0; i < ctx->n_sections; i++) {
        if (ctx->section_headers[i].sh_type == SHT_SYMTAB) {
            sym_sec = &ctx->section_headers[i];
            break;
        }
    }
    if (!sym_sec) return NULL;

    uint64_t n = sym_sec->sh_size / sizeof(Elf64_Sym);
    // Allocate memory for the symbol table
    printf("Allocating space for %lu symbols\n", n);
    ctx->symbols = calloc(sym_sec->sh_size, sizeof(char));

    if (fseek(fp, sym_sec->sh_offset, SEEK_SET) < 0) {
        perror("fseek");
        free(ctx->symbols);
        rewind(fp);
        return NULL;
    }

    if (fread(ctx->symbols, sym_sec->sh_size, 1, fp) != 1) {
        perror("fread");
        free(ctx->symbols);
        rewind(fp);
        return NULL;
    }
    ctx->n_symbols = n;

    rewind(fp);
    return ctx->symbols;
}

int parse_elf(FILE *fp, elf_ctx *ctx) {
    if (read_elf_header(fp, ctx) != 0) {
        printf("Error reading ELF header\n");
        return -1;
    }

    read_program_headers(fp, ctx);
    if (ctx->program_headers == NULL) {
        printf("Error reading program headers\n");
        return -1;
    }

    read_section_headers(fp, ctx);
    if (ctx->section_headers == NULL) {
        printf("Error reading section headers\n");
        return -1;
    }

    read_sym_table(fp, ctx);
    if (ctx->symbols == NULL) {
        printf("Error reading symbol table\n");
        return -1;
    }

    for (int i = 0; i < ctx->n_sections; i++) {
        if (ctx->section_headers[i].sh_type == SHT_SYMTAB)
            ctx->symtab_sec_index = i;
    }

    ctx->fp = fp;
    return 0;
};

char *symbol_name(FILE *fp, elf_ctx *ctx, Elf64_Sym *sym) {
    char *name;
    Elf64_Shdr str_table =
        ctx->section_headers[ctx->section_headers[ctx->symtab_sec_index]
                                 .sh_link];
    name = calloc(256, sizeof(char));
    if (fseek(fp, str_table.sh_offset + sym->st_name, SEEK_SET) != 0) {
        perror("fseek");
        return name;
    }
    if (fread(name, 256, 1, fp) != 1) {
        perror("fread");
        rewind(fp);
        return name;
    }
    return name;
}

void print_symbols(FILE *fp, elf_ctx *ctx) {
    char *name;
    Elf64_Shdr str_table =
        ctx->section_headers[ctx->section_headers[ctx->symtab_sec_index]
                                 .sh_link];
    for (uint64_t i = 0; i < ctx->n_symbols; i++) {
        name = symbol_name(fp, ctx, &ctx->symbols[i]);
        printf("Symbol %lu:\n", i);
        printf("  st_name: %s\n", name);
        printf("  st_value: %lu\n", ctx->symbols[i].st_value);
        printf("  st_size: %lu\n", ctx->symbols[i].st_size);
        printf("  st_info: %d\n", ctx->symbols[i].st_info);
        printf("  st_other: %d\n", ctx->symbols[i].st_other);
        printf("  st_shndx: %d\n", ctx->symbols[i].st_shndx);
        free(name);
    }
    rewind(fp);
}

char *symbol_object_data(FILE *fp, elf_ctx *ctx, char *sym_name,
                         uint64_t *idx) {
    char *name;
    Elf64_Sym *sym_found = NULL;
    Elf64_Shdr *sec = NULL;
    char *data;

    for (uint64_t i = 0; i < ctx->n_symbols; i++) {
        Elf64_Sym *sym = &ctx->symbols[i];
        name = symbol_name(fp, ctx, sym);
        if (name[0] == 0) {
            free(name);
            continue;
        }
        if (strcmp(sym_name, name) == 0) {
            sym_found = &ctx->symbols[i];
            sec = &ctx->section_headers[ctx->symbols[i].st_shndx];
            *idx = i;
            break;
        }
    }

    if (!sym_found) {
        return NULL;
    }

    // we are only interested in object data embedded into the binary.
    if ((sym_found->st_info & 0x0F) != STT_OBJECT) {
        return NULL;
    }

    size_t sym_offset = sym_found->st_value - sec->sh_addr;

    if (fseek(fp, sec->sh_offset + sym_offset, SEEK_SET) != 0) {
        perror("fseek");
        return NULL;
    }

    data = calloc(sym_found->st_size, sizeof(char));

    if (fread(data, sym_found->st_size, 1, fp) != 1) {
        perror("fread");
        free(data);
        rewind(fp);
        return NULL;
    }

    return data;
};