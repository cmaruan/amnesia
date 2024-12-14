CFLAGS += -g -O0 -std=c99 \
          -Wall -Wextra -Werror \
          -pedantic -Iinclude -Wno-newline-eof

GPERF = $(wildcard *.gperf)
GPERF_SRC = $(patsubst %.gperf, %.c, $(GPERF))
OBJECTS = $(patsubst %.c, %.o, $(GPERF_SRC)) \
          $(patsubst %.c, %.o, $(wildcard *.c))
          

HEADERS = $(wildcard include/*.h)

all: main

$(OBJECTS): $(HEADERS) $(GPERF)

%.c: %.gperf
	gperf -t $< > $@ 

main: $(OBJECTS)

clean:
	rm -rf $(OBJECTS) $(GPERF_SRC) main 

run: main
	truncate -s 0 log
	GAME_LOG_LEVEL=TRACE ./main 2>log