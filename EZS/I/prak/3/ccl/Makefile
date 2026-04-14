PROGS = template arith prim rdtsc
CFLAGS   = -Wall -O3 -g

ifdef DOIT
CFLAGS += -DDOIT
endif

all: $(PROGS)

$(PROGS) : % : %.o ccl.o

clean:
	rm -f $(PROGS) *.o *~
