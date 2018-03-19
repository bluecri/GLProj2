#ifndef __TRANSFORM_MESSAGE_H__
#define __TRANSFORM_MESSAGE_H__

#include "./Message.h"
#include "../Transform.h"

struct TransfomrMessage : public Message {
	const Transform& t;
};

#endif