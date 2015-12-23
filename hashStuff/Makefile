.PHONY : all clean build_lib
LIB_MODULES := fnv1a_32
LIB_OBJS := $(LIB_MODULES:%=%.o)

TEST_MODULE := 
TEST_OBJS := $(TEST_MODULE:%=%.o) $(LIB_OBJS)

CFLAGS=-Wall
#ifeq ($(NO_MATH),TRUE)
#    CFLAGS+=-DNO_MATH
#endif

all : $(TEST_MODULE)

build_lib : $(LIB_FILE)($(LIB_OBJS))

.o.a : $(LIB_OBJS)
	$(AR) crv $@ $(LIB_OBJS)

$(TEST_MODULE) : $(TEST_OBJS)
	 $(CC) $(TEST_OBJS) -o $@

%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean :
	rm -f  $(TEST_OBJS) $(TEST_MODULE)

