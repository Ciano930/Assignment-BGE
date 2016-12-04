#include "Assignment.h"
#include "PhysicsController.h"
#include "Sphere.h"
#include "PhysicsCamera.h"
#include "Box.h"
#include "Cylinder.h"
#include "Steerable3DController.h"
#include "gravitycontroller2.h"
#include "hingeController.h"
#include "Ground.h"
#include "Content.h"
#include <btBulletDynamicsCommon.h>
#include <gtc/quaternion.hpp>
#include <gtx/quaternion.hpp>
#include <gtx/euler_angles.hpp>
#include <gtx/norm.hpp>
#include "VectorDrawer.h"
#include "Utils.h"

using namespace BGE;

Assignment::Assignment()
{
	elapsed = 10000;
}


Assignment::~Assignment()
{
}
/*
1. The first RigidBody
2. The second RigidBody
3. The point in the first body where you want the actual joint to be (relative to its centre)
4. The point in the second body where you want the actual joint to be (relative to its centre)
5. The axis of rotation around the first body
6. The axis of rotation around the second body
*/
bool Assignment::Initialise()
{
	
	scale = 0.50f;
	pos = glm::vec3(0.0f, 15.0f* scale, 0.0f);
	physicsFactory->CreateGroundPhysics();
	Attach(ground);
	firstRun = true;
	Game::dynamicsWorld->setGravity(btVector3(0, -20, 0));

	//testing with a ragdoll has shown that the physics only pays attention to the centre object. as such I will need to apply gravity as individual elements to each part
	/*shared_ptr<PhysicsController> ragdoll = physicsFactory->CreateCapsuleRagdoll(glm::vec3(0.0f, 10.0f, 0.0f));
	ragdoll->Attach(make_shared<GravityController2>());
	*/

	//Now lets define our robot

	shared_ptr<PhysicsController> robot = CreateBiped(pos);
	

	//pos = glm::vec3(0.0f, 15.0f* scale, 10.0f*scale);
	//shared_ptr<PhysicsController> robot2 = CreateBiped(pos);
	//robot->Attach(make_shared<GravityController2>());


	Game::Initialise();
	camera->transform->position = glm::vec3(0, 4, 20);
	return true;
}

void BGE::Assignment::Update(float timeDelta)
{
	//The biggest issue I have had is getting my controller updates to run more than once...
	Game::Update(timeDelta);
}

