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

OBJVM = $(LIB)/filename.o $(LIB)/string.o mm.c

sloth : cfg lkm qdlkm mm vm cserror ecfg elkm eqdlkm mkm

lint : lintcfg lintlkm lintmm

lintcfg : $(SRCCFG)
	lint $(SRCCFG)

lintlkm : $(SRCLKM)
	lint $(SRCLKM)

lintmm : $(SRCVM)
	lint $(SRCVM)

cfg: $(BIN)/cfg

lkm: $(BIN)/lkm

qdlkm: $(BIN)/qdlkm

mm : $(BIN)/mm

vm : $(BIN)/vm

cserror : $(BIN)/cserror

ecfg: $(BIN)/ecfg

elkm: $(BIN)/elkm

eqdlkm: $(BIN)/eqdlkm

mkm: $(BIN)/mkm

$(BIN)/cfg: $(OBJCFG)
	cc -o $(BIN)/cfg $(OBJCFG)

$(BIN)/lkm: $(OBJLKM)
	cc -o $(BIN)/lkm $(OBJLKM)

$(BIN)/qdcfg: qdcfg
	cp qdcfg $(BIN)/qdcfg

$(BIN)/qdlkm: qdlkm
	cp qdlkm $(BIN)/qdlkm

$(BIN)/mm : $(SRCVM)
	cc -o $(BIN)/mm -DVM=0 $(OBJVM)

$(BIN)/vm : $(SRCVM)
	cc -o $(BIN)/vm -DVM=1 $(OBJVM)

$(BIN)/cserror : $(GLOBALSH) $(LIB)/fileio.o $(LIB)/filename.o cserror.c
	cc -o $(BIN)/cserror -I$(GLOBALS) $(LIB)/fileio.o $(LIB)/filename.o cserror.c

$(BIN)/ecfg: ecfg $(BIN)/cserror
	cp ecfg $(BIN)/ecfg

$(BIN)/elkm: elkm $(BIN)/cserror
	cp elkm $(BIN)/elkm

$(BIN)/eqdcfg: eqdcfg $(BIN)/cserror
	cp eqdcfg $(BIN)/eqdcfg

$(BIN)/eqdlkm: eqdlkm $(BIN)/cserror
	cp eqdlkm $(BIN)/eqdlkm

$(BIN)/mkm: mkm
	cp mkm $(BIN)/mkm

mkmain.o : macros.h globals.h mkmain.c

mkextern.o : macros.h globals.h mkextern.c

compile.o : globals.h older.h compile.c

build.o : globals.h older.h build.c

string.o : globals.h string.c

filename.o : globals.h filename.c

fileio.o : globals.h fileio.c

lkm.o : globals.h lkm.c

cfg.o : globals.h cfg.c

clean:
	rm *.o
