#include "../header/simulator.h"

using namespace std;

int main(int argc, char *argv[]) {
	Simulator *mySim = new Simulator();

	if (argc > 1) {
		bool bDebug = false;
		bool bMap = false;
		bool bSim = false;
		for (int i=0; i<argc ; i++) {
			string arg = argv[i];
			if (!arg.compare("-d") && argc > i+1) {
				arg = argv[i+1];
				if (!arg.compare("on")) {
					mySim->setDebug(true);
					bDebug = true;
				} else if (!arg.compare("off")) {
					mySim->setDebug(false);
					bDebug = true;
				} else {
					mySim->usage(argv[i+1]);
					return 0;
				}
			} else if (!arg.compare("-m") && argc > i+1) {
				mySim->setMapID(argv[i+1]);
				bMap = true;
			} else if (!arg.compare("-s") && argc > i+1) {
				mySim->setSimID(argv[i+1]);
				bSim = true;
			} else if (!arg.compare("-v")) {
				mySim->setValidate(true);
			}
		}
		if (bDebug == true && bMap == true && bSim == true) {
			mySim->showtime();
		} else {
			cout << "Error input values. Aborting simulation!" << endl;
			mySim->usage(argv[0]);
		}
	} else {
		cout << "No input values. Aborting simulation!" << endl;
		mySim->usage(argv[0]);
	}
	return 0;
}

Simulator::Simulator() {
	(void)time(&m_TimeBegin);
	this->m_sMapID = "-1";
	this->m_sSimID = "-1";
	this->m_bDebug = false;
	this->m_bValidate = false;

	m_pCom= 0;
	m_pModelHandler = 0;
}

Simulator::~Simulator() {
}

void Simulator::usage(const std::string& p_name) {
		cout << "Usage: " << p_name << " commands" << endl;
		cout << "The following commands are available:" << endl;
		cout << "'-d val'. Debug. val can be either 'on' or 'off'. Required" << endl;
		cout << "'-m mapID'. Required" << endl;
		cout << "'-s simID'. Required" << endl;
		cout << "'-v'. Forces validation of network. Optional." << endl;
}

bool Simulator::showtime() {
	// Connect to the database
	m_pCom = new Com();
	m_pModelHandler = new ModelHandler();

	if (!m_pCom->connectDB(m_bDebug, m_sMapID, m_sSimID)) {
		if (m_bDebug)
			cout << "ERROR: connecting to database." << endl;
		return false;
	}
	
	// Initiate simulation values
	if (!m_pCom->readSim()) {
		if (m_bDebug)
			cout << "ERROR: reading simulation values." << endl;
		return false;
	}
	if (!m_pCom->readModel()) {
		if (m_bDebug)
			cout << "ERROR: reading model values." << endl;
		return false;
	}
	if (m_pCom->getModel() != (int)m_pCom->getModelValue(0)) {
		if (m_bDebug)
			cout << "ERROR: mismatch between simulation and model values." << endl;
		return false;
	}
	
	// Run simulation
	(void)time(&m_TimeCurrent);
	if (m_bDebug)
		cout << "Initalizing simulation: " << (int)m_TimeCurrent-m_TimeBegin << " seconds." << endl;
	if (!m_pModelHandler->run(m_pCom)) {
		if (m_bDebug)
			cout << "Simulation failed!" << endl;
	}
	(void)time(&m_TimeCurrent);
	if (m_bDebug)
		cout << "Finalizing simulation: " << (int)m_TimeCurrent-m_TimeBegin << " seconds." << endl;

	// Log simulation time
	if (!m_pCom->logSimulationTime((int)m_TimeCurrent-m_TimeBegin)) {
		if (m_bDebug)
			cout << "ERROR: logging simulation time." << endl;
		return false;
	}

	// Disconnect from the database
	if (!m_pCom->disconnectDB()) {
		if (m_bDebug)
			cout << "ERROR: disconnecting to database." << endl;
		return false;
	}
	return true;
}