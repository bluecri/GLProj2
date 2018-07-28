#include "Option.h"

Option *GOption = nullptr;

Option::Option() {
	_oldLightUse = true;
	_useHDR = true;
	_useToneMapping = false;
	_useBloom = true;		// can use if hdr on
	_useFXAA = true;
	_useReadPixel = false;
	_bUsePostEffect = false;
}
