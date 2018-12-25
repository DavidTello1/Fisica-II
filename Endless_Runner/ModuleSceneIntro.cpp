#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "PhysBody3D.h"
#include "PhysVehicle3D.h"

#define SIZE_ARRAY(_A_) (sizeof(_A_)/sizeof(_A_[0]))

// Circuit Def
struct CubeDef
{
	float size_x, size_y, size_z;
	float pos_x, pos_y, pos_z;
	Color _color;
	float _mass = 0;
	bool _rotate = false;
	float _angle = 0;
	const vec3 _axis = { 0, 0, 0 };
	bool add_collision_listener = false;
};

CubeDef cube_defs[] =
{
	{ 12, 1, 100, 0, 0.5, 50, White, 0, true, -20,{ 1, 0, 0 } },
	{ 25, 3, 100, 0, 1, 30, White },
	{ 25, 1, 40, 0, 17.6, 116.18, White },
	{ 25, 1, 55, 15.8, 17.55, 147.5, White, 0, true, 45,{ 0, 1, 0 } },
	{ 25, 1, 90, 71.6, 17.6, 163.4, White, 0, true, 90,{ 0, 1, 0 } },
	{ 25, 1, 90, 108, 17.6, 131, White },
	{ 25, 1, 40, 108, 17.6, 100, White, 0, true, 20,{ 1, 0, 0 } },
	{ 25, 1, 90, 108, 5.5, 0, White },
	{ 120, 3, 25, 47, 1, -32, White }
};

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{

}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	for (int i = 0; i < SIZE_ARRAY(cube_defs); i++)
	{
		Cube c;
		c.size.Set(cube_defs[i].size_x, cube_defs[i].size_y, cube_defs[i].size_z);
		c.SetPos(cube_defs[i].pos_x, cube_defs[i].pos_y, cube_defs[i].pos_z);
		c.color = cube_defs[i]._color;
		if (cube_defs[i]._rotate)
			c.SetRotation(cube_defs[i]._angle, cube_defs[i]._axis);
		PhysBody3D *p = App->physics->AddBody(c, cube_defs[i]._mass);
		if (cube_defs[i].add_collision_listener)
			p->collision_listeners.add(this);
		cubes.add(c);
	}

	CreateLapSensor(0, 0, -10, 30, 20, 0.1f, LapSensor);
	CreateLapSensor(110, 20, 100, 30, 20, 0.1f, HalfLap);

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

// Update
update_status ModuleSceneIntro::Update(float dt)
{
	if (App->player1->win == true)
	{
		char title[80];
		sprintf_s(title, "PLAYER 1 WINS    *Press Space to Start a New Game*");
		App->window->SetTitle(title);
	}
	else if (App->player2->win == true)
	{
		char title[80];
		sprintf_s(title, "PLAYER 2 WINS    *Press Space to Start a New Game*");
		App->window->SetTitle(title);
	}
	else
	{
		char title[80];
		sprintf_s(title, "Player 1: %d/3 || Player 2: %d/3", App->player1->laps, App->player2->laps);
		App->window->SetTitle(title);
	}

	if (abs(App->player1->vehicle->GetPos().getZ() - App->player2->vehicle->GetPos().getZ()) < 1
		&& abs(App->player1->vehicle->GetPos().getX() - App->player2->vehicle->GetPos().getX()) < 30)
	{
		if (App->player1->vehicle->GetKmh() > App->player2->vehicle->GetKmh()
			&& App->player1->laps == App->player2->laps)
		{
			App->player1->first = true;
			App->player2->first = false;
		}
		else if (App->player2->vehicle->GetKmh() > App->player1->vehicle->GetKmh()
			&& App->player1->laps == App->player2->laps)
		{
			App->player1->first = false;
			App->player2->first = true;
		}
	}
	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{
	if (body1->type == HalfLap)
	{
		if (body2->type == Car1 && App->player1->half_lap == false)
		{
			App->player1->half_lap = true;
		}
		else if (body2->type == Car2 && App->player2->half_lap == false)
		{
			App->player2->half_lap = true;
		}
	}

	else if (body1->type == LapSensor)
	{
		if (body2->type == Car1)
		{
			if (App->player1->laps < 3 && App->player1->half_lap == true)
			{
				App->player1->laps++;
				App->player1->half_lap = false;
			}
			else if (App->player1->laps == 3 && App->player1->half_lap == true)
			{
				App->player1->win = true;
			}
		}
		else if (body2->type == Car2)
		{
			if (App->player2->laps < 3 && App->player2->half_lap == true)
			{
				App->player2->laps++;
				App->player2->half_lap = false;
			}
			else if (App->player2->laps == 3 && App->player2->half_lap == true)
			{
				App->player2->win = true;
			}
		}
	}
}

void ModuleSceneIntro::CreateLapSensor(float x, float y, float z, float i, float j, float k, SensorType type)
{
	Cube ret(i,j,k);
	ret.SetPos(x, y, z);

	PhysBody3D* pbody = App->physics->AddBody(ret, 0, type);
	pbody->SetSensor();
	pbody->collision_listeners.add(this);
}
