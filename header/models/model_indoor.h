#ifndef _MODEL_INDOOR_H_
#define _MODEL_INDOOR_H_

#include <string>

#include "../com.h"

class Model_Indoor
{
private:

public:
	Model_Indoor();
	virtual ~Model_Indoor();

	bool run(Com* pCom);
};

#endif