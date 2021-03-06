#pragma once

#include <string>

using namespace std;

void jniGooglePlayInit();
void jniGooglePlayFree();

void jniGooglePlayRequestToken();
void jniGooglePlaySignIn(bool tryToResolveError);
bool jniIsGooglePlaySignedIn();
void jniGooglePlaySignOut();

string jniGooglePlayGetUserName();
string jniGooglePlayGetUserID();
bool jniGooglePlayGetTryResolveError();

void jniGooglePlayShowAchievements();

void jniGooglePlaySyncAchievements(const string& jsonAchievements);

void jniGooglePlaySubmitResult(int score, const string& leaderBoardID);
void jniGooglePlayShowLeaderboard(const string& leaderBoardID);