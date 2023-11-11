#include "utility"

using namespace std;

class TarFileHeader {
private:

    static const int FILE_NAME_SIZE = 100;
    static const int FILE_MODE_SIZE = 8;
    static const int OWNER_ID_SIZE = 8;
    static const int GROUP_ID_SIZE = 8;
    static const int FILE_SIZE_SIZE = 12;
    // ... Some other fields should be here but for simplicity’s sake lets ignore them as they are with no use to us

    char fileName[FILE_NAME_SIZE];
    char fileMode[FILE_MODE_SIZE];
    char ownerId[OWNER_ID_SIZE];
    char groupId[GROUP_ID_SIZE];
    char fileSize[FILE_SIZE_SIZE];

    int importedBytesCount;

    pair<char *, int> getFieldAndOffset();

public:
    void appendBytes(const char bytes[], int count);

    void appendByte(char byte);

    void clear();

    char *getFileName() {
        return this->fileName;
    }

    unsigned long long int getFileSize();

};


