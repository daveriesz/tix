
PROGRAM  = tix

CSOURCES = tix.c
COBJECTS = $(CSOURCES:.c=.o)

LDFLAGS  = -lrt

all: $(PROGRAM)

$(PROGRAM): $(COBJECTS)
	$(CC) -o $@ $^ $(LDFLAGS)

clean:
	$(RM) $(COBJECTS)
	$(RM) $(PROGRAM)

