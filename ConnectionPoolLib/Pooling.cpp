#include "stdafx.h"
#include "Pooling.h"
#include "PoolingImpl.h"

#define IsEqualGUID(rguid1, rguid2) (!memcmp(rguid1, rguid2, sizeof(GUID))) 

CPoolFactory* CPoolFactory::_singleton = NULL;

CPoolFactory* CPoolFactory::getPoolFactory()
{
	if (!_singleton) {
		_singleton = new  CPoolFactory; 
	}

	return _singleton;
}

CPoolFactory::CPoolFactory()
{
	m_pool = new CConnectionPool;
}

CPoolFactory::~CPoolFactory()
{
	m_pool = new CConnectionPool;
}

CPool* CPoolFactory::getPool()
{
	return m_pool;
}

bool CResource::operator==(const CResource& d)
{
	GUID guid1, guid2;
	guid1 = this->getID();	
	CResource * res = const_cast<CResource*>(&d);
	guid2 = res->getID();
 
	if (guid1 == guid2)
		return true;
	else
		return false;
}


