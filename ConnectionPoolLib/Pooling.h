
#if !defined(POOLING_H__9E330289_B455_4DE0_9588_98C155581F1C__INCLUDED_)
#define POOLING_H__9E330289_B455_4DE0_9588_98C155581F1C__INCLUDED_

#include "stdafx.h"

class CResource {
public:
	virtual void* getConnection() = 0;
    bool operator==(const CResource& d);
	virtual GUID getID() = 0;;
};

class CPool {
public:
	virtual CResource* aquire() = 0;
	virtual void release(CResource* res) = 0;
};

class CPoolFactory {
public:
	static CPoolFactory* getPoolFactory();
	CPool* getPool();
protected:
	CPoolFactory();
	~CPoolFactory();
private:
	static CPoolFactory* _singleton;
	CPool* m_pool;	
};


#endif
