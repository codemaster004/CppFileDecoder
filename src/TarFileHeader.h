class TarFileHeader {
private:
    char fileName[100];
    char fileMode[8];
    char ownerId[8];
    char groupId[8];
    unsigned long long int fileSize;
    char modificationDate[12];

    int importedBytesCount;

public:
    void appendBytes(const char bytes[], int count);

    void appendByte(char byte);
};


