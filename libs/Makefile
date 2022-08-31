CC = gcc

HDIR = heads
ODIR = objs

#########

CRSDIR = srcs/crash
CRODIR = objs/crash

CRSRCS = $(shell find $(CRSDIR) -name "*.c")
CROBJS = $(patsubst $(CRSDIR)/%.c, $(CRODIR)/%.o, $(CRSRCS))

CRLIB = libcrash.a

#########

MSDIR = srcs/memory
MODIR = objs/memory
MHDIR = heads/memory

MSRCS = $(shell find $(MSDIR) -name "*.c")
MOBJS = $(patsubst $(MSDIR)/%.c, $(MODIR)/%.o, $(MSRCS))

MLIB = libmemory.a

#########

ISDIR = srcs/mint
IODIR = objs/mint
IHDIR = heads/mint

ISRCS = $(shell find $(ISDIR) -name "*.c")
IOBJS = $(patsubst $(ISDIR)/%.c, $(IODIR)/%.o, $(ISRCS))

ILIB = libmint.a

#########

FSDIR = srcs/mfloat
FODIR = objs/mfloat
FHDIR = heads/mfloat

FSRCS = $(shell find $(FSDIR) -name "*.c")
FOBJS = $(patsubst $(FSDIR)/%.c, $(FODIR)/%.o, $(FSRCS))

FLIB = libmfloat.a

#########

SSDIR = srcs/mstr
SODIR = objs/mstr
SHDIR = heads/mstr

SSRCS = $(shell find $(SSDIR) -name "*.c")
SOBJS = $(patsubst $(SSDIR)/%.c, $(SODIR)/%.o, $(SSRCS))

SLIB = libmstr.a

#########

make: $(CRLIB) $(MLIB) $(ILIB) $(FLIB) $(SLIB)

remake: clean $(CRLIB) $(MLIB) $(ILIB) $(FLIB) $(SLIB)

#########

$(CRLIB): $(CROBJS)
	ar -r $(CRLIB) $(CROBJS)

$(CRODIR)/%.o: $(CRSDIR)/%.c
	$(CC) -c -o $@ $< -I $(HDIR)

#########

$(MLIB): $(MOBJS)
	ar -r $(MLIB) $(MOBJS)

$(MODIR)/%.o: $(MSDIR)/%.c
	$(CC) -c -o $@ $< -I $(HDIR) -I $(MHDIR)

#########

$(ILIB): $(IOBJS)
	ar -r $(ILIB) $(IOBJS)

$(IODIR)/%.o: $(ISDIR)/%.c
	$(CC) -c -o $@ $< -I $(HDIR) -I $(IHDIR)

#########

$(FLIB): $(FOBJS)
	ar -r $(FLIB) $(FOBJS)

$(FODIR)/%.o: $(FSDIR)/%.c
	$(CC) -c -o $@ $< -I $(HDIR) -I $(FHDIR)

#########

$(SLIB): $(SOBJS)
	ar -r $(SLIB) $(SOBJS)

$(SODIR)/%.o: $(SSDIR)/%.c
	$(CC) -c -o $@ $< -I $(HDIR) -I $(SHDIR)

#########

clean:
	@rm -f $(shell find $(ODIR) -name "*.o")
	@rm -f $(CRLIB) $(MLIB) $(ILIB) $(FLIB) $(SLIB)

.PHONY: make remake clean
