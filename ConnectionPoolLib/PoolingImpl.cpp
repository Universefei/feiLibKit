#include "stdafx.h"
#include "PoolingImpl.h"
#include "RegistryFacade.h"

#import "msado15.dll" no_namespace rename("EOF", "ADO_EOF") rename("BOF", "ADO_BOF")

CConnectionAdapter::CConnectionAdapter()
{
	CoInitialize(NULL);

	char* connstr = NULL;
	CRegistryFacade *registry = new CRegistryFacade;
	registry->getKeyFromRegistry("ConnectionString", &connstr);
	if (NULL == connstr || 0 == strlen(connstr))
	{
		registry->setKeyToRegistry("ConnectionString", "DATABASE=db;DSN=dsn;UID=sa;PWD=sa");
		// registry->setKeyToRegistry("ConnectionString", "data source=127.0.0.1;initial catalog=catalog;Integrated Security=SSPI");
		// registry->setKeyToRegistry("ConnectionString", "SQLOLEDB.1;Integrated Security=SSPI;Persist Security Info=False;Initial Catalog=catalog;Data Source=127.0.0.1");
		
		if (NULL != connstr) {
			free(connstr);
			connstr = NULL;
		}

		registry->getKeyFromRegistry("ConnectionString", &connstr);
	}
	delete registry;

	m_connection.CreateInstance(__uuidof(Connection)); 
	m_connection->CursorLocation = adUseServer; 
	m_connection->Open(	connstr, L"", L"", adConnectUnspecified);	
	
	m_currentlyUsed = true; 
	
	CoCreateGuid (&m_id);
}

CConnectionAdapter::~CConnectionAdapter()
{
	if (m_connection) {
		m_connection->Close();
		m_connection = NULL;
	}
}

GUID CConnectionAdapter::getID()
{
	return m_id;
}

void CConnectionAdapter::setInUse(bool value)
{
	if (true == value) {
		time_t seconds;
		struct tm * timeinfo;

		seconds = time (NULL);
		timeinfo = localtime ( &seconds );
		
		m_lastUsed.ticks = seconds;
		m_lastUsed.hour = timeinfo->tm_hour;
		m_lastUsed.minutes = timeinfo->tm_min;
		m_lastUsed.seconds = timeinfo->tm_sec;
		m_lastUsed.month = timeinfo->tm_mon;
		m_lastUsed.year = timeinfo->tm_year;
		m_lastUsed.day = timeinfo->tm_mday;
	}

	m_currentlyUsed = value;
}

DATETIME CConnectionAdapter::getLastUsed()
{
	return m_lastUsed;
}

bool CConnectionAdapter::getInUse()
{
	return m_currentlyUsed;
}

void* CConnectionAdapter::getConnection()
{
	return m_connection;
}

CConnectionPool::CConnectionPool()
{
	m_pool.clear();

	m_initSize = 0;

	long connstr = 0;
	
	CRegistryFacade *registry = new CRegistryFacade;
	registry->getKeyFromRegistry("InitSize", &connstr);
	if (0 == connstr)
	{
		registry->setKeyToRegistry("InitSize", "10");
		registry->getKeyFromRegistry("InitSize", &connstr);
	}
	delete registry;

	m_initSize = connstr;

	for (int i=0; i<m_initSize; i++)
	{		
		CConnectionAdapter * resource = new CConnectionAdapter;
		resource->setInUse(false);
		m_pool.push_back(resource);		
	}
}

CConnectionPool::~CConnectionPool()
{
	list<CResource*>::iterator ite;
	ite=m_pool.begin();

	while(ite!=m_pool.end()) {	
		CResource* entry = *ite;
		delete entry;
		entry = NULL;
		ite++;
	}	

	m_pool.clear();
}	


CResource* CConnectionPool::aquire()
{
	CResource * resource = NULL;
	if (NULL == (resource = findNewConnection())) {
		resource = new CConnectionAdapter;
		m_pool.push_back(resource);
	}		
	return resource;
}

void CConnectionPool::release(CResource* res)
{
	bool found = false;
	list<CResource*>::iterator ite;
	ite=m_pool.begin();

	while(ite!=m_pool.end() && !found) {	
		CResource* entry = *ite;
		if (entry->operator == (*res)) {
			CConnectionAdapter *adapter = dynamic_cast<CConnectionAdapter*>(entry);			
			adapter->setInUse(false);
			found = true;
		}

		ite++;
	}	
}

CResource* CConnectionPool::findNewConnection()
{
	bool found = false;
	CResource *entry;
	list<CResource*>::iterator ite;
	ite=m_pool.begin();

	while(ite!=m_pool.end() && !found) {
		entry = *ite;
		CConnectionAdapter * adapter = dynamic_cast<CConnectionAdapter*>(entry);
		if (! adapter->getInUse()) {
			adapter->setInUse(true);
			found = true;
		}

		ite++;
	}

	return entry;
}