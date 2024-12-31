main: ./main.c | builds
	@ gcc -o builds/output ./main.c
	@ ./builds/output

builds:
	mkdir -p $@
