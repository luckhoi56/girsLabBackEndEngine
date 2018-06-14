#pragma once

#include <iostream>
#include <fstream>

/**
 * Data output stream super class.
 *
 * Implementations of this class can be used by the 
 * engine to store its state, particularly generated
 * samples.
 *
 * @see BRASSEngine#writeSamples()
 * @see InputStream
 */
class OutputStream
{
public:
	OutputStream(void) {}
	virtual ~OutputStream(void) {}
	virtual int writeInt(const int val) = 0;
	virtual int writeFloatArray(const float* array, int size) = 0;
	virtual int writeByteArray(const char* array, int size) = 0;
};

/**
 * Data input stream super class.
 *
 * Implementations of this abstract class can be used by the 
 * engine to retrieve its state, particularly generated
 * samples.
 *
 * @see BRASSEngine#readSamples()
 * @see OutputStream
 */
class InputStream
{
public:
	InputStream(void) {}
	virtual ~InputStream(void) {}
	virtual int readInt(int & val) = 0;
	virtual int readFloatArray(float* array, int size) = 0;
	virtual int readByteArray(char* array, int size) = 0;
};

/**
 * Implementation of InputStream that reads from an istream.
 */
class StreamInputStream : public InputStream
{
public:
	StreamInputStream(std::istream &is);
	virtual ~StreamInputStream(void) {}
	virtual int readInt(int & val);
	virtual int readFloatArray(float* array, int size);
	virtual int readByteArray(char* array, int size);
private:
	std::istream * is;
};

/**
 * implementation of OutputStream that writes to an ostream.
 */
class StreamOutputStream : public OutputStream
{
public:
	StreamOutputStream(std::ostream &os);
	~StreamOutputStream(void);
	int writeInt(const int val);
	int writeFloatArray(const float* array, int size);
	int writeByteArray(const char* array, int size);
private:
	std::ostream * os;
};


