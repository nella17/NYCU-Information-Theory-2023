# Arithmetic Coding

This is a implementation of Arithmetic Coding in C++.

## Build

Use `make build` to build the binary, and use `make clean` to cleanup.
The binary will be generated at `bin/release/arithmetic-coding.exe`.

## Usage

### Example

There are some example shell scripts in `scripts` folder.

### Help

```bash
$ ./bin/release/arithmetic-coding.exe -h
Usage: ./bin/release/arithmetic-coding.exe -t <type> [-e | -d] [options...] [-b <bits>] [-i <file>] [-o <file>]
    -t, --type <type>       Set coding algorithm
    -e, --encode            Encode
    -d, --decode            Decode
    -i, --input <file>      Set input file (default STDIN)
    -o, --output <file>     Set output file (default STDOUT)
    -b, --bits <bits>       Tread input file as <bits> data source (default 8) (1 / 8)
    -n, --order <order>     Set PPM context order (default 2)
    -v, --verbose           Show debug / analysis /time info
    --pmf                   Show pmf freq
    --no-time               No show time info
    --no-skip               No excludesive in ppm
    -h, --help              Show this help

Coding Algorithms
    arithmetic-ppm  Context Arithmetic Coding Algorithm (Prediction with Partial Match)
    arithmetic-fpm  Arithmetic Coding Algorithm (Fixed Probability Model)
```

### Fixed Probability Model

`./scripts/example-fpm.sh`

```bash
# encode
./bin/release/arithmetic-coding.exe \
  -t arithmetic-fpm -b 8 -v -e \
  -i ./alexnet.pth -o "./alexnet.pth.fpm.8.enc"
# decode
./bin/release/arithmetic-coding.exe \
  -t arithmetic-fpm -b 8 -v -d \
  -i "./alexnet.pth.fpm.8.enc" -o "./alexnet.pth.fpm.8.dec"
# verify
diff ./alexnet.pth ./alexnet.pth.fpm.8.dec && echo "OK" || echo "Failed"
# cleanup
rm ./alexnet.pth.fpm.8.enc ./alexnet.pth.fpm.8.dec
```

### Prediction with Partial Match

`./scripts/example-ppm.sh`

```bash
# encode
./bin/release/arithmetic-coding.exe \
  -t arithmetic-ppm -b 8 -n 1 -v -e \
  -i ./alexnet.pth -o "./alexnet.pth.ppm.8.1.enc"
# decode
./bin/release/arithmetic-coding.exe \
  -t arithmetic-ppm -b 8 -n 1 -v -d \
  -i "./alexnet.pth.ppm.8.1.enc" -o "./alexnet.pth.ppm.8.1.dec"
# verify
diff ./alexnet.pth ./alexnet.pth.ppm.8.1.dec && echo "OK" || echo "Failed"
# cleanup
rm ./alexnet.pth.ppm.8.1.enc ./alexnet.pth.ppm.8.1.dec
```
