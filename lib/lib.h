#include <elf.h>
#include <stdio.h>

/**
 * The in-memory representation of an ELF file.
 */
typedef struct elf_ctx {
    // Handle to the open ELF file.
    FILE *fp;
    // The ELF header of the parsed file.
    Elf64_Ehdr elf_header;

    // An array of program headers for the parsed file.
    Elf64_Phdr *program_headers;

    // The number of program headers in the 'program_headers' array.
    uint64_t n_prog_hdrs;

    // An array of section headers for the parsed file.
    Elf64_Shdr *section_headers;

    // The number of section headers in the 'section_headers' array.
    uint64_t n_sections;

    // The index of the symbol table in the 'section_headers' array.
    uint64_t symtab_sec_index;

    // An array of symbols for the parsed file.
    Elf64_Sym *symbols;

    // The number of symbols in the 'symbols' array.
    uint64_t n_symbols;
} elf_ctx;

/**
 * Parses an ELF file and stores the relevant information in the given elf_ctx
 * struct.
 *
 * Reads the ELF header, program headers, section headers, and symbol table from
 * the given file pointer. If any of these operations fail, returns -1.
 *
 * @param fp A pointer to the ELF file to parse.
 * @param ctx A pointer to the elf_ctx struct to store the parsed information
 * in.
 * @return 0 on success, -1 on failure.
 */
int parse_elf(FILE *fp, elf_ctx *ctx);

/**
 * Reads the ELF header from the given file pointer and stores it in the
 * provided Elf64_Ehdr struct. Sets the file pointer to the beginning of the
 * file before returning.
 *
 * @param fp A pointer to the file to read the ELF header from.
 * @param elf_header A pointer to the Elf64_Ehdr struct to store the ELF header
 * in.
 * @return 0 on success, or a negative value on failure.
 */
int read_elf_header(FILE *fp, elf_ctx *ctx);

/**
 * Reads the program headers from the given file pointer and stores them in the
 * provided Elf64_Phdr array. Also sets the integer pointed to by n to the
 * number of program headers read. Sets the file pointer to the beginning of the
 * file before returning.
 *
 * @param fp A pointer to the file to read the program headers from.
 * @param ctx A pointer to the elf_ctx struct to store the parsed information
 * in.
 * @return A pointer to the array of Elf64_Phdr structs on success, or NULL on
 * failure.
 */
Elf64_Phdr *read_program_headers(FILE *fp, elf_ctx *ctx);

/**
 * Reads the section headers from the given file pointer and stores them in the
 * provided elf_ctx struct. Also sets the integer pointed to by n to the number
 * of section headers read. Sets the file pointer to the beginning of the file
 * before returning.
 *
 * @param fp A pointer to the file to read the section headers from.
 * @param ctx A pointer to the elf_ctx struct to store the parsed information
 * in.
 * @return A pointer to the array of Elf64_Shdr structs on success, or NULL on
 * failure.
 */
Elf64_Shdr *read_section_headers(FILE *fp, elf_ctx *ctx);

/**
 * Reads the symbol table from the given file pointer and stores it in the
 * provided elf_ctx struct. Also sets the integer pointed to by n to the number
 * of symbols read. Sets the file pointer to the beginning of the file before
 * returning.
 *
 * @param fp A pointer to the file to read the symbol table from.
 * @param ctx A pointer to the elf_ctx struct to store the parsed information
 * in.
 * @return A pointer to the array of Elf64_Sym structs on success, or NULL on
 * failure.
 */
Elf64_Sym *read_sym_table(FILE *fp, elf_ctx *ctx);

/**
 * Returns the name of the symbol pointed to by the provided Elf64_Sym struct.
 * The name is read from the string table section of the ELF file.
 *
 * If the symbol has no name, a single null character is returned.
 *
 * The caller must always free the returned char buffer.
 *
 * @param fp A pointer to the file to read the string table from.
 * @param ctx A pointer to the elf_ctx struct containing the parsed information.
 * @param sym A pointer to the Elf64_Sym struct to get the symbol name from.
 * @return A pointer to the symbol name on success, or a single null character if
 * the symbol has no name.
 */
char *symbol_name(FILE *fp, elf_ctx *ctx, Elf64_Sym *sym);

/**
 * Prints the contents of the provided ELF header to stdout.
 *
 * @param elf_header A pointer to the Elf64_Ehdr struct to print.
 */
void print_elf_header(Elf64_Ehdr *elf_header);

/**
 * Prints the contents of the provided program headers to stdout.
 *
 * @param program_headers A pointer to the array of Elf64_Phdr structs to print.
 * @param n The number of program headers to print.
 */
void print_program_headers(Elf64_Phdr *program_headers, uint64_t n);

/**
 * Prints the contents of the provided section headers to stdout.
 * Also prints the name of each section using the section_name function.
 * Sets the file pointer to the beginning of the file before returning.
 *
 * @param fp A pointer to the file to read the section headers from.
 * @param elf_header A pointer to the Elf64_Ehdr struct to store the ELF header
 * in.
 * @param section_headers A pointer to the array of Elf64_Shdr structs to print.
 * @param n The number of section headers to print.
 */
void print_section_headers(FILE *fp, Elf64_Ehdr *elf_header,
                           Elf64_Shdr *section_headers, uint64_t n);

/**
 * Returns the name of the given section using the provided string table section
 * header and the given section header. Sets the file pointer to the beginning
 * of the file before returning.
 *
 * @param fp A pointer to the file to read the section name from.
 * @param strtbl A pointer to the string table section header.
 * @param sec A pointer to the section header to get the name of.
 * @return The name of the section.
 */
char *section_name(FILE *fp, Elf64_Shdr *strtbl, Elf64_Shdr *sec);

/**
 * Reads a section's data into a buffer and returns it.
 *
 * @param fp A pointer to the file to read the section from.
 * @param sec A pointer to the section header to read.
 * @return A pointer to the buffer containing the section data.
 */
char *read_section(FILE *fp, Elf64_Shdr *sec);

/**
 * Prints the symbol table to stdout. The symbol table is read from the section
 * headers array, which should be obtained using the read_section_headers
 * function. Sets the file pointer to the beginning of the file before
 * returning.
 *
 * @param fp A pointer to the file to read the symbol table from.
 * @param ctx A pointer to the elf_ctx struct containing the ELF file's
 * in-memory context.
 */
void print_symbols(FILE *fp, elf_ctx *ctx);

/**
 * Reads the object data of a symbol with the given name from the provided file pointer and elf_ctx struct.
 * Also sets the integer pointed to by idx to the index of the symbol in the symbol table.
 * Sets the file pointer to the beginning of the file before returning.
 *
 * @param fp A pointer to the file to read the symbol object data from.
 * @param ctx A pointer to the elf_ctx struct containing the ELF file's in-memory context.
 * @param sym_name The name of the symbol to read the object data of.
 * @param idx A pointer to an integer to store the index of the symbol in the symbol table.
 * @return A pointer to the buffer containing the symbol object data on success, or NULL on failure.
 */
char *symbol_object_data(FILE *fp, elf_ctx *ctx, char *sym_name, uint64_t *idx);