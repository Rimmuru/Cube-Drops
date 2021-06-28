#include "CubeDiscord.h"
#include <cstdarg>

void SetUpDiscord() 
{
	DiscordEventHandlers handler;
	memset(&handler, 0, sizeof(handler));
	Discord_Initialize("853443253071904789", &handler, 1, NULL);
	std::cout << "Discord::Presence Initialised\n";
}

void UpdateDiscord()
{
	DiscordRichPresence presence;
	memset(&presence, 0, sizeof(presence));
	presence.largeImageKey = "large";
	presence.largeImageText = "Cube Drops By Rimuru";
	presence.smallImageKey = "small";

	presence.state = "Cube Drops";
	presence.details = "Now with particles!";
	Discord_UpdatePresence(&presence);
}