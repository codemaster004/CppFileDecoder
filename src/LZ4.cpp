//
// Created by Filip Dabkowski on 25/11/2023.
//

#include "iostream"

#include "LZ4.h"

void LZ4::appendBytes(char byte) {
	if (!fileHeaderSet) {
		handleFileHeader(byte);
	} else if (fileEnded) {}
	else {
		handleDataBlock(byte);
	}

	bytesCount++;
}

void LZ4::handleFileHeader(char byte) {
	int totalCheck = 0;
	if (bytesCount < (totalCheck += MAGIC_N_SIZE)) {
		return;
	} else if (bytesCount < (totalCheck += FLG_SIZE)) {
		flg = byte;
	} else if (bytesCount < (totalCheck += BD_SIZE)) {
		bd = byte;
	} else if (useConSize && bytesCount < (totalCheck += CON_SIZE_LEN)) {

	} else if (useDictId && bytesCount < (totalCheck += DICT_ID_LEN)) {

	} else if (bytesCount < (totalCheck += HC_SIZE)) {
		hc = byte;
		fileHeaderSet = true;
	}
}

void LZ4::handleDataBlock(char byte) {
	if (blockSizeCount < BLOCK_SIZE_HEADER) {
		blockSize += ((int) (byte) << (8 * blockSizeCount));

		if (++blockSizeCount == BLOCK_SIZE_HEADER) {
			decodeBlockSize();
			if (!blockSize) {
				fileEnded = true;
			}
		}
	} else if (blockSize--) {
		if (!dataBlockCompressed) {
			output.addElement(byte);
		} else {
			decompressDataBlock(byte);
		}
		if (!blockSize) {
			blockSizeCount = 0;
		}
	}
}

void LZ4::decodeBlockSize() {
	if (blockSize <= 0) {
		int mask = createBitMask(1, 4 * 8 - 1);
		blockSize &= ~mask;
	} else {
		dataBlockCompressed = true;
	}
}

int LZ4::createBitMask(int numberOfUpBits, int rightOffset) {
	return ((1 << numberOfUpBits) - 1) << rightOffset;
}

bool LZ4::endOfFile() const {
	return fileEnded;
}

void LZ4::decompressDataBlock(unsigned char byte) {
	int mask;
	switch (seqState) {
		case TOKEN:
			mask = createBitMask(4, 4);
			literalBytes = (byte & mask) >> 4;
			mask = createBitMask(4, 0);
			matchLength = 4 + (byte & mask);
			if (literalBytes == 15) {
				seqState = EXTENDED_LITERAL_LEN;
			} else {
				seqState = LITERALS;
				if (!literalBytes) {
					seqState = OFFSET;
				}
			}
			break;
		case LITERALS:
			output.addElement((char) (byte));
			if (!--literalBytes) {
				seqState = OFFSET;
			}
			break;
		case OFFSET:
			offset += (unsigned short) (byte) << (offsetCount++ * 8);
			if (offsetCount >= 2) {
				seqState = matchLength == 19 ? EXTENDED_MATCH_LEN : TOKEN;
				if (matchLength != 19) {
					readyToDecompressSeq = true;
				}
				offsetCount = 0;
			}
			break;
		case EXTENDED_LITERAL_LEN:
			literalBytes += byte;
			if (byte < 255) {
				seqState = LITERALS;
			}
			break;
		case EXTENDED_MATCH_LEN:
			matchLength += byte;
			if (byte < 255) {
				readyToDecompressSeq = true;
				seqState = TOKEN;
			}
			break;
	}
	if (readyToDecompressSeq) {
		unsigned long len = output.size();
		vector<char> decodedBytes = output.getRange((int) (len - offset), (int) (offset));
		for (int i = 0; i < matchLength; ++i) {
			output.addElement(decodedBytes[i % offset]);
		}
		readyToDecompressSeq = false;
		matchLength = 0;
		offset = 0;
	}
}