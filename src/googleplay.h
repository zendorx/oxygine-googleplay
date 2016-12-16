#pragma once

#include "EventDispatcher.h"
#include "Event.h"
#include "json/json.h"
#include <string>
#include <list>

using namespace std;
using namespace oxygine;

namespace google
{
	void init();
	void free();

	spEventDispatcher dispatcher();

	namespace interstitial
	{
		class OnAdLoaded : public Event
		{ public:	enum { EVENT = eventID('G', 'I', 'A', 'L') }; OnAdLoaded() : Event(EVENT) {}; };
		
		class OnAdFailedToLoad : public Event
		{ public:	enum { EVENT = eventID('G', 'I', 'A', 'À') }; OnAdFailedToLoad() : Event(EVENT) {}; };

		class OnAdClosed : public Event
		{ public:	enum { EVENT = eventID('G', 'I', 'A', 'C') }; OnAdClosed() : Event(EVENT) {};	};

		void load();
		void show();
		bool isLoaded();
		bool isLoading();
	}

	namespace rewarded
	{
		class OnRewardVideoLoaded : public Event
		{ public:	enum { EVENT = eventID('G', 'I', 'R', 'L') }; OnRewardVideoLoaded() : Event(EVENT) {};	};
		
		class OnRewardedVideoAdClosed : public Event
		{ public:	enum { EVENT = eventID('G', 'I', 'R', 'C') }; OnRewardedVideoAdClosed() : Event(EVENT) {};	};

		class OnRewarded : public Event
		{ public:	enum { EVENT = eventID('G', 'I', 'R', 'R') }; OnRewarded() : Event(EVENT) {}; };

		class OnRewardedVideoAdFailedToLoad : public Event
		{ public:	enum { EVENT = eventID('G', 'I', 'R', 'F') }; OnRewardedVideoAdFailedToLoad() : Event(EVENT) {}; };

		void show();
		void load(const string& unitID = "");
		bool isLoaded();
	}

    /*void init();
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
    }*/
};