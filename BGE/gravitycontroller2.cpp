#include "gravitycontroller2.h"
#include "Utils.h"
#include "Game.h"
using namespace BGE;

GravityController2::GravityController2()
{
	gravity = glm::vec3(0.0f, -9.8f, 0.0f);

}

void GravityController2::Update(float timeDelta)
{
	//now lets add some bounce loigic

	if (transform->position.y  < 1){
		transform->velocity = -(transform->velocity)*0.5f;//We'll make this value be itself in negative
	}
	else if (transform->position.y == 0.0f)
	{
		//No gravity on the ground
	}
	else
	{
		transform->velocity = transform->velocity + gravity  * timeDelta;
	}
	transform->position = transform->position + transform->velocity * timeDelta;
	//GameComponent::Update(timeDelta);

	GameComponent::Update(timeDelta);
}