#include "Option.h"

Option *GOption = nullptr;

Option::Option() {
	_oldLightUse = true;
	_useHDR = true;
	_useBloom = false;		// can use if hdr on
	_useFXAA = false;
	_bUsePostEffect = false;
}
