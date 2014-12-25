#pragma once

#include "json/json.h"

class Callback {
public:
	virtual void callback(Json::Value &) = 0;
};