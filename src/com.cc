#include "../header/com.h"
#include "../header/defs.h"

using namespace std;

Com::Com() {
	m_fCP = 0;
	m_fGravity = 0;
	m_fWaterDensity = 0;
	m_fKinematicViscosity = 0;
	m_fPipeRoughness = 0;
	m_fInitialWaterTemperature = 0;
	m_fStillWaterLimit = 0;

	m_iStepLength = 0;
	m_iLogFrequency = 0;
	m_iSimulationLength = 0;
	m_iErrorMargin = 0;
	m_iIterationLoops = 0;
	m_iModel = 0;
	m_iOutdoorModel = 0;
}

Com::~Com() {
}

bool Com::connectDB(bool pDebug, const std::string& p_mapID, const std::string& p_simID) {
	m_bDebug = pDebug;
	m_sMapID = p_mapID;
	m_sSimID = p_simID;
	return true;
}

bool Com::disconnectDB() {
	return true;
}


bool Com::logSimulationTime(int pTimeInSeconds) {
	// insert pTimeInSeconds into the current DB
	return true;
}

int Com::getModel() {
	return m_iModel;
}

int Com::getOutdoorModel() {
	return m_iOutdoorModel;
}

void split2(const string& s, char c, vector<string>& v) {
   string::size_type i = 0;
   string::size_type j = s.find(c);
   while (j != string::npos) {
      v.push_back(s.substr(i, j-i));
      i = ++j;
      j = s.find(c, j);
      if (j == string::npos)
         v.push_back(s.substr(i, s.length( )));
   }
}

/*
 *  Extracts all the components from the db, sorts them into correct order and returns the matrix
 * 
 */
