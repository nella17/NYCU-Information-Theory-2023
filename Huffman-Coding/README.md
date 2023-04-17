# Huffman Coding

This is a implementation of Huffman Coding in C++, including basic, adaptive and limited extended Huffman Coding Algorithm.

## Build

Use `make build` to build the binary, and use `make clean` to cleanup.
The binary will be generated at `bin/release/huffman-coding.exe`.

## Usage

### Example

There are some example shell scripts in `scripts` folder.

### Help

```bash
$ ./bin/release/huffman-coding.exe -h
Usage: ./bin/release/huffman-coding.exe -t <type> [-e | -d] [options...] [-i <file>] [-o <file>]
    -t, --type <type>       Set coding algorithm
    -e, --encode            Encode
    -d, --decode            Decode
    -i, --input <file>      Set input file (default STDIN)
    -o, --output <file>     Set output file (default STDOUT)
    --ostream               Output stream
    -b, --bits <bits>       Tread input file as <bits> data source (default 8)
    -s, --split <size>      Split input file every <size> bytes of data (default ∞)
    -v, --verbose           Show debug / analysis /time info
    --pmf                   Show pmf freq
    --no-time               No show time info
    -h, --help              Show this help

Coding Algorithms
    analysis    Analysis entropy
    basic       Basic Huffman Coding Algorithm (bits: 8 <= 8k <= 64)
    adaptive    Adaptive Huffman Coding Algorithm
    extended    Extended Huffman Coding Algorithm (TODO)
```

### Basic Huffman Coding

`./scripts/example-basic.sh`

```bash
# encode
./bin/release/huffman-coding.exe \
  -t basic -b 8 -v -e \
  -i ./alexnet.pth -o "./alexnet.pth.basic.8.enc"
# decode
./bin/release/huffman-coding.exe \
  -t basic -b 8 -v -d \
  -i "./alexnet.pth.basic.8.enc" -o "./alexnet.pth.basic.8.dec"
# verify
diff ./alexnet.pth ./alexnet.pth.basic.8.dec && echo "OK" || echo "Failed"
# cleanup
rm ./alexnet.pth.basic.8.enc ./alexnet.pth.basic.8.dec
```

### Basic Huffman Coding with Split every 40MB

`./scripts/example-basic-split-40MB.sh`

```bash
# encode
./bin/release/huffman-coding.exe \
  -t basic -b 8 -v -e -s 41943040 \
  -i ./alexnet.pth -o "./alexnet.pth.basic.8.s40MB.enc"
# decode
./bin/release/huffman-coding.exe \
  -t basic -b 8 -v -d -s 41943040 \
  -i "./alexnet.pth.basic.8.s40MB.enc" -o "./alexnet.pth.basic.8.s40MB.dec"
# verify
diff ./alexnet.pth ./alexnet.pth.basic.8.s40MB.dec && echo "OK" || echo "Failed"
# cleanup
rm ./alexnet.pth.basic.8.s40MB.enc ./alexnet.pth.basic.8.s40MB.dec
```

### Adaptive Huffman Coding

Currently, adaptive Huffman Coding only support 8-bits data source.

`./scripts/example-adaptive.sh`

```bash
# encode
./bin/release/huffman-coding.exe \
  -t adaptive -v -e \
  -i ./alexnet.pth -o "./alexnet.pth.adaptive.8.enc"
# decode
./bin/release/huffman-coding.exe \
  -t adaptive -v -d \
  -i "./alexnet.pth.adaptive.8.enc" -o "./alexnet.pth.adaptive.8.dec"
# verify
diff ./alexnet.pth ./alexnet.pth.adaptive.8.dec && echo "OK" || echo "Failed"
# cleanup
rm ./alexnet.pth.adaptive.8.enc ./alexnet.pth.adaptive.8.dec
```

### Adaptive Huffman Coding with Stream Output

`./scripts/example-adaptive-stream.sh`

```bash
# encode and decode and verify
./bin/release/huffman-coding.exe \
  -t adaptive -v --no-time -e \
  -i ./alexnet.pth --ostream \
  | \
./bin/release/huffman-coding.exe \
  -t adaptive -v -d  --ostream \
  | \
diff ./alexnet.pth /dev/stdin \
  && echo "OK" || echo "Failed"
```

### Extended Huffman Coding

Currently, extended Huffman Coding in `basic` algorithm parameter, only support $n$-bits data source, where $n$ is a multiple of 8 and $8 ≤ n ≤ 64$.

`./scripts/example-extended.sh`

```bash
# encode
./bin/release/huffman-coding.exe \
  -t basic -b 32 -v -e \
  -i ./alexnet.pth -o "./alexnet.pth.basic.32.enc"
# decode
./bin/release/huffman-coding.exe \
  -t basic -b 32 -v -d \
  -i "./alexnet.pth.basic.32.enc" -o "./alexnet.pth.basic.32.dec"
# verify
diff ./alexnet.pth ./alexnet.pth.basic.32.dec && echo "OK" || echo "Failed"
# cleanup
rm ./alexnet.pth.basic.32.enc ./alexnet.pth.basic.32.dec
```
