#include <jni.h>
#include <android/log.h>
#include <assert.h>
#include "core/oxygine.h"
#include "core/Object.h"
#include "core/ThreadMessages.h"
#include "core/oxygine.h"
#include "core/android/jniHelper.h"
#include "core/android/jniUtils.h"
#include "json/json.h"
#include "src/googleplay.h"


using namespace oxygine;

//jclass _jGoogleClass = 0;
//jobject _jGoogleObject = 0;

jclass _jGoogleInterstitialClass = 0;
jobject _jGoogleInterstitialObject = 0;

jclass _jGoogleRewardedClass = 0;
jobject _jGoogleRewardedObject = 0;

jclass _jGoogleFirebaseClass = 0;
jobject _jGoogleFirebaseObject = 0;


jclass _jGooglePlayClass = 0;
jobject _jGooglePlayObject = 0;


bool isGooglePlayEnabled()
{
	return _jGooglePlayObject && _jGooglePlayClass;
}

bool isGoogleFirebaseEnabled()
{
	return _jGoogleFirebaseClass && _jGoogleFirebaseObject;
}

bool isGoogleInterstitialEnabled()
{
	return _jGoogleInterstitialClass && _jGoogleInterstitialObject;
}

bool isGoogleRewardedEnabled()
{
	return _jGoogleRewardedClass && _jGoogleRewardedObject;
}

/*bool isGoogleEnabled()
{
    return _jGoogleClass && _jGoogleObject;
}*/


extern "C"
{
	/* Interstitial */
	JNIEXPORT void JNICALL Java_org_oxygine_googleplay_InterstitialAdapter_nativeOnAdLoaded(JNIEnv* env, jobject obj)
	{
		core::getMainThreadDispatcher().postCallback([=]()
		{
			OX_ASSERT(google::dispatcher());
			if (google::dispatcher())
			{
				google::interstitial::OnAdLoaded ev;
				google::dispatcher()->dispatchEvent(&ev);
			}			
		});
	}

	JNIEXPORT void JNICALL Java_org_oxygine_googleplay_InterstitialAdapter_nativeonAdFailedToLoad(JNIEnv* env, jobject obj)
	{
		core::getMainThreadDispatcher().postCallback([=]()
		{
			OX_ASSERT(google::dispatcher());
			if (google::dispatcher())
			{
				google::interstitial::OnAdFailedToLoad ev;
				google::dispatcher()->dispatchEvent(&ev);
			}			
		});
	}

	JNIEXPORT void JNICALL Java_org_oxygine_googleplay_InterstitialAdapter_nativeonOnAdClosed(JNIEnv* env, jobject obj)
	{
		core::getMainThreadDispatcher().postCallback([=]()
		{
			OX_ASSERT(google::dispatcher());
			if (google::dispatcher())
			{
				google::interstitial::OnAdClosed ev;
				google::dispatcher()->dispatchEvent(&ev);
			}			
		});
	}

	/* Rewarded */
	JNIEXPORT void JNICALL Java_org_oxygine_googleplay_RewardedAdapter_nativeOnRewardVideoLoaded(JNIEnv* env, jobject obj)
	{
		core::getMainThreadDispatcher().postCallback([=]()
		{
			OX_ASSERT(google::dispatcher());
			if (google::dispatcher())
			{
				google::rewarded::OnRewardVideoLoaded ev;
				google::dispatcher()->dispatchEvent(&ev);
			}			
		});
	}

	JNIEXPORT void JNICALL Java_org_oxygine_googleplay_RewardedAdapter_nativeOnRewardedVideoAdClosed(JNIEnv* env, jobject obj)
	{
		core::getMainThreadDispatcher().postCallback([=]()
		{
			OX_ASSERT(google::dispatcher());
			if (google::dispatcher())
			{
				google::rewarded::OnRewardedVideoAdClosed ev;
				google::dispatcher()->dispatchEvent(&ev);
			}			
		});
	}

	JNIEXPORT void JNICALL Java_org_oxygine_googleplay_RewardedAdapter_nativeOnRewarded(JNIEnv* env, jobject obj)
	{
		core::getMainThreadDispatcher().postCallback([=]()
		{
			OX_ASSERT(google::dispatcher());
			if (google::dispatcher())
			{
				google::rewarded::OnRewarded ev;
				google::dispatcher()->dispatchEvent(&ev);
			}			
		});
	}

	JNIEXPORT void JNICALL Java_org_oxygine_googleplay_RewardedAdapter_nativeOnRewardedVideoAdFailedToLoad(JNIEnv* env, jobject obj)
	{
		core::getMainThreadDispatcher().postCallback([=]()
		{
			OX_ASSERT(google::dispatcher());
			if (google::dispatcher())
			{
				google::rewarded::OnRewardedVideoAdFailedToLoad ev;
				google::dispatcher()->dispatchEvent(&ev);
			}			
		});
	}




	JNIEXPORT void JNICALL Java_org_oxygine_googleplay_GooglePlayAdapter_nativeOnConnected(JNIEnv* env, jobject obj)
	{
		core::getMainThreadDispatcher().postCallback([=]()
		{
			OX_ASSERT(google::dispatcher());
			if (google::dispatcher())
			{
				google::play::OnConnected ev;
				google::dispatcher()->dispatchEvent(&ev);
			}
		});
	}

	JNIEXPORT void JNICALL Java_org_oxygine_googleplay_GooglePlayAdapter_nativeOnConnectionFailed(JNIEnv* env, jobject obj)
	{
		core::getMainThreadDispatcher().postCallback([=]()
		{
			OX_ASSERT(google::dispatcher());
			if (google::dispatcher())
			{
				google::play::OnConnectionFailed ev;
				google::dispatcher()->dispatchEvent(&ev);
			}
		});
	}


   /* JNIEXPORT void JNICALL Java_org_oxygine_googleplay_GooglePlayAdapter_nativeOnGetToken(JNIEnv* env, jobject obj,jstring uid, jstring token)
    {
        string token_c = jniGetString(env, token);
        string uid_c = jniGetString(env, uid);
        
        core::getMainThreadDispatcher().postCallback([ = ]()
        {
            log::messageln("SDL nativeOnGetToken %s", token_c.c_str());
            googleplay::internal::onGetTokenResult(uid_c, token_c);
        });
    }

    JNIEXPORT void JNICALL Java_org_oxygine_googleplay_GooglePlayAdapter_nativeOnSignInResult(JNIEnv* env, jobject obj, int error)
    {
        core::getMainThreadDispatcher().postCallback([ = ]()
        {
            googleplay::internal::onSignInResult(error);
        });
    }*/


}


