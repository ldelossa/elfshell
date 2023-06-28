#include <asm-generic/errno-base.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "../cmd_tree/include/cmd_tree.h"
#include "../lib/lib.h"

// command nodes are implemented in their own .c files.
extern cmd_tree_node_t program_headers_node;

int root_cmd_exec(void *ctx, uint8_t argc, char **argv) {
    printf("No handler for this command.\n");
    return 1;
}

cmd_tree_node_t root = {
    .name = "root",
    .exec = root_cmd_exec,
};

int shell_start(elf_ctx *ctx) {
    char cmd[1024];
    cmd_tree_node_t *target_cmd = NULL;

    if (!isatty(STDIN_FILENO)) {
        printf("[Error] STDIN is not a tty, cannot start shell.\n");
        return -1;
    }
    // build up command tree.

    // 'programs' command to list program headers.
    cmd_tree_node_add_child(&root, &program_headers_node);

    for (;;) {
        int r;
        write(STDIN_FILENO, "ELF> ", sizeof("ELF> "));
        while ((r = read(STDIN_FILENO, cmd, 1024)) == EINTR)
            ;
        if (r < 0) {
            perror("read");
            return -1;
        }
        cmd[r - 1] = '\0';

        if (cmd_tree_search(&root, cmd, &target_cmd) != 1) continue;

        target_cmd->exec((void *)ctx, target_cmd->argc, target_cmd->argv);

        cmd_tree_node_free(target_cmd);
    }
}
