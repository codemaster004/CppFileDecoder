//
// Created by Filip Dabkowski on 25/11/2023.
//

#ifndef BASE64FILEDECODER_LZ4_H
#define BASE64FILEDECODER_LZ4_H

#define MAGIC_N_SIZE 4
#define FLG_SIZE 1
#define BD_SIZE 1
#define HC_SIZE 1
#define CON_SIZE_LEN 8
#define DICT_ID_LEN 4

class LZ4 {
private:
	int bytesCount = 0;
	char flg = 0, bd = 0, hc = 0;
	bool useConSize = false;
	char contentSize[CON_SIZE_LEN]{};
	bool useDictId = false;
	char dictID[DICT_ID_LEN]{};
	bool fileHeaderSet = false;

	void handleFileHeader(char byte);

public:
	void appendBytes(char byte);
};


#endif //BASE64FILEDECODER_LZ4_H
