#include "Lab7.h"
#include "Content.h"
#include "VectorDrawer.h"
#include "PhysicsFactory.h"
#include "gravitycontroller2.h"

using namespace BGE;

Lab7::Lab7(void)
{
	elapsed = 10000;
}

bool Lab7::Initialise(){

	std::shared_ptr<GameComponent> ground = make_shared<Ground>();
	Attach(ground);

	sphere1 = make_shared<GameComponent>(true);
	sphere1->Attach(Content::LoadModel("cube", glm::mat4()));// = physicsFactory->CreateSphere(2.0f, glm::vec3(0, 20, 0), glm::quat(), true, true);
	sphere1->transform->position.y = 20.0f;
	sphere1->Attach(make_shared<GravityController2>());
	sphere1->Attach(make_shared<VectorDrawer>());

	Attach(sphere1);
	Game::Initialise();

	camera->transform->position = glm::vec3(0, 4, 20);
	return true;
}

void Lab7::Update(float timeDelta){
	Game::Update(timeDelta);
}

