#pragma once
#include <string>

using namespace std;

void googleplaySimulatorInit();


void googleplaySimulator_SignIn();
bool googleplaySimulator_isSignedIn();
void googleplaySimulator_SignOut();
void googleplaySimulator_RequestToken();

string googleplaySimulator_getUserID();
string googleplaySimulator_getUserName();