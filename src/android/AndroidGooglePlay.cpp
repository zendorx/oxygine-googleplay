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

jclass _jGooglePlayClass = 0;
jobject _jGooglePlayObject = 0;

bool isGooglePlayEnabled()
{
    return _jGooglePlayClass && _jGooglePlayObject;
}


extern "C"
{
    JNIEXPORT void JNICALL Java_org_oxygine_googleplay_GooglePlayAdapter_nativeOnGetToken(JNIEnv* env, jobject obj,jstring uid, jstring token)
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
    }


}


void jniGooglePlayInit()
{
    try
    {
        JNIEnv* env = jniGetEnv();
        LOCAL_REF_HOLDER(env);
        JNI_NOT_NULL(env);

        _jGooglePlayClass = env->FindClass("org/oxygine/googleplay/GooglePlayAdapter");
        JNI_NOT_NULL(_jGooglePlayClass);

        _jGooglePlayClass = (jclass) env->NewGlobalRef(_jGooglePlayClass);
        JNI_NOT_NULL(_jGooglePlayClass);

        _jGooglePlayObject = env->NewGlobalRef(jniFindExtension(env, _jGooglePlayClass));
        JNI_NOT_NULL(_jGooglePlayObject);
    }
    catch (const notFound&)
    {
        log::error("jniGooglePlayInit failed, class/member not found");
    }
}

void jniGooglePlayFree()
{
    if (!isGooglePlayEnabled())
        return;

	

    try
    {
        JNIEnv* env = jniGetEnv();
        LOCAL_REF_HOLDER(env);

        env->DeleteGlobalRef(_jGooglePlayClass);
        _jGooglePlayClass = 0;

        env->DeleteGlobalRef(_jGooglePlayObject);
        _jGooglePlayObject = 0;
    }
    catch (const notFound&)
    {

    }
}

void jniGooglePlaySyncAchievements(const string& jsonAchievements)
{
    if (!isGooglePlayEnabled())
            return;

    log::messageln("jniGooglePlaySyncAchievements called");

     try
    {
        JNIEnv* env = jniGetEnv();
        LOCAL_REF_HOLDER(env);
        jstring jachs = env->NewStringUTF(jsonAchievements.c_str());
        jmethodID jisMethod = env->GetMethodID(_jGooglePlayClass, "syncAchievements", "(Ljava/lang/String;)V");
        JNI_NOT_NULL(jisMethod);
        env->CallVoidMethod(_jGooglePlayObject, jisMethod, jachs);
    }
    catch (const notFound&)
    {
        log::error("jniGooglePlaySyncAchievements failed, class/member not found");
    }
}

void jniGooglePlaySubmitResult(int score, const string& leaderBoardID)
{
    if (!isGooglePlayEnabled())
            return;

    log::messageln("jniGooglePlaySubmitResult called");

     try
    {
        JNIEnv* env = jniGetEnv();
        LOCAL_REF_HOLDER(env);
        jstring jlbID = env->NewStringUTF(leaderBoardID.c_str());
        jmethodID jisMethod = env->GetMethodID(_jGooglePlayClass, "submitScore", "(ILjava/lang/String;)V");
        JNI_NOT_NULL(jisMethod);
        env->CallVoidMethod(_jGooglePlayObject, jisMethod, score, jlbID);
    }
    catch (const notFound&)
    {
        log::error("jniGooglePlaySubmitResult failed, class/member not found");
    }

}

void jniGooglePlaySignIn(bool tryToResolveError)
{
    if (!isGooglePlayEnabled())
            return;

	log::messageln("jniGooglePlaySignIn called");

    try
    {
        JNIEnv* env = jniGetEnv();
        LOCAL_REF_HOLDER(env);
        //jstring jappLinkUrl = env->NewStringUTF(appLinkUrl.c_str()); //to jstring example
        jmethodID jisMethod = env->GetMethodID(_jGooglePlayClass, "signin", "(Z)V");
        JNI_NOT_NULL(jisMethod);
        //string data = jniGetString(env, (jstring) obj); //string example
        env->CallVoidMethod(_jGooglePlayObject, jisMethod, tryToResolveError);
    }
    catch (const notFound&)
    {
        log::error("jniGooglePlaySignIn failed, class/member not found");
    }
}

string jniGooglePlayGetUserName()
{
    if (!isGooglePlayEnabled())
            return "";

    log::messageln("jniGooglePlayGetUserName called");

    try
    {
        JNIEnv* env = jniGetEnv();
        LOCAL_REF_HOLDER(env);
        jmethodID jisMethod = env->GetMethodID(_jGooglePlayClass, "getUserName", "()Ljava/lang/String;");
        JNI_NOT_NULL(jisMethod);
        jobject result = env->CallObjectMethod(_jGooglePlayObject, jisMethod);
        string data = jniGetString(env, (jstring) result);
        return data;
    }
    catch (const notFound&)
    {
        log::error("jniGooglePlayGetUserName failed, class/member not found");
    }

    return "";
}

