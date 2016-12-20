package org.oxygine.googleplay;

import android.app.Activity;
import android.content.Intent;
import android.content.IntentSender;
import android.os.Bundle;
import android.support.annotation.NonNull;
import android.util.Log;

import com.google.android.gms.common.ConnectionResult;
import com.google.android.gms.common.api.GoogleApiClient;
import com.google.android.gms.common.api.PendingResult;
import com.google.android.gms.common.api.ResultCallback;
import com.google.android.gms.games.Game;
import com.google.android.gms.games.Games;
import com.google.android.gms.games.GamesActivityResultCodes;
import com.google.android.gms.games.GamesStatusCodes;
import com.google.android.gms.games.achievement.Achievement;
import com.google.android.gms.games.achievement.AchievementBuffer;
import com.google.android.gms.games.achievement.Achievements;

import org.json.JSONException;
import org.json.JSONObject;
import org.oxygine.lib.extension.ActivityObserver;

/**
 * Created by user on 12.12.2016.
 */
public class GooglePlayAdapter extends ActivityObserver implements GoogleApiClient.ConnectionCallbacks, GoogleApiClient.OnConnectionFailedListener {

    private static String TAG = "GooglePlayAdapter";

    private static int AR_REQUEST_ERROR_RESOLUTION = 25551;

    private GoogleApiClient mGoogleApiClient;
    private static GooglePlayAdapter mGooglePlayAdapter;
    private boolean mTryResolveError = false;
    private Activity mActivity;

    public static native void nativeOnConnected();
    public static native void nativeOnConnectionFailed();
    //public static native void nativeOnConnectionSuspended();


    public static GooglePlayAdapter getInstance()
    {
        return mGooglePlayAdapter;
    }

    public GooglePlayAdapter(Activity activity, GoogleApiClient googleApiClient)
    {
        mActivity = activity;

        mGoogleApiClient = googleApiClient;
        googleApiClient.registerConnectionCallbacks(this);
        googleApiClient.registerConnectionFailedListener(this);
    }

    public void disconnect()
    {
        mGoogleApiClient.disconnect();
    }

    public boolean isConnected()
    {
        return mGoogleApiClient.isConnected();
    }

    public boolean isConnecting()
    {
        return mGoogleApiClient.isConnecting();
    }

    public void connect(boolean tryResolveError)
    {
        mTryResolveError = tryResolveError;
        mGoogleApiClient.connect();
    }

    public String getCurrentAccountName()
    {
        return Games.getCurrentAccountName(mGoogleApiClient);
    }
    public String getDisplayName() { return Games.Players.getCurrentPlayer(mGoogleApiClient).getDisplayName(); }
    @Override
    public void onConnected(Bundle bundle) {
        nativeOnConnected();
    }

    @Override
    public void onConnectionSuspended(int i) {
        connect(false);
        //nativeOnConnectionSuspended();
    }

    @Override
    public void onStop()
    {
        mGoogleApiClient.disconnect();
    }

    private boolean mResolvingConnectionFailure = false;

    @Override
    public void onConnectionFailed(ConnectionResult connectionResult) {
        if (mResolvingConnectionFailure)
            return;

        Log.i(TAG, "onConnectionFailed: " +
                "1) " + connectionResult.getErrorMessage() + "\n " +
                "2) " + connectionResult.toString() + "\n" +
                "3) " + String.valueOf(connectionResult.getErrorCode()));


        if (mTryResolveError && connectionResult.hasResolution())
        {
            mResolvingConnectionFailure = true;
            try {
                connectionResult.startResolutionForResult(mActivity, AR_REQUEST_ERROR_RESOLUTION);
            } catch (IntentSender.SendIntentException e) {
                e.printStackTrace();
                nativeOnConnectionFailed();
            }
        } else {
            nativeOnConnectionFailed();
        }
    }


    private String getStringJsonParam(final String jsonStr, final String paramId) {
        try {
            JSONObject jsonObj = new JSONObject(jsonStr);
            return jsonObj.getString(paramId);
        } catch (JSONException e) {
            // Log.i(TAG, "getStringJsonParam json exception");
        }
        return "";
    }

