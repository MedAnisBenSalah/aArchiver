#include "Util.h"

int CUtil::Round(float fValue)
{
	// Get the numeric part of the float
	float fNumeric = fValue - (int)fValue;
	return (fNumeric >= 0.5f ? (int)fValue + 1 : (int)fValue);
}