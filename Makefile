MODE ?= release

SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)

# All files except for the one containing executable main function.
NONMAIN_OBJS = $(filter-out main.o,$(OBJS))
# All files except for the one containing test executable main function.
NONTEST_OBJS = $(filter-out tests.o,$(OBJS))


EXTRAFLAGS_debug = -O0 -g3
EXTRAFLAGS_release = -O2 -Werror
EXTRAFLAGS_test = --coverage -O0 -g3

CFLAGS = -std=c11 -Wall -Wextra -pedantic -MMD -MP $(EXTRAFLAGS_$(MODE))


default: balg


RDESC_DIR := vendor/rdesc
RDESC_FEATURES := full
RDESC_MODE := $(MODE)

$(RDESC_DIR)/rdesc.mk:
	git clone https://github.com/metwse/rdesc.git $(RDESC_DIR) \
		--branch v0.2.0

include $(RDESC_DIR)/rdesc.mk

# Link librdesc public headers folder to rdesc/, so we can access them via
# `#include "rdesc/..."` instead of `#include "vendor/rdesc/include/..."`
rdesc: $(RDESC)
	ln -s $(RDESC_INCLUDE_DIR) $@


# Source files depend on public headers.
%.o: %.c rdesc
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
