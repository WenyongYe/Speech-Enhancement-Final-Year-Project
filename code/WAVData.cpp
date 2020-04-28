#include "WAVData.h"
#include <fstream>

TWAVData::TWAVData()
{
	hdr = 0;
	Sample = 0;
}

TWAVData::~TWAVData()
{
	if(hdr) delete hdr;
	if(Sample) delete[] Sample;
}

bool TWAVData::load(char* name, bool header_only)
{
	ifstream inf(name, ios::binary);
	if(!inf) return 0;

	hdr = new TRIFF;
	hdr->Read(&inf);
	nSamples = hdr->Data_Hdr.Len / hdr->Fmt_Chunk.BytesPerSam;

	// read wave data
	if(!header_only) {
		Sample = new short int[nSamples];
		inf.read((char*)Sample, hdr->Data_Hdr.Len);
	}

	inf.close();
	return 1;
}

void TWAVData::write(char* name)
{
    ofstream of(name, ios::binary);
    hdr->Write(&of);
    of.write((char*)Sample, hdr->Data_Hdr.Len);
    of.close();
}


