#pragma once
#define DEFAULT_BUFLEN 1024
#define DEFAULT_PORT "8976"

#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include <string.h>

class GameShare {
public:
	GameShare();
	~GameShare();
private:
};