## Bake

a GNU Make downgrade

### Development

##### [valgrind](https://valgrind.org) recommended to test any mem leaks

```bash
valgrind --leak-check=yes ./builds/output
```

#### Using `make`
- `make` to build and run
- `make memchk` to build Bake and test using `valgrind`
