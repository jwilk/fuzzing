.PHONY: all

CFLAGS = -Wall -O2 -g $(shell perl -MExtUtils::Embed -e ccopts)
LDLIBS = $(shell perl -MExtUtils::Embed -e ldopts)

all: pfuzz

clean:
	rm -f pfuzz

# vim:ts=4 sts=4 sw=4 noet
