#include "RIFF.h"

bool
TRIFF::Read(ifstream* inf)
{
	char c;

	// Read RIFF header
	inf->read((char*)&Riff_Hdr, sizeof(Riff_Hdr));
	if(!strstr(Riff_Hdr.ID, "RIFF")) return 0;
	// Read wave id
	inf->read((char*)Wave_ID, 4);

	// Read format chunk header
	inf->read((char*)&Format_Hdr, sizeof(Format_Hdr));
	// Read format chunk
	inf->read((char*)&Fmt_Chunk, sizeof(Fmt_Chunk));
	unsigned int tmp = Format_Hdr.Len - sizeof(Fmt_Chunk);
	for(unsigned int i = 0; i < tmp; i++) inf->get(c);

	// PCM only
	if(Fmt_Chunk.FormatTag != 1) return 0;

	// Search for data chunk header
	for(;;) {
		inf->read((char*)&Data_Hdr, sizeof(Data_Hdr));
		if(strstr(Data_Hdr.ID, "data")) break;
		for(unsigned int i = 0; i < Data_Hdr.Len; i++) inf->get(c);
	}

	return 1;
}

void
TRIFF::Write(ofstream* of)
{
	char c = ' ';

	// Write RIFF header
	of->write((char*)&Riff_Hdr, sizeof(Riff_Hdr));
	// Write wave id
	of->write((char*)Wave_ID, 4);

	// Write format chunk header
	of->write((char*)&Format_Hdr, sizeof(Format_Hdr));
	// Write format chunk
	of->write((char*)&Fmt_Chunk, sizeof(Fmt_Chunk));
	unsigned int tmp = Format_Hdr.Len - sizeof(Fmt_Chunk);
	for(unsigned int i = 0; i < tmp; i++) of->put(c);

	// Write data chunk header
	of->write((char*)&Data_Hdr, sizeof(Data_Hdr));
}

void
TRIFF::Compose(unsigned int nSamples, int rate, int bytes_per_samp)
{
	Riff_Hdr.ID[0] = 'R'; Riff_Hdr.ID[1] = 'I'; Riff_Hdr.ID[2] = 'F'; Riff_Hdr.ID[3] = 'F';

	Wave_ID[0] = 'W'; Wave_ID[1] = 'A'; Wave_ID[2] = 'V'; Wave_ID[3] = 'E';

	Format_Hdr.ID[0] = 'f'; Format_Hdr.ID[1] = 'm'; Format_Hdr.ID[2] = 't'; Format_Hdr.ID[3] = ' ';
	Format_Hdr.Len = 16;
	Fmt_Chunk.FormatTag = 0x0001; 	// PCM
	Fmt_Chunk.Channels = 1; 		// Mono
	Fmt_Chunk.Rate = rate;
	Fmt_Chunk.BytesPerSam = bytes_per_samp;
	Fmt_Chunk.BytesPerSec = rate * bytes_per_samp;
	Fmt_Chunk.BitsPerSam = 8 * bytes_per_samp;

	Data_Hdr.ID[0] = 'd'; Data_Hdr.ID[1] = 'a'; Data_Hdr.ID[2] = 't'; Data_Hdr.ID[3] = 'a';
	Data_Hdr.Len = nSamples * bytes_per_samp;

	Riff_Hdr.Len = 36 + Data_Hdr.Len;
}
