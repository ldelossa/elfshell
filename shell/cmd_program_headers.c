#include "../cmd_tree/include/cmd_tree.h"
#include "../lib/lib.h"

int program_cmd_exec(void *ctx, uint8_t argc, char **argv) {
    elf_ctx *elf = (elf_ctx *)ctx;
    if (argc == 0) {
        print_program_headers(elf->program_headers, elf->n_prog_hdrs);
        return 1;
    }

    return 1;
}

cmd_tree_node_t program_headers_node = {
    .name = "program",
    .exec = program_cmd_exec,
};