#include "Lab3.h"
#include "Content.h"
#include "VectorDrawer.h"
#include "LazerBeam.h"

using namespace BGE;

Lab3::Lab3(void)
{
	elapsed = 10000;
}


bool Lab3::Initialise()
{
	std::shared_ptr<GameComponent> ground = make_shared<Ground>();
	Attach(ground);	

	ship1 = make_shared<GameComponent>(true);
	ship1->Attach(Content::LoadModel("cobramk3", glm::rotate(glm::mat4(1), 180.0f, glm::vec3(0,1,0))));
	ship1->Attach(make_shared<VectorDrawer>(glm::vec3(5,5,5)));
	ship1->transform->position = glm::vec3(-10, 2, -10);
	Attach(ship1);

	ship2 = make_shared<GameComponent>(true);
	ship2->Attach(Content::LoadModel("python", glm::rotate(glm::mat4(1), 180.0f, glm::vec3(0,1,0))));
	ship2->Attach(make_shared<VectorDrawer>(glm::vec3(5,5,5)));
	ship2->transform->position = glm::vec3(10, 2, -10);
	Attach(ship2);
	Game::Initialise();

	camera->transform->position = glm::vec3(0, 4, 20);
	return true;
}

void Lab3::Update(float timeDelta)
{
	static float timeToFire = 1.0f / 2.0f;

	// Movement of ship2
	if (keyState[SDL_SCANCODE_UP])
	{
		ship2->transform->position += ship2->transform->look * speed * timeDelta;
	}
	if (keyState[SDL_SCANCODE_DOWN])
	{
		ship2->transform->position -= ship2->transform->look * speed * timeDelta;
	}

	//rotations of the ship2
	if (keyState[SDL_SCANCODE_LEFT])
	{
		ship2->transform->Yaw(speed *speed * timeDelta);
	}
	if (keyState[SDL_SCANCODE_RIGHT])
	{
		ship2->transform->Yaw(-speed *speed * timeDelta);
	}

	//movement of Ship 1
	if (keyState[SDL_SCANCODE_U])
	{
		ship1->transform->position += ship1->transform->look * speed * timeDelta;
	}
	if (keyState[SDL_SCANCODE_J])
	{
		ship1->transform->position -= ship1->transform->look * speed * timeDelta;
	}

	//rotations of the ship1
	if (keyState[SDL_SCANCODE_H])
	{
		ship1->transform->Yaw(speed *speed * timeDelta);
	}
	if (keyState[SDL_SCANCODE_K])
	{
		ship1->transform->Yaw(-speed *speed * timeDelta);
	}


	//now lets calculate the distance between the ships
	//equation of a line: m= y2-y1/x2-x1

	glm::vec3 distance = ship1->transform->position - ship2->transform->position;

	if (glm::length(distance) < 5)
	{
		PrintText("Target is in range");
	}
	else
	{
		PrintText("Target is not in range");
	}

	//now we need to calculate if the ships are in front of each other
	//we do this with the dot product.
	distance = glm::normalize(distance);// so first we normailse the distance 
	float dot = glm::dot(distance, ship1->transform->look);
	//for some reason my dot product is in the reverse so I will make it negative before checking
	dot = 0 - dot;
	if(dot > 0){//By checking if dot is negative we know it is behind the forward axis of the ship
		PrintText("Ship is Ahead");
	}
	else{
		PrintText("Ship is Behind");
	}

	//now lets check the FOV
	float angle = glm::acos(dot);
	float halffov = glm::radians(45.0f) / 2.0f;
	if (angle < halffov){
		PrintText("In FOV FIRE!");

		//now lets add the firing code
		if (elapsed > timeToFire)
		{
			shared_ptr<LazerBeam> lazerBeam = make_shared<LazerBeam>();//so now we create our pointer for our lazerbeam
			lazerBeam->transform->position = ship1->transform->position;//as it is being fired from ship 1 we can set the starting pos as the current pos of ship1.
			lazerBeam->transform->look = ship1->transform->look;//here we set the look rotation of the LazerBeam
			Attach(lazerBeam);//now lets attach the lazerBeam to the scene
			elapsed = 0.0f;//finally we reset the elapsed time to 0.


		}
	}
	else
	{
		PrintText("Target is not in Sight!");
	}

	elapsed += timeDelta;

	Game::Update(timeDelta);
}
