# Copyright © 2017-2018 Jakub Wilk <jwilk@jwilk.net>
# SPDX-License-Identifier: MIT

.PHONY: all
all: aflpost-size-limit.so

%.so: %.c
	$(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) -shared -fPIC $(<) -o $(@)

.PHONY: clean
clean:
	rm -rf *.so

.error = GNU make is required

# vim:ts=4 sts=4 sw=4 noet
