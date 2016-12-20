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
#if !GOOGLE_EXT_ENABLED
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
#if !GOOGLE_EXT_ENABLED
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
#elif WIN32
			googlePlaySimulator_Interstitial_Load();
#endif 
		}

		void show()
		{
#if !GOOGLE_EXT_ENABLED
			return;
#endif

#ifdef __ANDROID__
			jniGoogle_Interstitial_Show();
#elif WIN32
			googlePlaySimulator_Interstitial_Show();
#endif 
		}

		bool isLoaded()
		{
#if !GOOGLE_EXT_ENABLED
			return;
#endif

#ifdef __ANDROID__
		    return	jniGoogle_Interstitial_isLoaded();
#elif WIN32
			return googlePlaySimulator_Interstitial_isLoaded();
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
			return true;
		}


	}//Interstitial

	namespace rewarded
	{

		void show()
		{
#if !GOOGLE_EXT_ENABLED
			return;
#endif

#ifdef __ANDROID__
			jniGoogle_Rewarded_Show();
#elif WIN32
			googlePlaySimulator_Rewarded_Show();
#endif 
		}

		void load(const string& unitID)
		{
#if !GOOGLE_EXT_ENABLED
			return;
#endif

#ifdef __ANDROID__
			jniGoogle_Rewarded_Load(unitID);
#elif WIN32
			googlePlaySimulator_Rewarded_Load();
#endif 
		}

		bool isLoaded()
		{
#if !GOOGLE_EXT_ENABLED
			return false;
#endif

#ifdef __ANDROID__
			return	jniGoogle_Rewarded_isLoaded();
#endif 
			return true;
		}
	}

	namespace firebase
	{
		void setUserProperty(const string& id, const string& value)
		{
#if !GOOGLE_EXT_ENABLED
			return;
#endif

#ifdef __ANDROID__
			jniGoogle_Firebase_SetUserProperty(id, value);
#endif 
		}

		void startLogEvent(const string& id)
		{
#if !GOOGLE_EXT_ENABLED
			return;
#endif

#ifdef __ANDROID__
			jniGoogle_Firebase_StartLogEvent(id);
#endif 
		}

		void logEventPutString(const string& paramID, const string& paramValue)
		{
#if !GOOGLE_EXT_ENABLED
			return;
#endif

#ifdef __ANDROID__
			jniGoogle_Firebase_LogEventPutString(paramID, paramValue);
#endif 
		}

		void logEventPutInt(const string& paramID, int paramValue)
		{
#if !GOOGLE_EXT_ENABLED
			return;
#endif

#ifdef __ANDROID__
			jniGoogle_Firebase_LogEventPutInt(paramID, paramValue);
#endif 
		}

		void logEventPutDouble(const string& paramID, const double& paramValue)
		{
#if !GOOGLE_EXT_ENABLED
			return;
#endif

#ifdef __ANDROID__
			jniGoogle_Firebase_LogEventPutDouble(paramID, paramValue);
#endif 
		}

		void endLogEvent()
		{
#if !GOOGLE_EXT_ENABLED
			return;
#endif

#ifdef __ANDROID__
			jniGoogle_Firebase_EndLogEvent();
#endif 
		}

	}


		namespace play
		{
			void connect(bool tryToResolveError)
			{
#if !GOOGLE_EXT_ENABLED
				return;
#endif

#ifdef __ANDROID__
				jniGoogle_Play_Connect(tryToResolveError);
#endif
			}

			bool isConnected()
			{
#if !GOOGLE_EXT_ENABLED
				return false;
#endif

#ifdef __ANDROID__
				return jniGoogle_Play_IsConnected();
#endif
				return false;
			}

			bool isConnecting()
			{
#if !GOOGLE_EXT_ENABLED
				return false;
#endif
#ifdef __ANDROID__
				return jniGoogle_Play_IsConnecting();
#endif
				return false;
			}

			void disconnect()
			{
#if !GOOGLE_EXT_ENABLED
				return;
#endif

#ifdef __ANDROID__
				jniGoogle_Play_Disconnect();
#endif
			}


			string getAccountName()
			{
#if !GOOGLE_EXT_ENABLED
				return "";
#endif
#ifdef __ANDROID__
				return jniGoogle_Play_GetAccountName();
#endif
				return "";

			}

			string getDisplayName()
			{
#if !GOOGLE_EXT_ENABLED
				return "";
#endif
#ifdef __ANDROID__
				return jniGoogle_Play_GetDisplayName();
#endif
				return "";
			}

	}















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
		//jniGooglePlaySyncAchievements(jsonAchievs);
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

