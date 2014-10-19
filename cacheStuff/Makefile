.PHONY : all clean build_lib
LIB_MODULES := cacheStuff 
LIB_OBJS := $(LIB_MODULES:%=%.o)

INCLUDE_DIRS := listStuff 

TEST_MODULE := cacheTest
TEST_OBJS := $(TEST_MODULE:%=%.o) $(LIB_OBJS)

CFLAGS=-Wall -g -I.. $(INCLUDE_DIRS:%=-I../%) -DDEBUG_OUT

all : $(TEST_MODULE)

build_lib : $(LIB_FILE)($(LIB_OBJS))

.o.a : $(LIB_OBJS)
	$(AR) crv $@ $(LIB_OBJS)

$(TEST_MODULE) : $(TEST_OBJS)
	 $(CC) $(CFLAGS) $(TEST_OBJS) -o $@

%.o : %.c
	$(CC) $(CFLAGS) $(DFLAGS) -c $< -o $@

clean :
	rm -f  $(TEST_OBJS) $(TEST_MODULE)

