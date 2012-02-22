.PHONY: all
all: returns inherit

%: %.c class.h
	gcc -g -o $@ $<
