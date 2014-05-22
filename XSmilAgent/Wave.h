  // Wave.h: interface for the CWave class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WAVE_H__5DF5361A_4047_40DF_A13A_5A5CA8971DEA__INCLUDED_)
#define AFX_WAVE_H__5DF5361A_4047_40DF_A13A_5A5CA8971DEA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <string>
#include "Windows.h"
using namespace std;	

#define RIFF_SIZE 12
#define FMT_SIZE 24
#define DATA_SIZE 8
#define FACT_SIZE 8
class CWave  
{
public:
	CWave insertSilenceBefore(unsigned int dur);
	unsigned int getDuration() const {return duration;}
	static string getFileTitle(string fullPath);
	string getFileName();
	void setFileName(string _fileName);
	void saveToFile();
	CWave(string _fileName) throw (LPCSTR) ;
	CWave(const CWave& w);
	CWave();
	virtual ~CWave();

	CWave operator+(const CWave& wave) const throw (LPCSTR) ;
	CWave& operator= (const CWave &w);
	typedef struct _RIFF{
		char riffID[4]; //4
		DWORD riffSIZE; //4
		char riffFORMAT[4];//4

	}RIFF ;
	
	typedef struct _FMT{
		char fmtID[4]; //4
		DWORD fmtSIZE; //4

		WORD wFormatTag; //2
		WORD nChannels; //2
		DWORD nSamplesPerSec; //4
		DWORD nAvgBytesPerSec; //4
		WORD nBlockAlign; //2
		WORD wBitsPerSample; //2
		
	}FMT;
	
	typedef struct _DATA{
		char dataID[4]; //4 bytes
		DWORD dataSIZE; // 4 bytes
	}DATA ;
	typedef struct _FACT{
		DWORD samplesNumber;
		DWORD t;
	}FACT;
	static string getFileFolder(string fullPath);
private:
	
	void init(const CWave&);
	DWORD snoop;
	BYTE* wave;
	RIFF riff;
	FMT fmt;
	FACT fact;
	DATA data;
	string fileName;
	WORD extraParamLength;
	BYTE* extraParam;
	DWORD duration;
	
};

#endif // !defined(AFX_WAVE_H__5DF5361A_4047_40DF_A13A_5A5CA8971DEA__INCLUDED_)
