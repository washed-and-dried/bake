CFLAGS= -Wall -Wextra

main: build | builds
	@ # passing arguments to the program [bake TARGET1 TARGET2]
	@ ./builds/output bake main build

memchk: memvar = -g -O0
memchk: build | builds
	@ valgrind --leak-check=yes ./builds/output

build: main.cpp util.cpp | builds
	@ g++ $(CFLAGS) -o builds/output main.cpp $(memvar)

builds:
	@ mkdir -p $@

clean:
	@ rm -rdf builds
