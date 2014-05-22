 // Wave.cpp: implementation of the CWave class.
//
//////////////////////////////////////////////////////////////////////

#include "Wave.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWave::CWave(string _fileName){
	extraParam = NULL;
	wave=NULL;
	fileName=_fileName;
	extraParamLength = 0;
	fact.samplesNumber=-1;
	HANDLE hF = CreateFile(fileName.data(),GENERIC_READ,0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_READONLY,NULL);
	if (hF==INVALID_HANDLE_VALUE){ // error case
		LPVOID lpMsgBuf;
		FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
				NULL,
				GetLastError(),
				MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
				(LPTSTR) &lpMsgBuf,
				0,
				NULL );
		string errorMsg((LPTSTR) lpMsgBuf);
		LocalFree( lpMsgBuf );
		return;//throw errorMsg.data();
	}
	DWORD read;
	ReadFile(hF,(LPVOID)&riff,RIFF_SIZE,&read,NULL);
	ReadFile(hF,(LPVOID)&fmt,FMT_SIZE,&read,NULL);
	ReadFile(hF,(LPVOID)&snoop,4,&read,NULL); // snoop four bytes ahead
	SetFilePointer(hF,-4,NULL,FILE_CURRENT); // and reset file pointer
	//if (fmt.wFormatTag!=1){
	if ( ((snoop >> 16) == 0x6166) || ((snoop >> 16) == 0x6164) ){ // are we going to find something like "??fa" or "??da" (string) when we read ?
		ReadFile(hF,(LPVOID)&extraParamLength,2,&read,NULL); //2 bytes
		if (extraParamLength>0){
			extraParam=new BYTE[extraParamLength];
			ReadFile(hF,(LPVOID)extraParam,extraParamLength,&read,NULL); 
		}
	}
	
	ReadFile(hF,(LPVOID)&data.dataID,4,&read,NULL);
	if (data.dataID[0]=='f' &&
		data.dataID[1]=='a' &&
		data.dataID[2]=='c' &&
		data.dataID[3]=='t'){
		ReadFile(hF,(LPVOID)&fact,FACT_SIZE,&read,NULL);
		ReadFile(hF,(LPVOID)&data,DATA_SIZE,&read,NULL);
	}
	else
		ReadFile(hF,(LPVOID)&data.dataSIZE,4,&read,NULL);
	duration = unsigned int((float)data.dataSIZE / ((float)fmt.nAvgBytesPerSec / 1000.0f));
	wave = new BYTE[data.dataSIZE];
	ReadFile(hF,(LPVOID)wave,data.dataSIZE,&read,NULL);
	CloseHandle(hF);
}

CWave::CWave(){
	extraParam = NULL;
	wave=NULL;
	fileName="";
	fact.samplesNumber=-1;
}
CWave::CWave(const CWave& w){
	init(w);
}

CWave& CWave::operator= (const CWave &w){
	init(w);
	return *this;
}
CWave::~CWave(){
	if (extraParam)
		delete[] extraParam;
	if (wave)
		delete[] wave;
}

CWave CWave::operator+ (const CWave &w) const{
	if (fmt.wFormatTag!=w.fmt.wFormatTag)
		throw "Can't concatenate waves with different format tags";
	CWave ret_val;
	ret_val.fmt = w.fmt;
	ret_val.riff = w.riff;
	ret_val.data = w.data;
	ret_val.data.dataSIZE= data.dataSIZE+w.data.dataSIZE;

	 
	ret_val.extraParamLength = w.extraParamLength;
	ret_val.extraParam = w.extraParam;
	ret_val.wave = new BYTE[ret_val.data.dataSIZE];
	ret_val.duration = ret_val.data.dataSIZE / ((float)fmt.nAvgBytesPerSec / 1000.0f);
	memcpy(ret_val.wave,wave,data.dataSIZE);
	memcpy(ret_val.wave+data.dataSIZE,w.wave,w.data.dataSIZE);

	string folder = getFileFolder(fileName);
	string title1 = getFileTitle(fileName);
	string title2 = getFileTitle(w.fileName);
	
	
	ret_val.fileName = folder;
	ret_val.fileName.append(title1);
	ret_val.fileName.append(title2);
	ret_val.fileName.append(".wav");

	return ret_val;
}

CWave CWave::insertSilenceBefore(unsigned int dur)
{
	CWave ret_val;
	ret_val.fmt = fmt;
	ret_val.riff = riff;
	ret_val.data = data;
	unsigned int inc = dur * (unsigned int)(fmt.nAvgBytesPerSec / 1000.0f);
	ret_val.data.dataSIZE = data.dataSIZE + inc;
	ret_val.duration = duration + dur;
	
	ret_val.extraParamLength = extraParamLength;
	ret_val.extraParam = extraParam;
	ret_val.wave = new BYTE[ret_val.data.dataSIZE];
	memset(ret_val.wave, 0, inc);
	memcpy(ret_val.wave + inc, wave, data.dataSIZE);
	
	ret_val.fileName = fileName;
	ret_val.saveToFile();
	
	return ret_val;
}


void CWave::init(const CWave& w){
	fmt = w.fmt;
	riff = w.riff;
	data = w.data;
	fact = w.fact;
	fileName = w.fileName;
	duration = w.duration;

	extraParamLength = w.extraParamLength;
	extraParam = w.extraParam;
	wave = new BYTE[data.dataSIZE];
	memcpy(wave,w.wave,data.dataSIZE);
}


void CWave::saveToFile(){
	HANDLE hFile = CreateFile(fileName.data(),GENERIC_WRITE,0,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	DWORD written;
	WriteFile(hFile,(LPCVOID)&riff,RIFF_SIZE,&written,NULL);
	WriteFile(hFile,(LPCVOID)&fmt,FMT_SIZE,&written,NULL);
	//if (fmt.wFormatTag>1){
	if (snoop != 0x74636166){ //write extraParams UNLESS we found "fact" earlier
		WriteFile(hFile,(LPCVOID)&extraParamLength,2,&written,NULL);
		if (extraParamLength>0)
			WriteFile(hFile,(LPCVOID)extraParam,extraParamLength,&written,NULL);
	}
	if (fact.samplesNumber>-1){
		WriteFile(hFile,(LPCVOID)"fact",4,&written,NULL);
		WriteFile(hFile,(LPCVOID)&fact,FACT_SIZE,&written,NULL);
	}
	WriteFile(hFile,(LPCVOID)&data,DATA_SIZE,&written,NULL);
	WriteFile(hFile,(LPCVOID)wave,data.dataSIZE,&written,NULL);

	CloseHandle(hFile);
}

string CWave::getFileName(){
	return fileName;
}

void CWave::setFileName(string _fileName){
 	fileName=_fileName;
}

string CWave::getFileFolder(string fullPath)
{
	string::size_type last= fullPath.find_last_of("\\");
	if (last==string::npos)
		return "";
	char* buffer = new char[fullPath.size()];
	memset(buffer,0,fullPath.size());
	string::size_type copied = fullPath.copy(buffer,last+1);

	string folder(buffer);
	delete[] buffer;
	return folder;
}

string CWave::getFileTitle(string fullPath){
	int begin= fullPath.find_last_of("\\");
	if (begin ==string::npos)
		begin=-1;
	string::size_type end= fullPath.find_last_of(".");
	
	char* buffer = new char[end-begin];
	memset(buffer,0,end-begin);
	fullPath.copy(buffer,end-begin-1,begin+1);
	string title(buffer);
	delete[] buffer;
	return title;
}
