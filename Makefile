main: build | builds
	@ ./builds/output

memchk: memvar = -g -O0
memchk: build | builds
	@ valgrind --leak-check=yes ./builds/output

build: ./main.c | builds
	@ gcc -o builds/output ./main.c $(memvar)

builds:
	mkdir -p $@