#pragma once

class IBuff
{
	IBuff()
	{}

	virtual void buffAdjust(Player * player) = 0;
	virtual void buffRemove(Player * player) = 0;


}
