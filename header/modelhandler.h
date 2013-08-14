#ifndef _MODELHANDLER_H_
#define _MODELHANDLER_H_

#include <string>

#include "com.h"
#include "models/model_specialcase.h"
#include "models/model_lineartheory.h"
#include "models/model_indoor.h"
#include "models/model_outdoor.h"
#include "models/model_hannover.h"

class ModelHandler
{
private:
	int m_iModel;

	// available models
	Model_SpecialCase* m_pSpecialCase;
	Model_LinearTheory* m_pLinearTheory;
	Model_Indoor* m_pIndoor;
	Model_Outdoor* m_pOutdoor;
	Model_Hannover* m_pHannover;
	
public:
	ModelHandler();
	virtual ~ModelHandler();

	bool run(Com* pCom);
};

#endif