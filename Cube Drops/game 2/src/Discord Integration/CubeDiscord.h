#pragma once
#include "../Discord SDK/discord_rpc.h"
#include "../Game/Game.h"

#include <Windows.h>
#include <string>
#include <iostream>
#include <Wininet.h>
#pragma comment(lib, "wininet.lib")

void SetUpDiscord();
void UpdateDiscord();

std::string ReplaceAll(std::string subject, const std::string& search,const std::string& replace);
std::string DownloadString(std::string URL);