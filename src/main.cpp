#include "iostream"
#include "vector"
#include "string"

#include "PrintingFunctions.h"
#include "DecodedCharHolder.h"

using namespace std;

#define LINE_LENGTH 16

const int baseSize = 64;
const char base64table[baseSize] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R',
                              'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i',
                              'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
                              '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/'};


int indexOf(const char *searchTable, char searchElement, int maxElement);

int createBitMask(int numberOfUpBits, int rightOffset);

void inputAndDecodeBase64(DecodedCharHolder *store);

void binaryBrowser(DecodedCharHolder *store);

void readZipFile(DecodedCharHolder *store);


int main() {
    int mode;
    cin >> mode;
    getchar();
    getchar();

    DecodedCharHolder decodedBytes{};

    if (mode != 2)
        inputAndDecodeBase64(&decodedBytes);

    if (mode == 0) {
        binaryBrowser(&decodedBytes);
    } else if (mode == 1) {
//        binaryBrowser(&decodedBytes);
//        cout << endl;
        readZipFile(&decodedBytes);
    } else if (mode == 2) {

    } else {
        cout << mode << endl;
    }

    return 0;
}

void binaryBrowser(DecodedCharHolder *store) {
    unsigned long size = store->size();

    for (int i = 0; i < size / LINE_LENGTH; ++i) {
        vector<char> decodedBytes = store->getRange(LINE_LENGTH * i, LINE_LENGTH * (i + 1));
        int bytes[LINE_LENGTH];
        for (int j = 0; j < LINE_LENGTH; ++j) {
            bytes[j] = (unsigned char) (decodedBytes[j]);
        }
        printOutputStructure(i * LINE_LENGTH, bytes, LINE_LENGTH);
    }

    if (size % LINE_LENGTH != 0) {
        vector<char> decodedBytes = store->getRange(LINE_LENGTH * (size / LINE_LENGTH), (int) (size % LINE_LENGTH));
        int bytes[LINE_LENGTH];
        for (int j = 0; j < size % LINE_LENGTH; ++j) {
            bytes[j] = (unsigned char) (decodedBytes[j]);
        }
        printOutputStructure((int) (size / LINE_LENGTH * LINE_LENGTH), bytes, (int) (size % LINE_LENGTH));
    }

    printInHex((int) (size), 8);
}

void inputAndDecodeBase64(DecodedCharHolder *store) {
    int encodedNumber = 0;
    int enteredCharCountCheck = 0, paddingCount = 0;

    char tempChar;
    while (cin.get(tempChar) && tempChar != '\n') {
        enteredCharCountCheck++;

        if (tempChar == '=') {
            encodedNumber <<= 6;
            paddingCount++;
        } else {
            encodedNumber <<= 6;
            encodedNumber += indexOf(base64table, tempChar, baseSize);
        }

        if (enteredCharCountCheck == 4) {
            enteredCharCountCheck = 0;

            char decodedBytes[3];
            for (int i = 0; i < 3 - paddingCount; ++i) {

                int mask = createBitMask(8, 8 * (2 - i));
                int decodedNumber = (encodedNumber & mask) >> 8 * (2 - i);

                decodedBytes[i] = (char) (decodedNumber);
            }
            store->addElements(decodedBytes, 3 - paddingCount);
        }
    }
}

int reverseConvertToInt(const vector<char>& bytes) {
    int result = 0;
    for (int i = bytes.size() - 1; i >= 0; i--) {
        if (i != bytes.size())
            result <<= 8;
        result += bytes[i];
    }
    return result;
}

void readZipFile(DecodedCharHolder *store) {
    char pattern[4] = {80, 75, 5, 6};
    int index = store->findIndexOfPatternReverse(pattern, 4);
    int numberOfCd = reverseConvertToInt(store->getRange(index + 10, 2));
    int offsetToCd = reverseConvertToInt(store->getRange(index + 16, 4));

    vector<int> files;
    vector<int> dicts;
    for (int i = 0; i < numberOfCd; ++i) {
        int fileNameSize = reverseConvertToInt(store->getRange(offsetToCd + 28, 2));
        int extraFieldsSize = reverseConvertToInt(store->getRange(offsetToCd + 30, 2));
        int commentSize = reverseConvertToInt(store->getRange(offsetToCd + 32, 2));

        vector<char> fileName = store->getRange(offsetToCd + 46, fileNameSize);
        if (fileName[fileNameSize - 1] != '/') {
            for (char letter : fileName)
                files.push_back(letter);
            files.push_back(10);
        } else {
            for (char letter : fileName)
                dicts.push_back(letter);
            dicts.push_back(10);
        }

        offsetToCd += 46 + fileNameSize + extraFieldsSize + commentSize;
    }

    for (int letter : files)
        cout << (char) (letter);
    for (int letter : dicts)
        cout << (char) (letter);

}

int indexOf(const char *searchTable, char searchElement, int maxElement) {
    for (int i = 0; i < maxElement; ++i) {
        if (searchTable[i] == searchElement) {
            return i;
        }
    }

    return -1;
}

int createBitMask(int numberOfUpBits, int rightOffset) {
    // Other option: ((uint) (-1)) << (32-upBits) >> (32-upBits-offset)
    return ((1 << numberOfUpBits) - 1) << rightOffset;
}
