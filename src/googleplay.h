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

	namespace firebase {

		void setUserProperty(const string& id, const string& value);
		
		void startLogEvent(const string& id);
		void logEventPutString(const string& paramID, const string& paramValue);
		void logEventPutInt(const string& paramID, int paramValue);
		void logEventPutDouble(const string& paramID, const double& paramValue);
		void endLogEvent();


		void invite();
	}

	namespace play 
	{
		class OnConnected : public Event
		{ public:	enum { EVENT = eventID('G', 'I', 'P', 'C') }; OnConnected() : Event(EVENT) {};	};

		class OnConnectionFailed : public Event
		{ public:	enum { EVENT = eventID('G', 'I', 'P', 'F') }; OnConnectionFailed() : Event(EVENT) {}; };

		void connect(bool tryToResolveError);
		bool isConnected();
		bool isConnecting();
		void disconnect();

		string getAccountName();
		string getDisplayName();

		void unlockAchievement(const string& id);

		//{ "CgkI1JqekoMWEAIQAg" : 1 , "CgkI1JqekoMWEAIQCu" : 4, "CgkI1JqekoMWEAIQOx" : 2}
		void syncAchievements(const string& jsonAchievements);

		/* Simulator options*/
		void _debugSetup(const string& accountName, const string& displayName);
	}

	namespace auth
	{
		class OnDataReceived : public Event
		{
			public:	
				enum { EVENT = eventID('G', 'A', 'D', 'R') }; OnDataReceived() : Event(EVENT) {};

				string uid;
				string token;
		};

		class OnRequestFailed : public Event
		{ public:	enum { EVENT = eventID('G', 'A', 'D', 'F') }; OnRequestFailed() : Event(EVENT) {};	};

		void requestData();

		/* Simulator options*/
		void _debugSetup(const string& uid, const string& token);
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