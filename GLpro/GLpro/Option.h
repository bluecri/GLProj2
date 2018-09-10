#pragma once

class Option
{
public:
	Option();
	~Option() {};

	void _debugInfoPrintProgress(long long transferKeyInput);

public:
	bool _oldLightUse;
	bool _useHDR;
	bool _useToneMapping;
	bool _useBloom;		// can use if hdr on
	int	 _useFXAA;
	bool _bUsePostEffect;
	bool _useReadPixel;

	bool _debugKeyInfo;
	bool _printInfo;

	bool _printFrustumSphere;
	bool _printCollisionBox;
	bool _printOctree;
	int	 _printOctreeLevel;

	bool _offSkybox;
	bool _offNormalObj;
	bool _offParticle;
};

extern Option *GOption;