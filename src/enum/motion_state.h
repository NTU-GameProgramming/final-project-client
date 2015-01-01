#pragma once

enum MotionState{
	//live status
	IDLE = 0,
	DEAD = 1,
	
	//attack related status
	ATTACK = 2,
	DAMAGED = 3,
	COOL_DOWN = 4,

	//moving  start from 256
	MOVE_FORWARD = 1<<8,
	MOVE_BACKWARD = MOVE_FORWARD<<1,
	MOVE_LEFT = MOVE_BACKWARD<<2,
	MOVE_RIGHT = MOVE_LEFT<<3,
	MOVE_LEFT_FORWARD = MOVE_LEFT + MOVE_FORWARD,
	MOVE_LEFT_BACKWARD = MOVE_LEFT+ MOVE_BACKWARD,
	MOVE_RIGHT_FORWARD = MOVE_RIGHT + MOVE_FORWARD,
	MOVE_RIGHT_BACKWARD = MOVE_RIGHT + MOVE_BACKWARD
};