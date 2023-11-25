#include "vector"

#include "DecodedCharHolder.h"

using namespace std;

// Add multiple elements to the array
void DecodedCharHolder::addElements(const char *values, int numElements) {
	unsigned long size = charBytes.size();
	charBytes.resize(size + numElements);
	for (int i = 0; i < numElements; ++i) {
		charBytes[size + i] = values[i];
	}
}

void DecodedCharHolder::addElement(const char value) {
	unsigned long size = charBytes.size();
	charBytes.resize(size + 1);
	charBytes[size] = value;
}

// Get size of private byte array
unsigned long DecodedCharHolder::size() {
	return charBytes.size();
}

// Get range of elements with specified beginning index and length
vector<char> DecodedCharHolder::getRange(int startIndex, int numOfElements) {
	vector<char> requestedBytes;
	requestedBytes.resize(numOfElements);

	for (int i = 0; i < numOfElements; ++i) {
		requestedBytes[i] = charBytes[startIndex + i];
	}

	return requestedBytes;
}

// Private function for checking if pattern matches
bool DecodedCharHolder::patternMatched(const char *pattern, int patternLength, long index) {
	bool patterMatched = true;
	for (int i = 0; i < patternLength; ++i) {
		if (charBytes[index + i] != pattern[i]) {
			patterMatched = false;
		}
	}
	return patterMatched;
}

// Find index of given pattern starting at the beginning of the vector
int DecodedCharHolder::findIndexOfPattern(const char *pattern, int patternLength) {
	for (int i = 0; i <= charBytes.size() - patternLength; ++i)
		if (this->patternMatched(pattern, patternLength, i))
			return i;

	return -1;
}

// Find index of given pattern starting at the end of the vector
long DecodedCharHolder::findIndexOfPatternReverse(const char *pattern, int patternLength) {
	for (long i = (long) (charBytes.size()) - patternLength; i > 0; i--)
		if (this->patternMatched(pattern, patternLength, i))
			return i;

	return -1;
}
