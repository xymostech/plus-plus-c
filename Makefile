.PHONY: all
all: test returns inherit

%: %.c class.h
	gcc -o $@ $<
