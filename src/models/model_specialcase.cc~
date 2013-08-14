#include "../../header/models/model_specialcase.h"

using namespace std;

Model_SpecialCase::Model_SpecialCase() {
	m_iSimulationLength = 0;
	m_iStepLength = 0;
	m_bAgentActive = false;
	m_bSecondMode = false;
	m_pCom = 0;
	m_pAgentHandler = 0;
	m_pSystemFunctions = 0;
	m_iLinkConsumerPos = 0;
	m_iLinkConsumerNr = 0;
	
	// model value positions
	CP = 1;
	GRAVITY = 2;
	DENSITY = 3;
	KINEMATICVISCOSITY = 4;
	PIPEROUGHNESS = 5;
	INTIALWATERTEMPERATURE = 6;
	STILLWATERLIMIT = 7;
	PIPUNITLENGTH = 8;
	AGENTACTIVE = 9;
}

Model_SpecialCase::~Model_SpecialCase() {
}

bool Model_SpecialCase::run(Com* pCom) {
	m_pCom = pCom;
	m_pAgentHandler = new AgentHandler();
	m_pSystemFunctions = new SystemFunctions();
	if (!this->initiate()) {
		return false;
	}
	for (int i=0; i<m_iSimulationLength;i++) {
		preStep(i);
		step();
		postStep(i);
	}
	return true;
}

void Model_SpecialCase::preStep(int p_step) {
	this->updateEvents(p_step);
	if (m_bAgentActive)
		this->stepAgent(p_step);
	this->updateBoundaries();
}

