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
	int error = mciSendString(sw, NULL, 0, NULL);
	//std::cout << error << std::endl;
}

void MusicHandler::setVolume(std::string command, int volume) {
	command = "setaudio " + command + " volume to " + std::to_string(volume);
	std::wstring stemp = std::wstring(command.begin(), command.end());
	LPCWSTR sw = stemp.c_str();
	int error = mciSendString(sw, NULL, 0, NULL);
	//std::cout << error << std::endl;
}

void MusicHandler::pause(std::string command) {
	command = "pause " + command;
	std::wstring stemp = std::wstring(command.begin(), command.end());
	LPCWSTR sw = stemp.c_str();
	int error = mciSendString(sw, NULL, 0, NULL);
	//std::cout << error << std::endl;
}
