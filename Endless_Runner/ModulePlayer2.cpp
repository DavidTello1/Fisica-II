#include "Globals.h"
#include "Application.h"
#include "ModulePlayer2.h"
#include "Primitive.h"
#include "PhysVehicle3D.h"
#include "PhysBody3D.h"

ModulePlayer2::ModulePlayer2(Application* app, bool start_enabled) : Module(app, start_enabled), vehicle(NULL)
{
	turn = acceleration = brake = 0.0f;
	first_load = true;
}

ModulePlayer2::~ModulePlayer2()
{}

// Load assets
bool ModulePlayer2::Start()
{
	LOG("Loading player");

	VehicleInfo car;

	// Car properties ----------------------------------------
	car.chassis_size.Set(2, 2, 4);
	car.chassis_offset.Set(0, 1.5, 0);
	car.mass = 500.0f;
	car.suspensionStiffness = 15.88f;
	car.suspensionCompression = 0.83f;
	car.suspensionDamping = 0.88f;
	car.maxSuspensionTravelCm = 1000.0f;
	car.frictionSlip = 50.5;
	car.maxSuspensionForce = 6000.0f;

	// Wheel properties ---------------------------------------
	float connection_height = 1.2f;
	float wheel_radius = 0.6f;
	float wheel_width = 0.5f;
	float suspensionRestLength = 1.2f;

	// Don't change anything below this line ------------------

	float half_width = car.chassis_size.x*0.5f;
	float half_length = car.chassis_size.z*0.5f;
	
	vec3 direction(0,-1,0);
	vec3 axis(-1,0,0);
	
	car.num_wheels = 4;
	car.wheels = new Wheel[4];

	// FRONT-LEFT ------------------------
	car.wheels[0].connection.Set(half_width - 0.3f * wheel_width, connection_height, half_length - wheel_radius);
	car.wheels[0].direction = direction;
	car.wheels[0].axis = axis;
	car.wheels[0].suspensionRestLength = suspensionRestLength;
	car.wheels[0].radius = wheel_radius;
	car.wheels[0].width = wheel_width;
	car.wheels[0].front = true;
	car.wheels[0].drive = true;
	car.wheels[0].brake = false;
	car.wheels[0].steering = true;

	// FRONT-RIGHT ------------------------
	car.wheels[1].connection.Set(-half_width + 0.3f * wheel_width, connection_height, half_length - wheel_radius);
	car.wheels[1].direction = direction;
	car.wheels[1].axis = axis;
	car.wheels[1].suspensionRestLength = suspensionRestLength;
	car.wheels[1].radius = wheel_radius;
	car.wheels[1].width = wheel_width;
	car.wheels[1].front = true;
	car.wheels[1].drive = true;
	car.wheels[1].brake = false;
	car.wheels[1].steering = true;

	// REAR-LEFT ------------------------
	car.wheels[2].connection.Set(half_width - 0.3f * wheel_width, connection_height, -half_length + wheel_radius);
	car.wheels[2].direction = direction;
	car.wheels[2].axis = axis;
	car.wheels[2].suspensionRestLength = suspensionRestLength;
	car.wheels[2].radius = wheel_radius;
	car.wheels[2].width = wheel_width;
	car.wheels[2].front = false;
	car.wheels[2].drive = false;
	car.wheels[2].brake = true;
	car.wheels[2].steering = false;

	// REAR-RIGHT ------------------------
	car.wheels[3].connection.Set(-half_width + 0.3f * wheel_width, connection_height, -half_length + wheel_radius);
	car.wheels[3].direction = direction;
	car.wheels[3].axis = axis;
	car.wheels[3].suspensionRestLength = suspensionRestLength;
	car.wheels[3].radius = wheel_radius;
	car.wheels[3].width = wheel_width;
	car.wheels[3].front = false;
	car.wheels[3].drive = false;
	car.wheels[3].brake = true;
	car.wheels[3].steering = false;

	vehicle = App->physics->AddVehicle(car);
	vehicle->SetPos(0, 0, 0);

	pos = vec3(vehicle->info.chassis_offset.x, vehicle->info.chassis_offset.y + 5, vehicle->info.chassis_offset.z - 10);
	
	return true;
}

// Unload assets
bool ModulePlayer2::CleanUp()
{
	LOG("Unloading player");

	return true;
}

vec3 ModulePlayer2::GetPos()
{
	return pos;
}

// Update: draw background
update_status ModulePlayer2::Update(float dt)
{
	turn = acceleration = brake = 0.0f;

	if(App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
	{
		if (vehicle->GetKmh() < 0)
		{
			brake = BRAKE_POWER;
		}
		else
		{
			acceleration = MAX_ACCELERATION;
		}
	}

	if(App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		if(turn < TURN_DEGREES)
			turn +=  TURN_DEGREES;
	}

	if(App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		if(turn > -TURN_DEGREES)
			turn -= TURN_DEGREES;
	}

	if(App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
	{
		if (vehicle->GetKmh() > 0)
		{
			brake = BRAKE_POWER;
		}
		else
		{
			acceleration = -MAX_ACCELERATION;
		}
	}

	p2List_item<Cube>* item = App->scene_intro->cubes.getFirst();
	btVector3 position = vehicle->vehicle->getChassisWorldTransform().getOrigin();
	vec3 last_block;
	vec3 respawn;

	while (item != NULL)
	{
		last_block.x = item->data.transform.M[12];
		last_block.y = item->data.transform.M[13];
		last_block.z = item->data.transform.M[14];

		bool isonX = (last_block.x - item->data.size.x / 2 < position.getX() && last_block.x + item->data.size.x / 2 > position.getX());
		bool isonZ = (last_block.z - item->data.size.z / 2 < position.getZ() && last_block.z + item->data.size.z / 2 > position.getZ());

		if (isonX && isonZ)
		{
			respawn.x = last_block.x;
			respawn.y = last_block.y + 5;
			respawn.z = last_block.z;

			LOG("RESPAWN 2 : X%f  ,  Y%f  ,  Z%f  ;", respawn.x, respawn.y, respawn.z);

			break;
		}

		item = item->next;
	}

	//LOG("CAR 2 : %f", position.getY());

	if ((position.getY() < 0.5f && !first_load) || App->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN)
	{
		ResetVehicle(respawn);
	}

	vehicle->ApplyEngineForce(acceleration);
	vehicle->Turn(turn);
	vehicle->Brake(brake);

	//ReCalcPos(acceleration);

	char title[80];
	sprintf_s(title, "Complete 3 Laps to WIN");
	App->window->SetTitle(title);

	first_load = false;

	return UPDATE_CONTINUE;
}

void ModulePlayer2::ResetVehicle(vec3 spawn)
{
	float transformReset[16];

	for (int i = 0; i < 16; ++i)
	{
		transformReset[i] = 0;
	}
	transformReset[0] = 1;
	transformReset[5] = 1;
	transformReset[10] = 1;
	vehicle->SetTransform(transformReset);

	vehicle->vehicle->getRigidBody()->setLinearVelocity({ 0,0,0, });
	vehicle->vehicle->getRigidBody()->setAngularVelocity({ 0,0,0 });
	vehicle->SetPos(spawn.x, spawn.y, spawn.z);

}