Matrix Com::createComponents() {
	
	/*
	 		while (fModel.good()) {
			getline (fModel, sLine);
			if (sLine.size() < 1) {
				break;
			} else {
				// read all lines except commented lines
				if (sLine.substr(0,1) != "'") {
					fValue = 0;
					sscanf(sLine.c_str(), "%f", &fValue);
					m_vModelValues.push_back(fValue);
				}
			}
		}
	}
	 * */
	
	/////////////////////////////////
	// open map.cvs and count number of physical components of the network
	//////////////////////////////////////////
	string sLine;
	int iCounter = 0;
	int iType = 0;
	string sAgentpath = "maps/" + m_sMapID + "/map.csv";
	ifstream fAgents(sAgentpath.c_str());
	vector<string> v;
	if (fAgents.is_open()) {
		while (fAgents.good()) {
			getline (fAgents, sLine);
			if (sLine.size() < 1) {
				break;
			} else {
				// read all lines execept commented lines
				if (sLine.substr(0,1) != "'") {
					v.clear();
					split2(sLine, ',', v);
					iType = 0;
					if(EOF == sscanf(v[1].c_str(), "%d", &iType)) {
						if (m_bDebug)
							cout << "agents.csv: type not valid" << endl;
					}
					if (iType == TYPE_PRODUCER_BOUNDARY || iType == TYPE_CONSUMER_BOUNDARY || iType == TYPE_PIPE || iType == TYPE_PUMP || iType == TYPE_VALVE) {
						iCounter++;
					}
				}
			}
		}
	} else {
		if (m_bDebug)
			cout << "Couldn't open " << m_sMapID << endl;
		Matrix mErr = Matrix (1,1,-1);
		return mErr;
	}
	fAgents.close();
	int nrOfComponents = iCounter;
	Matrix comps = Matrix (nrOfComponents, NR_OF_COMPONENTVALUES, 0);
	
	///////////////////////////////////
	// insert agents.csv into comps
	/////////////////////////////////////
	int iID = 0;
	int iBegin = 0;
	int iEnd = 0;
	float fValue0 = 0;
	float fValue1 = 0;
	float fValue2 = 0;
	float fValue3 = 0;
	float fValue4 = 0;
	float fValue5 = 0;
	float fValue6 = 0;
	float fValue7 = 0;
	ifstream fComps(sAgentpath.c_str());
	if (fComps.is_open()) {
		while (fComps.good()) {
			getline (fComps, sLine);
			if (sLine.size() < 1) {
				break;
			} else {
				// read all lines except commented lines
				if (sLine.substr(0,1) != "'") {
					v.clear();
					split2(sLine, ',', v);
					// check type
					if(EOF == sscanf(v[1].c_str(), "%d", &iType)) {
						if (m_bDebug)
							cout << "agents.csv: type not valid" << endl;
					}
					if (iType == TYPE_PRODUCER_BOUNDARY) {
						if (EOF == sscanf(v[0].c_str(), "%d", &iID)) {
							if (m_bDebug)
								cout << "agents.csv: producer id not valid" << endl;
						}
						if(EOF == sscanf(v[2].c_str(), "%d", &iBegin)) {
							if (m_bDebug)
								cout << "agents.csv: producer begin not valid" << endl;
						}
						if(EOF == sscanf(v[3].c_str(), "%d", &iEnd)) {
							if (m_bDebug)
								cout << "agents.csv: producer end not valid" << endl;
						}
						if(EOF == sscanf(v[4].c_str(), "%f", &fValue0)) {
							if (m_bDebug)
								cout << "agents.csv: producer head not valid" << endl;
						}
						if(EOF == sscanf(v[5].c_str(), "%f", &fValue1)) {
							if (m_bDebug)
								cout << "agents.csv: producer forward temperature (low) not valid" << endl;
						}
						if(EOF == sscanf(v[6].c_str(), "%f", &fValue2)) {
							if (m_bDebug)
								cout << "agents.csv: producer outdoor temperature (low) not valid" << endl;
						}
						if(EOF == sscanf(v[7].c_str(), "%f", &fValue3)) {
							if (m_bDebug)
								cout << "agents.csv: producer forward temperature (high) not valid" << endl;
						}
						if(EOF == sscanf(v[8].c_str(), "%f", &fValue4)) {
							if (m_bDebug)
								cout << "agents.csv: producer outdoor temperature (high) not valid" << endl;
						}
						comps (iID, PRODUCER_POS_OF_ID) = iID;
						comps (iID, PRODUCER_POS_OF_TYPE) = iType;
						comps (iID, PRODUCER_POS_OF_BEGIN) = iBegin;
						comps (iID, PRODUCER_POS_OF_END) = iEnd;
						comps (iID, PRODUCER_POS_OF_HEAD) = fValue0;
						comps (iID, PRODUCER_POS_OF_TFOR_LOW) = fValue1;
						comps (iID, PRODUCER_POS_OF_TOUT_LOW) = fValue2;
						comps (iID, PRODUCER_POS_OF_TFOR_HIGH) = fValue3;
						comps (iID, PRODUCER_POS_OF_TOUT_HIGH) = fValue4;							
					} else if (iType == TYPE_CONSUMER_BOUNDARY) {
						if(EOF == sscanf(v[0].c_str(), "%d", &iID)) {
							if (m_bDebug)
								cout << "agents.csv: consumer id not valid" << endl;
						}
						if(EOF == sscanf(v[2].c_str(), "%d", &iBegin)) {
							if (m_bDebug)
								cout << "agents.csv: consumer begin not valid" << endl;
						}
						if(EOF == sscanf(v[3].c_str(), "%d", &iEnd)) {
							if (m_bDebug)
								cout << "agents.csv: consumer end not valid" << endl;
						}
						if(EOF == sscanf(v[4].c_str(), "%f", &fValue0)) {
							if (m_bDebug)
								cout << "agents.csv: consumer energy signature not valid" << endl;
						}
						if(EOF == sscanf(v[5].c_str(), "%f", &fValue1)) {
							if (m_bDebug)
								cout << "agents.csv: consumer time constant not valid" << endl;
						}
						if(EOF == sscanf(v[6].c_str(), "%f", &fValue2)) {
							if (m_bDebug)
								cout << "agents.csv: consumer wanted indoor temperature not valid" << endl;
						}
						if(EOF == sscanf(v[7].c_str(), "%f", &fValue3)) {
							if (m_bDebug)
								cout << "agents.csv: consumer primary dT not valid" << endl;
						}
						if(EOF == sscanf(v[8].c_str(), "%f", &fValue4)) {
							if (m_bDebug)
								cout << "agents.csv: consumer dimensioning outdoor temperature not valid" << endl;
						}
						if(EOF == sscanf(v[14].c_str(), "%f", &fValue5)) {
							if (m_bDebug)
								cout << "agents.csv: consumer load control not valid" << endl;
						}
						if(EOF == sscanf(v[15].c_str(), "%f", &fValue6)) {
							if (m_bDebug)
								cout << "agents.csv: consumer heat load quota not valid" << endl;
						}
						if(EOF == sscanf(v[17].c_str(), "%f", &fValue7)) {
							if (m_bDebug)
								cout << "agents.csv: consumer actual indoor temperature not valid" << endl;
						}
						comps (iID, CONSUMER_POS_OF_ID) = iID;
						comps (iID, CONSUMER_POS_OF_TYPE) = iType;
						comps (iID, CONSUMER_POS_OF_BEGIN) = iBegin;
						comps (iID, CONSUMER_POS_OF_END) = iEnd;
						comps (iID, CONSUMER_POS_OF_ESIG) = fValue0;
						comps (iID, CONSUMER_POS_OF_TCONST) = fValue1;
						comps (iID, CONSUMER_POS_OF_TIN_WANTED) = fValue2;
						comps (iID, CONSUMER_POS_OF_DT_PRIM) = fValue3;
						comps (iID, CONSUMER_POS_OF_DIM_TOUT) = fValue4;
						comps (iID, CONSUMER_POS_OF_LOADCONTROL) = fValue5;
						comps (iID, CONSUMER_POS_OF_HEATLOAD_QUOTA) = fValue6; // used for return temp also (so that the temp is a weighted average)
						comps (iID, CONSUMER_POS_OF_TIN_ACTUAL) = fValue7;						
					} else if (iType == TYPE_PIPE) {
						if(EOF == sscanf(v[0].c_str(), "%d", &iID)) {
							if (m_bDebug)
								cout << "agents.csv: pipe id not valid" << endl;
						}
						if(EOF == sscanf(v[2].c_str(), "%d", &iBegin)) {
							if (m_bDebug)
								cout << "agents.csv: pipe begin not valid" << endl;
						}
						if(EOF == sscanf(v[3].c_str(), "%d", &iEnd)) {
							if (m_bDebug)
								cout << "agents.csv: pipe end not valid" << endl;
						}
						if(EOF == sscanf(v[4].c_str(), "%f", &fValue0)) {
							if (m_bDebug)
								cout << "agents.csv: pipe length not valid" << endl;
						}
						if(EOF == sscanf(v[5].c_str(), "%f", &fValue1)) {
							if (m_bDebug)
								cout << "agents.csv: inner diameter not valid" << endl;
						}
						if(EOF == sscanf(v[6].c_str(), "%f", &fValue2)) {
							if (m_bDebug)
								cout << "agents.csv: thickness not valid" << endl;
						}
						if(EOF == sscanf(v[7].c_str(), "%f", &fValue3)) {
							if (m_bDebug)
								cout << "agents.csv: k value not valid" << endl;
						}
						comps (iID, PIPE_POS_OF_ID) = iID;
						comps (iID, PIPE_POS_OF_TYPE) = iType;
						comps (iID, PIPE_POS_OF_BEGIN) = iBegin;
						comps (iID, PIPE_POS_OF_END) = iEnd;
						comps (iID, PIPE_POS_OF_LENGTH) = fValue0;
						comps (iID,PIPE_POS_OF_INNER_D) =	fValue1;
						comps (iID,PIPE_POS_OF_THICKNESS) = fValue2;
						comps (iID,PIPE_POS_OF_KVALUE) = fValue3;						
					} else if (iType == TYPE_PUMP) {

					} else if (iType == TYPE_VALVE) {

					}
				}
			}
		}
	}
	fComps.close();
	return comps;
}

