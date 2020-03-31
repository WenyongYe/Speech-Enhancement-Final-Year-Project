#ifndef WAVDataH
#define WAVDataH

#include "RIFF.h"

class TWAVData
{
public:
	TWAVData();
	~TWAVData();

	// RIFF header
	TRIFF* hdr;

	unsigned int nSamples;
	short int* Sample;

	bool load(char* name, bool header_only = false);
	void write(char* name);
};
#endif
