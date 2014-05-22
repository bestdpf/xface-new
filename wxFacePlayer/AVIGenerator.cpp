// AVIGenerator.cpp: implementation of the CAVIGenerator class.
//
//////////////////////////////////////////////////////////////////////

//#include "stdafx.h"
#include "AVIGenerator.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#include <pshpack1.h>


CAVIGenerator::CAVIGenerator()
: m_sFile(_T("Untitled.avi")), m_dwRate(30),
m_pAVIFile(NULL), m_pStream(NULL), m_pStreamCompressed(NULL)
{
	memset(&m_bih,0,sizeof(BITMAPINFOHEADER));
}

#ifdef _AVIGENERATOR_USE_MFC
CAVIGenerator::CAVIGenerator(LPCTSTR sFileName, CView* pView, DWORD dwRate)
: m_sFile(sFileName), m_dwRate(dwRate),
m_pAVIFile(NULL), m_pStream(NULL), m_pStreamCompressed(NULL)
{
		MakeExtAvi();
		SetBitmapHeader(pView);
}
#endif

CAVIGenerator::CAVIGenerator(LPCTSTR sFileName, LPBITMAPINFOHEADER lpbih, DWORD dwRate)
: m_sFile(sFileName), m_dwRate(dwRate),
m_pAVIFile(NULL), m_pStream(NULL), m_pStreamCompressed(NULL)
{
		MakeExtAvi();
		SetBitmapHeader(lpbih);
}

CAVIGenerator::~CAVIGenerator()
{
	// Just checking that all allocated ressources have been released
//	ASSERT(m_pStream==NULL);
//	ASSERT(m_pStreamCompressed==NULL);
//	ASSERT(m_pAVIFile==NULL);
}

void CAVIGenerator::SetBitmapHeader(LPBITMAPINFOHEADER lpbih)
{
	// checking that bitmap size are multiple of 4
//	ASSERT(lpbih->biWidth%4==0);
//	ASSERT(lpbih->biHeight%4==0);

	// copying bitmap info structure.
	// corrected thanks to Lori Gardi
	memcpy(&m_bih,lpbih, sizeof(BITMAPINFOHEADER));
}

#ifdef _AVIGENERATOR_USE_MFC
void CAVIGenerator::SetBitmapHeader(CView *pView)
{
	ASSERT_VALID(pView);

	////////////////////////////////////////////////
	// Getting screen dimensions
	// Get client geometry 
	CRect rect; 
	pView->GetClientRect(&rect); 
	CSize size(rect.Width(),rect.Height()); 

	/////////////////////////////////////////////////
	// changing size of image so dimension are multiple of 4
	size.cx=(size.cx/4)*4;
	size.cy=(size.cy/4)*4;

	// initialize m_bih
	memset(&m_bih,0, sizeof(BITMAPINFOHEADER));
	// filling bitmap info structure.
	m_bih.biSize=sizeof(BITMAPINFOHEADER);
	m_bih.biWidth=size.cx;
	m_bih.biHeight=size.cy;
	m_bih.biPlanes=1;
	m_bih.biBitCount=24;
	m_bih.biSizeImage=((m_bih.biWidth*m_bih.biBitCount+31)/32 * 4)*m_bih.biHeight; 
	m_bih.biCompression=BI_RGB;		//BI_RGB means BRG in reality
}
#endif

