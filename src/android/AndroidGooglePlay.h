#pragma once

#include <string>

using namespace std;

void jniGoogleInit();
void jniGoogleFree();

/*void jniGooglePlaySignIn(bool tryToResolveError);
bool jniIsGooglePlaySignedIn();
void jniGooglePlaySignOut();

string jniGooglePlayGetUserID();
bool jniGooglePlayGetTryResolveError();

void jniGooglePlayShowAchievements();

void jniGooglePlaySyncAchievements(const string& jsonAchievements);

void jniGooglePlaySubmitResult(int score, const string& leaderBoardID);
void jniGooglePlayShowLeaderboard(const string& leaderBoardID);*/


void jniGoogle_Interstitial_Show();
void jniGoogle_Interstitial_Load();
bool jniGoogle_Interstitial_isLoaded();
bool jniGoogle_Interstitial_isLoading();

void jniGoogle_Rewarded_Show();
void jniGoogle_Rewarded_Load(const string& unitID);
bool jniGoogle_Rewarded_isLoaded();

void jniGoogle_Firebase_SetUserProperty(const string& id, const string& value);
void jniGoogle_Firebase_StartLogEvent(const string& id);
void jniGoogle_Firebase_LogEventPutString(const string& id, const string& value);
void jniGoogle_Firebase_LogEventPutInt(const string& id, int value);
void jniGoogle_Firebase_LogEventPutDouble(const string& id, const double& value);
void jniGoogle_Firebase_EndLogEvent();


void jniGoogle_Play_Connect(bool tryToResolveError);
bool jniGoogle_Play_IsConnected();
bool jniGoogle_Play_IsConnecting();
void jniGoogle_Play_Disconnect();
string jniGoogle_Play_GetAccountName();
string jniGoogle_Play_GetDisplayName();
void jniGoogle_Play_UnlockAchievement(const string& id);
