# Copyright © 2017-2018 Jakub Wilk <jwilk@jwilk.net>
# SPDX-License-Identifier: MIT

CC = afl-clang-fast
PERL = perl
CFLAGS = -Wall -O2 -g $(shell $(PERL) -MExtUtils::Embed -e ccopts)
LDLIBS = $(shell $(PERL) -MExtUtils::Embed -e ldopts)

_suffix = $(and $(AFL_USE_ASAN),.asan)
_progs = fuzz-enc fuzz-re
progs = $(addsuffix $(_suffix),$(_progs))

.PHONY: all
all: $(progs)

%$(_suffix): %.o
	$(LINK.c) $(^) $(LDLIBS) -o $(@)

.PHONY: clean
clean:
	rm -f $(_progs) $(addsuffix .asan,$(_progs))

.error = GNU make is required

# vim:ts=4 sts=4 sw=4 noet