void jniGoogleInit()
{
    try
    {
        JNIEnv* env = jniGetEnv();
        LOCAL_REF_HOLDER(env);
        JNI_NOT_NULL(env);
		
		_jGoogleInterstitialClass = (jclass)env->NewGlobalRef( env->FindClass("org/oxygine/googleplay/InterstitialAdapter") );
		JNI_NOT_NULL(_jGoogleInterstitialClass);

		_jGoogleInterstitialObject = env->NewGlobalRef(jniFindExtension(env, _jGoogleInterstitialClass));
		JNI_NOT_NULL(_jGoogleInterstitialObject);



		_jGoogleRewardedClass = (jclass)env->NewGlobalRef(env->FindClass("org/oxygine/googleplay/RewardedAdapter"));
		JNI_NOT_NULL(_jGoogleRewardedClass);

		_jGoogleRewardedObject = env->NewGlobalRef(jniFindExtension(env, _jGoogleRewardedClass));
		JNI_NOT_NULL(_jGoogleRewardedObject);



		_jGoogleFirebaseClass = (jclass)env->NewGlobalRef(env->FindClass("org/oxygine/googleplay/FirebaseAdapter"));
		JNI_NOT_NULL(_jGoogleFirebaseClass);

		_jGoogleFirebaseObject = env->NewGlobalRef(jniFindExtension(env, _jGoogleFirebaseClass));
		JNI_NOT_NULL(_jGoogleFirebaseObject);


		_jGooglePlayClass = (jclass)env->NewGlobalRef(env->FindClass("org/oxygine/googleplay/GooglePlayAdapter"));
		JNI_NOT_NULL(_jGooglePlayClass);

		_jGooglePlayObject = env->NewGlobalRef(jniFindExtension(env, _jGooglePlayClass));
		JNI_NOT_NULL(_jGooglePlayObject);

        /*_jGoogleClass = env->FindClass("org/oxygine/googleplay/GooglePlayAdapter");
        JNI_NOT_NULL(_jGoogleClass);

        _jGoogleClass = (jclass) env->NewGlobalRef(_jGoogleClass);
        JNI_NOT_NULL(_jGoogleClass);

        _jGoogleObject = env->NewGlobalRef(jniFindExtension(env, _jGoogleClass));
        JNI_NOT_NULL(_jGoogleObject);*/
    }
    catch (const notFound&)
    {
        log::error("jniGooglePlayInit failed, class/member not found");
    }
}

