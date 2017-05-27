#include "googleplay.h"

#ifdef __ANDROID__
    #include "android/AndroidGooglePlay.h"
#elif __APPLE__
    #include <TargetConditionals.h>
    #include "ios/iosGooglePlay.h"
#else
    #include "sim/GooglePlaySimulator.h"
#endif



#define GOOGLEPLAY_EXT_ENABLED 1

namespace googleplay
{

    spEventDispatcher _dispatcher;

    spEventDispatcher dispatcher()
    {
        return _dispatcher;
    }

    void init()
    {
#if !GOOGLEPLAY_EXT_ENABLED
        return;
#endif

        log::messageln("googleplay::init");

        OX_ASSERT(_dispatcher == 0);
        _dispatcher = new EventDispatcher;

#ifdef __ANDROID__
        jniGooglePlayInit();
#elif TARGET_OS_IPHONE

#else
        googleplaySimulatorInit();
#endif
        log::messageln("googleplay::init done");
    }

    void free()
    {
#if !GOOGLEPLAY_EXT_ENABLED
        return;
#endif

        log::messageln("googleplay::free");

        OX_ASSERT(_dispatcher);

#ifdef __ANDROID__
        jniGooglePlayFree();
#endif
        _dispatcher->removeAllEventListeners();
        _dispatcher = 0;
        log::messageln("googleplay::free done");
    }

    void requestToken()
    {
#if !GOOGLEPLAY_EXT_ENABLED
        return;
#endif

#ifdef __ANDROID__
        jniGooglePlayRequestToken();
#elif OX_DEBUG
		googleplaySimulator_RequestToken();
#endif
    }


    void signin(bool tryToResolveError)
    {
#if !GOOGLEPLAY_EXT_ENABLED
        return;
#endif

#ifdef __ANDROID__
        jniGooglePlaySignIn(tryToResolveError);
#elif OX_DEBUG
		googleplaySimulator_SignIn();
#endif
    }


    void submitResult(int score, const string& leaderBoardID)
    {
#if !GOOGLEPLAY_EXT_ENABLED
        return;
#endif

#ifdef __ANDROID__
        jniGooglePlaySubmitResult(score, leaderBoardID);
#endif        
    }

    string getUserName()
    {
#if !GOOGLEPLAY_EXT_ENABLED
        return "";
#endif

#ifdef __ANDROID__
        return jniGooglePlayGetUserName();
#elif OX_DEBUG
		return googleplaySimulator_getUserName();
#endif

        return "";
    }

    string getUserID()
    {
#if !GOOGLEPLAY_EXT_ENABLED
        return "";
#endif

#ifdef __ANDROID__
        return jniGooglePlayGetUserID();
#elif OX_DEBUG
		return googleplaySimulator_getUserID();
#endif 

        return "";        
    }

    void showLeaderBoard(const string& leaderBoardID)
    {
#if !GOOGLEPLAY_EXT_ENABLED
        return;
#endif

#ifdef __ANDROID__
        jniGooglePlayShowLeaderboard(leaderBoardID);
#endif   
    }

	bool getTryResolveError()
	{
#if !GOOGLEPLAY_EXT_ENABLED
		return false;
#endif

#ifdef __ANDROID__
		return jniGooglePlayGetTryResolveError();
#endif
		return false;
	}

	void showAchievements()
	{
#if !GOOGLEPLAY_EXT_ENABLED
		return;
#endif

#ifdef __ANDROID__
		jniGooglePlayShowAchievements();
#endif

	}


    void signout()
    {
#if !GOOGLEPLAY_EXT_ENABLED
        return;
#endif

#ifdef __ANDROID__
        jniGooglePlaySignOut();
#elif OX_DEBUG
		googleplaySimulator_SignOut();
#endif
    }

    bool isSignedIn()
    {
#if !GOOGLEPLAY_EXT_ENABLED
        return false;
#endif

#ifdef __ANDROID__
        return jniIsGooglePlaySignedIn();
#elif OX_DEBUG
		return googleplaySimulator_isSignedIn();
#endif

        return false;
    }

	void syncAchievements(const string& jsonAchievs)
	{
#if !GOOGLEPLAY_EXT_ENABLED
		return false;
#endif

#ifdef __ANDROID__
		jniGooglePlaySyncAchievements(jsonAchievs);
#endif
	}

    /*void doSomething()
    {
#ifdef __ANDROID__
        jniDoSomethingInJava();
#elif TARGET_OS_IPHONE
        jniDoSomethingInObjectiveC();
#else
        simulatorFunction();
#endif
        
    }*/


    namespace internal
    {
        void onSignInResult(int errorCode)
        {
            if (!_dispatcher)
                return;

            OnSignInResult ev(errorCode);
            _dispatcher->dispatchEvent(&ev);    
        }

        void onGetTokenResult(const string& uid, const string& token)
        {
            if (uid.empty())
                return;

            if (token.empty())
                return;

            if (!_dispatcher)
                return;

            log::messageln("internal onGetToken: %s %s", uid.c_str(), token.c_str());
            OnGetTokenEvent ev(uid, token);
            _dispatcher->dispatchEvent(&ev);
        }
    }
}

