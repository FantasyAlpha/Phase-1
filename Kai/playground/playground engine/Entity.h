#pragma once

#include "Mesh.h"

struct Entity
{
	void *Component;
	Transform Local_Transform;
	Transform Global_Transform;
};