void jniGoogleFree()
{
	try
    {
        JNIEnv* env = jniGetEnv();
        LOCAL_REF_HOLDER(env);


		env->DeleteGlobalRef(_jGoogleInterstitialClass); _jGoogleInterstitialClass = 0;
		env->DeleteGlobalRef(_jGoogleInterstitialObject); _jGoogleInterstitialObject = 0;

		env->DeleteGlobalRef(_jGoogleRewardedClass); _jGoogleRewardedClass = 0;
		env->DeleteGlobalRef(_jGoogleRewardedObject); _jGoogleRewardedObject = 0;

		env->DeleteGlobalRef(_jGoogleFirebaseClass); _jGoogleFirebaseClass = 0;
		env->DeleteGlobalRef(_jGoogleFirebaseObject); _jGoogleFirebaseObject = 0;

		env->DeleteGlobalRef(_jGooglePlayClass); _jGooglePlayClass = 0;
		env->DeleteGlobalRef(_jGooglePlayObject); _jGooglePlayObject = 0;

    }
    catch (const notFound&)
    {

    }
}
/* /// INTERSTITIAL */

void jniGoogle_Interstitial_Show()
{
	if (!isGoogleInterstitialEnabled())
		return;

	log::messageln("jniGoogle_Admob_Show called");

	try
	{
		JNIEnv* env = jniGetEnv();
		LOCAL_REF_HOLDER(env);
		jmethodID jisMethod = env->GetMethodID(_jGoogleInterstitialClass, "show", "()V");
		JNI_NOT_NULL(jisMethod);
		env->CallVoidMethod(_jGoogleInterstitialObject, jisMethod);
	}
	catch (const notFound&)
	{
		log::error("jniGoogle_Admob_Show failed, class/member not found");
	}
}

void jniGoogle_Interstitial_Load()
{
	if (!isGoogleInterstitialEnabled())
		return;

	log::messageln("jniGoogle_Interstitial_Load called");

	try
	{
		JNIEnv* env = jniGetEnv();
		LOCAL_REF_HOLDER(env);
		jmethodID jisMethod = env->GetMethodID(_jGoogleInterstitialClass, "load", "()V");
		JNI_NOT_NULL(jisMethod);
		env->CallVoidMethod(_jGoogleInterstitialObject, jisMethod);
	}
	catch (const notFound&)
	{
		log::error("jniGoogle_Interstitial_Load failed, class/member not found");
	}
}

bool jniGoogle_Interstitial_isLoaded()
{
	if (!isGoogleInterstitialEnabled())
		return false;

	log::messageln("jniGoogle_Interstitial_isLoaded called");

	bool result = false;
	try
	{
		JNIEnv* env = jniGetEnv();
		LOCAL_REF_HOLDER(env);
		jmethodID jisMethod = env->GetMethodID(_jGoogleInterstitialClass, "isLoaded", "()Z");
		JNI_NOT_NULL(jisMethod);
		jboolean jb = env->CallBooleanMethod(_jGoogleInterstitialObject, jisMethod);
		result = (bool)jb;

	}
	catch (const notFound&)
	{
		log::error("jniGoogle_Interstitial_isLoaded failed, class/member not found");
	}

	return result;
}

