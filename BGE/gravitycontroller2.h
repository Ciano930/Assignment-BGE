#pragma once
#include "GameComponent.h"

namespace BGE
{
	class GravityController2 :
		public GameComponent
	{
	public:

		GravityController2();
		glm::vec3 gravity;
		void Update(float timeDelta);

	};
}
