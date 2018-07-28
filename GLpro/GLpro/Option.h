#pragma once

class Option
{
public:
	Option();;
	~Option() {};

public:
	bool _oldLightUse;
	bool _useHDR;
	bool _useToneMapping;
	bool _useBloom;		// can use if hdr on
	bool _useFXAA;
	bool _bUsePostEffect;
	bool _useReadPixel;
	
};

extern Option *GOption;