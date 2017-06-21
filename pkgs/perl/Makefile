CC = afl-clang-fast
CFLAGS = -Wall -O2 -g $(shell perl -MExtUtils::Embed -e ccopts)
LDLIBS = $(shell perl -MExtUtils::Embed -e ldopts)

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

# vim:ts=4 sts=4 sw=4 noet
