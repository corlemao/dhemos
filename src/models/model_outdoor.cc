#include "../../header/models/model_outdoor.h"

using namespace std;

Model_Outdoor::Model_Outdoor() {

}

Model_Outdoor::~Model_Outdoor() {
}

////////////////////////////
// populates the events.csv file with outdoor and ambient values
////////////////////////
bool Model_Outdoor::events(Com* pCom) {
		if (pCom->getOutdoorModel() == 1) {
		// default, don't do anything, let the events.cvs file be populated manually
		return true;
	} else if (pCom->getOutdoorModel() == 2) {
		// implement db connection (from yr.no)
		return true;
	} else if (pCom->getOutdoorModel() == 3) {
		// implement ensemble forecasts based on db connection (from yr.no)
		return true;
	} else {
		return false;
	}
	return true;
}