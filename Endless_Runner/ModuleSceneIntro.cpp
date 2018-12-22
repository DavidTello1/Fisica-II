#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "PhysBody3D.h"

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
	{ 25, 1, 100, 0, 0.5, 50, White, 0, true, -20, { 1, 0, 0 } },
	{ 25, 3, 100, 0, 1, 30, White },
	{ 25, 1, 40, 0, 17.6, 116.18, White },
	{ 25, 1, 55, 15.8, 17.55, 147.5, White, 0, true, 45, { 0, 1, 0 } },
	{ 25, 1, 90, 71.6, 17.6, 163.4, White, 0, true, 90,{ 0, 1, 0 } },
	{ 25, 1, 90, 108, 17.6, 131, White },
	{ 25, 1, 40, 108, 17.6, 100, White, 0, true, 20, { 1, 0, 0 }},
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
	Plane p(0, 1, 0, 0);
	p.axis = true;
	p.color = Red;
	p.Render();

	p2List_item<Cube>* item = cubes.getFirst();
	while (item)
	{
		item->data.Render();
		item = item->next;
	}

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{}

