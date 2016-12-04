#pragma once
#include "Game.h"
#include "PhysicsController.h"
#include "PhysicsFactory.h"
#include <btBulletDynamicsCommon.h>

namespace BGE
{
	class Assignment :
		public Game
	{
	private:
		btBroadphaseInterface* broadphase;

		// Set up the collision configuration and dispatcher
		btDefaultCollisionConfiguration * collisionConfiguration;
		btCollisionDispatcher * dispatcher;

		// The actual physics solver
		btSequentialImpulseConstraintSolver * solver;
	public:

		Assignment();
		~Assignment();

		shared_ptr<PhysicsController> CreateBiped(glm::vec3);
		
		//No longer in use
		//shared_ptr<PhysicsController> CreateHingedLeg(glm::vec3);

		btHingeConstraint * RkneeHinge;
		btHingeConstraint * LkneeHinge;
		btHingeConstraint * LhipHinge;
		btHingeConstraint * RhipHinge;
		btHingeConstraint * abHinge;

		bool firstRun;

		float scale;
		glm::vec3 pos;
		float elapsed;
		bool Initialise();
		void Update(float timeDelta);
	};
}

