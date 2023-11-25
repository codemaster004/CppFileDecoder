# Magic number

---

```
04 22 4d 18
```

# Frame Descriptor

---

| FLG    | BD     | (Content Size) | (Dictionary ID) | HC     |
|--------|--------|:--------------:|:---------------:|--------|
| 1 byte | 1 byte |  0 - 8 bytes   |   0 - 4 bytes   | 1 byte |

## FLG byte

```
64 = 0110 0100
```

| Version | B.Indep | B.Checksum | C.Size | C.Checksum | Reserved | DictID |
|:-------:|:-------:|:----------:|:------:|:----------:|:--------:|:------:|
|   01    |    1    |     0      |   0    |     1      |    0     |   0    |

B.Indep - Blocks are considered to be independent

C.Checksum - At the end of a file is a 32 bit strucuture regarding size of deocded sequnce

## BD byte

```
40 = 0100 0000
```

| Reserved | Block MaxSize | Reserved |
|:--------:|:-------------:|:--------:|
|    0     |      100      |   0000   |

Block MaxSize - As the value is 4 we know the maximal size od decoded data is 64 KB

## Header Checksum

```
a7 = 1010 0111
```

# Data Blocks

---

| Block Size | data | (Block Checksum) |
|:----------:|------|:----------------:|
|  4 bytes   |      |   0 - 4 bytes    |

_In my case B.Checksum = 0 so it is NOT present_

```
0b 00 00 80
0000 1011  0000 0000  0000 0000  1000 0000
```

The highest bit being `1` means block is uncompressed cna be read normally

If it was set to `0` this would mean the block is LZ4-compressed

_Remember the documentation is in Big Endian meaning for normal machines "highest" means last (third) byte fist bit!_

ExampleBlock size `0b 00 00 80` without flag bit translates to `11` Bytes