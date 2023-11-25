//
// Created by Filip Dabkowski on 25/11/2023.
//

#include "LZ4.h"

void LZ4::appendBytes(char byte) {
	if (!fileHeaderSet) {
		handleFileHeader(byte);
	}
	bytesCount++;
}

void LZ4::handleFileHeader(char byte) {
	int totalCheck = 0;
	if (bytesCount < (totalCheck += MAGIC_N_SIZE)) {
		return;
	} else if (bytesCount <= (totalCheck += FLG_SIZE)) {
		flg = byte;
	} else if (bytesCount <= (totalCheck += BD_SIZE)) {
		bd = byte;
	} else if (useConSize && bytesCount <= (totalCheck += CON_SIZE_LEN)) {

	} else if (useDictId && bytesCount <= (totalCheck += DICT_ID_LEN)) {

	} else if (bytesCount <= (totalCheck += HC_SIZE)) {
		hc = byte;
		fileHeaderSet = true;
	}
}
