#ifndef _AGENTHANDLER_H_
#define _AGENTHANDLER_H_

#include <string>
#include <octave/oct.h>

#include "com.h"
#include "defs.h"
#include "systemfunctions.h"

class AgentHandler
{
private:

public:
	AgentHandler();
	virtual ~AgentHandler();

	bool stepAgent(Com* p_Com, int p_step);
	bool events(Com* pCom);
};

#endif