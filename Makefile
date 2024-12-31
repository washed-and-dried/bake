main: build | builds
	@ ./builds/output

memchk: build | builds
	@ valgrind --leak-check=yes ./builds/output

build: ./main.c | builds
	@ gcc -o builds/output ./main.c

builds:
	mkdir -p $@
