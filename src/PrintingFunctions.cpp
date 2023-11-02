#include "iostream"

#include "PrintingFunctions.h"

using namespace std;

#define LINE_LENGTH 16

void printOutputStructure(int byteIndex, int *bytes, int maxByte) {
    printInHex(byteIndex, 8);
    cout << "  ";

    for (int i = 0; i < maxByte; ++i) {
        if (i == 8)
            cout << " ";
        printInHex(bytes[i], 2);
        cout << " ";
    }
    for (int i = 0; i < LINE_LENGTH - maxByte; ++i) {
        if (i + maxByte == 8)
            cout << " ";
        cout << "   ";
    }
    cout << " ";
    printDecodedAsChars(bytes, maxByte);
    cout << endl;
}

void printInHex(int value, int desiredLength) {
    char decodedValue[8];
    for (int i = 0; i < desiredLength; ++i) {
        decodedValue[i] = '0';
    }

    int index = desiredLength - 1;
    while (value > 0) {
        int newPart = value % 16;
        decodedValue[index--] = (char) (newPart >= 10 ? newPart + 87 : newPart + 48);
        value /= 16;
    }

    for (int i = 0; i < desiredLength; ++i) {
        cout << decodedValue[i];
    }
}

void printDecodedAsChars(int *decoded, int length) {
    cout << '|';
    for (int i = 0; i < length; ++i) {
        cout << (char) (decoded[i] >= 32 && decoded[i] < 127 ? decoded[i] : 46);
    }
    cout << '|';
}