#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "../Transform.h"

class Player : public Transform {
	public:
		Player() {};
		virtual ~Player() {};

		void collidsionCheck(*this, _colidBox);
		void update(float deltaTime);
		

	private:
		PhysicManager * physicsManager;
};

#endif