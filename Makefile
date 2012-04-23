.PHONY: all

all: hc hunc

hc: hc.cpp
	g++ -o $@ $<

hunc: hunc.cpp
	g++ -o $@ $<
