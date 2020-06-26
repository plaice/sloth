SLOTH = ..

BIN = $(SLOTH)/bin

LIB = $(SLOTH)/src

GLOBALS = $(SLOTH)/src

GLOBALSH = $(GLOBALS)/globals.h

SRCCFG = options.c fileio.c filename.c mkextern.c string.c older.c build.c \
		compile.c cfg.c

OBJCFG = options.o fileio.o filename.o mkextern.o string.o older.o build.o compile.o cfg.o

SRCLKM = fileio.c filename.c mkextern.c string.c older.c build.c compile.c \
	options.c mkmain.c lkm.c

OBJLKM = fileio.o filename.o mkextern.o string.o older.o build.o compile.o \
	options.o mkmain.o lkm.o

SRCVM = filename.c string.c mm.c

OBJVM = filename.o string.o mm_vm.o

OBJMM = filename.o string.o mm_mm.o

sloth : cfg lkm mm vm cserror # qdlkm ecfg elkm eqdlkm

cfg: $(OBJCFG)
	cc -o cfg $(OBJCFG)

lkm: $(OBJLKM)
	cc -o lkm $(OBJLKM)

mm : $(OBJMM)
	cc -o mm $(OBJMM)

vm : $(OBJVM)
	cc -o vm $(OBJVM)

cserror : $(GLOBALSH) fileio.o filename.o cserror.o
	cc -o cserror -I$(GLOBALS) fileio.o filename.o cserror.o

lint : lintcfg lintlkm lintmm

lintcfg : $(SRCCFG)
	lint $(SRCCFG)

lintlkm : $(SRCLKM)
	lint $(SRCLKM)

lintmm : $(SRCVM)
	lint $(SRCVM)

#$(BIN)/ecfg: ecfg $(BIN)/cserror
#	cp ecfg $(BIN)/ecfg
#
#$(BIN)/elkm: elkm $(BIN)/cserror
#	cp elkm $(BIN)/elkm
#
#$(BIN)/eqdcfg: eqdcfg $(BIN)/cserror
#	cp eqdcfg $(BIN)/eqdcfg
#
#$(BIN)/eqdlkm: eqdlkm $(BIN)/cserror
#	cp eqdlkm $(BIN)/eqdlkm
#
#$(BIN)/mkm: mkm
#	cp mkm $(BIN)/mkm

mkmain.o : macros.h globals.h mkmain.c

mkextern.o : macros.h globals.h mkextern.c

compile.o : globals.h older.h compile.c

build.o : globals.h older.h build.c

string.o : globals.h string.c

filename.o : globals.h filename.c

fileio.o : globals.h fileio.c

lkm.o : globals.h lkm.c

cfg.o : globals.h cfg.c

mm_mm.o : mm.c
	cc -DVM=0 -o mm_mm.o -c mm.c

mm_vm.o : mm.c
	cc -DVM=1 -o mm_vm.o -c mm.c

clean:
	rm -f *.o cfg lkm mm vm cserror
