#include "Option.h"
#include "src/Control/InputManager.h"

Option *GOption = nullptr;

Option::Option()
{
	_oldLightUse = true;
	_useHDR = true;
	_useToneMapping = false;
	_useBloom = true;		// can use if hdr on
	_useFXAA = 0;	// 0 : use / red color / none
	_useReadPixel = false;
	_bUsePostEffect = false;

	_debugKeyInfo = false;
	_printInfo = false;
	_printFrustumSphere = false;
	_printCollisionBox = false;
	_printOctree = false;
	_printOctreeLevel = 0;

	_offNormalObj = false;
	_offSkybox = false;
	_offParticle = false;
}

void Option::_debugInfoPrintProgress(long long transferKeyInput)
{
	if (GInputManager->controlCheck(transferKeyInput, ENUM_BEHAVIOR::KEY_F1_ONCE))
		_debugKeyInfo = !_debugKeyInfo;

	if (GInputManager->controlCheck(transferKeyInput, ENUM_BEHAVIOR::KEY_F2_ONCE))
		_printInfo = !_printInfo;

	if (GInputManager->controlCheck(transferKeyInput, ENUM_BEHAVIOR::KEY_F3_ONCE))
		_offNormalObj = !_offNormalObj;

	if (GInputManager->controlCheck(transferKeyInput, ENUM_BEHAVIOR::KEY_F4_ONCE))
		_offSkybox = !_offSkybox;

	if (GInputManager->controlCheck(transferKeyInput, ENUM_BEHAVIOR::KEY_F5_ONCE))
		_offParticle = !_offParticle;

	if (GInputManager->controlCheck(transferKeyInput, ENUM_BEHAVIOR::KEY_F6_ONCE))
		_printFrustumSphere = !_printFrustumSphere;

	if (GInputManager->controlCheck(transferKeyInput, ENUM_BEHAVIOR::KEY_F7_ONCE))
		_printCollisionBox = !_printCollisionBox;

	if (GInputManager->controlCheck(transferKeyInput, ENUM_BEHAVIOR::KEY_F8_ONCE))
		_printOctree = !_printOctree;

	if (GInputManager->controlCheck(transferKeyInput, ENUM_BEHAVIOR::KEY_F9_ONCE))
		_useHDR = !_useHDR;

	if (GInputManager->controlCheck(transferKeyInput, ENUM_BEHAVIOR::KEY_F10_ONCE))
		_useToneMapping = !_useToneMapping;

	if (GInputManager->controlCheck(transferKeyInput, ENUM_BEHAVIOR::KEY_F11_ONCE))
		_useBloom = !_useBloom;

	if (GInputManager->controlCheck(transferKeyInput, ENUM_BEHAVIOR::KEY_F12_ONCE))
		_useFXAA = (_useFXAA + 1) % 3;

	if (GInputManager->controlCheck(transferKeyInput, ENUM_BEHAVIOR::KEY_MIDDLE_PLUS_ONCE))
		_printOctreeLevel += 1;

	if (GInputManager->controlCheck(transferKeyInput, ENUM_BEHAVIOR::KEY_MIDDLE_MINUS_ONCE))
		_printOctreeLevel -= 1;
}
