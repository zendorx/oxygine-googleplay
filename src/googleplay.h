#pragma once

#include "EventDispatcher.h"
#include "Event.h"
#include "json/json.h"
#include <string>
#include <list>

using namespace std;
using namespace oxygine;

namespace googleplay
{
    void init();
    void free();

    void signin(bool tryToResolveError);
    void signout();

    bool isSignedIn();
	bool getTryResolveError();

    void submitResult(int score, const string& leaderBoardID);
    void showLeaderBoard(const string& leaderBoardID);

	void showAchievements();

    string getUserID();
    string getUserName();

    class OnSignInResult : public Event
    {
    public:
        enum { EVENT = eventID('G', 'P', 'S', 'E')};
        OnSignInResult(int errorCode) : Event(EVENT), errorCode(errorCode) {};
        int errorCode = 0;
    };

    class OnGetTokenEvent : public Event
    {
    public:
        enum { EVENT = eventID('G', 'P', 'T', 'G')};
        OnGetTokenEvent(const string& uid, const string& token): Event(EVENT), token(token), uid(uid) {};
        string token;
        string uid;
    };

    spEventDispatcher dispatcher();

    //void doSomething();
	void syncAchievements(const string& jsonAchievs);

    namespace internal
    {
            void onSignInResult(int errorCode);
            void onGetTokenResult(const string& uid, const string& token);
    }
};