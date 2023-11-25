#include "vector"

using namespace std;

class DecodedCharHolder {
private:
	vector<char> charBytes;

	bool patternMatched(const char *pattern, int patternLength, long index);

public:
	void addElements(const char *values, int numElements);

	void addElement(char value);

	unsigned long size();

	vector<char> getRange(int startIndex, int numOfElements);

	int findIndexOfPattern(const char *pattern, int patternLength);

	long findIndexOfPatternReverse(const char *pattern, int patternLength);
};