HRESULT CAVIGenerator::InitEngine()
{
	AVISTREAMINFO strHdr; // information for a single stream 
	static AVICOMPRESSOPTIONS opts;
	static AVICOMPRESSOPTIONS FAR * aopts[1] = {&opts};
	static bool first = true;

	TCHAR szBuffer[1024];
	HRESULT hr;

	m_sError=_T("Ok");

	// Step 0 : Let's make sure we are running on 1.1 
	DWORD wVer = HIWORD(VideoForWindowsVersion());
	if (wVer < 0x010a)
	{
		 // oops, we are too old, blow out of here 
		m_sError=_T("Version of Video for Windows too old. Come on, join the 21th century!");
		return S_FALSE;
	}

	// Step 1 : initialize AVI engine
	AVIFileInit();

	// Step 2 : Open the movie file for writing....
	hr = AVIFileOpen(&m_pAVIFile,			// Address to contain the new file interface pointer
		       (LPCSTR)m_sFile,				// Null-terminated string containing the name of the file to open
		       OF_WRITE | OF_CREATE,	    // Access mode to use when opening the file. 
		       NULL);						// use handler determined from file extension.
											// Name your file .avi -> very important

	if (hr != AVIERR_OK)
	{
//		sprintf(szBuffer,_T("AVI Engine failed to initialize. Check filename %s."),m_sFile);
		m_sError=szBuffer;
		// Check it succeded.
		switch(hr)
		{
		case AVIERR_BADFORMAT: 
			m_sError+=_T("The file couldn't be read, indicating a corrupt file or an unrecognized format.");
			break;
		case AVIERR_MEMORY:		
			m_sError+=_T("The file could not be opened because of insufficient memory."); 
			break;
		case AVIERR_FILEREAD:
			m_sError+=_T("A disk error occurred while reading the file."); 
			break;
		case AVIERR_FILEOPEN:		
			m_sError+=_T("A disk error occurred while opening the file.");
			break;
		case REGDB_E_CLASSNOTREG:		
			m_sError+=_T("According to the registry, the type of file specified in AVIFileOpen does not have a handler to process it");
			break;
		}

		return hr;
	}

	// Fill in the header for the video stream....
	memset(&strHdr, 0, sizeof(strHdr));
	strHdr.fccType                = streamtypeVIDEO;	// video stream type
	strHdr.fccHandler             = 0;
	strHdr.dwScale                = 1;					// should be one for video
	strHdr.dwRate                 = m_dwRate;		    // fps
	strHdr.dwSuggestedBufferSize  = m_bih.biSizeImage;	// Recommended buffer size, in bytes, for the stream.
	SetRect(&strHdr.rcFrame, 0, 0,		    // rectangle for stream
	    (int) m_bih.biWidth,
	    (int) m_bih.biHeight);

	// Step 3 : Create the stream;
	hr = AVIFileCreateStream(m_pAVIFile,		    // file pointer
			         &m_pStream,		    // returned stream pointer
			         &strHdr);	    // stream header

	// Check it succeded.
	if (hr != AVIERR_OK)
	{
		m_sError=_T("AVI Stream creation failed. Check Bitmap info.");
		if (hr==AVIERR_READONLY)
		{
			m_sError+=_T(" Read only file.");
		}
		return hr;
	}

//	if (first)
	{
		// Step 4: Get codec and infos about codec
		memset(&opts, 0, sizeof(opts));
		// Poping codec dialog
		if (!AVISaveOptions(NULL, 0, 1, &m_pStream, (LPAVICOMPRESSOPTIONS FAR *) &aopts))
		{
			AVISaveOptionsFree(1,(LPAVICOMPRESSOPTIONS FAR *) &aopts);
			return S_FALSE;
		}
//		first = false;
	}
	
	// Step 5:  Create a compressed stream using codec options.
	hr = AVIMakeCompressedStream(&m_pStreamCompressed, 
				m_pStream, 
				&opts, 
				NULL);

	if (hr != AVIERR_OK)
	{
		m_sError=_T("AVI Compressed Stream creation failed.");
		
		switch(hr)
		{
		case AVIERR_NOCOMPRESSOR:
			m_sError+=_T(" A suitable compressor cannot be found.");
				break;
		case AVIERR_MEMORY:
			m_sError+=_T(" There is not enough memory to complete the operation.");
				break; 
		case AVIERR_UNSUPPORTED:
			m_sError+=_T("Compression is not supported for this type of data. This error might be returned if you try to compress data that is not audio or video.");
			break;
		}

		return hr;
	}

	// releasing memory allocated by AVISaveOptionFree
	hr = AVISaveOptionsFree(1,(LPAVICOMPRESSOPTIONS FAR *) &aopts);
	if (hr!=AVIERR_OK)
	{
		m_sError=_T("Error releasing memory");
		return hr;
	}

	// Step 6 : sets the format of a stream at the specified position
	hr = AVIStreamSetFormat(m_pStreamCompressed, 
					0,			// position
					&m_bih,	    // stream format
					m_bih.biSize +   // format size
					m_bih.biClrUsed * sizeof(RGBQUAD));

	if (hr != AVIERR_OK)
	{
		m_sError=_T("AVI Compressed Stream format setting failed.");
		return hr;
	}

	// Step 6 : Initialize step counter
	m_lFrame=0;

	return hr;
}

