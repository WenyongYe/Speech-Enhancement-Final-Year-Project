#include <fstream>
#include <cstring>
using namespace std;

#ifndef RIFF_HH
#define RIFF_HH

class TRIFF
{
public:
	// RIFF header
	struct {
		char ID[4];			// "RIFF"
		int Len;			// Remaining length after this header
	} Riff_Hdr;

	// Wave id
	char Wave_ID[4];		// "WAVE"

	// RIFF format chunk header
	struct {
		char ID[4];			// "fmt"
		unsigned int Len;	// Remaining chunk length after this header
	} Format_Hdr;

	// RIFF format chunk
	struct {
		short int FormatTag;	// 0x0001->PCM, ...
		short int Channels;		// Number of channels
		int Rate;				// Sampling rate
		int BytesPerSec;		// Bytes per second
		short int BytesPerSam;	// Bytes per sample
		short int BitsPerSam;	// Bits per sample
	} Fmt_Chunk;

	// RIFF data chunk header
	struct {
		char ID[4];			// "data"
		unsigned int Len;	// Remaining chunk length after this header
	} Data_Hdr;

	// Read header
	bool Read(ifstream* inf);

	// Wrire header
	void Write(ofstream* of);

	// Compose header
	void Compose(unsigned int nSamples, int rate, int bytes_per_samp);
};
#endif

