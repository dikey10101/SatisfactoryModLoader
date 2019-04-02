#include <stdafx.h>
#include <string>
#include <event/Event.h>
#include <event/game/PlayerEvents.h>
#include "BaseMod.h"
#include "ModInfo.h"

// example events
/*

	[Tips]
	Pointers passed from the original function to the mod loader are not
		able to be used in a mod. They are fake pointers, so you need to
		send it back to the mod loader when you call a function from there

	Here is an example of getting a value from the args:
	float value							  : float f = *(float*)args[0];
	float pointer						  : float* fPtr = (float*)args[0];

	[Useful functions]
	get_mod_loader_function(functionName) - runs a modloader event by name
	dispatcher.subscribe(type, function)  - subscribes a function to an event
	GetAsyncKeyState(key)				  - checks a key press
	mod_log(modName, msg)				  - logs a message to the console

*/

void* player;

void killing_player() {
	if (player != NULL) {
		mod_log("BaseMod", "Attempting to kill the player");
		auto pointer = (void(WINAPI*)(void*))get_mod_loader_function("player_suicide");
		pointer(player);
	}
}

bool check_g_key(std::vector<void*>& args) {
	if (GetAsyncKeyState('G')) {
		mod_log("BaseMod", "G key pressed");
		killing_player();
	}
	return true;
}

bool getting_player(std::vector<void*>& args) {
	mod_log("BaseMod", "Got player");
	player = args[0];
	return true;
}

GLOBAL void setup() { 
	dispatcher.subscribe(EventType::InputKey, check_g_key);
	dispatcher.subscribe(EventType::PlayerControllerBeginPlay, getting_player);
}

GLOBAL bool run(EventType type, std::vector<void*>& args) {
	return dispatcher.post(type, args);
}