CFLAGS += -O0 -g3

SHELL_OBJS += 	shell/shell.o					\
				shell/cmd_program_headers.o     \
				lib/lib.o                       \
				cmd_tree/cmd_tree.o 			\
				main.o

all: sample main

sample: sample.o

main: $(SHELL_OBJS)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -rf shell/*.o
	rm -rf lib/*.o
	rm -rf *.o
	rm -rf main
	rm -rf sample
