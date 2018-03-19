#include <string>
/******************************************************
*  Document   :	Entity.h
*  Description: 최상위 object
*******************************************************/
class Entity {
	public:
		static int _sMaxID;		//object 고유 ID(0 ~ ). 자동 생성.

		Entity();
		Entity(const int &type);
		virtual ~Entity() {}

		int getType();
		int getID();

	private:
		int _type = 0;
		int _ID;

};