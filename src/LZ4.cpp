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
//		std::cout << byte;
		output.addElement(byte);
		if (!blockSize) {
			blockSizeCount = 0;
		}
	}
}

void LZ4::decodeBlockSize() {
	if (blockSize <= 0) {
		int mask = createBitMask();
		blockSize &= ~mask;
	} else {
		std::cout << "Huston we have a problem" << std::endl;
	}
}

int LZ4::createBitMask() {
	return ((1 << 1) - 1) << 31;
}

bool LZ4::endOfFile() const {
	return fileEnded;
}
