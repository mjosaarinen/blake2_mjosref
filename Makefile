# Makefile
# 25-Jan-15 Markku-Juhani O. Saarinen <mjos@iki.fi>

DIST	= blake2_mjosref
BIN	= xb2test
OBJS	= blake2b.o blake2s.o test_main.o

CC	= gcc
CFLAGS	= -Wall -O
LIBS	=
LDFLAGS	=
INCS	=

$(BIN):	$(OBJS)
	$(CC) $(LDFLAGS) -o $(BIN) $(OBJS) $(LIBS)

test:	$(BIN)
	bash testvec.sh

.c.o:
	$(CC) $(CFLAGS) $(INCS) -c $< -o $@

clean:
	rm -f $(DIST)-*.t?z $(OBJS) $(BIN) *~ 

dist:	clean		
	cd ..; \
	tar cfvJ $(DIST)/$(DIST)-`date "+%Y%m%d%H%M00"`.txz $(DIST)/*
