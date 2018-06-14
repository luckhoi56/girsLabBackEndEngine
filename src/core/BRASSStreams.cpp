#include "BRASSStreams.h"

using namespace std;

/**
 * Constructor.
 */
StreamOutputStream::StreamOutputStream(ostream &os)
{
	this->os = &os;
}

StreamOutputStream::~StreamOutputStream(void)
{
}

/**
 * Writes an integer from the stream.
 * Returns -1 on failure. Returns 0 on success.
 */
int StreamOutputStream::writeInt(const int val)
{
	return writeByteArray((const char*)&val,4);
}

/**
 * Writes a float array to the stream.
 * Returns -1 on failure. Returns 0 on success.
 */
int StreamOutputStream::writeFloatArray(const float* array, int size)
{
	return writeByteArray((const char*)array,size * 4);
}

/**
 * Writes a byte array to the stream.
 * Returns -1 on failure. Returns 0 on success.
 */
int StreamOutputStream::writeByteArray(const char* array, int size)
{
	try {
		os->write((const char *)array,size);
	} catch (...) {
		return -1;
	} 
	return 0;
}

/// Constructor.
StreamInputStream::StreamInputStream(std::istream &is)
{
	this->is = &is;
}

/**
 * Reads an integer from the stream.
 * Returns -1 on failure. Returns 0 on success.
 */
int StreamInputStream::readInt(int & val)
{
	return readByteArray((char*)&val,4);
}

/**
 * Reads a float array from the stream.
 * Returns -1 on failure. Returns 0 on success.
 */
int StreamInputStream::readFloatArray(float* array, int size)
{
	return readByteArray((char*)array,size * 4);
}

/**
 * Reads a byte array from the stream.
 * Returns -1 on failure. Returns 0 on success.
 */
int StreamInputStream::readByteArray(char* array, int size)
{
	try {
		is->read(array,size);
	} catch (...) {
		return -1;
	}
	return 0;
}