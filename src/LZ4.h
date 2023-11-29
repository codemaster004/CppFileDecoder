//
// Created by Filip Dabkowski on 25/11/2023.
//

#ifndef BASE64FILEDECODER_LZ4_H
#define BASE64FILEDECODER_LZ4_H

#include "DecodedCharHolder.h"
#include "ByteContainer.h"

#define MAGIC_N_SIZE 4
#define FLG_SIZE 1
#define BD_SIZE 1
#define HC_SIZE 1
#define CON_SIZE_LEN 8
#define DICT_ID_LEN 4
#define BLOCK_SIZE_HEADER 4

class LZ4 {
private:
	int bytesCount = 0;
	char flg = 0, bd = 0, hc = 0;
	bool useConSize = false;
	char contentSize[CON_SIZE_LEN]{};
	bool useDictId = false;
	char dictID[DICT_ID_LEN]{};
	bool fileHeaderSet = false;
	bool fileEnded = false;

	bool dataBlockCompressed = false;
	enum CompressedSequenceState {
		TOKEN, LITERALS, OFFSET, EXTENDED_LITERAL_LEN, EXTENDED_MATCH_LEN
	};
	CompressedSequenceState seqState = TOKEN;
	unsigned long literalBytes = 0;
	unsigned long matchLength = 0;
	unsigned short offset = 0;
	short offsetCount = 0;
	bool readyToDecompressSeq = false;

	short blockSizeCount = 0;
	int blockSize = 0;

	void handleFileHeader(char byte);

	void handleDataBlock(char byte);

	void decodeBlockSize();

	static int createBitMask(int numberOfUpBits, int rightOffset);

	void decompressDataBlock(unsigned char byte);

public:
	explicit LZ4(size_t N, size_t secondDimSize)
		: output(N, secondDimSize) {
		// Other initialization code (if any)
	}

	void appendBytes(char byte);

	bool endOfFile() const;

	ByteContainer output;
};


#endif //BASE64FILEDECODER_LZ4_H
