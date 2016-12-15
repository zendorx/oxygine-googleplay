#include "googleplay.h"

#ifdef __ANDROID__
    #include "android/AndroidGooglePlay.h"
#elif __APPLE__
    #include <TargetConditionals.h>
    #include "ios/iosGooglePlay.h"
#else
    #include "sim/GooglePlaySimulator.h"
#endif



#define GOOGLE_EXT_ENABLED 1

namespace google
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
		jniGoogleInit();
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
        jniGoogleFree();
#endif
        _dispatcher->removeAllEventListeners();
        _dispatcher = 0;
        log::messageln("googleplay::free done");
    }


	namespace interstitial
	{
		void load()
		{
#if !GOOGLE_EXT_ENABLED
			return;
#endif

#ifdef __ANDROID__
			jniGoogle_Interstitial_Load();
#endif 
		}

		void show()
		{
#if !GOOGLE_EXT_ENABLED
			return;
#endif

#ifdef __ANDROID__
			jniGoogle_Interstitial_Show();
#endif 
		}

		bool isLoaded()
		{
#if !GOOGLE_EXT_ENABLED
			return;
#endif

#ifdef __ANDROID__
		    return	jniGoogle_Interstitial_isLoaded();
#endif 
			return false;
		}

		bool isLoading()
		{
#if !GOOGLE_EXT_ENABLED
			return;
#endif

#ifdef __ANDROID__
			return	jniGoogle_Interstitial_isLoading();
#endif 
			return false;
		}


	}//Interstitial

















    void signin(bool tryToResolveError)
    {
#if !GOOGLEPLAY_EXT_ENABLED
        return;
#endif

#ifdef __ANDROID__
        jniGooglePlaySignIn(tryToResolveError);
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

    string getUserID()
    {
#if !GOOGLEPLAY_EXT_ENABLED
        return "";
#endif

#ifdef __ANDROID__
        return jniGooglePlayGetUserID();
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
#endif
    }

    bool isSignedIn()
    {
#if !GOOGLEPLAY_EXT_ENABLED
        return false;
#endif

#ifdef __ANDROID__
        return jniIsGooglePlaySignedIn();
#endif

        return false;
    }

	void syncAchievements(const string& jsonAchievs)
	{
#if !GOOGLE_EXT_ENABLED
		return;
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
}

