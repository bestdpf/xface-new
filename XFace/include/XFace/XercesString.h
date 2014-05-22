/*!
	\file XercesString.h
	\brief Xerces string operations.
*/

#ifndef _XERCESSTRING_H
#define _XERCESSTRING_H

#include <xercesc/util/XMLString.hpp>

#ifdef XERCES_CPP_NAMESPACE_USE
XERCES_CPP_NAMESPACE_USE
#endif

namespace XFace{
/*!
	\brief Xerces string operations
	\ingroup XFace
	\author Koray Balci
	\version 1.0
	\date   Feb 2005

	Handles Xerces string operations, taken from sample in http://www-106.ibm.com/developerworks/xml/library/x-xercc2/
	written by Rick Parrish
*/
class XercesString
{
	XMLCh *_wstr;
public:
	XercesString() : _wstr(0L) { };
	XercesString(const char *str);
	XercesString(XMLCh *wstr);
	XercesString(const XMLCh *wstr);
	XercesString(const XercesString &copy);
	~XercesString();
	bool append(const XMLCh *tail);
	bool erase(const XMLCh *head, const XMLCh *tail);
	const XMLCh* begin() const;
	const XMLCh* end() const;
	int size() const;
	XMLCh & operator [] (const int i);
	bool operator!= (const XercesString& str) const;
	bool operator!= (const XMLCh* str) const;
	bool operator!= (const char* str) const;
	bool operator== (const XercesString& str) const;
	bool operator== (const XMLCh* str) const;
	bool operator== (const char* str) const;
	const XMLCh operator [] (const int i) const;
	operator const XMLCh * () const { return _wstr; };
};
} // namespace XFace
#endif
