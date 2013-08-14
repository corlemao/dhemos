#ifndef _MODEL_HANNOVER_H_
#define _MODEL_HANNOVER_H_

#include <string>

#include "../com.h"

class Model_Hannover
{
private:

public:
	Model_Hannover();
	virtual ~Model_Hannover();

	bool run(Com* pCom);
};

#endif