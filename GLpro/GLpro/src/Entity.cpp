#include "./Entity.h"

int Entity::_sMaxID = 0;

Entity::Entity() {
	_ID = _sMaxID;
	++_sMaxID;
	_type = 0;
}
Entity::Entity(const int &type) {
	_ID = _sMaxID;
	++_sMaxID;

	_type = type;
}

int Entity::getType() {
	return _type;
}

int Entity::getID() {
	return _ID;
}