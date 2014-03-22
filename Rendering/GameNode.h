#pragma once

#include "Rendering.h"
#include <list>

namespace Rendering
{

class GameNode
{
public:
	GameNode(void);
	~GameNode(void);

public:
	void Init();
	void Update();
	void Render(/*Shader* shader*/);
private:
	//std::list<Rendering::GameNode&> childrenGameNodes;
	GLuint vertexBuffer;
	GLuint elementBuffer;
}; /* end class GameNode */

} /* end namespace Rendering */