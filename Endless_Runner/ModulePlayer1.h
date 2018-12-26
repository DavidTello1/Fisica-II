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

	void ResetVehicle(btVector3 spawn);
	bool CleanUp();

public:

	PhysVehicle3D*	vehicle;
	float			turn;
	float			acceleration;
	float			brake;

	btVector3		respawn_pos;

	bool			first_load;
	bool			first;
	bool			win;
	bool			half_lap;
	uint			laps;
};