#pragma once

#include <Windows.h>
#include <string>
#include <iostream>

using namespace std;

class SystemInfo
{

public:
    SystemInfo();
    virtual ~SystemInfo();

    string GetWinVerStr();
};
