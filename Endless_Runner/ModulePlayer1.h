#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2Point.h"

struct PhysVehicle3D;

#define MAX_ACCELERATION 1000.0f
#define TURN_DEGREES 15.0f * DEGTORAD
#define BRAKE_POWER 1000.0f

class ModulePlayer1 : public Module
{
public:
	ModulePlayer1(Application* app, bool start_enabled = true);
	virtual ~ModulePlayer1();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	vec3 GetPos();
	void ReCalcPos(float move);

public:

	PhysVehicle3D* vehicle;
	vec3		   pos;
	float		   turn;
	float		   acceleration;
	float		   brake;
};