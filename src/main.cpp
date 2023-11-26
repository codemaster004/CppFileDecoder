#include "iostream"
#include "vector"

#include "PrintingFunctions.h"
#include "TarFileHeader.h"
#include "LZ4.h"

using namespace std;

#define LINE_LENGTH 16

const int baseSize = 64;
const char base64table[baseSize] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q',
									'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h',
									'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y',
									'z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/'};


int indexOf(const char *searchTable, char searchElement, int maxElement);

int createBitMask(int numberOfUpBits, int rightOffset);

void inputAndDecodeBase64(DecodedCharHolder *store);

void binaryBrowser(DecodedCharHolder *store);

vector<char> decodeBase64(const char base64String[]);

void readZipFile(DecodedCharHolder *store);

void handleTarFileDecoding();

void handleLz4Decoding();

int main() {
	int mode;
	cin >> mode;
	getchar();
	getchar();

	DecodedCharHolder decodedBytes{};

	if (mode < 2)
		inputAndDecodeBase64(&decodedBytes);

	if (mode == 0) {
		binaryBrowser(&decodedBytes);
	} else if (mode == 1) {
//        binaryBrowser(&decodedBytes);
//        cout << endl;
		readZipFile(&decodedBytes);
	} else if (mode == 2) {
//        binaryBrowser(&decodedBytes);
		handleTarFileDecoding();

	} else if (mode == 3) {
		handleLz4Decoding();
	} else {
		cout << mode << endl;
	}

	return 0;
}

vector<char> decodeBase64(const char base64String[]) {
	int paddingCount = 0;
	int encodedNumber = 0;
	for (int i = 0; i < 4; ++i) {
		char tempChar = base64String[i];
		if (tempChar == '=') {
			encodedNumber <<= 6;
			paddingCount++;
		} else {
			encodedNumber <<= 6;
			encodedNumber += indexOf(base64table, tempChar, baseSize);
		}

		if (i == 3) {
			vector<char> decodedBytes;
			for (int j = 0; j < 3 - paddingCount; ++j) {
				int mask = createBitMask(8, 8 * (2 - j));
				int decodedNumber = (encodedNumber & mask) >> 8 * (2 - j);

				decodedBytes.push_back((char) (decodedNumber));
			}
			return decodedBytes;
		}
	}
	return {};
}

void handleLz4Decoding() {
	LZ4 compressedFile{};
//	compressedFile.output.

	int tempBase64Count = 0;
	char tempBase64[4];

	char tempChar = '\0';
	while (tempChar != '\n') {
		tempChar = getchar();
		tempBase64[tempBase64Count++] = tempChar;

		if (tempBase64Count == 4) {
			tempBase64Count = 0;

			for (char decodedChar: decodeBase64(tempBase64)) {
				compressedFile.appendBytes(decodedChar);
			}
		}

		if (compressedFile.endOfFile()) {
			binaryBrowser(&compressedFile.output);
			return;
		}
	}
}

void handleTarFileDecoding() {
	TarFileHeader fileHeader{};
	fileHeader.clear();

	int tempBase64Count = 0;
	char tempBase64[4];

	vector<char> files;
	for (char letter: "Files: \n")
		files.push_back(letter);
	files.pop_back();

	vector<char> dirs;
	for (char letter: "Directories: \n")
		dirs.push_back(letter);
	dirs.pop_back();


	int decodedHeaderBytesCount = 0;
	unsigned long long int bytesToIgnore = 0;
	char tempChar = '\0';
	while (tempChar != '\n') {
		cin >> tempChar;
		tempBase64[tempBase64Count++] = tempChar;

		if (tempBase64Count == 4) {
			tempBase64Count = 0;

			for (char decodedChar: decodeBase64(tempBase64)) {
				if (bytesToIgnore > 0) {
					bytesToIgnore--;
					continue;
				}
				if (decodedHeaderBytesCount == 0 && decodedChar == 0) {
					for (int letter: files)
						cout << (char) (letter);
					for (int letter: dirs)
						cout << (char) (letter);
					return;
				}

				fileHeader.appendByte(decodedChar);
				decodedHeaderBytesCount++;
				if (decodedHeaderBytesCount == 512) {
					char *tempName = fileHeader.getFileName();
					int fileNameLength = 0;
					for (int i = 0; i < 100; ++i) {
						if (tempName[i] != '\0')
							fileNameLength++;
						else
							break;
					}
					if (tempName[fileNameLength - 1] != '/') {
						for (int i = 0; i < fileNameLength; ++i) {
							files.push_back(tempName[i]);
						}
						files.push_back(10);
					} else {
						for (int i = 0; i < fileNameLength; ++i) {
							dirs.push_back(tempName[i]);
						}
						dirs.push_back(10);
					}


					bytesToIgnore = fileHeader.getFileSize();
					bytesToIgnore = bytesToIgnore % 512 ? bytesToIgnore + (512 - bytesToIgnore % 512) : bytesToIgnore;

					// Clean up
					fileHeader.clear();
					decodedHeaderBytesCount = 0;
				}
			}
		}

	}

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
	char tempBase64[4];
	while (cin.get(tempChar) && tempChar != '\n') {
		tempBase64[enteredCharCountCheck++] = tempChar;
		if (enteredCharCountCheck == 4) {
			enteredCharCountCheck = 0;
			vector<char> decodedChars = decodeBase64(tempBase64);

			char decodedBytes[3];
			for (char temp: decodedChars) {
				decodedBytes[enteredCharCountCheck++] = temp;
			}
			enteredCharCountCheck = 0;

			store->addElements(decodedBytes, (int) (decodedChars.size()));
		}
	}
}

int reverseConvertToInt(const vector<char> &bytes) {
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
			for (char letter: fileName)
				files.push_back(letter);
			files.push_back(10);
		} else {
			for (char letter: fileName)
				dicts.push_back(letter);
			dicts.push_back(10);
		}

		offsetToCd += 46 + fileNameSize + extraFieldsSize + commentSize;
	}

	for (int letter: files)
		cout << (char) (letter);
	for (int letter: dicts)
		cout << (char) (letter);

}

int indexOf(const char *searchTable, char searchElement, int maxElement) {
	for (int i = 0; i < maxElement; ++i)
		if (searchTable[i] == searchElement)
			return i;
	return -1;
}

int createBitMask(int numberOfUpBits, int rightOffset) {
	// Other option: ((uint) (-1)) << (32-upBits) >> (32-upBits-offset)
	return ((1 << numberOfUpBits) - 1) << rightOffset;
}
