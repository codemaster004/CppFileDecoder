//
// Created by Filip Dabkowski on 09/11/2023.
//

#include "TarFileHeader.h"

void TarFileHeader::appendBytes(const char bytes[], int count) {
    for (int i = 0; i < count; ++i) {
        this->appendByte(bytes[i]);
    }
}

void TarFileHeader::appendByte(char byte) {
    if (this->importedBytesCount < 100) {
        this->fileName[this->importedBytesCount] = byte;
    } else if (this->importedBytesCount < 108) {
        this->fileMode[this->importedBytesCount - 100] = byte;
    } else if (this->importedBytesCount < 116) {
        this->ownerId[this->importedBytesCount - 108] = byte;
    } else if (this->importedBytesCount < 124) {
        this->groupId[this->importedBytesCount - 116] = byte;
    } else if (this->importedBytesCount < 136) {
        this->fileSize <<= 8;
        this->fileSize += (short) (byte);
    } else if (this->importedBytesCount < 148) {
        this->modificationDate[this->importedBytesCount - 136] = byte;
    }
    this->importedBytesCount++;
}
