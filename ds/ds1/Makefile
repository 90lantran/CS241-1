.PHONY: all clean sol doc

BUILTINS = strlen strcmp strcpy strcat strchr strrchr strpbrk strsep
FLAGS    = $(addprefix -fno-builtin-, $(BUILTINS))

all: test.o ds1.o std doc
	gcc -std=gnu99 -Wall $(FLAGS) -o ds1 test.o ds1.o

sol: test.o ds1_sol.o
	gcc -std=gnu99 -Wall $(FLAGS) -o ds1_sol test.o ds1_sol.o

std: test.o
	gcc -std=gnu99 -o ds1_std test.o

test.o: test.c
	gcc -std=gnu99 -c $(FLAGS) test.c

ds1.o: ds1.c
	gcc -std=gnu99 -c $(FLAGS) ds1.c

ds1_sol.o: ds1_sol.c
	gcc -std=gnu99 -c $(FLAGS) ds1_sol.c

doc: test.c ds1.c doc/Doxyfile doc/main.dox doc/ds1.html
	doxygen doc/Doxyfile

clean:
	[ ! -f ds1.o     ] || rm ds1.o
	[ ! -f ds1_sol.o ] || rm ds1_sol.o
	[ ! -f test.o    ] || rm test.o
	[ ! -f ds1       ] || rm ds1
	[ ! -f ds1_sol   ] || rm ds1_sol
	[ ! -f ds1_std   ] || rm ds1_std 
	[ ! -d doc/html  ] || rm -rf doc/html