bool jniGoogle_Interstitial_isLoading()
{
	if (!isGoogleInterstitialEnabled())
		return false;

	log::messageln("jniGoogle_Interstitial_isLoading called");

	bool result = false;
	try
	{
		JNIEnv* env = jniGetEnv();
		LOCAL_REF_HOLDER(env);
		jmethodID jisMethod = env->GetMethodID(_jGoogleInterstitialClass, "isLoading", "()Z");
		JNI_NOT_NULL(jisMethod);
		jboolean jb = env->CallBooleanMethod(_jGoogleInterstitialObject, jisMethod);
		result = (bool)jb;

	}
	catch (const notFound&)
	{
		log::error("jniGoogle_Interstitial_isLoading failed, class/member not found");
	}

	return result;
}

/* INTERSTITIAL ////*/


/* REWARDED */
void jniGoogle_Rewarded_Show()
{
	if (!isGoogleRewardedEnabled())
		return;

	log::messageln("jniGoogle_Rewarded_Show called");

	try
	{
		JNIEnv* env = jniGetEnv();
		LOCAL_REF_HOLDER(env);
		jmethodID jisMethod = env->GetMethodID(_jGoogleRewardedClass, "show", "()V");
		JNI_NOT_NULL(jisMethod);
		env->CallVoidMethod(_jGoogleRewardedObject, jisMethod);
	}
	catch (const notFound&)
	{
		log::error("jniGoogle_Rewarded_Show failed, class/member not found");
	}
}

void jniGoogle_Rewarded_Load(const string& unitID)
{
	if (!isGoogleRewardedEnabled())
		return;

	log::messageln("jniGoogle_Rewarded_Load called");

	try
	{
		JNIEnv* env = jniGetEnv();
		LOCAL_REF_HOLDER(env);
		jstring jid = env->NewStringUTF(unitID.c_str());


		jmethodID jisMethod = env->GetMethodID(_jGoogleRewardedClass, "load", "(Ljava/lang/String;)V");
		JNI_NOT_NULL(jisMethod);
		env->CallVoidMethod(_jGoogleRewardedObject, jisMethod, jid);
	}
	catch (const notFound&)
	{
		log::error("jniGoogle_Rewarded_Load failed, class/member not found");
	}
}

bool jniGoogle_Rewarded_isLoaded()
{
	if (!isGoogleRewardedEnabled())
		return false;

	log::messageln("jniGoogle_Rewarded_isLoaded called");

	bool result = false;
	try
	{
		JNIEnv* env = jniGetEnv();
		LOCAL_REF_HOLDER(env);
		jmethodID jisMethod = env->GetMethodID(_jGoogleRewardedClass, "isLoaded", "()Z");
		JNI_NOT_NULL(jisMethod);
		jboolean jb = env->CallBooleanMethod(_jGoogleRewardedObject, jisMethod);
		result = (bool)jb;

	}
	catch (const notFound&)
	{
		log::error("jniGoogle_Rewarded_isLoaded failed, class/member not found");
	}

	return result;

}

/* REWARDED ***** /


/* FIREBASE */


void jniGoogle_Firebase_SetUserProperty(const string& id, const string& value)
{
	if (!isGoogleFirebaseEnabled())
		return;

	log::messageln("jniGoogle_Firebase_SetUserProperty called");

	try
	{
		JNIEnv* env = jniGetEnv();
		LOCAL_REF_HOLDER(env);
		
		jstring jid = env->NewStringUTF(id.c_str());
		jstring jvalue = env->NewStringUTF(value.c_str());
		jmethodID jisMethod = env->GetMethodID(_jGoogleFirebaseClass, "setUserProperty", "(Ljava/lang/String;Ljava/lang/String;)V");
		JNI_NOT_NULL(jisMethod);
		env->CallVoidMethod(_jGoogleFirebaseObject, jisMethod, jid, jvalue);
	}
	catch (const notFound&)
	{
		log::error("jniGoogle_Firebase_SetUserProperty failed, class/member not found");
	}
}

void jniGoogle_Firebase_StartLogEvent(const string& id)
{
	if (!isGoogleFirebaseEnabled())
		return;

	log::messageln("jniGoogle_Firebase_StartLogEvent called");

	try
	{
		JNIEnv* env = jniGetEnv();
		LOCAL_REF_HOLDER(env);
		
		jstring jid = env->NewStringUTF(id.c_str());

		jmethodID jisMethod = env->GetMethodID(_jGoogleFirebaseClass, "startLogEvent", "(Ljava/lang/String;)V");
		JNI_NOT_NULL(jisMethod);
		env->CallVoidMethod(_jGoogleFirebaseObject, jisMethod, jid);
	}
	catch (const notFound&)
	{
		log::error("jniGoogle_Firebase_StartLogEvent failed, class/member not found");
	}
}

