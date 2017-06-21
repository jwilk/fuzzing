CFLAGS = -Wall -O2 -g $(shell perl -MExtUtils::Embed -e ccopts)
LDLIBS = $(shell perl -MExtUtils::Embed -e ldopts)

progs = fuzz-enc fuzz-re

.PHONY: all
all: $(progs)

.PHONY: clean
clean:
	rm -f $(progs)

# vim:ts=4 sts=4 sw=4 noet
