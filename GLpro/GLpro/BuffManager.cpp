#include "BuffManager.h"
#include "BuffSum.h"

BuffManager::BuffManager()
{
	_minBuffDuration = 0.0f;
	_buffSum = new BuffSum();
}

BuffManager::~BuffManager()
{
	for (auto elem : _buffList)
		delete elem.first;
	delete _buffSum;
}

// buff를 순회하며 buffSum 생성. buffSum + cur/origin -> new curInfo 적용.
int BuffManager::addNewBuff(ENUM_FieldItemForBuff_TYPE fieldItemForBuffType, IBuff * buff)
{
	int ret = 0;
	auto it = _lastBuffPushedMap.find(buff->getBuffType());

	if (_lastBuffPushedMap.end() != it)
	{
		switch (fieldItemForBuffType)
		{
		// always reset to new buff
		case ENUM_FieldItemForBuff_UNIQUE_RESET_ALWAYS:

			// remove buff from bufflist
			delete (*it).second->first;
			_buffList.erase((*it).second);
			_minBuffDuration = -1.0f;

			_buffList.push_front(std::make_pair(buff, true));
			(*it).second = _buffList.begin();

			ret = 1;
			break;

		// reset to new buff only more powerful buff
		case ENUM_FieldItemForBuff_UNIQUE_RESET_NOTMOREPOWERFUL:
			if ((*it).second->first->getPowerful() < buff->getPowerful())
			{
				// remove buff from bufflist
				delete (*it).second->first;
				_buffList.erase((*it).second);
				_minBuffDuration = -1.0f;

				_buffList.push_front(std::make_pair(buff, true));
				(*it).second = _buffList.begin();
			}

			ret = 1;
			break;

		// no reset if has same kind buff
		case ENUM_FieldItemForBuff_UNIQUE_NONRESET:
			break;

		// always stack new buff
		case ENUM_FieldItemForBuff_NONUNIQUE:
			(*it).second->second = false;
			_minBuffDuration = -1.0f;

			_buffList.push_front(std::make_pair(buff, true));
			(*it).second = _buffList.begin();

			ret = 1;
			break;

		case ENUM_FieldItemForBuff_NUM:
			break;
		}
	}
	else
	{
		_buffList.push_front(std::make_pair(buff, true));
		_lastBuffPushedMap.insert(std::make_pair(buff->getBuffType(), _buffList.begin()));
		_minBuffDuration = -1.0f;
		ret = 1;
	}
	return ret;
}

bool BuffManager::isNeedToTransferBuffSum(float deltaTime, float acc)
{
	bool ret = false;
	_minBuffDuration -= deltaTime;

	if (_minBuffDuration < 0.0f)
	{
		ret = true;
		_minBuffDuration = 1000.0f;
		_buffSum->partialInitBuffSum();

		for (auto it = _buffList.begin(); it != _buffList.end(); )
		{
			// remove end buff
			(*it).first->_lifeTime -= deltaTime;
			if ((*it).first->_lifeTime < 0.0f)
			{
				if (it->second == true)
				{
					_lastBuffPushedMap.erase(it->first->getBuffType());
				}
				it = _buffList.erase(it);
				continue;
			}

			// calc buffSum
			(*it).first->adjustBuffSum(_buffSum);

			// calc min duration
			_minBuffDuration = std::min(_minBuffDuration, (*it).first->_lifeTime);

			++it;
		}
	}
	else
	{
		for (auto it = _buffList.begin(); it != _buffList.end(); )
		{
			(*it).first->_lifeTime -= deltaTime;
		}
	}

	return ret;
}

BuffSum * BuffManager::getBuffSum()
{
	return _buffSum;
}
