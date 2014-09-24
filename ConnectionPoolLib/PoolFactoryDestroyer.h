
#if !defined(CPOOLFACTORY_H__9E330289_B455_4DE0_9588_98C155581F1C__INCLUDED_)
#define CPOOLFACTORY_H__9E330289_B455_4DE0_9588_98C155581F1C__INCLUDED_

#include "stdafx.h"

class CPoolFactory;

class CPoolFactoryDestroyer {
public:
	virtual ~CPoolFactoryDestroyer();

    virtual void SetSingleton(CPoolFactory* s) = 0;
private:
    CPoolFactory* _singleton;
};



#endif