void jniGoogle_Firebase_LogEventPutString(const string& id, const string& value)
{
	if (!isGoogleFirebaseEnabled())
		return;

	log::messageln("jniGoogle_Firebase_LogEventPutString called");

	try
	{
		JNIEnv* env = jniGetEnv();
		LOCAL_REF_HOLDER(env);
		
		jstring jid = env->NewStringUTF(id.c_str());
		jstring jvalue = env->NewStringUTF(value.c_str());


		jmethodID jisMethod = env->GetMethodID(_jGoogleFirebaseClass, "logEventPutString", "(Ljava/lang/String;Ljava/lang/String;)V");
		JNI_NOT_NULL(jisMethod);
		env->CallVoidMethod(_jGoogleFirebaseObject, jisMethod, jid, jvalue);
	}
	catch (const notFound&)
	{
		log::error("jniGoogle_Firebase_LogEventPutString failed, class/member not found");
	}
}

void jniGoogle_Firebase_LogEventPutInt(const string& id, int value)
{
	if (!isGoogleFirebaseEnabled())
		return;

	log::messageln("jniGoogle_Firebase_LogEventPutInt called");

	try
	{
		JNIEnv* env = jniGetEnv();
		LOCAL_REF_HOLDER(env);

		jstring jid = env->NewStringUTF(id.c_str());

		jmethodID jisMethod = env->GetMethodID(_jGoogleFirebaseClass, "logEventPutInt", "(Ljava/lang/String;I)V");
		JNI_NOT_NULL(jisMethod);
		env->CallVoidMethod(_jGoogleFirebaseObject, jisMethod, jid, value);
	}
	catch (const notFound&)
	{
		log::error("jniGoogle_Firebase_LogEventPutInt failed, class/member not found");
	}
}

void jniGoogle_Firebase_LogEventPutDouble(const string& id, const double& value)
{
	if (!isGoogleFirebaseEnabled())
		return;

	log::messageln("jniGoogle_Firebase_LogEventPutDouble called");

	try
	{
		JNIEnv* env = jniGetEnv();
		LOCAL_REF_HOLDER(env);

		jstring jid = env->NewStringUTF(id.c_str());

		jmethodID jisMethod = env->GetMethodID(_jGoogleFirebaseClass, "logEventPutDouble", "(Ljava/lang/String;D)V");
		JNI_NOT_NULL(jisMethod);
		env->CallVoidMethod(_jGoogleFirebaseObject, jisMethod, jid, value);
	}
	catch (const notFound&)
	{
		log::error("jniGoogle_Firebase_LogEventPutDouble failed, class/member not found");
	}
}

void jniGoogle_Firebase_EndLogEvent()
{
	if (!isGoogleFirebaseEnabled())
		return;

	log::messageln("jniGoogle_Firebase_EndLogEvent called");

	try
	{
		JNIEnv* env = jniGetEnv();
		LOCAL_REF_HOLDER(env);

		jmethodID jisMethod = env->GetMethodID(_jGoogleFirebaseClass, "endLogEvent", "()V");
		JNI_NOT_NULL(jisMethod);
		env->CallVoidMethod(_jGoogleFirebaseObject, jisMethod);
	}
	catch (const notFound&)
	{
		log::error("jniGoogle_Firebase_EndLogEvent failed, class/member not found");
	}
}

/* FIREBASE *********/


/* PLAY */

void jniGoogle_Play_Connect(bool tryToResolveError)
{
	if (!isGooglePlayEnabled())
		return;

	log::messageln("jniGoogle_Play_Connect called");

	try
	{
		JNIEnv* env = jniGetEnv();
		LOCAL_REF_HOLDER(env);

		jmethodID jisMethod = env->GetMethodID(_jGooglePlayClass, "connect", "(Z)V");
		JNI_NOT_NULL(jisMethod);
		env->CallVoidMethod(_jGooglePlayObject, jisMethod, tryToResolveError);
	}
	catch (const notFound&)
	{
		log::error("jniGoogle_Play_Connect failed, class/member not found");
	}
}


