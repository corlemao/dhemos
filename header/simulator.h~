#ifndef _SIMULATOR_H_
#define _SIMULATOR_H_

#include <time.h>
#include <stdlib.h>
#include <iostream>
#include <string>

#include "../header/com.h"
#include "../header/modelhandler.h"

class Simulator 
{
private:
	// private variables
	time_t m_TimeBegin;
	time_t m_TimeCurrent;

	std::string m_sMapID;
	std::string m_sSimID;

	bool m_bDebug;
	bool m_bValidate;

	Com* m_pCom;
	ModelHandler* m_pModelHandler;
public:
	
	Simulator();
	virtual ~Simulator();
	
	bool showtime();
	void usage(const std::string& p_name);
	
	void setMapID(const std::string& p_mapID) {m_sMapID = p_mapID;};
	void setSimID(const std::string& p_simID) {m_sSimID = p_simID;};
	void setDebug(bool p) {m_bDebug = p;};
	void setValidate(bool p) {m_bValidate = p;};	
};

#endif
