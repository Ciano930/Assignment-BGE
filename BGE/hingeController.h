
#pragma once
#include "GameComponent.h"
#include "PhysicsController.h"
#include "Game.h"
#include "Content.h"

namespace BGE
{
	class hingeController :public PhysicsController{

	public:
		hingeController(btHingeConstraint *, btHingeConstraint *, btHingeConstraint *, btHingeConstraint *, btHingeConstraint *, bool);
		~hingeController();
		void Update(float timeDelta);
		void MoveLegs();
		
		btHingeConstraint * RkneeHinge;
		btHingeConstraint * LkneeHinge;
		btHingeConstraint * RhipHinge;
		btHingeConstraint * LhipHinge;
		btHingeConstraint * abHinge;

		float RkneeHingeAngle = 0.0f;
		float LkneeHingeAngle = 0.0f;
		bool firstRun;

	};
}
