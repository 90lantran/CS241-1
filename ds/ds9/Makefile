#
# CS 241
# The University of Illinois
#

CC = gcc
FLAGS += -W -g -O0 -I.

all: ds9b ds9a ds9c

ds9a: ds9a.c
	$(CC) $(FLAGS) $^ -o $@

ds9b: ds9b.c
	$(CC) $(FLAGS) $^ -o $@

ds9c: ds9c.c ds9c-child-work.c
	$(CC) $(FLAGS) $^ -o $@

clean:
	-rm -f ds9a ds9b ds9c
