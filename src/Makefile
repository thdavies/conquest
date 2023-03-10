CC = cc
#CFLAGS = -Wall
CFLAGS = -Wno-implicit-function-declaration -Wno-implicit-int -Wno-incompatible-pointer-types
LDFLAGS = -lcurses -lm


OBJ = conqmain.o conqmp.o conqsubs.o conqs1.o conq2.o conq4.o conq1.o \
	conqtf.o conqlib.o conqbat.o conqb2.o conqi2.o conqd3.o conqs3.o conqpl.o \
	conqmc.o conqve.o conqmo.o conqin.o

conquest: $(OBJ)
		$(CC) -o conquest $(OBJ) $(LDFLAGS)

%.o: %.c vars.h structs.h defs.h
		$(CC) -c $(CFLAGS) $<

.PHONY: clean

clean:
	rm -f *.o
