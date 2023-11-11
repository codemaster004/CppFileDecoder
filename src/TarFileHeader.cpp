//
// Created by Filip Dabkowski on 09/11/2023.
//

#include "utility"

#include "TarFileHeader.h"

using namespace std;

void TarFileHeader::appendBytes(const char bytes[], int count) {
    for (int i = 0; i < count; ++i) {
        this->appendByte(bytes[i]);
    }
}

void TarFileHeader::appendByte(char byte) {
    auto [field, offset] = getFieldAndOffset();
    if (field)
        field[offset] = byte;
    importedBytesCount++;
}

void TarFileHeader::clear() {
    this->importedBytesCount = 0;

    for (char &i: fileName)
        i = 0;
    for (char &i: fileMode)
        i = 0;
    for (char &i: ownerId)
        i = 0;
    for (char &i: groupId)
        i = 0;
    for (char &i: fileSize)
        i = 0;

}

unsigned long long int TarFileHeader::getFileSize() {
    // TODO: it is octal based do something.
    unsigned int size = 0;
    for (char charDigit: fileSize) {
        if (charDigit > 48 && charDigit < 58) {
            size *= 10;
            size += (unsigned short) (charDigit);
        }
    }
    return size;
}

pair<char *, int> TarFileHeader::getFieldAndOffset() {
    int totalSize = 0;
    if (importedBytesCount < (totalSize += FILE_NAME_SIZE))
        return {fileName, importedBytesCount};

    if (importedBytesCount < (totalSize += FILE_MODE_SIZE))
        return {fileMode, importedBytesCount - totalSize + FILE_MODE_SIZE};

    if (importedBytesCount < (totalSize += OWNER_ID_SIZE))
        return {ownerId, importedBytesCount - totalSize + OWNER_ID_SIZE};

    if (importedBytesCount < (totalSize += GROUP_ID_SIZE))
        return {groupId, importedBytesCount - totalSize + GROUP_ID_SIZE};

    if (importedBytesCount < (totalSize += FILE_SIZE_SIZE))
        return {fileSize, importedBytesCount - totalSize + FILE_SIZE_SIZE};

    return {nullptr, 0};
}