bool jniGoogle_Play_IsConnected()
{
	if (!isGooglePlayEnabled())
		return false;

	log::messageln("jniGoogle_Play_IsConnected called");
	bool result = false;
	try
	{
		JNIEnv* env = jniGetEnv();
		LOCAL_REF_HOLDER(env);

		jmethodID jisMethod = env->GetMethodID(_jGooglePlayClass, "isConnected", "()Z");
		JNI_NOT_NULL(jisMethod);
		jboolean jb = env->CallBooleanMethod(_jGooglePlayObject, jisMethod);
		result = (bool)jb;
	}
	catch (const notFound&)
	{
		log::error("jniGoogle_Play_IsConnected failed, class/member not found");
	}

	return result;
}


bool jniGoogle_Play_IsConnecting()
{
	if (!isGooglePlayEnabled())
		return false;

	log::messageln("jniGoogle_Play_IsConnecting called");
	bool result = false;
	try
	{
		JNIEnv* env = jniGetEnv();
		LOCAL_REF_HOLDER(env);

		jmethodID jisMethod = env->GetMethodID(_jGooglePlayClass, "isConnecting", "()Z");
		JNI_NOT_NULL(jisMethod);
		jboolean jb = env->CallBooleanMethod(_jGooglePlayObject, jisMethod);
		result = (bool)jb;
	}
	catch (const notFound&)
	{
		log::error("jniGoogle_Play_IsConnecting failed, class/member not found");
	}

	return result;
}


void jniGoogle_Play_Disconnect()
{
	if (!isGooglePlayEnabled())
		return;

	log::messageln("jniGoogle_Play_Disconnect called");

	try
	{
		JNIEnv* env = jniGetEnv();
		LOCAL_REF_HOLDER(env);

		jmethodID jisMethod = env->GetMethodID(_jGooglePlayClass, "disconnect", "()V");
		JNI_NOT_NULL(jisMethod);
		env->CallVoidMethod(_jGooglePlayObject, jisMethod);
	}
	catch (const notFound&)
	{
		log::error("jniGoogle_Play_Disconnect failed, class/member not found");
	}
}

string jniGoogle_Play_GetAccountName()
{
	if (!isGooglePlayEnabled())
		return "";

	log::messageln("jniGoogle_Play_GetAccountName called");

	try
	{
		JNIEnv* env = jniGetEnv();
		LOCAL_REF_HOLDER(env);
		jmethodID jisMethod = env->GetMethodID(_jGooglePlayClass, "getCurrentAccountName", "()Ljava/lang/String;");
		JNI_NOT_NULL(jisMethod);
		jobject result = env->CallObjectMethod(_jGooglePlayObject, jisMethod);
		string data = jniGetString(env, (jstring)result);
		return data;
	}
	catch (const notFound&)
	{
		log::error("jniGoogle_Play_GetAccountName failed, class/member not found");
	}

	return "";
}


void jniGoogle_Play_UnlockAchievement(const string& id)
{
	if (!isGooglePlayEnabled())
		return;

	log::messageln("jniGoogle_Play_UnlockAchievement called");

	try
	{
		JNIEnv* env = jniGetEnv();
		LOCAL_REF_HOLDER(env);
		jstring jid = env->NewStringUTF(id.c_str());
		jmethodID jisMethod = env->GetMethodID(_jGooglePlayClass, "unlockAchievement", "(Ljava/lang/String;)V");
		JNI_NOT_NULL(jisMethod);
		env->CallVoidMethod(_jGooglePlayObject, jisMethod, jid);
	}
	catch (const notFound&)
	{
		log::error("jniGoogle_Play_UnlockAchievement failed, class/member not found");
	}
}