string jniGooglePlayGetUserID()
{
    if (!isGooglePlayEnabled())
            return "";

    log::messageln("jniGooglePlayGetUserID called");

    try
    {
        JNIEnv* env = jniGetEnv();
        LOCAL_REF_HOLDER(env);
        jmethodID jisMethod = env->GetMethodID(_jGooglePlayClass, "getUserID", "()Ljava/lang/String;");
        JNI_NOT_NULL(jisMethod);
        jobject result = env->CallObjectMethod(_jGooglePlayObject, jisMethod);
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
    if (!isGooglePlayEnabled())
        return false;

    log::messageln("jniGooglePlayGetTryResolveError called");

    bool result = false;
    try
    {
        JNIEnv* env = jniGetEnv();
        LOCAL_REF_HOLDER(env);
        jmethodID jisMethod = env->GetMethodID(_jGooglePlayClass, "getTryResolveError", "()Z");
        JNI_NOT_NULL(jisMethod);
        jboolean jb = env->CallBooleanMethod(_jGooglePlayObject, jisMethod);
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
    if (!isGooglePlayEnabled())
        return false;

	log::messageln("jniIsGooglePlaySignedIn called");

    bool result = false;
    try
    {
        JNIEnv* env = jniGetEnv();
        LOCAL_REF_HOLDER(env);
        //jstring jappLinkUrl = env->NewStringUTF(appLinkUrl.c_str()); //to jstring example
        jmethodID jisMethod = env->GetMethodID(_jGooglePlayClass, "isSignedIn", "()Z");
        JNI_NOT_NULL(jisMethod);
        //string data = jniGetString(env, (jstring) obj); //string example
        jboolean jb = env->CallBooleanMethod(_jGooglePlayObject, jisMethod);
        result = (bool) jb;

    }
    catch (const notFound&)
    {
        log::error("jniIsGooglePlaySignedIn failed, class/member not found");
    }

    return result;

}


void jniGooglePlayRequestToken()
{
    if (!isGooglePlayEnabled())
        return;

    log::messageln("jniGooglePlayRequestToken called");

    try
    {
        JNIEnv* env = jniGetEnv();
        LOCAL_REF_HOLDER(env);
        jmethodID jisMethod = env->GetMethodID(_jGooglePlayClass, "requestToken", "()V");
        JNI_NOT_NULL(jisMethod);
        env->CallVoidMethod(_jGooglePlayObject, jisMethod);
    }
    catch (const notFound&)
    {
        log::error("jniGooglePlayRequestToken failed, class/member not found");
    }
}


void jniGooglePlayShowAchievements()
{
      if (!isGooglePlayEnabled())
        return;

    log::messageln("jniGooglePlayShowAchievements called");

    try
    {
        JNIEnv* env = jniGetEnv();
        LOCAL_REF_HOLDER(env);
        jmethodID jisMethod = env->GetMethodID(_jGooglePlayClass, "showAchievements", "()V");
        JNI_NOT_NULL(jisMethod);
        env->CallVoidMethod(_jGooglePlayObject, jisMethod);
    }
    catch (const notFound&)
    {
        log::error("jniGooglePlayShowAchievements failed, class/member not found");
    }
}

void jniGooglePlayShowLeaderboard(const string& leaderBoardID)
{
     if (!isGooglePlayEnabled())
        return;

    log::messageln("jniGooglePlayShowLeaderboard called");

    try
    {
        JNIEnv* env = jniGetEnv();
        LOCAL_REF_HOLDER(env);
        jstring jldID = env->NewStringUTF(leaderBoardID.c_str());
        jmethodID jisMethod = env->GetMethodID(_jGooglePlayClass, "showLeaderBoard", "(Ljava/lang/String;)V");
        JNI_NOT_NULL(jisMethod);
        env->CallVoidMethod(_jGooglePlayObject, jisMethod, jldID);
    }
    catch (const notFound&)
    {
        log::error("jniGooglePlayShowLeaderboard failed, class/member not found");
    }
}

void jniGooglePlaySignOut()
{
    if (!isGooglePlayEnabled())
        return;
	
	log::messageln("jniGooglePlaySignOut called");

    try
    {
        JNIEnv* env = jniGetEnv();
        LOCAL_REF_HOLDER(env);
        //jstring jappLinkUrl = env->NewStringUTF(appLinkUrl.c_str()); //to jstring example
        jmethodID jisMethod = env->GetMethodID(_jGooglePlayClass, "signout", "()V");
        JNI_NOT_NULL(jisMethod);
        //string data = jniGetString(env, (jstring) obj); //string example
        env->CallVoidMethod(_jGooglePlayObject, jisMethod);

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