void Model_SpecialCase::step() {
	///////////////////////
	// HEAD AND FLOW
	///////////////////////
	// solve the unknown massflow (mhT and mpT in this case)
	Matrix mMT = m_mDL * m_mMML;
	mMT = -mMT;
	
	// insert the flow into m_mComponents
	for (int i=0; i<m_iTreeSize; i++) {
		if (m_mComponents(i,POS_OF_TYPE) == TYPE_PRODUCER_BOUNDARY) {
			m_mComponents(i,PRODUCER_POS_OF_FLOW) = -mMT(i,0); //changing sign because of head source connection to ground node
		} else if (m_mComponents(i,POS_OF_TYPE) == TYPE_PIPE) {
			m_mComponents(i,PIPE_POS_OF_FLOW) = mMT(i,0);
		}
	}
	for (int i=m_iLinkConsumerPos; i< (m_iLinkConsumerPos+m_iLinkConsumerNr); i++) {
		if (m_mComponents(i,POS_OF_TYPE) == TYPE_CONSUMER_BOUNDARY) {
			m_mComponents(i,CONSUMER_POS_OF_FLOW) = m_mMML(i-m_iLinkConsumerPos,0);
		}
	}
	
	// solve the pipe head
	Matrix mTmp = -m_mF41 * m_mMML;
	Matrix mHPT = Matrix(m_iNrOfPipes,1,0);
	for (int i=0; i<m_iNrOfPipes; i++) {
		mHPT(i,0) = headloss(m_iPipePos+i);
	}

	// solve the flow source head
	int m1 = m_mHHT.rows();
	int m2 = mHPT.rows();
	Matrix mHT = Matrix(m1+m2,1,0);
	for (int i=0; i<m1; i++) {
		mHT(i,0) = m_mHHT(i,0);
	}
	for (int i=0; i<m2; i++) {
		mHT(m1+i,0) = mHPT(i,0);
	}
	Matrix mHML = -m_mDL.transpose() * mHT;
	mHML = -mHML;

	// insert head into m_mComponents (for pipe and consumers)
	for (int i=0; i<m2; i++) {
		m_mComponents(i+m_iPipePos,PIPE_POS_OF_HEADLOSS) = mHPT(i,0);
	}
	int m3 = mHML.rows();
	for (int i=0; i<m3; i++) {
		m_mComponents(i+m_iLinkConsumerPos,CONSUMER_POS_OF_HEAD) = mHML(i,0);
	}

	// TODO check for sanity of flow and head solution

	////////////////////////////////
	// DISTRIBUTION TEMPERATURE
	///////////////////////////////
	//updateTempList(); only done once in a non-reversible flow situation
	
	// TODO denna kör enligt hour, men ska vara SIS-standard i sekund Tamb - dT*e^(-k*A/flow/cp) 
	// flow is in m³/s and cp is in J/(m³*K) where m³ is used as a tonne (below is fixed for hour case, i.e dividing the flow with 3600
	// also, the ambient temperature of the soil cannot equal the outdoor temperature
	// step length fixed as long as steady state is assumed
	
	for (int i=0; i<m_iComponentsSize; i++) {
		if (m_mComponents(m_mTempList(i,0),POS_OF_TYPE) == TYPE_PRODUCER_BOUNDARY) {
		} else if (m_mComponents(m_mTempList(i,0),POS_OF_TYPE) == TYPE_PIPE) {
			if (m_mComponents(m_mTempList(i,1), POS_OF_TYPE) == TYPE_PIPE) {
				m_mComponents(m_mTempList(i,0), PIPE_POS_OF_T_START) = m_mComponents(m_mTempList(i,1), PIPE_POS_OF_T_END);
			} else if (m_mComponents(m_mTempList(i,1), POS_OF_TYPE) == TYPE_PRODUCER_BOUNDARY) {
				m_mComponents(m_mTempList(i,0), PIPE_POS_OF_T_START) = m_mComponents(m_mTempList(i,1), PRODUCER_POS_OF_TFOR);
			}
			float A = m_mGlobal(POS_OF_PI,0) * m_mComponents(m_mTempList(i,0), PIPE_POS_OF_INNER_D) * m_mComponents(m_mTempList(i,0), PIPE_POS_OF_LENGTH);
			m_mComponents(m_mTempList(i,0), PIPE_POS_OF_T_END) = m_mGlobal(POS_OF_T_AMBIENT,0) - (m_mGlobal(POS_OF_T_AMBIENT,0)-m_mComponents(m_mTempList(i,0),PIPE_POS_OF_T_START)) * exp(-m_mComponents(i,PIPE_POS_OF_KVALUE)*A/(m_mComponents(m_mTempList(i,0), PIPE_POS_OF_FLOW)/m_iStepLength)/m_mGlobal(POS_OF_CP,0));
		} else if (m_mComponents(m_mTempList(i,0),POS_OF_TYPE) == TYPE_CONSUMER_BOUNDARY) {
			// agent behaviour
			m_mComponents(m_mTempList(i,0),CONSUMER_POS_OF_FTEMP) = m_mComponents(m_mTempList(i,1), PIPE_POS_OF_T_END);
			float Qwanted = m_mComponents(m_mTempList(i,0),CONSUMER_POS_OF_ESIG) * (m_mComponents(m_mTempList(i,0), CONSUMER_POS_OF_TIN_WANTED) - m_mGlobal(POS_OF_TOUT,0)) * m_mComponents(m_mTempList(i,0), CONSUMER_POS_OF_LOADCONTROL);
			Qwanted = Qwanted + m_mComponents(m_mTempList(i,0),CONSUMER_POS_OF_HL_STOCH);
			if (Qwanted < 0) {
				m_mComponents(m_mTempList(i,0),CONSUMER_POS_OF_RTEMP) = m_mComponents(m_mTempList(i,0),CONSUMER_POS_OF_FTEMP);
			} else {
				float dT = (Qwanted*3600)/(m_mComponents(m_mTempList(i,0),CONSUMER_POS_OF_FLOW) * m_mGlobal(POS_OF_CP,0));
				m_mComponents(m_mTempList(i,0),CONSUMER_POS_OF_RTEMP) = m_mComponents(m_mTempList(i,0),CONSUMER_POS_OF_FTEMP) - dT;
			}
			/*
			m_mComponents(m_mTempList(i,0),CONSUMER_POS_OF_FTEMP) = m_mComponents(m_mTempList(i,1), PIPE_POS_OF_T_END);
			float dTDim = m_mComponents(m_mTempList(i,0),CONSUMER_POS_OF_TIN_WANTED) - m_mComponents(m_mTempList(i,0),CONSUMER_POS_OF_DIM_TOUT);
			float dTactual = m_mComponents(m_mTempList(i,0),CONSUMER_POS_OF_TIN_WANTED) - m_mGlobal(POS_OF_TOUT,0);
			float quota = dTactual / dTDim;
			m_mComponents(m_mTempList(i,0),CONSUMER_POS_OF_RTEMP) = m_mComponents(m_mTempList(i,0),CONSUMER_POS_OF_FTEMP) - (quota*dTDim);
			*/
		}
	}
	
	// TODO calculate head/flow/temperature for return network
	// head loss in each pipe is known
	// flow in each pipe is known
	// should report head and temperature at original head sources
	
	// set returtemp for producer boundary, iterate all consumers, get average of returntemp
	float fVal = 0;
	for (int i=m_iLinkConsumerPos; i<m_iComponentsSize; i++) {
		fVal = fVal + (m_mComponents(i,CONSUMER_POS_OF_RTEMP)*m_mComponents(i,CONSUMER_POS_OF_HEATLOAD_QUOTA));
	}
	m_mComponents(0,PRODUCER_POS_OF_TRET) = fVal;
	//m_mComponents(0,PRODUCER_POS_OF_TRET) = fVal / m_iLinkConsumerNr;
}

