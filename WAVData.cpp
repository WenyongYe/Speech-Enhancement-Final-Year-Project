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

	// read 24-bit samples and conversion to 16-bit samples
	/* int* Sample_int = new int[nSamples];
	int maxSamp = 0;
	for(unsigned int n = 0; n < nSamples; n++) {
		int Samp4 = 0;
		inf.read((char*)&Samp4, hdr->Fmt_Chunk.BytesPerSam);

		unsigned char padding_byte = 0;
		if(((char*)&Samp4)[hdr->Fmt_Chunk.BytesPerSam - 1] & 128) padding_byte = 255;

		 for(int i = hdr->Fmt_Chunk.BytesPerSam; i < sizeof(Samp4); i++)
			((char*)&Samp4)[i] = padding_byte;

		 //Sample[n] = (short int)(Samp4/700);

		 Sample_int[n] = Samp4;
		 if(abs(Samp4) > maxSamp) maxSamp = abs(Samp4);
	}
	for(unsigned int n = 0; n < nSamples; n++)
		Sample[n] = (short int)(((float)Sample_int[n] / (float)maxSamp) * 32767);
	delete [] Sample_int;

	hdr->Compose(nSamples, hdr->Fmt_Chunk.Rate, 2);
	write("test.wav");*/

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


