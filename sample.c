#include <stdio.h>
#include <unistd.h>
#include <stdint.h>

// global zero init'd data in .bss
char global_zero[1024];

// global init'd data in .data
char global_data[4] = {0xDE, 0xAD, 0xBE, 0xEF};

// global init'd read only data in .rodata
const char global_ro_data[4] = {0xFE, 0xED, 0XDE, 0xED};

// Declare a function and place it in a custom ELF section called "my_section"
 __attribute__((section("my_section"))) void my_function() {
     printf("Hello from my_function!\n");
}

// Declare some data in a new section called "custom_data"
uint32_t custom_data __attribute__((section(".custom_data"))) = 0xFEBEBEFE;

typedef struct node {
    int type;
    struct node *next;
} node_t;

int main (int argc, char *argv[]) {
    printf("pid: %d\n", getpid());
    getchar();
    return 0;
}