#include <string>
/******************************************************
*  Document   :	Entity.h
*  Description: �ֻ��� object
*******************************************************/
class Entity {
	public:
		static int _sMaxID;		//object ���� ID(0 ~ ). �ڵ� ����.

		Entity();
		Entity(const int &type);
		virtual ~Entity() {}

		int getType();
		int getID();

	private:
		int _type = 0;
		int _ID;

};