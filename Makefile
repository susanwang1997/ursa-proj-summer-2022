ROOT_DIR:=$(shell dirname $(realpath $(firstword $(MAKEFILE_LIST))))
# LDFLAGS=-L/usr/local/lib -fsanitize=thread -g -lpthread -latomic -Ltbb2020/lib/intel64/gcc4.8 -ltbb
# LD_PRELOAD=/usr/lib/liblunar-calendar-preload.so
LDFLAGS=-L/usr/local/lib -lpthread -latomic -Ltbb2020/lib/intel64/gcc4.8 -ltbb
BLISS_LDFLAGS=-L$(ROOT_DIR)/core/bliss-0.73/ -lbliss
CFLAGS=-O3 -g -std=c++2a -Wall -Wextra -Wpedantic -fPIC -fconcepts -I$(ROOT_DIR)/core/ -Itbb2020/include -fopenmp
CFLAGS_KWS_TEST=-O0 -g -std=c++2a -Wall -Wextra -Wpedantic -fPIC -fconcepts -I$(ROOT_DIR)/core/ -Itbb2020/include -fopenmp
OBJ=core/DataGraph.o core/PO.o core/utils.o core/PatternGenerator.o $(ROOT_DIR)/core/showg.o
OBJ_KWS=kws_algorithms/NewDataGraph.o kws_algorithms/VecOps.o kws_algorithms/AvgShortestDistance.o core/PO.o core/utils.o core/PatternGenerator.o $(ROOT_DIR)/core/showg.o
OUTDIR=bin/
CC=g++-10

ifdef TEST
TESTING = -DTESTING
endif

ifdef EDGE_INDUCED
EDGE_OPTIONS = -DEDGE_INDUCED
endif

all: bliss fsm count test existence-query convert_data kws kws_test

core/roaring.o: core/roaring/roaring.c
	gcc -c core/roaring/roaring.c -o $@ -O3 -Wall -Wextra -Wpedantic -fPIC 

%.o: %.cc
	$(CC) -c $? -o $@ $(CFLAGS)

fsm: apps/fsm.cc $(OBJ) core/roaring.o bliss
	$(CC) apps/fsm.cc $(OBJ) core/roaring.o -o $(OUTDIR)/$@ $(BLISS_LDFLAGS) $(LDFLAGS) $(CFLAGS)

existence-query: apps/existence-query.cc $(OBJ) bliss
	$(CC) apps/existence-query.cc $(OBJ) -o $(OUTDIR)/$@ $(BLISS_LDFLAGS) $(LDFLAGS) $(CFLAGS)

enumerate: apps/enumerate.cc $(OBJ) bliss
	$(CC) apps/enumerate.cc $(OBJ) -o $(OUTDIR)/$@ $(BLISS_LDFLAGS) $(LDFLAGS) $(CFLAGS)

count: apps/count.cc $(OBJ) bliss
	$(CC) apps/count.cc $(OBJ) -o $(OUTDIR)/$@ $(BLISS_LDFLAGS) $(LDFLAGS) $(CFLAGS)

output: apps/output.cc $(OBJ) bliss
	$(CC) apps/output.cc $(OBJ) -o $(OUTDIR)/$@ $(BLISS_LDFLAGS) $(LDFLAGS) $(CFLAGS)

test: core/test.cc $(OBJ) core/DataConverter.o core/roaring.o bliss
	$(CC) core/test.cc -DTESTING $(OBJ) core/DataConverter.o core/roaring.o -o $(OUTDIR)/$@ $(BLISS_LDFLAGS) $(LDFLAGS) -lUnitTest++ $(CFLAGS)

convert_data: core/convert_data.cc core/DataConverter.o core/utils.o
	$(CC) -o $(OUTDIR)/$@ $? $(LDFLAGS) $(CFLAGS)

bliss:
	make -C ./core/bliss-0.73

kws: kws_algorithms/main.cc $(OBJ_KWS)  bliss
	$(CC) kws_algorithms/main.cc $(OBJ_KWS) $(TESTING) $(EDGE_OPTIONS) $(PARAMS) -o $(OUTDIR)/$@ $(BLISS_LDFLAGS) $(LDFLAGS) $(CFLAGS) -Wl,--no-as-needed -lprofiler -Wl,--as-needed -fopenmp
	
kws_test: kws_algorithms/tests.cc $(OBJ_KWS) bliss
	$(CC) -fopenmp kws_algorithms/main.cc $(OBJ_KWS) -o $(OUTDIR)/$@ $(BLISS_LDFLAGS) $(LDFLAGS)  $(CFLAGS_KWS_TEST) -fopenmp
	$(CC) -fopenmp kws_algorithms/tests.cc -DTESTING $(OBJ_KWS) -o $(OUTDIR)/$@ $(BLISS_LDFLAGS) $(LDFLAGS) -lUnitTest++ $(CFLAGS_KWS_TEST)

clean:
	make -C ./core/bliss-0.73 clean
	rm -f core/*.o bin/* kws_algorithms/*.o
