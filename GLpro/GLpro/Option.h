#pragma once

class Option
{
public:
	Option() {
		_oldLightUse = true;
	};
	~Option() {};

public:
	bool _oldLightUse = true;
};

extern Option *GOption;