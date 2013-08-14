#ifndef _MODEL_OUTDOOR_H_
#define _MODEL_OUTDOOR_H_

#include <string>

#include "../com.h"

class Model_Outdoor
{
private:

public:
	Model_Outdoor();
	virtual ~Model_Outdoor();

	bool events(Com* pCom);
};

#endif