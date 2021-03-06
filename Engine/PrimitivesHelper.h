#pragma once

#include "EngineDLL.h"
#include "Model.h"

class Vertex;

class PrimitivesHelper
{
public:
	ENGINE_API static Model CreateTriangle(Vertex a, Vertex b, Vertex c);
	ENGINE_API static Model CreateQuad(Vertex a, Vertex b, Vertex c, Vertex d);
};