void CAVIGenerator::ReleaseEngine()
{
	if (m_pStream)
	{
		AVIStreamRelease(m_pStream);
		m_pStream=NULL;
	}

	if (m_pStreamCompressed)
	{
		AVIStreamRelease(m_pStreamCompressed);
		m_pStreamCompressed=NULL;
	}

	if (m_pAVIFile)
	{
		AVIFileRelease(m_pAVIFile);
		m_pAVIFile=NULL;
	}

	// Close engine
	AVIFileExit();
}

HRESULT CAVIGenerator::AddFrame(BYTE *bmBits)
{
	HRESULT hr;

	// compress bitmap
	hr = AVIStreamWrite(m_pStreamCompressed,	// stream pointer
		m_lFrame,						// time of this frame
		1,						// number to write
		bmBits,					// image buffer
		m_bih.biSizeImage,		// size of this frame
		AVIIF_KEYFRAME,			// flags....
		NULL,
		NULL);

	// updating frame counter
	m_lFrame++;

	return hr;
}

void CAVIGenerator::MakeExtAvi()
{
	// finding avi
	if( _tcsstr((const char *) m_sFile,_T("avi"))==NULL )
	{
		m_sFile+=_T(".avi");
	}
}

bool CAVIGenerator::AddWav(LPCTSTR wavFileName)
{
	_bstr_t wavName = wavFileName;
	if( _tcsstr((const char *) wavName,_T("wav"))==NULL )
	{
		wavName +=_T(".wav");
	}
	wavFileName = wavName;

	struct FmtChunk { 
            char id[4];            //="fmt "
	unsigned long size;              //=16
			short wFormatTag;       //=WAVE_FORMAT_PCM=1
	unsigned short wChannels;        //=1 or 2 for mono or stereo
	unsigned long  dwSamplesPerSec;  //=11025 or 22050 or 44100
	unsigned long  dwAvgBytesPerSec; //=wBlockAlign * dwSamplesPerSec
	unsigned short wBlockAlign;      //=wChannels * (wBitsPerSample==8?1:2)
	unsigned short wBitsPerSample;   //=8 or 16, for bits per sample
	};

	struct DataChunk { 
			char id[4];   //="data"
	unsigned long size;    //=datsize, size of the following array
	unsigned char data[1]; //=the raw data goes here
	};


	struct WavChunk { 
			char id[4];				//="RIFF"
			unsigned long size;		//=datsize+8+16+4
			char type[4];			//="WAVE"
			FmtChunk      fmt;
			DataChunk     dat;
	};

	char *buf=0; 
	
	HANDLE hf=CreateFile(wavFileName,
		GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);
    
	if (hf==INVALID_HANDLE_VALUE) 
		return false;
	
	DWORD size = GetFileSize(hf,0);
	buf = new char[size];
	DWORD red; 
	ReadFile(hf,buf,size,&red,0);
	CloseHandle(hf);
	WavChunk *wav = (WavChunk*)buf;
	
	//
	WAVEFORMATEX  wfx;
	wfx.wFormatTag		= wav->fmt.wFormatTag;
	wfx.cbSize			= 0;
	wfx.nAvgBytesPerSec = wav->fmt.dwAvgBytesPerSec;
	wfx.nBlockAlign		= wav->fmt.wBlockAlign;
	wfx.nChannels		= wav->fmt.wChannels;
	wfx.nSamplesPerSec	= wav->fmt.dwSamplesPerSec;
	wfx.wBitsPerSample	= wav->fmt.wBitsPerSample;
	
	AVISTREAMINFO ahdr; 
	ZeroMemory(&ahdr,sizeof(ahdr));
	ahdr.fccType	= streamtypeAUDIO;
	ahdr.dwScale	= wfx.nBlockAlign;
	ahdr.dwRate		= wfx.nSamplesPerSec*wfx.nBlockAlign; 
	ahdr.dwSampleSize =wfx.nBlockAlign;
	ahdr.dwQuality = (DWORD)-1;

	HRESULT hr = AVIFileCreateStream(m_pAVIFile, &m_pWavStream, &ahdr);
	if (hr!=AVIERR_OK) 
	{
		if (buf!=0) 
			delete[] buf; 
		return false;
	}
	
	hr = AVIStreamSetFormat(m_pWavStream, 0, &wfx, sizeof(WAVEFORMATEX));
	if (hr!=AVIERR_OK)
	{
		if (buf!=0) 
			delete[] buf; 
		return false;
	}
	
	// now we can write the data
	unsigned long numbytes = wav->size;//wav->dat.size;
	unsigned long numsamps = numbytes*8 / wfx.wBitsPerSample;
	hr = AVIStreamWrite(m_pWavStream, 0, numsamps, wav->dat.data,
		numbytes, 0, 0, 0);
	if (buf != 0) 
		delete[] buf;

	AVIStreamRelease(m_pWavStream);
	return true;
//	au->nsamp+=numsamps; 
}

