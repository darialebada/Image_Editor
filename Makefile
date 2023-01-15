# compiler setup
CC=gcc
CFLAGS=-Wall -Wextra -std=c99

# define targets
TARGETS=image_editor

build: $(TARGETS)

image_editor: *.c
	$(CC) $(CFLAGS) image_editor.c functions.c -o image_editor -lm

pack:
	zip -FSr 313CA_LebadaDariaCristiana_Tema3.zip README Makefile *.c *.h

clean:
	rm -f $(TARGETS)

.PHONY: pack clean
