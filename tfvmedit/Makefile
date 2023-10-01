CC = gcc
LD = gcc

CFLAGS  = -g -fsanitize=address $(shell pkg-config --cflags gtk+-3.0)
LDFLAGS = -lasan $(shell pkg-config --libs gtk+-3.0)

CFILES = $(shell find -name '*.c')

OBJ = $(patsubst ./%.c,build/%.o,$(CFILES))

all: bin/tfvmedit

bin/tfvmedit: $(OBJ)
	@mkdir -p $(@D)
	$(info $s  LD $(notdir $^) ==> $@)
	@$(LD) $(LDFLAGS) $^ -o $@

build/%.o: %.c
	@mkdir -p $(@D)
	$(info $s  CC $< ==> $(notdir $@))
	@$(CC) $(CFLAGS) -c $< -o $@


clean:
	rm -rf bin build