    /*
            syncAchievements : { "CgkI1JqekoMWEAIQAg" : 1 }
    */
    public void syncAchievements(final String json_achievs) {
        Log.i(TAG, "syncAchievements called: " + json_achievs);
        PendingResult<Achievements.LoadAchievementsResult> achievementsResult = Games.Achievements.load(mGoogleApiClient, false);
        achievementsResult.setResultCallback(new ResultCallback<Achievements.LoadAchievementsResult>() {
            @Override
            public void onResult(Achievements.LoadAchievementsResult loadAchievementsResult) {
                if (loadAchievementsResult != null) {
                    if (loadAchievementsResult.getStatus().getStatusCode() == GamesStatusCodes.STATUS_OK) {
                        if (loadAchievementsResult.getAchievements() != null) {
                            AchievementBuffer achievementBuffer = loadAchievementsResult.getAchievements();
                            for (Achievement achievement : achievementBuffer) {
                                //Log.i(TAG, "1syncAchievements: " + achievement.toString());

                                String id = achievement.getAchievementId();
                                String svalue = getStringJsonParam(json_achievs, id);

                                if (svalue != "") {

                                    int type = achievement.getType();
                                    int value = Integer.parseInt(svalue);

                                    if (type == Achievement.TYPE_INCREMENTAL) {
                                        int add = value - achievement.getCurrentSteps();
                                        if (add > 0) {
                                            Log.i(TAG, "syncAchievements increment: [" + id + "] " + String.valueOf(add));
                                            //todo: add achievements increment
                                            //Games.Achievements.increment(mHelper.getApiClient(), id, add);
                                        } else {
                                            Log.i(TAG, "syncAchievements wrong add value: " + String.valueOf(add));
                                        }
                                    }

                                    if (type == Achievement.TYPE_STANDARD) {
                                        if (value == 1 && achievement.getState() == Achievement.STATE_REVEALED) {
                                            Log.i(TAG, "syncAchievements unlocked: " + id);
                                            Games.Achievements.unlock(mGoogleApiClient, id);
                                        }
                                    }
                                }

                            }
                            achievementBuffer.release();
                        }
                    }
                }

            }
        });
    }

    @Override
    public void onActivityResult(int requestCode, int resultCode, Intent data)
    {
        if (requestCode == AR_REQUEST_ERROR_RESOLUTION) {
            mResolvingConnectionFailure = false;
            if (resultCode == Activity.RESULT_OK) {
                connect(false);
            } else if (resultCode == GamesActivityResultCodes.RESULT_RECONNECT_REQUIRED) {
                connect(false);
            } else if (resultCode == Activity.RESULT_CANCELED) {
                // User cancelled.
                mGoogleApiClient.disconnect();
                nativeOnConnectionFailed();
            } else {
                // Whatever the problem we were trying to solve, it was not
                // solved. So give up and show an error message.
                Log.e(TAG, "onActivityResult: responseCode=" + activityResponseCodeToString(resultCode) + ", so giving up.");
                nativeOnConnectionFailed();
            }
        }
    }

    static String activityResponseCodeToString(int respCode) {
        switch (respCode) {
            case Activity.RESULT_OK:
                return "RESULT_OK";
            case Activity.RESULT_CANCELED:
                return "RESULT_CANCELED";
            case GamesActivityResultCodes.RESULT_APP_MISCONFIGURED:
                return "RESULT_APP_MISCONFIGURED";
            case GamesActivityResultCodes.RESULT_LEFT_ROOM:
                return "RESULT_LEFT_ROOM";
            case GamesActivityResultCodes.RESULT_LICENSE_FAILED:
                return "RESULT_LICENSE_FAILED";
            case GamesActivityResultCodes.RESULT_RECONNECT_REQUIRED:
                return "RESULT_RECONNECT_REQUIRED";
            case GamesActivityResultCodes.RESULT_SIGN_IN_FAILED:
                return "SIGN_IN_FAILED";
            default:
                return String.valueOf(respCode);
        }
    }
}
