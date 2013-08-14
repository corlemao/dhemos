#ifndef _MODEL_LINEARTHEORY_H_
#define _MODEL_LINEARTHEORY_H_

#include <string>

#include "../com.h"

class Model_LinearTheory
{
private:

public:
	Model_LinearTheory();
	virtual ~Model_LinearTheory();

	bool run(Com* pCom);
};

#endif