#include "CubeDiscord.h"
#include <cstdarg>

void SetUpDiscord() 
{
	DiscordEventHandlers handler;
	memset(&handler, 0, sizeof(handler));
	Discord_Initialize("853443253071904789", &handler, 1, NULL);
	std::cout << "Discord Presence initialised\n";
}


void UpdateDiscord()
{
	DiscordRichPresence presence;
	memset(&presence, 0, sizeof(presence));
	presence.largeImageKey = "large";
	presence.smallImageKey = "small";

	presence.state = "Cube Drops By Rimuru";
	presence.startTimestamp = 60;
	Discord_UpdatePresence(&presence);

}