shared_ptr<PhysicsController> Assignment::CreateBiped(glm::vec3 position)
{
	std::shared_ptr<PhysicsController> waist = physicsFactory->CreateBox(3.0f*scale, 2.0f*scale, 2.0f*scale, position, glm::quat());
	std::shared_ptr<PhysicsController> chest = physicsFactory->CreateBox(2.0f*scale, 5.0f*scale, 2.0f*scale, glm::vec3(position.x, position.y + (4.0f*scale), position.z), glm::quat());
	//============================================================================================================================================================================//
		//Originally legs were generated in a single function however the need to access the joints made this impracticle. 
	//============================================================================================================================================================================//
	//left leg
	glm::vec3 pos = glm::vec3(position.x + (3.0f* scale), position.y - (1.5f* scale), position.z);
	std::shared_ptr<PhysicsController> Lthigh = physicsFactory->CreateBox(2.0f*scale, 5.0f*scale, 2.0f*scale, pos, glm::quat());
	std::shared_ptr<PhysicsController> Lshin = physicsFactory->CreateBox(2.0f*scale, 6.0f*scale, 2.0f*scale, glm::vec3(pos.x, pos.y - (6.5f*scale), pos.z), glm::quat());
	//right leg
	pos = glm::vec3(glm::vec3(position.x - (3.0f*scale), position.y - (1.5f* scale), position.z));
	std::shared_ptr<PhysicsController> Rthigh = physicsFactory->CreateBox(2.0f*scale, 5.0f*scale, 2.0f*scale, pos, glm::quat());
	std::shared_ptr<PhysicsController> Rshin = physicsFactory->CreateBox(2.0f*scale, 6.0f*scale, 2.0f*scale, glm::vec3(pos.x, pos.y - (6.5f*scale), pos.z), glm::quat());

	//============================================================================================================================================================================//

	 LkneeHinge = new btHingeConstraint(*Lthigh->rigidBody, *Lshin->rigidBody, GLToBtVector(glm::vec3(0, 0 - 2.5f*scale, -0.5f*scale)), GLToBtVector(glm::vec3(0, 0 + 4.0f*scale, -0.5f*scale)), btVector3(1, 0, 0), btVector3(1, 0, 0));
	 RkneeHinge = new btHingeConstraint(*Rthigh->rigidBody, *Rshin->rigidBody, GLToBtVector(glm::vec3(0, 0 - 2.5f*scale, -0.5f*scale)), GLToBtVector(glm::vec3(0, 0 + 4.0f*scale, -0.5f*scale)), btVector3(1, 0, 0), btVector3(1, 0, 0));

	 glm::vec3 LhipHingePos = glm::vec3(position.x + (1.75f* scale), position.y, position.z);
	 glm::vec3 RhipHingePos = glm::vec3(position.x - (1.75f* scale), position.y, position.z);

	 LhipHinge = new btHingeConstraint(*waist->rigidBody, *Lthigh->rigidBody, GLToBtVector(glm::vec3(2.5f* scale, 0, 0)), GLToBtVector(glm::vec3(0, 2.0f*scale, 0)), btVector3(1, 0, 0), btVector3(1, 0, 0));
	 RhipHinge = new btHingeConstraint(*waist->rigidBody, *Rthigh->rigidBody, GLToBtVector(glm::vec3(-2.5f* scale, 0, 0)), GLToBtVector(glm::vec3(0, 2.0f*scale, 0)), btVector3(1, 0, 0), btVector3(1, 0, 0));
	 abHinge = new btHingeConstraint(*waist->rigidBody, *chest->rigidBody, GLToBtVector(glm::vec3(0, 2.0f*scale, 0)), GLToBtVector(glm::vec3(0, -2.0f*scale, 0)), btVector3(1, 0, 0), btVector3(1, 0, 0));

	 //Joint Limits
	 LkneeHinge->setLimit(-100, 0);
	 RkneeHinge->setLimit(-100, 0);
	 LhipHinge->setLimit(-30, 20);
	 RhipHinge->setLimit(-30, 20);
	 abHinge->setLimit(-10, 10);

	 std::shared_ptr<hingeController> hingeCont = make_shared<hingeController>(RkneeHinge, LkneeHinge, RhipHinge, LhipHinge, abHinge, firstRun);
	 waist->Attach(hingeCont);

	//============================================================================================================================================================================//

	Game::dynamicsWorld->addConstraint(LkneeHinge);
	Game::dynamicsWorld->addConstraint(RkneeHinge);
	Game::dynamicsWorld->addConstraint(LhipHinge);
	Game::dynamicsWorld->addConstraint(RhipHinge);
	Game::dynamicsWorld->addConstraint(abHinge);

	//============================================================================================================================================================================//

	//now we need our base motor positions

	LkneeHinge->enableAngularMotor(true, 0, 30);
	RkneeHinge->enableAngularMotor(true, 0, 30);
	LhipHinge->enableAngularMotor(true, 0, 20);
	RhipHinge->enableAngularMotor(true, 0, 20);
	abHinge->enableAngularMotor(true, 0, 20);

	//============================================================================================================================================================================//

	//Now lets give him a head

	btTransform neckHead;
	neckHead.setIdentity();
	btTransform neckChest;
	neckChest.setIdentity();
	neckHead.setOrigin(btVector3(0, -2*scale, 0));
	neckChest.setOrigin(btVector3(0, 2*scale, 0));
	std::shared_ptr<PhysicsController> head = physicsFactory->CreateSphere(2.0f*scale, glm::vec3(position.x, position.y + (6.0f*scale), position.z), glm::quat());
	btFixedConstraint * neck = new btFixedConstraint(*head->rigidBody, *chest->rigidBody, neckHead, neckChest);

	Game::dynamicsWorld->addConstraint(neck);

	waist->Attach(chest);
	waist->Attach(Lshin);
	waist->Attach(Lthigh);
	waist->Attach(Rshin);
	waist->Attach(Rthigh);

	return waist;
}
