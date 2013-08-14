#ifndef _SYSTEMFUNCTIONS_H_
#define _SYSTEMFUNCTIONS_H_

#include <iostream>
#include <string>
#include <octave/oct.h>

#include "defs.h"

class SystemFunctions
{
private:

public:
	SystemFunctions();
	virtual ~SystemFunctions();

	Matrix m_mComponents;
	Matrix m_mGlobal;
	Matrix m_mMML;
	Matrix m_mHHT;
	Matrix m_mA;
	Matrix m_mAT;
	Matrix m_mDL;
	Matrix m_mD;
	Matrix m_mF11;
	Matrix m_mF41;

	Matrix createA(Matrix p_Components);
	Matrix createAT(Matrix p_A);
	Matrix createDL(Matrix p_D);
	Matrix createF11(Matrix p_DL, Matrix p_Components);
	Matrix createF41(Matrix p_DL, Matrix p_Components);
	Matrix createMML(Matrix p_Components);
	Matrix createHHT(Matrix p_Components);

	Matrix subMatrix(Matrix p_M, int p_startM, int p_endM, int p_startN, int p_endN);
};

#endif