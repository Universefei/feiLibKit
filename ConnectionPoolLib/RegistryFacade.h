// RegistryFacade.h: interface for the CRegistryFacade class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_REGISTRYFACADE_H__5C397B76_1093_4C52_9A0B_416C57B89534__INCLUDED_)
#define AFX_REGISTRYFACADE_H__5C397B76_1093_4C52_9A0B_416C57B89534__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <atlbase.h>

class CRegistryFacade  
{
public:
	CRegistryFacade();
	virtual ~CRegistryFacade();

	void setKeyToRegistry(const char* name, const char* value);
	void getKeyFromRegistry(const char* name, char **value);
	void getKeyFromRegistry(const char* name, long *value);
private:
	CRegKey m_registry;
};

#endif // !defined(AFX_REGISTRYFACADE_H__5C397B76_1093_4C52_9A0B_416C57B89534__INCLUDED_)
