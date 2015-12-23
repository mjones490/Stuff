MODULES = timeStuff cmdlineStuff configStuff cacheStuff hashStuff
LIB_OBJS = 

CLEAN_MODULES = $(MODULES) listStuff

OBJS = $(MODULES:%=%.o)
SUBDIR_CLEAN := $(CLEAN_MODULES:%=%_CLEAN)

TAR_FILE := ../Stuff.tar.bz2

CFLAGS=-Wall -g -I.. $(INCLUDE_DIRS:%=-I../%)

export LIB_FILE = $(PWD)/libStuff.a

.PHONY : all $(MODULES) clean $(SUBDIR_CLEAN)
all : libStuff.a

libStuff.a : $(MODULES) $(LIB_OBJS)

.o.a : $(LIB_OBJS)
	$(AR) crv $@ $(LIB_OBJS)

%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(MODULES) : 
	$(MAKE) -C $@ build_lib

clean : $(SUBDIR_CLEAN)
	rm -f libStuff.a

$(SUBDIR_CLEAN) :
	$(MAKE) -C $(@:%_CLEAN=%) clean

tarball : clean
	cd ..
	tar --exclude=CVS -cvjf $(TAR_FILE) Stuff
	cd -

