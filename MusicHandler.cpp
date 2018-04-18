#include "MusicHandler.h"


MusicHandler::MusicHandler()
{
}


MusicHandler::~MusicHandler()
{
}

void MusicHandler::play(std::string command, bool repeat) {
	command = "play " + command + (repeat ? " repeat":"");
	std::wstring stemp = std::wstring(command.begin(), command.end());
	LPCWSTR sw = stemp.c_str();
	mciSendString(sw, NULL, 0, NULL);
}
