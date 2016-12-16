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

jclass _jGoogleClass = 0;
jobject _jGoogleObject = 0;

jclass _jGoogleInterstitialClass = 0;
jobject _jGoogleInterstitialObject = 0;

jclass _jGoogleRewardedClass = 0;
jobject _jGoogleRewardedObject = 0;

bool isGoogleInterstitialEnabled()
{
	return _jGoogleInterstitialClass && _jGoogleInterstitialObject;
}

bool isGoogleRewardedEnabled()
{
	return _jGoogleRewardedClass && _jGoogleRewardedObject;
}

bool isGoogleEnabled()
{
    return _jGoogleClass && _jGoogleObject;
}


extern "C"
{
	/* Interstitial */
	JNIEXPORT void JNICALL Java_org_oxygine_googleplay_InterstitialAdapter_nativeOnAdLoaded(JNIEnv* env, jobject obj)
	{
		core::getMainThreadDispatcher().postCallback([=]()
		{
			google::interstitial::OnAdLoaded ev;
			google::dispatcher()->dispatchEvent(&ev);
		});
	}

	JNIEXPORT void JNICALL Java_org_oxygine_googleplay_InterstitialAdapter_nativeonAdFailedToLoad(JNIEnv* env, jobject obj)
	{
		core::getMainThreadDispatcher().postCallback([=]()
		{
			google::interstitial::OnAdFailedToLoad ev;
			google::dispatcher()->dispatchEvent(&ev);
		});
	}

	JNIEXPORT void JNICALL Java_org_oxygine_googleplay_InterstitialAdapter_nativeonOnAdClosed(JNIEnv* env, jobject obj)
	{
		core::getMainThreadDispatcher().postCallback([=]()
		{
			google::interstitial::OnAdClosed ev;
			google::dispatcher()->dispatchEvent(&ev);
		});
	}

	/* Rewarded */
	JNIEXPORT void JNICALL Java_org_oxygine_googleplay_RewardedAdapter_nativeOnRewardVideoLoaded(JNIEnv* env, jobject obj)
	{
		core::getMainThreadDispatcher().postCallback([=]()
		{
			google::rewarded::OnRewardVideoLoaded ev;
			google::dispatcher()->dispatchEvent(&ev);
		});
	}

	JNIEXPORT void JNICALL Java_org_oxygine_googleplay_RewardedAdapter_nativeOnRewardedVideoAdClosed(JNIEnv* env, jobject obj)
	{
		core::getMainThreadDispatcher().postCallback([=]()
		{
			google::rewarded::OnRewardedVideoAdClosed ev;
			google::dispatcher()->dispatchEvent(&ev);
		});
	}

	JNIEXPORT void JNICALL Java_org_oxygine_googleplay_RewardedAdapter_nativeOnRewarded(JNIEnv* env, jobject obj)
	{
		core::getMainThreadDispatcher().postCallback([=]()
		{
			google::rewarded::OnRewarded ev;
			google::dispatcher()->dispatchEvent(&ev);
		});
	}

	JNIEXPORT void JNICALL Java_org_oxygine_googleplay_RewardedAdapter_nativeOnRewardedVideoAdFailedToLoad(JNIEnv* env, jobject obj)
	{
		core::getMainThreadDispatcher().postCallback([=]()
		{
			google::rewarded::OnRewardedVideoAdFailedToLoad ev;
			google::dispatcher()->dispatchEvent(&ev);
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
    if (!isGoogleEnabled())
        return;

	try
    {
        JNIEnv* env = jniGetEnv();
        LOCAL_REF_HOLDER(env);

        env->DeleteGlobalRef(_jGoogleClass);
        _jGoogleClass = 0;

        env->DeleteGlobalRef(_jGoogleObject);
        _jGoogleObject = 0;
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
	if (!isGoogleEnabled())
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
	if (!isGoogleEnabled())
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

	log::messageln("jniGoogle_Rewarded_Show called");

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
		log::error("jniGoogle_Rewarded_Show failed, class/member not found");
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






void jniGooglePlaySyncAchievements(const string& jsonAchievements)
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
