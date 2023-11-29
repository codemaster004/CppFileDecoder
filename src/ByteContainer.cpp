//
// Created by Filip Dabkowski on 28/11/2023.
//

#include "ByteContainer.h"

void ByteContainer::expandFirstDimension() {
	auto **newData = new uint8_t *[nPartitions + 1];

	std::copy(data, data + nPartitions, newData);

	newData[nPartitions] = new uint8_t[partitionSize];

	delete[] data;
	data = newData;

	nPartitions++;
}

void ByteContainer::addElement(uint8_t byte) {
	size_t rowIndex = dataCount / partitionSize;
	size_t columnIndex = dataCount % partitionSize;

	if (rowIndex >= nPartitions) {
		// Expand the first dimension if needed
		expandFirstDimension();
	}

	data[rowIndex][columnIndex] = byte;
	dataCount++;
}

uint8_t ByteContainer::getByte(size_t index) {
	size_t rowIndex = index / partitionSize;
	size_t columnIndex = index % partitionSize;

	return data[rowIndex][columnIndex];
}
