#include "hingeController.h"
#include <btBulletDynamicsCommon.h>
#include "Utils.h"
using namespace BGE;

hingeController::hingeController(btHingeConstraint * j1, btHingeConstraint * j2, btHingeConstraint * j3, btHingeConstraint * j4, btHingeConstraint * j5, bool bool1) {
	RkneeHinge = j1;
	LkneeHinge = j2;
	RhipHinge = j3;
	LhipHinge = j4;
	abHinge = j5;
	RkneeHingeAngle = 0.0f;
	LkneeHingeAngle = 135.0f;
}

hingeController::~hingeController() {

}

void hingeController::Update(float timeDelta) {

	MoveLegs();
	GameComponent::Update(timeDelta);
}

void hingeController::MoveLegs(){		
	
	if (RkneeHingeAngle < 135 )
		{
			LkneeHingeAngle = LkneeHingeAngle - 20;
			RkneeHingeAngle = RkneeHingeAngle + 20;
			RkneeHinge->enableAngularMotor(true, 135, -10);
			RhipHinge->enableAngularMotor(true, 40, -5);
			LkneeHinge->enableAngularMotor(true, 135, 20);
			LhipHinge->enableAngularMotor(true, 40, 5);
		}
		else
		{
			RkneeHingeAngle = RkneeHingeAngle - 20;
			LkneeHingeAngle = LkneeHingeAngle + 20;
			RkneeHinge->enableAngularMotor(true, 135, -10);
			RhipHinge->enableAngularMotor(true, 40, -5);
			LkneeHinge->enableAngularMotor(true, 135, 20);
			LhipHinge->enableAngularMotor(true, 40, 5);
		}
}

