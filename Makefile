cc_flags = -w -std=c99 -O2 -g
cc := gcc

all: $(OBJS)
		$(cc) src/main.c $(cc_flags) -o passgen
