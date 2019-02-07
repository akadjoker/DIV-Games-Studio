
//様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様�
//  Convierte de PCM a WAV
//様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様�

//#include <dos.h>
#include "osdep.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define byte unsigned char
#define word unsigned short

int SaveWAV(byte *byte_ptr, int length, FILE *f);
int _IsWAV(FILE *FileName);

//様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様�
//  Programa principal
//様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様�

int pcm2wav(FILE *in, long inlen, FILE *out, long outlen)
{
	byte *ptr;
	int c;
	if (_IsWAV(in)) {
		return 1;
	}

	if ((ptr = (byte *)malloc(inlen)) == NULL) {
		printf("\n\nError: Memoria insuficiente.\n");
		return 4;
	}

	fseek(in, 0, SEEK_SET);

	if ((c = fread(ptr, 1, inlen, in)) != inlen) {
		printf("\n\nError: %d %d No se pudo leer el archivo.\n", c,
		       inlen);
		free(ptr);
		return 3;
	}

	if (!SaveWAV(ptr, inlen, out)) {
		printf("...Error\n");
		free(ptr);
		return 2;
	}

	//    printf("...Ok\n");

	free(ptr);
	return 0; // success
}

typedef struct _HeadDC {
	unsigned int dwUnknow;
	unsigned short wFormatTag;
	unsigned short wChannels;
	unsigned int dwSamplePerSec;
	unsigned int dwAvgBytesPerSec;
	unsigned short wBlockAlign;
	unsigned short wBits;
} HeadDC;

int SaveWAV(byte *byte_ptr, int length, FILE *dstfile)
{
	HeadDC MyHeadDC;
	// int    con;
	// float  paso,pos;

	fputc('R', dstfile);
	fputc('I', dstfile);
	fputc('F', dstfile);
	fputc('F', dstfile);

	length += 36;
	if (fwrite(&length, 1, 4, dstfile) != 4) {
		return (0);
	}
	length -= 36;

	fputc('W', dstfile);
	fputc('A', dstfile);
	fputc('V', dstfile);
	fputc('E', dstfile);
	fputc('f', dstfile);
	fputc('m', dstfile);
	fputc('t', dstfile);
	fputc(' ', dstfile);

	MyHeadDC.dwUnknow = 16;
	MyHeadDC.wFormatTag = 1;
	MyHeadDC.wChannels = 1;
	MyHeadDC.dwSamplePerSec = 11025;
	MyHeadDC.dwAvgBytesPerSec = 11025;
	MyHeadDC.wBlockAlign = 1;
	MyHeadDC.wBits = 8;

	if (fwrite(&MyHeadDC, 1, sizeof(HeadDC), dstfile) != sizeof(HeadDC)) {
		return (0);
	}

	fputc('d', dstfile);
	fputc('a', dstfile);
	fputc('t', dstfile);
	fputc('a', dstfile);

	if (fwrite(&length, 1, 4, dstfile) != 4) {
		return (0);
	}

	if (fwrite(byte_ptr, 1, length, dstfile) != length) {
		return (0);
	}

	return (1);
}

int _IsWAV(FILE *f)
{
	//  FILE *f;
	int ok = 1;

	// if((f=fopen(FileName,"rb"))==NULL) return(0);

	if (f == NULL)
		return (0);

	if (fgetc(f) != 'R')
		ok = 0;
	if (fgetc(f) != 'I')
		ok = 0;
	if (fgetc(f) != 'F')
		ok = 0;
	if (fgetc(f) != 'F')
		ok = 0;
	fseek(f, 4, SEEK_CUR);
	if (fgetc(f) != 'W')
		ok = 0;
	if (fgetc(f) != 'A')
		ok = 0;
	if (fgetc(f) != 'V')
		ok = 0;
	if (fgetc(f) != 'E')
		ok = 0;

	fseek(f, 0, SEEK_SET);

	return (ok);
}
