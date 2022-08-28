CC = gcc

HDIR = heads

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

SSDIR = srcs/mstring
SODIR = objs/mstring
SHDIR = heads/mstring

SSRCS = $(shell find $(SSDIR) -name "*.c")
SOBJS = $(patsubst $(SSDIR)/%.c, $(SODIR)/%.o, $(SSRCS))

SLIB = libmstring.a

#########

make: $(CRLIB) $(MLIB) $(SLIB)

remake: clean $(CRLIB) $(MLIB) $(SLIB)

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

$(SLIB): $(SOBJS)
	ar -r $(SLIB) $(SOBJS)

$(SODIR)/%.o: $(SSDIR)/%.c
	$(CC) -c -o $@ $< -I $(HDIR) -I $(SHDIR)

#########

clean:
	@rm -f $(CROBJS) $(MOBJS) $(SOBJS)
	@rm -f $(CRLIB) $(MLIB) $(SLIB)

.PHONY: make remake clean
