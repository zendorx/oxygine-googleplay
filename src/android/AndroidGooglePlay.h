#pragma once

#include <string>

using namespace std;

void jniGoogleInit();
void jniGoogleFree();

void jniGooglePlaySignIn(bool tryToResolveError);
bool jniIsGooglePlaySignedIn();
void jniGooglePlaySignOut();

string jniGooglePlayGetUserID();
bool jniGooglePlayGetTryResolveError();

void jniGooglePlayShowAchievements();

void jniGooglePlaySyncAchievements(const string& jsonAchievements);

void jniGooglePlaySubmitResult(int score, const string& leaderBoardID);
void jniGooglePlayShowLeaderboard(const string& leaderBoardID);


void jniGoogle_Interstitial_Show();
void jniGoogle_Interstitial_Load();
bool jniGoogle_Interstitial_isLoaded();
bool jniGoogle_Interstitial_isLoading();

void jniGoogle_Rewarded_Show();
void jniGoogle_Rewarded_Load(const string& unitID);
bool jniGoogle_Rewarded_isLoaded();