float Com::getModelValue(int pPos) {
	if (pPos >= (int)m_vModelValues.size() || pPos < 0) {
		return -1;
	} else {
		return m_vModelValues.at(pPos);
	}
}

bool Com::readModel() {
	float fValue = 0;
	string sLine;
	string sModelPath = "maps/" + m_sMapID + "/simulations/" + m_sSimID + "/model.csv";
	ifstream fModel(sModelPath.c_str());
	if (fModel.is_open()) {
		while (fModel.good()) {
			getline (fModel, sLine);
			if (sLine.size() < 1) {
				break;
			} else {
				// read all lines except commented lines
				if (sLine.substr(0,1) != "'") {
					fValue = 0;
					sscanf(sLine.c_str(), "%f", &fValue);
					m_vModelValues.push_back(fValue);
				}
			}
		}
	} else {
		if (m_bDebug) 
			cout << "Couldn't open model.csv" << endl;
		return false;
	}
	fModel.close();
	return true;
}

bool Com::readSim() {
	int iValue = 0;
	int iCounter = 0;
	string sLine;
	string sConstantPath = "maps/" + m_sMapID + "/simulations/" + m_sSimID + "/sim.csv";
	ifstream fConstants(sConstantPath.c_str());
	if (fConstants.is_open()) {
		while (fConstants.good()) {
			getline (fConstants, sLine);
			if (sLine.size() < 1) {
				break;
			} else {
				// read all lines except commented lines
				if (sLine.substr(0,1) != "'") {
					iValue = 0;
					if (iCounter == 0) { // first readable line (dhemos model) 
						sscanf(sLine.c_str(), "%d", &iValue);
						m_iModel = iValue;
					} else if (iCounter == 1) { // second line (outdoor model)
						sscanf(sLine.c_str(), "%d", &iValue);
						m_iOutdoorModel = iValue;
					} else if (iCounter == 2) { // third line (simulation length)
						sscanf(sLine.c_str(), "%d", &iValue);
						m_iSimulationLength = iValue;
					} else if (iCounter == 3) { // fourth line (step length)
						sscanf(sLine.c_str(), "%d", &iValue);
						m_iStepLength = iValue;
					} else if (iCounter == 4) { // fourth line (step length)
						sscanf(sLine.c_str(), "%d", &iValue);
						m_iLogFrequency = iValue;
					}
					iCounter++;
				}
			}
		}
	} else {
		if (m_bDebug) 
			cout << "Couldn't open sim.csv" << endl;
		return false;
	}
	fConstants.close();
	return true;
}