void Model_SpecialCase::postStep(int p_step) {
	// gå igenom ComponentArray och räkna upp värden, t ex Tin hos consumenter och energi/miljö/ekonomi hos producenter
	// räkna ut energiuttaget hos comsumern (dT*m*cp) , räkna ut Tin hos consumern
	// while looping ComponentArray, check if component should log, if so then log values to DB

	///////////////////
	// Update indoor temperature for all consumers
	///////////////////
	// move this to agent behaviour 
	float fActual_old = 0;
	for (int i=0; i<m_iComponentsSize; i++) {
		if (m_mComponents(i,POS_OF_TYPE) == TYPE_CONSUMER_BOUNDARY) {
			// save the current value for buffer comparison
			fActual_old = m_mComponents(i,CONSUMER_POS_OF_TIN_ACTUAL);
			//Tin_end = Tin_start - (Tin_start - (1/(e_sig/(e_sig*dT*LC)) + Tout)) * (1 - e^(-t/tc)) (eq 3)			
			m_mComponents(i,CONSUMER_POS_OF_TIN_ACTUAL) = m_mComponents(i,CONSUMER_POS_OF_TIN_ACTUAL) - (m_mComponents(i,CONSUMER_POS_OF_TIN_ACTUAL) - (1/
				(m_mComponents(i,CONSUMER_POS_OF_ESIG)/(m_mComponents(i,CONSUMER_POS_OF_ESIG)*(m_mComponents(i,CONSUMER_POS_OF_TIN_WANTED)-m_mGlobal(POS_OF_TOUT,0))*m_mComponents(i,CONSUMER_POS_OF_LOADCONTROL)))
				+ m_mGlobal(POS_OF_TOUT,0)))
				* (1-exp(-1/m_mComponents(i,CONSUMER_POS_OF_TCONST)));
			// buffering in order to avoid the problem with the exponential function close to the end values
			if ((m_mComponents(i,CONSUMER_POS_OF_TIN_ACTUAL) > fActual_old) && (m_mComponents(i,CONSUMER_POS_OF_TIN_ACTUAL) < m_mComponents(i,CONSUMER_POS_OF_TIN_WANTED)) && (fActual_old < m_mComponents(i,CONSUMER_POS_OF_TIN_WANTED))) {
				m_mComponents(i,CONSUMER_POS_OF_TIN_ACTUAL) = m_mComponents(i,CONSUMER_POS_OF_TIN_ACTUAL) - (m_mComponents(i,CONSUMER_POS_OF_TIN_ACTUAL) - (1/
					(m_mComponents(i,CONSUMER_POS_OF_ESIG)/(m_mComponents(i,CONSUMER_POS_OF_ESIG)*(m_mComponents(i,CONSUMER_POS_OF_TIN_WANTED)+m_mGlobal(POS_OF_TIN_OFFSET,0)-m_mGlobal(POS_OF_TOUT,0))*m_mComponents(i,CONSUMER_POS_OF_LOADCONTROL)))
					+ m_mGlobal(POS_OF_TOUT,0)))
					* (1-exp(-1/m_mComponents(i,CONSUMER_POS_OF_TCONST)));
			}
		}
	}
	
	///////////////////
	// Log data
	///////////////////
	// TODO extract relevant data from m_mComponents and send to m_pCom->log()
	float fExternal = 3; // accounts for external sources of heat energy, i.e. social behaviour, computer heat, dogs running around etc
				// external should be done in the algorithm above 
	for (int i=0; i<m_iComponentsSize; i++) {
		// create/open file with same name as id of current component
		string sFile = m_sDir;
		std::ostringstream result; 
		result << m_mComponents(i,0);
		sFile.append(result.str());
		sFile.append(".csv");
		ofstream fLogFile;
		fLogFile.open(sFile.c_str(), ios::app);
		if (m_mComponents(i,POS_OF_TYPE) == TYPE_CONSUMER_BOUNDARY) {
			fLogFile << p_step << "," << m_mComponents(i,CONSUMER_POS_OF_ID) << "," << m_mComponents(i,CONSUMER_POS_OF_FLOW) << "," << m_mComponents(i,CONSUMER_POS_OF_FTEMP)
				<< "," << m_mComponents(i,CONSUMER_POS_OF_RTEMP) << "," << m_mComponents(i,CONSUMER_POS_OF_HEAD) << "," << m_mComponents(i,CONSUMER_POS_OF_HEATLOAD)
				<< "," << m_mComponents(i,CONSUMER_POS_OF_LOADCONTROL) << "," << m_mComponents(i,CONSUMER_POS_OF_TIN_ACTUAL) + fExternal << endl;
		} else if (m_mComponents(i,POS_OF_TYPE) == TYPE_PIPE) {
			fLogFile << p_step << "," << m_mComponents(i,PIPE_POS_OF_ID) << "," << m_mComponents(i,PIPE_POS_OF_FLOW)/m_iStepLength << "," << m_mComponents(i,PIPE_POS_OF_HEADLOSS)
				<< "," << m_mComponents(i,PIPE_POS_OF_T_START) << "," << m_mComponents(i,PIPE_POS_OF_T_END) << endl;
		} else if (m_mComponents(i,POS_OF_TYPE) == TYPE_PRODUCER_BOUNDARY) {
			fLogFile << p_step << "," << m_mComponents(i,PRODUCER_POS_OF_ID) << "," << m_mComponents(i,PRODUCER_POS_OF_TFOR) << "," << m_mComponents(i,PRODUCER_POS_OF_TRET)
				<< "," << m_mComponents(i,PRODUCER_POS_OF_FLOW) << endl;
		}
		fLogFile.close();
	}
}

