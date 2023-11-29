//
// Created by Filip Dabkowski on 28/11/2023.
//

#ifndef BASE64FILEDECODER_BYTECONTAINER_H
#define BASE64FILEDECODER_BYTECONTAINER_H

#include <cstdint>
#include <cstddef>
#include <algorithm>

class ByteContainer {
private:
	uint8_t **data;
	size_t nPartitions;
	size_t partitionSize;
	size_t dataCount = 0;

	// Private method to expand the first-dimension array
	void expandFirstDimension();

public:
	// Constructor
	explicit ByteContainer(size_t N, size_t secondDimSize)
		: nPartitions(N), partitionSize(secondDimSize) {
		data = new uint8_t *[N];

		for (size_t i = 0; i < N; ++i) {
			data[i] = new uint8_t[secondDimSize];
		}
	}

	// Destructor to free allocated memory
	~ByteContainer() {
		for (size_t i = 0; i < nPartitions; ++i) {
			delete[] data[i];
		}
		delete[] data;
	}

	// Add a byte at a specific 'linear' index
	void addElement(uint8_t byte);

	// Get a byte from a specific 'linear' index
	uint8_t getByte(size_t index);

	size_t size() const {
		return dataCount;
	}
};


#endif //BASE64FILEDECODER_BYTECONTAINER_H
