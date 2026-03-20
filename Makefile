MODE ?= release

SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)

NONMAIN_OBJS = $(filter-out main.o,$(OBJS))
NONTEST_OBJS = $(filter-out tests.o,$(OBJS))


EXTRAFLAGS_debug = -O0 -g3
EXTRAFLAGS_release = -O2 -Werror
EXTRAFLAGS_test = --coverage -O0 -g3

CFLAGS = -std=c11 -Wall -Wextra -pedantic -MMD -MP $(EXTRAFLAGS_$(MODE))


default: balg


RDESC_DIR := vendor/rdesc
RDESC_FEATURES := full
RDESC_MODE := debug

$(RDESC_DIR)/rdesc.mk:
	git clone https://github.com/metwse/rdesc.git $(RDESC_DIR)

include $(RDESC_DIR)/rdesc.mk

# rdesc public headers
rdesc: $(RDESC)
	ln -s $(RDESC_INCLUDE_DIR) rdesc


%.o: %.c rdesc $(RDESC)
	$(CC) $(CFLAGS) -c $<

balg: $(NONTEST_OBJS) $(RDESC)
	$(CC) $(CFLAGS) $^ -o $@

balg-tests: $(NONMAIN_OBJS) $(RDESC)
	$(CC) $(CFLAGS) $^ -o $@


clean:
	$(RM) *.o *.d *.gcno *.gcda balg balg-tests

clean-vendor:
	$(RM) -r vendor rdesc

.PHONY: clean clean-vendor default


-include $(OBJS:.o=.d)
