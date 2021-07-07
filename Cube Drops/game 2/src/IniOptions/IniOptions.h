#pragma once
#include "../SimpleIni/INIReader.h"
#include "../Game/Game.h"

class Cube_IniReader 
{
public:
	Cube_IniReader();
	void InitFuncts();
	void InitValues();

	bool DiscordPresence;
	bool Music;

	int FpsLimit;

private:
	void InitReader();
	void IniCreator();

	bool ReaderInitilised;
};

