CC = gcc

SDIR = srcs
ODIR = objs
HDIR = heads

SRCS = $(shell find $(SDIR) -name "*.c")
OBJS = $(patsubst $(SDIR)/%.c, $(ODIR)/%.o, $(SRCS))
LIBS = -lmpc -lmpfr -lgmp
OUT = metareal.exe

make: $(OUT)

remake: clean make

$(OUT): $(OBJS)
	$(CC) -o $(OUT) $(OBJS) $(LIBS)

$(ODIR)/%.o: $(SDIR)/%.c
	$(CC) -c -o $@ $< -I $(HDIR)

clean:
	@rm -f $(shell find $(ODIR) -name "*.o")
	@rm -f $(OUT)

.PHONY: make remake clean
