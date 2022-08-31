CC = gcc

SDIR = srcs
ODIR = objs
HDIR = heads
LDIR = libs

SRCS = $(shell find $(SDIR) -name "*.c")
OBJS = $(patsubst $(SDIR)/%.c, $(ODIR)/%.o, $(SRCS))
LIBS = $(LDIR)/libmint.a $(LDIR)/libmstr.a $(LDIR)/libmemory.a $(LDIR)/libcrash.a
OUT = metareal.exe

make: lmake $(OUT)

remake: clean $(OUT)

$(OUT): $(OBJS)
	$(CC) -o $(OUT) $(OBJS) $(LIBS)

$(ODIR)/%.o: $(SDIR)/%.c
	$(CC) -c -o $@ $< -I $(HDIR) -I $(LDIR)/$(HDIR)

clean:
	@rm -f $(shell find $(ODIR) -name "*.o")
	@rm -f $(OUT)
	@cd $(LDIR) && $(MAKE) clean

lmake:
	@cd $(LDIR) && $(MAKE)

rlmake:
	@cd $(LDIR) && $(MAKE) remake

.PHONY: make remake clean lmake
