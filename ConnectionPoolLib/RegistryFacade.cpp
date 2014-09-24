// RegistryFacade.cpp: implementation of the CRegistryFacade class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RegistryFacade.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRegistryFacade::CRegistryFacade()
{
	LONG res = m_registry.Create(HKEY_LOCAL_MACHINE, "SOFTWARE\\ConnectionPool");
	if (ERROR_SUCCESS != res)
		ATLTRACE("REGISTRY FEJL!!!");
}

CRegistryFacade::~CRegistryFacade()
{
	m_registry.Close();
}

void CRegistryFacade::setKeyToRegistry(const char* name, const char* value)
{	
	m_registry.SetValue( value, name );
}

void CRegistryFacade::getKeyFromRegistry(const char* name, char **value)
{
	LONG ret = ERROR_SUCCESS;
	unsigned long lngValue = 0;
	DWORD count = 255;
	char szRetval[255];
	ret = m_registry.QueryValue( szRetval, name, &count );
	if (ERROR_SUCCESS != ret) {
		count = 0;
		ATLTRACE("REGISTRY FEJL!!!");
	}

	if (0 != count) {
		*value = (char*)malloc(count +1);
		memset(*value, 0, count +1);
		strncpy(*value, szRetval, count);
	}
}

void CRegistryFacade::getKeyFromRegistry(const char* name, long *value)
{

	DWORD dwType = 256, bufSize = 256;
	char retval[256];

	LONG ret = RegQueryValueEx(m_registry.m_hKey, name, 0, &dwType, (unsigned char *)retval, &bufSize);
	if (ERROR_SUCCESS != ret) {
		ATLTRACE("REGISTRY FEJL!!!");
	}

	*value = atoi(retval);
}