#include "../header/modelhandler.h"

using namespace std;

ModelHandler::ModelHandler() {
	m_iModel = 0;
	
	// available models
	m_pSpecialCase = 0;
	m_pLinearTheory = 0;
	m_pIndoor = 0;
	m_pOutdoor = 0;
	m_pHannover = 0;
}

ModelHandler::~ModelHandler() {
}

bool ModelHandler::run(Com* pCom) {
	
	//////////////////////////
	// populate the event file using the outdoor model
	//////////////////////////////
	m_pOutdoor = new Model_Outdoor();
	if (!m_pOutdoor->events(pCom))
		return false;
	
	
	//////////////////////
	// run the simulation/optimisation model 
	//////////////////////
	m_iModel = pCom->getModel();
	if (m_iModel == 1) {
		m_pSpecialCase = new Model_SpecialCase();
		if (!m_pSpecialCase->run(pCom))
			return false;
	} else if (m_iModel == 2) {
		m_pLinearTheory = new Model_LinearTheory();
		if (!m_pLinearTheory->run(pCom))
			return false;
	} else if (m_iModel == 3) {
		m_pIndoor = new Model_Indoor();
		if (!m_pIndoor->run(pCom))
			return false;
	} else if (m_iModel == 4) {
		m_pHannover = new Model_Hannover();
		if (!m_pHannover->run(pCom))
			return false;
	} else {
		return false;
	}
	return true;
}