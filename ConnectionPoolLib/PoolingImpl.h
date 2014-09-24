#include "stdafx.h"
#include "Pooling.h"

#include <list> 
#include <string> 
#include <iostream> 
#include <stdio.h>
#include <time.h>

#import "msado15.dll" rename_namespace("") rename("EOF", "ADO_EOF") rename("BOF", "ADO_BOF")

using namespace std;

typedef struct {
	long ticks;
	int hour;
	int minutes;
	int seconds;
	int month;
	int day;
	int year;
	char *comment;
} DATETIME;


class CConnectionAdapter : public CResource {
public:
	CConnectionAdapter();
	~CConnectionAdapter();
	void* getConnection();
	void setInUse(bool value);
	bool getInUse();
	DATETIME getLastUsed();
	GUID getID();
private:
	DATETIME m_lastUsed;
	bool m_currentlyUsed;
	_ConnectionPtr m_connection;
	GUID m_id;
};

class CConnectionPool : public CPool {
public:
	CConnectionPool();
	~CConnectionPool();
	CResource* aquire();
	void release(CResource* res);
protected:
	CResource* findNewConnection();
private:
	std::list<CResource*> m_pool;
	long m_initSize;
};
