#pragma once
#include <iostream>
#include <string>
#include <Windows.h>
class MusicHandler
{
public:
	MusicHandler();
	~MusicHandler();

	static void play(std::string command, bool repeat);
	static void pause(std::string command);
	static void setVolume(std::string, int volume);
};

