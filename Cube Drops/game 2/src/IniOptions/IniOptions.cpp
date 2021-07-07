#include "IniOptions.h"
#include "../Game/Game.h"

namespace fs = std::filesystem;

Cube_IniReader::Cube_IniReader()
{
	FpsLimit = 0;
}

void Cube_IniReader::InitReader()
{

	INIReader CubeDropsIni("CubeDrops.ini");

	if (CubeDropsIni.ParseError() < 0)
		std::cout << "Couldnt load CubeDrops.ini\n";
	else
	{
		ReaderInitilised = true;
		std::cout << "Initilized::IniReader\n";
	}

	DiscordPresence = CubeDropsIni.GetBoolean("Discord", "status", true);
	DiscordPresence = CubeDropsIni.GetBoolean("Discord", "status", true);
	FpsLimit = CubeDropsIni.GetInteger("Variables", "fps", 60);
}

void Cube_IniReader::IniCreator()
{
	std::ofstream OpenFile;
	char* appdata = getenv("APPDATA");
	std::string strChar; //= game.appdata;

	strChar.append("\CubeDrops");
	//std::cout << strChar << std::endl;
	//OpenFile.open("test.ini");
	//OpenFile << "Writing this to a file.\n";
	//OpenFile.close();

	//fs::path filePath(strChar);
	//std::error_code ec;
	//if (!fs::exists(filePath, ec) && !ec)
	//{
	//	// Save to file, e.g. with std::ofstream file(filePath);
	//}

}

void Cube_IniReader::InitValues()
{
	Music = true;
	DiscordPresence = true;
}

void Cube_IniReader::InitFuncts()
{
	InitReader();
	IniCreator();
}

//Save game is here because i want saving to have file access
void Game::SaveGame()
{
	//Sets the current path to appdata
	fs::current_path(appdata);

	//If cube drops folder directory doesnt exist create it
	if (!fs::exists("CubeDrops"))
		fs::create_directory("CubeDrops");


}