/*
// SetAviVideoCompression - allows compression of the video. If compression is desired,
// then this function must have been called before any bitmap frames had been added.
// The bitmap hbm must be a DIBSection (so that avi knows what format/size you're giving it),
// but won't actually be added to the movie.
// This function can display a dialog box to let the user choose compression. In this case,
// set ShowDialog to true and specify the parent window. If opts is non-NULL and its
// dwFlags property includes AVICOMPRESSF_VALID, then opts will be used to give initial
// values to the dialog. If opts is non-NULL then the chosen options will be placed in it.
// This function can also be used to choose a compression without a dialog box. In this
// case, set ShowDialog to false, and hparent is ignored, and the compression specified
// in 'opts' is used, and there's no need to call GotAviVideoCompression afterwards.
HRESULT SetAviVideoCompression(HAVI avi, HBITMAP hbm, AVICOMPRESSOPTIONS *opts, bool ShowDialog, HWND hparent)
{ if (avi==NULL) return AVIERR_BADHANDLE;
  if (hbm==NULL) return AVIERR_BADPARAM;
  DIBSECTION dibs; int sbm = GetObject(hbm,sizeof(dibs),&dibs);
  if (sbm!=sizeof(DIBSECTION)) return AVIERR_BADPARAM;
  TAviUtil *au = (TAviUtil*)avi;
  if (au->iserr) return AVIERR_ERROR;
  if (au->psCompressed!=0) return AVIERR_COMPRESSOR;
  //
  if (au->ps==0) // create the stream, if it wasn't there before
  { AVISTREAMINFO strhdr; ZeroMemory(&strhdr,sizeof(strhdr));
    strhdr.fccType = streamtypeVIDEO;// stream type
    strhdr.fccHandler = 0; 
    strhdr.dwScale = au->period;
    strhdr.dwRate = 1000;
    strhdr.dwSuggestedBufferSize  = dibs.dsBmih.biSizeImage;
    SetRect(&strhdr.rcFrame, 0, 0, dibs.dsBmih.biWidth, dibs.dsBmih.biHeight);
    HRESULT hr=AVIFileCreateStream(au->pfile, &au->ps, &strhdr);
    if (hr!=AVIERR_OK) {au->iserr=true; return hr;}
  }
  //
  if (au->psCompressed==0) // set the compression, prompting dialog if necessary
  { AVICOMPRESSOPTIONS myopts; ZeroMemory(&myopts,sizeof(myopts));
    AVICOMPRESSOPTIONS *aopts[1];
    if (opts!=NULL) aopts[0]=opts; else aopts[0]=&myopts;
    if (ShowDialog)
    { BOOL res = (BOOL)AVISaveOptions(hparent,0,1,&au->ps,aopts);
      if (!res) {AVISaveOptionsFree(1,aopts); au->iserr=true; return AVIERR_USERABORT;}
    }
    HRESULT hr = AVIMakeCompressedStream(&au->psCompressed, au->ps, aopts[0], NULL);
    AVISaveOptionsFree(1,aopts);
    if (hr != AVIERR_OK) {au->iserr=true; return hr;}
    DIBSECTION dibs; GetObject(hbm,sizeof(dibs),&dibs);
    hr = AVIStreamSetFormat(au->psCompressed, 0, &dibs.dsBmih, dibs.dsBmih.biSize+dibs.dsBmih.biClrUsed*sizeof(RGBQUAD));
    if (hr!=AVIERR_OK) {au->iserr=true; return hr;}
  }
  //
  return AVIERR_OK;
}
*/
