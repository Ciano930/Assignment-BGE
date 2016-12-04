#pragma once
#include "Game.h"
#include "GameComponent.h"
#include "PhysicsFactory.h"

using namespace std;

namespace BGE
{
	class Lab7 :
		public Game
	{
	public:
		Lab7(void);

		shared_ptr<GameComponent> sphere1;
		shared_ptr<GameComponent> block1;
		float elapsed;
		bool Initialise();
		void Update(float timeDelta);
	};
}
