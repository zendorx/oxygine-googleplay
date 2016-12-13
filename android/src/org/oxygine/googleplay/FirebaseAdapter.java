package org.oxygine.googleplay;

import android.app.Activity;
import android.content.Intent;
import android.net.Uri;
import android.os.Bundle;
import android.support.annotation.NonNull;
import android.util.Log;

import com.google.android.gms.appinvite.AppInvite;
import com.google.android.gms.appinvite.AppInviteInvitation;
import com.google.android.gms.appinvite.AppInviteInvitationResult;
import com.google.android.gms.appinvite.AppInviteReferral;
import com.google.android.gms.common.api.GoogleApiClient;
import com.google.android.gms.common.api.ResultCallback;
import com.google.firebase.analytics.FirebaseAnalytics;

import org.oxygine.lib.extension.ActivityObserver;

/**
 * Created by qVadro on 12.12.2016.
 */
public class FirebaseAdapter extends ActivityObserver {

    private static String TAG = "FirebaseAdapter";
    private static int AR_INVITE;

    private Activity mActivity;
    private FirebaseAnalytics mFirebaseAnalytics;
    private GoogleApiClient mGoogleApiClient;

    private static native void nativeOnInviteActivityResult(int count);
    private static native void nativeOnInvitionResult();

    private static FirebaseAdapter mInstance;

    public static FirebaseAdapter getInstance()
    {
        return mInstance;
    }

    public FirebaseAdapter(Activity activity, GoogleApiClient googleApiClient)
    {
        mInstance = this;
        mActivity = activity;
        mGoogleApiClient = googleApiClient;
        mFirebaseAnalytics = FirebaseAnalytics.getInstance(mActivity);

        handleDynamicLink();
    }



    /* Log Events */

    public void setUserProperty(String id, String param)
    {
        Log.i(TAG, "setUserProperty " + id + " " + param);
        mFirebaseAnalytics.setUserProperty(id, param);
    }

    private Bundle mEventBundle = null;
    private String mEventID = null;

    public void startLogEvent(String id)
    {
        if (mEventID != null)
        {
            Log.w(TAG, "startLogEvent: _eventID not empty!");
        }

        if (mEventBundle != null)
        {
            Log.w(TAG, "startLogEvent: _eventBundle not null!");
        }

        mEventID = new String(id);
        mEventBundle = new Bundle();
    }

    public void logEventPutString(String paramID, String paramValue)
    {
        if (mEventBundle == null)
        {
            Log.e(TAG, "logEventPutString: _eventBundle is null! call startLogEvent before");
            return;
        }

        mEventBundle.putString(paramID, paramValue);
    }

    public void logEventPutDouble(String paramID, Double paramValue)
    {
        if (mEventBundle == null)
        {
            Log.e(TAG, "logEventPutDouble: _eventBundle is null! call startLogEvent before");
            return;
        }

        mEventBundle.putDouble(paramID, paramValue);
    }

    public void logEventPutInt(String paramID, int paramValue)
    {
        if (mEventBundle == null)
        {
            Log.e(TAG, "logEventPutInt: _eventBundle is null! call startLogEvent before");
            return;
        }
        mEventBundle.putInt(paramID, paramValue);
    }

    public void endLogEvent()
    {
        if (mEventID == null || mEventID.isEmpty())
        {
            Log.e(TAG, "endLogEvent: _eventID is empty!");
            return;
        }

        if (mEventBundle == null)
        {
            Log.e(TAG, "endLogEvent: _eventBundle is null!");
            return;
        }

        if (mFirebaseAnalytics == null)
        {
            Log.e(TAG, "endLogEvent: _analytics is null!");
            return;
        }
        mFirebaseAnalytics.logEvent(mEventID, mEventBundle);
        mEventID = null;
        mEventBundle = null;
    }

    /* ---------- */

    /* Invites */
    public void invite(final String title, final String message, final String link)
    {
        _activity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                /*Log.i(TAG, "invite 1");
                Log.i(TAG, "title: " + title);
                Log.i(TAG, "message: " + message);
                Log.i(TAG, "link: " + link);
                Log.i(TAG, "imageUrl: " + imageUrl);
                Log.i(TAG, "actionText: " + actionText);*/

                Intent intent = new AppInviteInvitation.IntentBuilder(title)
                        .setMessage(message)
                        .setDeepLink(Uri.parse(link))
                        .build();
                _activity.startActivityForResult(intent, AR_INVITE);
            }
        });
    }

    private void handleDynamicLink()
    {
        boolean autoLaunchDeepLink = false;
        AppInvite.AppInviteApi.getInvitation(mGoogleApiClient, mActivity, autoLaunchDeepLink)
                .setResultCallback(
                        new ResultCallback<AppInviteInvitationResult>() {
                            @Override
                            public void onResult(@NonNull AppInviteInvitationResult result) {
                                if (result.getStatus().isSuccess()) {
                                    // Extract deep link from Intent
                                    Intent intent = result.getInvitationIntent();
                                    String deepLink = AppInviteReferral.getDeepLink(intent);
                                    String invitationId = AppInviteReferral.getInvitationId(intent);

                                    nativeOnInvitionResult();
                                } else {
                                }
                            }
                        });

    }

    /* ------- */

    /* Common */

    @Override
    public void onActivityResult(int requestCode, int resultCode, Intent data) {
        if (requestCode == AR_INVITE) {
            if (resultCode == Activity.RESULT_OK) {
                String[] ids = AppInviteInvitation.getInvitationIds(resultCode, data);
                nativeOnInviteActivityResult(ids.length);
            }
        }
    }

    /* ------ */
}
