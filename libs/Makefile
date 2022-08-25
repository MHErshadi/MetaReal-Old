CC = gcc

HDIR = heads

#########

SSDIR = srcs/str
SODIR = objs/str

SSRCS = $(shell find $(SSDIR) -name "*.c")
SOBJS = $(patsubst $(SSDIR)/%.c, $(SODIR)/%.o, $(SSRCS))

SLIB = libstr.a

#########

make: $(SLIB)

remake: clean $(SLIB)

#########

$(SLIB): $(SOBJS)
	ar -r $(SLIB) $(SOBJS)

$(SODIR)/%.o: $(SSDIR)/%.c
	$(CC) -c -o $@ $< -I $(HDIR) -I ../$(HDIR)

#########

clean:
	@rm -f $(SOBJS)
	@rm -f $(SLIB)

.PHONY: make remake clean