bool Model_SpecialCase::initiate() {
	///////////////////////
	// check if there is any active agent behaviour during simulation
	/////////////////////////
	m_bAgentActive = m_pCom->getModelValue(AGENTACTIVE);
	m_pAgentHandler->events(m_pCom);
	
	////////////////////////
	// check for hour or second case
	////////////////////////
	if (m_pCom->getStepLength() == 3600) {
		m_bSecondMode = false;
	} else if (m_pCom->getStepLength() == 1) {
		m_bSecondMode = true;
	} else 
		return false;
	
	//////////////////////////////
	// create all matrices
	//////////////////////////////
	m_mGlobal = Matrix (NR_OF_GLOBALVALUES,1,0);
	m_mComponents = m_pCom->createComponents();
	if (m_mComponents.rows() == 1 && m_mComponents.cols() == 1) {
		return false;
	}
	m_mA = m_pSystemFunctions->createA(m_mComponents);
	m_mAT = m_pSystemFunctions->createAT(m_mA);
	m_mD = m_mAT.inverse() * m_mA;
	m_mDL = m_pSystemFunctions->createDL(m_mD);
	m_mF11 = m_pSystemFunctions->createF11(m_mDL, m_mComponents);
	m_mF41 = m_pSystemFunctions->createF41(m_mDL, m_mComponents);
	m_mMML = m_pSystemFunctions->createMML(m_mComponents);
	m_mHHT = m_pSystemFunctions->createHHT(m_mComponents);
	m_mTempList = Matrix(m_mComponents.rows(),2,0);
	updateTempList();
	
	/////////////////////////////////////////////////////////
	// calculate number of pipes, number of link consumers, position of first link consumer 
	// (regarding pipes, in special case all pipes are part of the trees)
	//////////////////////////////////////////////////7
	m_iNrOfPipes = 0;
	m_iPipePos = 0;
	m_iLinkConsumerNr = 0;
	bool bConsumerFirst = true;
	bool bPipeFirst = true;
	int count = (int)m_mComponents.rows();
	for (int i=0; i<count; i++) {
		if (m_mComponents(i,POS_OF_TYPE) == TYPE_PIPE) {
			m_iNrOfPipes++;
			if (bPipeFirst) {
				m_iPipePos = i;
				bPipeFirst = false;
			}
		}	else if (m_mComponents(i,POS_OF_TYPE) == TYPE_CONSUMER_BOUNDARY) {
			m_iLinkConsumerNr++;
			if (bConsumerFirst) {
				m_iLinkConsumerPos = i;
				m_iTreeSize = i;
				bConsumerFirst = false;
			}
		}
	}
	
	m_iComponentsSize = m_mComponents.rows();
	
	//////////////////////////////////////
	// set constant global values
	///////////////////////////////////
	m_mGlobal(POS_OF_CP, 0) = m_pCom->getModelValue(CP);
	m_mGlobal(POS_OF_GRAVITY, 0) = m_pCom->getModelValue(GRAVITY);
	m_mGlobal(POS_OF_WATER_DENSITY, 0) = m_pCom->getModelValue(DENSITY);
	m_mGlobal(POS_OF_KINEMATIC_VISCOSITY, 0) = m_pCom->getModelValue(KINEMATICVISCOSITY);
	m_mGlobal(POS_OF_PI, 0) = m_pCom->getPi();
	m_mGlobal(POS_OF_PIPE_ROUGHNESS, 0) = m_pCom->getModelValue(PIPEROUGHNESS);
	m_mGlobal(POS_OF_TIN_OFFSET,0) = 1;
	
	m_iSimulationLength = m_pCom->getSimulationLength();
	m_iStepLength = m_pCom->getStepLength();
	
	///////////////////////////////////////////////////////
	// set path for events, this is used in updateEvents()
	///////////////////////////////////////////////
	string sMap = m_pCom->getMapID();
	string sSim = m_pCom->getSimID();
	m_sEventpath = "maps/" + sMap + "/simulations/" + sSim + "/events.csv";
	
	//////////////////////////////////////
	// create folder to save log data
	////////////////////////////////////
	time_t rawtime;
	struct tm* timeinfo;
	time (&rawtime);
	timeinfo = localtime(&rawtime);
	m_sDir = asctime(timeinfo);
	m_sDir = "maps/" + sMap + "/log/" + m_sDir.substr(0,24);
	mkdir(m_sDir.c_str(), 0777);
	m_sDir.append("/");
	
	return true;
}

