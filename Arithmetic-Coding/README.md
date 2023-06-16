# Arithmetic Coding

This is a implementation of Arithmetic Coding in C++.

## Build

Use `make build` to build the binary, and use `make clean` to cleanup.
The binary will be generated at `bin/release/arithmetic-coding.exe`.

## Usage

### Example

There is a experiment shell scripts in `scripts/exp.sh`.

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
