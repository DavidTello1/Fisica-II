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

	void ResetVehicle(btVector3 spawn, btQuaternion rotation);
	bool CleanUp();

public:

	PhysVehicle3D*	vehicle;
	float			turn;
	float			acceleration;
	float			brake;

	btVector3		respawn_pos;
	btQuaternion	respawn_rot;
<<<<<<< HEAD

	btVector3		initial_pos;
	btQuaternion	initial_rot;
=======
>>>>>>> 7d10ce2cf27dfe608a7f547c5a0c39c11569812b

	bool			first_load;
	bool			first;
	bool			win;
	bool			half_lap;
	uint			laps;
};