void split(const string& s, char c, vector<string>& v) {
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

bool exists(Matrix pM, float pVal) {
	int m = pM.rows();
	int n = pM.cols();
	bool bCheck = false;
	for (int i=0; i<m; i++) {
		for (int j=0; j<n; j++) {
			if (pM(i,j) == pVal)
				bCheck = true;
		}
	}
	return bCheck;
}

void Model_SpecialCase::updateEvents(int p_step) {
	string sLine;
	ifstream fEvents(m_sEventpath.c_str());
	vector<string> v;
	int iTime = -1;
	int iEventType = -1;
	float fEventValue0 = 0;
	float fEventValue1 = 0;
	if (fEvents.is_open()) {
		while (fEvents.good()) {
			getline (fEvents, sLine);
			if (sLine.substr(0,1) != "'") {
				v.clear();
				split(sLine, ',', v);
				// get values
				if(EOF == sscanf(v[0].c_str(), "%d", &iTime)) {
					cout << "event.csv: time step not valid" << endl; // time step
				}
				if(EOF == sscanf(v[1].c_str(), "%d", &iEventType)) {
					cout << "event.csv: event type not valid" << endl; // event type (defined in defs.h)
				}
				if(EOF == sscanf(v[2].c_str(), "%f", &fEventValue0)) {
					cout << "event.csv: event value not valid" << endl; // event value (content depending on event type)
				}
				if(EOF == sscanf(v[3].c_str(), "%f", &fEventValue1)) {
					cout << "event.csv: event value not valid" << endl; // event value (content depending on event type)
				}
				// first check for default values (i.e time step = -1) for the different event types
				if (iTime == -1 && iEventType == EVENT_TOUT) {
					m_mGlobal(POS_OF_TOUT,0) = fEventValue0;
				} else if (iTime == -1 && iEventType == EVENT_AMBIENT) {
					m_mGlobal(POS_OF_T_AMBIENT,0) = fEventValue0;
				} else if (iTime == -1 && iEventType == EVENT_LOADCONTROL) {
					m_mComponents(fEventValue0,CONSUMER_POS_OF_LOADCONTROL) = fEventValue1;
				} else {
					if (iTime == p_step) {
						if (iEventType == EVENT_TOUT) {	// Global outdoor temperature
							m_mGlobal(POS_OF_TOUT,0) = fEventValue0;
						} else if (iEventType == EVENT_AMBIENT) {
							m_mGlobal(POS_OF_T_AMBIENT,0) = fEventValue0;
						} else if (iEventType == EVENT_LOADCONTROL) { // Load control
							m_mComponents(fEventValue0,CONSUMER_POS_OF_LOADCONTROL) = fEventValue1;
						}
					}
				}
			}
		}
	} else {
		cout << "couldn't open events.csv" << endl; // DEBUG should be checked here 
	}
	fEvents.close();
}

////////////////////////////////////////////////////////////////////////
/// if agents are active this function will enforce their behaviour
/////////////////////////////////////////////////////////////////////////
void Model_SpecialCase::stepAgent(int p_step) {
	// use this function to insert arbitrary behaviour in components
}


/////////////////////////////
/// calculates the flow and head sources required for the distribution solution 
/////////////////////////////
void Model_SpecialCase::updateBoundaries() {
	///////////////////////////////
	// calculate the flow boundaries
	///////////////////////////////
	int m = (int)m_mMML.rows();
	for (int i=0; i<m; i++) {
		// building heating
		float dTActual = m_mComponents(i+m_iLinkConsumerPos, CONSUMER_POS_OF_TIN_WANTED) - m_mGlobal(POS_OF_TOUT,0);
		float fFlow = 0;
		if (dTActual > 0) {
			float Qwanted = m_mComponents(i+m_iLinkConsumerPos,CONSUMER_POS_OF_ESIG) * dTActual * m_mComponents(i+m_iLinkConsumerPos,CONSUMER_POS_OF_LOADCONTROL);
			// calculate amount of maximum cooling on primary side to use
			//float dTDim = m_mComponents(m_mTempList(i,0),CONSUMER_POS_OF_TIN_WANTED) - m_mComponents(m_mTempList(i,0),CONSUMER_POS_OF_DIM_TOUT);
			float dTDim = m_mComponents(i+m_iLinkConsumerPos,CONSUMER_POS_OF_TIN_WANTED) - m_mComponents(i+m_iLinkConsumerPos,CONSUMER_POS_OF_DIM_TOUT);
			float quota = 1;
			if (dTActual < dTDim) {
				quota = dTActual / dTDim;
			}
			// calculate flow ONLY FOR 1 HOUR STEPS below
			fFlow = (Qwanted*3600) / (m_mComponents(i+m_iLinkConsumerPos,CONSUMER_POS_OF_DT_PRIM) * quota * m_mGlobal(POS_OF_CP,0));
		}

		// tap water heating
		fFlow = fFlow + calculateStochasticLoad(); // TODO

		m_mMML(i,0) = fFlow;
	}

	//////////////////////////////
	// calculate the head boundaries
	//////////////////////////////
	m = (int)m_mHHT.rows();
	for (int i=0; i<m; i++) {
		m_mHHT(i,0) = m_mComponents(0,PRODUCER_POS_OF_HEAD);
	}
	
	//////////////////////////
	// calculate temperature boundaries
	///////////////////////
	// for veri this is set in updateEvents (for landvetter this should be calculated here 
	for (int i=0; i<m_iTreeSize; i++) {
		if (m_mComponents(i,POS_OF_TYPE) == TYPE_PRODUCER_BOUNDARY) {
			float k = (m_mComponents(i,PRODUCER_POS_OF_TFOR_LOW) - m_mComponents(i,PRODUCER_POS_OF_TFOR_HIGH)) / (m_mComponents(i,PRODUCER_POS_OF_TOUT_LOW) - m_mComponents(i,PRODUCER_POS_OF_TOUT_HIGH));
			float m = m_mComponents(i,PRODUCER_POS_OF_TFOR_LOW) - k*m_mComponents(i,PRODUCER_POS_OF_TOUT_LOW);
			m_mComponents(i,PRODUCER_POS_OF_TFOR) = k*m_mGlobal(POS_OF_TOUT,0)+m;
		}
	}
}

/*
 * Returns the headloss of the pipe at p_pos in m_mComponents
 */
float Model_SpecialCase::headloss(int p_pos) {
	// calculate velocity
	float A = (m_mComponents(p_pos,PIPE_POS_OF_INNER_D)*m_mComponents(p_pos,PIPE_POS_OF_INNER_D)*m_mGlobal(POS_OF_PI,0))/4;
	float v = m_mComponents(p_pos,PIPE_POS_OF_FLOW)/(m_mGlobal(POS_OF_WATER_DENSITY,0)*A);
	
	// calculate relative roughness
	//float r = m_mGlobal(POS_OF_PIPE_ROUGHNESS,0)/m_mComponents(p_pos, PIPE_POS_OF_INNER_D);
	
	// calculate Reynolds number
	float Re = (abs(v) * m_mComponents(p_pos,PIPE_POS_OF_INNER_D)) / m_mGlobal(POS_OF_KINEMATIC_VISCOSITY,0);
	
	// calculate friction factor (swamee-jain equation)
	float f = 0.25 / pow((log10((m_mGlobal(POS_OF_PIPE_ROUGHNESS,0)/(3.7*m_mComponents(p_pos,PIPE_POS_OF_INNER_D))) + (5.74/pow(Re,0.9)))),2);
	
	// calculate head loss
	return f * (m_mComponents(p_pos,PIPE_POS_OF_LENGTH) * pow(v,2)) / (m_mComponents(p_pos,PIPE_POS_OF_INNER_D) * 2 * m_mGlobal(POS_OF_GRAVITY,0));
}

float Model_SpecialCase::calculateStochasticLoad() {
	
	return 0;
}

void Model_SpecialCase::updateTempList() {
	/////////////////////////////////////////////////
	// this only needs to be done at start, since it is static in non-reversible flow special case
	// The TempList consists of two values , the first shows the id of the current component, the second shows where the flow is coming from to that specific component
	//////////////////////////////////////////
	int iRows = m_mComponents.rows();
	int iBegin = -1;
	int iCount = 0;
	for (int i=0; i<iRows; i++) {
		m_mTempList(i,0) = m_mComponents(i,POS_OF_ID);
		if (m_mComponents(i, POS_OF_TYPE) == TYPE_PRODUCER_BOUNDARY) {
			m_mTempList(i,1) = m_mComponents(i, POS_OF_ID);
		} else {
			iBegin = m_mComponents(i, POS_OF_BEGIN);
			if (iBegin == 0) {
				m_mTempList(i,1) = m_mComponents(0, POS_OF_ID);
			} else {
				iCount = 0;
				while (iCount < iRows) {
					if (m_mComponents(iCount, POS_OF_END) == iBegin) {
						break;
					}
					iCount++;
				}
				m_mTempList(i,1) = m_mComponents(iCount, POS_OF_ID);
			}
		}
	}
}