bool Com::log(Matrix pLogMatrix) {
	
	// TODO - log pLogMatrix into m_sLogDir
	// will log each row (except first column) in a file named the content of the first column of that row
	
	return true;
}

bool Com::logFolder() {
	//////////////////////////////////////
	// create folder to save log data
	////////////////////////////////////
	time_t rawtime;
	struct tm* timeinfo;
	time (&rawtime);
	timeinfo = localtime(&rawtime);
	m_sLogDir = asctime(timeinfo);
	m_sLogDir = "maps/" + m_sMapID + "/log/" + m_sLogDir.substr(0,24);
	mkdir(m_sLogDir.c_str(), 0777);
	m_sLogDir.append("/");	
	
	return true;
}

const std::string& Com::getMapID() {
	return m_sMapID;
}

const std::string& Com::getSimID() {
	return m_sSimID;
}

float Com::getPi() {
	return 3.141592;
}

/////////////////////////////////////
// Simulation preferences
/////////////////////////////////////
bool Com::getDebug() {
	return m_bDebug;
}

int Com::getStepLength() {
	return m_iStepLength;
}

int Com::getLogFrequency() {
	return m_iLogFrequency;
}

// used
int Com::getSimulationLength() {
	return m_iSimulationLength;
}

/*
///////////////////////////
// Agent preferences
//////////////////////////
int Com::getAgentID() {
	return 1;
}
*/