string jniGoogle_Play_GetDisplayName()
{
	if (!isGooglePlayEnabled())
		return "";

	log::messageln("jniGoogle_Play_GetDisplayName called");

	try
	{
		JNIEnv* env = jniGetEnv();
		LOCAL_REF_HOLDER(env);
		jmethodID jisMethod = env->GetMethodID(_jGooglePlayClass, "getDisplayName", "()Ljava/lang/String;");
		JNI_NOT_NULL(jisMethod);
		jobject result = env->CallObjectMethod(_jGooglePlayObject, jisMethod);
		string data = jniGetString(env, (jstring)result);
		return data;
	}
	catch (const notFound&)
	{
		log::error("jniGoogle_Play_GetDisplayName failed, class/member not found");
	}

	return "";
}
/* PLAY ********/


//////////////////////////////////////////////////////////////////////////////////////////////////
/*void jniGooglePlaySyncAchievements(const string& jsonAchievements)
{
    if (!isGoogleEnabled())
            return;

    log::messageln("jniGooglePlaySyncAchievements called");

     try
    {
        JNIEnv* env = jniGetEnv();
        LOCAL_REF_HOLDER(env);
        jstring jachs = env->NewStringUTF(jsonAchievements.c_str());
        jmethodID jisMethod = env->GetMethodID(_jGoogleClass, "syncAchievements", "(Ljava/lang/String;)V");
        JNI_NOT_NULL(jisMethod);
        env->CallVoidMethod(_jGoogleObject, jisMethod, jachs);
    }
    catch (const notFound&)
    {
        log::error("jniGooglePlaySyncAchievements failed, class/member not found");
    }
}

void jniGooglePlaySubmitResult(int score, const string& leaderBoardID)
{
    if (!isGoogleEnabled())
            return;

    log::messageln("jniGooglePlaySubmitResult called");

     try
    {
        JNIEnv* env = jniGetEnv();
        LOCAL_REF_HOLDER(env);
        jstring jlbID = env->NewStringUTF(leaderBoardID.c_str());
        jmethodID jisMethod = env->GetMethodID(_jGoogleClass, "submitScore", "(ILjava/lang/String;)V");
        JNI_NOT_NULL(jisMethod);
        env->CallVoidMethod(_jGoogleObject, jisMethod, score, jlbID);
    }
    catch (const notFound&)
    {
        log::error("jniGooglePlaySubmitResult failed, class/member not found");
    }

}

void jniGooglePlaySignIn(bool tryToResolveError)
{
    if (!isGoogleEnabled())
            return;

	log::messageln("jniGooglePlaySignIn called");

    try
    {
        JNIEnv* env = jniGetEnv();
        LOCAL_REF_HOLDER(env);
        //jstring jappLinkUrl = env->NewStringUTF(appLinkUrl.c_str()); //to jstring example
        jmethodID jisMethod = env->GetMethodID(_jGoogleClass, "signin", "(Z)V");
        JNI_NOT_NULL(jisMethod);
        //string data = jniGetString(env, (jstring) obj); //string example
        env->CallVoidMethod(_jGoogleObject, jisMethod, tryToResolveError);
    }
    catch (const notFound&)
    {
        log::error("jniGooglePlaySignIn failed, class/member not found");
    }
}


string jniGooglePlayGetUserID()
{
    if (!isGoogleEnabled())
            return "";

    log::messageln("jniGooglePlayGetUserID called");

    try
    {
        JNIEnv* env = jniGetEnv();
        LOCAL_REF_HOLDER(env);
        jmethodID jisMethod = env->GetMethodID(_jGoogleClass, "getUserID", "()Ljava/lang/String;");
        JNI_NOT_NULL(jisMethod);
        jobject result = env->CallObjectMethod(_jGoogleObject, jisMethod);
        string data = jniGetString(env, (jstring) result);
        return data;
    }
    catch (const notFound&)
    {
        log::error("jniGooglePlayGetUserID failed, class/member not found");
    }

    return "";
}

bool jniGooglePlayGetTryResolveError()
{
    if (!isGoogleEnabled())
        return false;

    log::messageln("jniGooglePlayGetTryResolveError called");

    bool result = false;
    try
    {
        JNIEnv* env = jniGetEnv();
        LOCAL_REF_HOLDER(env);
        jmethodID jisMethod = env->GetMethodID(_jGoogleClass, "getTryResolveError", "()Z");
        JNI_NOT_NULL(jisMethod);
        jboolean jb = env->CallBooleanMethod(_jGoogleObject, jisMethod);
        result = (bool) jb;

    }
    catch (const notFound&)
    {
        log::error("jniGooglePlayGetTryResolveError failed, class/member not found");
    }

    return result;
}

bool jniIsGooglePlaySignedIn()
{
    if (!isGoogleEnabled())
        return false;

	log::messageln("jniIsGooglePlaySignedIn called");

    bool result = false;
    try
    {
        JNIEnv* env = jniGetEnv();
        LOCAL_REF_HOLDER(env);
        //jstring jappLinkUrl = env->NewStringUTF(appLinkUrl.c_str()); //to jstring example
        jmethodID jisMethod = env->GetMethodID(_jGoogleClass, "isSignedIn", "()Z");
        JNI_NOT_NULL(jisMethod);
        //string data = jniGetString(env, (jstring) obj); //string example
        jboolean jb = env->CallBooleanMethod(_jGoogleObject, jisMethod);
        result = (bool) jb;

    }
    catch (const notFound&)
    {
        log::error("jniIsGooglePlaySignedIn failed, class/member not found");
    }

    return result;

}

void jniGooglePlayShowAchievements()
{
      if (!isGoogleEnabled())
        return;

    log::messageln("jniGooglePlayShowAchievements called");

    try
    {
        JNIEnv* env = jniGetEnv();
        LOCAL_REF_HOLDER(env);
        jmethodID jisMethod = env->GetMethodID(_jGoogleClass, "showAchievements", "()V");
        JNI_NOT_NULL(jisMethod);
        env->CallVoidMethod(_jGoogleObject, jisMethod);
    }
    catch (const notFound&)
    {
        log::error("jniGooglePlayShowAchievements failed, class/member not found");
    }
}

void jniGooglePlayShowLeaderboard(const string& leaderBoardID)
{
     if (!isGoogleEnabled())
        return;

    log::messageln("jniGooglePlayShowLeaderboard called");

    try
    {
        JNIEnv* env = jniGetEnv();
        LOCAL_REF_HOLDER(env);
        jstring jldID = env->NewStringUTF(leaderBoardID.c_str());
        jmethodID jisMethod = env->GetMethodID(_jGoogleClass, "showLeaderBoard", "(Ljava/lang/String;)V");
        JNI_NOT_NULL(jisMethod);
        env->CallVoidMethod(_jGoogleObject, jisMethod, jldID);
    }
    catch (const notFound&)
    {
        log::error("jniGooglePlayShowLeaderboard failed, class/member not found");
    }
}

void jniGooglePlaySignOut()
{
    if (!isGoogleEnabled())
        return;
	
	log::messageln("jniGooglePlaySignOut called");

    try
    {
        JNIEnv* env = jniGetEnv();
        LOCAL_REF_HOLDER(env);
        //jstring jappLinkUrl = env->NewStringUTF(appLinkUrl.c_str()); //to jstring example
        jmethodID jisMethod = env->GetMethodID(_jGoogleClass, "signout", "()V");
        JNI_NOT_NULL(jisMethod);
        //string data = jniGetString(env, (jstring) obj); //string example
        env->CallVoidMethod(_jGoogleObject, jisMethod);

    }
    catch (const notFound&)
    {
        log::error("jniGooglePlaySignOut failed, class/member not found");
    }
}

/*
bool jniGooglePlaySomeBoolFunc()
    {
    if (!isGooglePlayEnabled())
        return false;

    bool result = false;
    try
    {
        JNIEnv* env = jniGetEnv();
        LOCAL_REF_HOLDER(env);
        //jstring jappLinkUrl = env->NewStringUTF(appLinkUrl.c_str()); //to jstring example
        jmethodID jisMethod = env->GetMethodID(_jGooglePlayClass, "FUNC_NAME_JAVA", "()Z");
        JNI_NOT_NULL(jisMethod);
        //string data = jniGetString(env, (jstring) obj); //string example
        jboolean jb = env->CallBooleanMethod(_jGooglePlayObject, jisMethod);
        result = (bool) jb;

    }
    catch (const notFound&)
    {
        log::error("jniGooglePlaySomeBoolFunc failed, class/member not found");
    }

    return result;
}
*/
