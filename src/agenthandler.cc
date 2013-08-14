#include "../header/agenthandler.h"

using namespace std;

AgentHandler::AgentHandler() {

}

AgentHandler::~AgentHandler() {
}

bool AgentHandler::stepAgent(Com* p_Com, int p_step) {
	// use p_Com to get events from db with step=time=p_step

	// get all events with time=p_step (hard code for dalkia)
	if (p_step == 0) {
		//p_MH->m_mGlobal(0,0) = 10.2;
	}

	
	/*
1. Model_SpecialCase->preStep()
	1a. AgentHandler->updateEvents(*Com, ) -- läs ut alla events med time = step från db, put in EventArray
	1b. AgentHandler->stepAgent(*m_mL, *h_hT, *GlobalValues, *ComponentArray)
		-- börja med att läsa ut events här istället, för time == step (dvs i)
		-- baserat på event=dsm value=total wanted fördelas effektnivåer till alla consumer agents 
		-- set Tout för detta step i GlobalValues (och eventuella övriga värde)
		-- set boundary values i m_mL och h_hT
		1b_1. if LOGAGENT then log agent behaviour
		*/
	
	return true;
}

bool AgentHandler::events(Com* pCom) {
	// TODO add code for populating the event list, default is to ignore and let this be done manually
	return true;
}