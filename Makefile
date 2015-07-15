all:
	gcc -o emutiny main.c emu.c opcodes.c tests.c

clean:
	rm -rf *o emutiny
