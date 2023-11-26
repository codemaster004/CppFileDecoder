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

# Compressed block format

---
Each sequence starts with a `token`. The `token` is a one byte value, separated into two 4-bits fields.

```
0x27 = 0010 0111
```

The first field uses the 4 high-bits of the token. It provides the length of literals to follow.

> _This time value being `0010`= 2. So the sequence contain 2 output bytes_

If the value is less that 15 this represents **number of output bytes** in the sequence.

The value 15 is a special case: more bytes are required to indicate the full length. Each additional byte then
represents a value from 0 to 255, which is added to the previous value to produce a total length.

Following `token` and optional length bytes, are the literals themselves.

```
0x30 0x31 = 0 1
```

Following the literals is the match copy operation.

It starts by the `offset` value. This is a 2 bytes value, in little endian

The `offset` represents the position of the match to be copied from the past. For example, 1 means "current position - 1
byte".

```
0x0200 = 2
```

Then the `matchlength` can be extracted. For this, we use the second token field,
the low 4-bits. Here, 0 means that the copy operation is minimal.

The minimum length of a match, called `minmatch`, is 4. As a consequence, a 0 value means 4 bytes.
Similarly to literal length, any value smaller than 15 represents a length,
to which 4 (`minmatch`) must be added, thus ranging from 4 to 18.

Value of 15 means there are more bytes to be added to current count.

> _This time we have a value 0111 = 7. The full repetition count will be: 7 + minmatch = 11_

Decoding the `matchlength` reaches the end of current sequence.
Next byte will be the start of another sequence, and therefore a new `token`.

---

_Source: [LZ4 GitHub Docs](https://github.com/lz4/lz4/blob/dev/doc/lz4_Frame_format.md)_
