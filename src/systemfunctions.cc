#include "../header/systemfunctions.h"

using namespace std;

SystemFunctions::SystemFunctions() {

}

SystemFunctions::~SystemFunctions() {
}

Matrix SystemFunctions::createA(Matrix p_Components){
	/////////////////////////////////////////////////
	// find size of A matrix. n is rows, m is highest number of begin/end node plus one
	/////////////////////////////////////////////
	int n = (int)p_Components.rows();
	int m = 0;
	for (int i=0; i<n; i++) {
		if (p_Components(i,2) > m)
			m = p_Components(i,2);
		if (p_Components(i,3) > m)
			m = p_Components(i,3);
	}
	m = m+1;
	Matrix tmpA = Matrix (m,n,0);

	////////////////////////////
	// insert values begin/end values into A
	////////////////////////
	for (int i=0; i<n; i++) {
		if (p_Components(i,2) != -1)
			tmpA (p_Components(i,2),p_Components(i,0)) = 1;
		if (p_Components(i,3) != -1)
			tmpA (p_Components(i,3),p_Components(i,0)) = -1;
	}
	
	return tmpA;
}

Matrix SystemFunctions::createAT(Matrix p_A) {
	// AT is always the m by m part of p_A (at least in special case)
	int m = (int)p_A.rows();
	Matrix tmpAT = Matrix (m,m,0);
	
	for (int i=0; i<m ; i++) {
		for (int j=0; j<m; j++) {
			tmpAT(i,j) = p_A(i,j);
		}
	}
	return tmpAT;
}

/*
 * Returns the link part of the D matrix
 */
Matrix SystemFunctions::createDL(Matrix p_D) {
	int m = (int)p_D.rows();
	int n = (int)p_D.cols();
	Matrix tmpDL = Matrix (m,n-m,0);
	for (int i=0; i<m; i++) {
		for (int j=0; j<(n-m); j++) {
			tmpDL(i,j) = p_D(i,j+m);
		}
	}
	return tmpDL;
}

/*
 * Returns the F11 part of the DL matrix
 */
Matrix SystemFunctions::createF11(Matrix p_DL, Matrix p_Components) {
	int n = (int)p_DL.cols();
	Matrix tmpF11 = Matrix (1,n,0); // only handles special case (general case might use more than 1 producer)
	for (int i=0; i<1; i++) {
		for (int j=0; j<n; j++) {
			tmpF11(i,j) = p_DL(i,j);
		}
	}
	return tmpF11;
}

Matrix SystemFunctions::createF41(Matrix p_DL, Matrix p_Components) {
	int m = (int)p_DL.rows();
	int n = (int)p_DL.cols();
	Matrix tmpF41 = Matrix (m-1,n,0); // only handles special case with 1 producer and rest pipes
	for (int i=1; i<m; i++) {
		for (int j=0; j<n; j++) {
			tmpF41(i-1,j) = p_DL(i,j);
		}
	}
	return tmpF41;
}

/*
 * Returns a matrix with m = nr of flow sources (i.e. boundary consumers)
 */
Matrix SystemFunctions::createMML(Matrix p_Components) {
	// count the number of consumers (all boundary consumers are links)
	int iNrOfConsumers = 0;
	int count = (int)p_Components.rows();
	for (int i=0; i<count; i++) {
		if (p_Components(i,POS_OF_TYPE) == TYPE_CONSUMER_BOUNDARY)
			iNrOfConsumers++;
	}
	Matrix tmpMML = Matrix (iNrOfConsumers,1,0);
	return tmpMML;
}

/*
 * Returns a matrix with m = nr of head sources (i.e. boundary producers)
 */
Matrix SystemFunctions::createHHT(Matrix p_Components) {
	// count the number of boundary producers
	int iNrOfProducers = 0;
	int count = (int)p_Components.rows();
	for (int i=0; i<count; i++) {
		if (p_Components(i,POS_OF_TYPE) == TYPE_PRODUCER_BOUNDARY)
			iNrOfProducers++;
	}
	Matrix tmpHHT = Matrix (iNrOfProducers,1,0);
	return tmpHHT;
}

/*
 * Returns a matrix containing the submatrix specified by the input parameters
 */
Matrix SystemFunctions::subMatrix(Matrix p_M, int p_startM, int p_endM, int p_startN, int p_endN) {
	Matrix tmpSub = Matrix(p_endM-p_startM+1, p_endN-p_startN+1);
	// implementation should include boundary check, e.g. that p_endM is within the range of p_M
	return tmpSub;
}
