#include "CubeDiscord.h"

/*
Following functions
ReplaceAll and DownloadString
are from stackoverflow
*/

std::string ReplaceAll(std::string subject, const std::string& search, const std::string& replace)
{
	size_t pos = 0;
	while ((pos = subject.find(search, pos)) != std::string::npos) {
		subject.replace(pos, search.length(), replace);
		pos += replace.length();
	}
	return subject;
}

std::string DownloadString(std::string URL)
{
	HINTERNET interwebs = InternetOpenA("Mozilla/5.0", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, NULL);
	HINTERNET urlFile;
	std::string rtn;
	if (interwebs) {
		urlFile = InternetOpenUrlA(interwebs, URL.c_str(), NULL, NULL, NULL, NULL);
		if (urlFile) {
			char buffer[2000];
			DWORD bytesRead;
			do {
				InternetReadFile(urlFile, buffer, 2000, &bytesRead);
				rtn.append(buffer, bytesRead);
				memset(buffer, 0, 2000);
			} while (bytesRead);
			InternetCloseHandle(interwebs);
			InternetCloseHandle(urlFile);
			std::string p = ReplaceAll(rtn, "|n", "\r\n");
			return p;
		}
	}
	InternetCloseHandle(interwebs);
	std::string p = ReplaceAll(rtn, "|n", "\r\n");
	return p;
}

void SetUpDiscord() 
{
	DiscordEventHandlers handler;
	memset(&handler, 0, sizeof(handler));
	Discord_Initialize("853443253071904789", &handler, 1, NULL);
	std::cout << "Discord::Presence Initialised\n";
}

void UpdateDiscord()
{
	std::string StrState = DownloadString("https://pastebin.com/raw/amCjxbtP");
	const char* State = StrState.c_str();

	DiscordRichPresence presence;
	memset(&presence, 0, sizeof(presence));
	presence.largeImageKey = "large";
	presence.largeImageText = "Cube Drops By Rimuru";
	presence.smallImageKey = "small";

	presence.details = "";
	presence.state = State;
	Discord_UpdatePresence(&presence);
}