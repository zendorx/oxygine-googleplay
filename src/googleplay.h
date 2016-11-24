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

    class OnSignInResult : public Event
    {
    public:
        enum { EVENT = eventID('G', 'P', 'S', 'E')};
        OnSignInResult(int errorCode) : Event(EVENT), errorCode(errorCode) {};
        int errorCode = 0;
    };

    spEventDispatcher dispatcher();

    //void doSomething();
	void syncAchievements(const string& jsonAchievs);

    namespace internal
    {
            void onSignInResult(int errorCode);
    }
};