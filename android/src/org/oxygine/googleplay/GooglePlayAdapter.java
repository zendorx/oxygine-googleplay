package org.oxygine.googleplay;

import android.app.Activity;
import android.content.Intent;
import android.content.IntentSender;
import android.os.Bundle;
import android.util.Log;

import com.google.android.gms.common.ConnectionResult;
import com.google.android.gms.common.api.GoogleApiClient;
import com.google.android.gms.common.api.PendingResult;
import com.google.android.gms.common.api.ResultCallback;
import com.google.android.gms.games.Games;
import com.google.android.gms.games.GamesActivityResultCodes;
import com.google.android.gms.games.GamesStatusCodes;
import com.google.android.gms.games.achievement.Achievement;
import com.google.android.gms.games.achievement.AchievementBuffer;
import com.google.android.gms.games.achievement.Achievements;

import com.google.android.gms.appinvite.AppInvite;
import com.google.android.gms.appinvite.AppInviteInvitation;
import com.google.android.gms.appinvite.AppInviteInvitationResult;

import org.json.JSONException;
import org.json.JSONObject;
import org.oxygine.lib.extension.ActivityObserver;

public class GooglePlayAdapter extends ActivityObserver implements GoogleApiClient.ConnectionCallbacks,
        GoogleApiClient.OnConnectionFailedListener
{
    private static String TAG = "GooglePlayAdapter";
    private GoogleApiClient mGoogleApiClient;
    private Activity _activity;

    private boolean _tryToResolveError = false;

    public static int ERR_RESULT_CANCELED = 10001;
    public static int ERR_WONT_RESOLVE = 10002;
    public static int ERR_UNKNOWN = 19999;

    public static int AR_REQUEST_LEADERBOARD = 20000;
    public static int AR_REQUEST_ACHIEVEMENTS = 20001;

    public static native void nativeOnSignInResult(int errorCode);

    public GooglePlayAdapter(Activity a) {
        _activity = a;
        Log.i(TAG, "GooglePlayAdapter");

        // Create the Google Api Client with access to the Play Games services
        mGoogleApiClient = new GoogleApiClient.Builder(_activity)
                .addApi(AppInvite.API)
                .addConnectionCallbacks(this)
                .addOnConnectionFailedListener(this)
                .addApi(Games.API).addScope(Games.SCOPE_GAMES)
               // .addApi(Plus.API)
                //.addScope(Plus.SCOPE_PLUS_PROFILE)
                        // add other APIs and scopes here as needed
                .build();

    }

    public GoogleApiClient getGoogleApiClient()
    {
    	return mGoogleApiClient;
    }

    boolean getTryResolveError()
    {
        return _tryToResolveError;
    }

    public String getUserID()
    {
        if (isSignedIn())
        {
            return Games.getCurrentAccountName(mGoogleApiClient);
        }

        return "";
    }

    @Override
    public void onActivityResult(int requestCode, int resultCode, Intent data) {
        Log.i(TAG, "onActivityResult "
                + String.valueOf(requestCode)
                + " "
                + String.valueOf(requestCode == Activity.RESULT_OK));

        /*if (requestCode == RC_SIGN_IN) {

            Log.i(TAG, "onActivityResult");

            if (resultCode == Activity.RESULT_OK) {
                Log.i(TAG, "onActivityResult result ok");
                mGoogleApiClient.connect();
            } else {
                Log.i(TAG, "onActivityResult result not ok");
                nativeOnConnectionError();
            }
        }*/

        if (requestCode == RC_RESOLVE)
        {
            mResolvingConnectionFailure = false;
            if (resultCode == Activity.RESULT_OK) {
                Log.i(TAG, "onActivityResult: Resolution was RESULT_OK, so connecting current client again.");
                signin(false);
            } else if (resultCode == GamesActivityResultCodes.RESULT_RECONNECT_REQUIRED) {
                Log.i(TAG, "onActivityResult: Resolution was RECONNECT_REQUIRED, so reconnecting.");
                signin(false);
            } else if (resultCode == Activity.RESULT_CANCELED) {
                // User cancelled.
                Log.i(TAG, "onActivityResult: Got a cancellation result, so disconnecting.");
                mGoogleApiClient.disconnect();
                nativeOnSignInResult(ERR_RESULT_CANCELED);
            } else {
                // Whatever the problem we were trying to solve, it was not
                // solved. So give up and show an error message.
                Log.i(TAG, "onActivityResult: responseCode="
                        + activityResponseCodeToString(resultCode)
                        + ", so giving up.");
                nativeOnSignInResult(ERR_UNKNOWN);
            }
        }
    }

    public boolean isSignedIn()
    {
        return mGoogleApiClient != null && mGoogleApiClient.isConnected();
    }

    public void signin(boolean tryToResolveError)
    {
        if (mGoogleApiClient == null)
            return;

        if (mGoogleApiClient.isConnected())
            return;

        if (mGoogleApiClient.isConnecting())
            return;

        Log.i(TAG, "signin");
        _tryToResolveError = tryToResolveError;

        _activity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                mGoogleApiClient.connect();
            }
        });
    }

    public void signout()
    {
        Log.i(TAG, "signout");
        _activity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                mGoogleApiClient.disconnect();
            }
        });

    }

    void showAchievements()
    {


        if (!isSignedIn())
            return;

        _activity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                _activity.startActivityForResult(Games.Achievements.getAchievementsIntent(mGoogleApiClient), AR_REQUEST_ACHIEVEMENTS);
            }
        });
    }

    /*public void achievement(final String id, final int score)
    {
        _activity.runOnUiThread( new Runnable() {
            @Override
            public void run() {
                if (score == 0)
                {
                    Games.Achievements.unlock(mGoogleApiClient, id);
                }
                else
                {
                    Games.Achievements.increment(mGoogleApiClient, id, score);
                }
            }});
    }*/

    public void showLeaderBoard(final String leaderBoardID)
    {
        _activity.runOnUiThread( new Runnable() {
            @Override
            public void run() {
                _activity.startActivityForResult(Games.Leaderboards.getLeaderboardIntent(mGoogleApiClient,
                        leaderBoardID), AR_REQUEST_LEADERBOARD);
            }});
    }

    public void submitScore(final int score, final String leaderBoardID)
    {
        _activity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                Games.Leaderboards.submitScore(mGoogleApiClient, leaderBoardID, score);
            }
        });
    }

    /*
    @Override
public void onConnected(Bundle bundle) {
    String data =  Plus.API.getName() + "\n" +
            Plus.PeopleApi.getCurrentPerson(mGoogleApiClient).getName() + "\n" +
            Plus.PeopleApi.getCurrentPerson(mGoogleApiClient).getId() + "\n" +
            Plus.PeopleApi.getCurrentPerson(mGoogleApiClient).getAboutMe() + "\n" +
            Plus.PeopleApi.getCurrentPerson(mGoogleApiClient).getBirthday() + "\n" +
            Plus.PeopleApi.getCurrentPerson(mGoogleApiClient).getBraggingRights() + "\n" +
            Plus.PeopleApi.getCurrentPerson(mGoogleApiClient).getCurrentLocation() + "\n" +
            Plus.PeopleApi.getCurrentPerson(mGoogleApiClient).getNickname() + "\n" +
            Plus.PeopleApi.getCurrentPerson(mGoogleApiClient).getDisplayName() + "\n" +
            Plus.PeopleApi.getCurrentPerson(mGoogleApiClient).getTagline();
    Toast.makeText(this, data, Toast.LENGTH_LONG).show();

     */


    /*
    syncAchievements : { "CgkI1JqekoMWEAIQAg" : 1 }
    * */
    public void syncAchievements(final String json_achievs)
    {
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

                                    if (type == Achievement.TYPE_INCREMENTAL)
                                    {
                                        int add = value - achievement.getCurrentSteps();
                                        if (add > 0) {
                                            Log.i(TAG, "syncAchievements increment: [" + id + "] " + String.valueOf(add));
                                            //Games.Achievements.increment(mHelper.getApiClient(), id, add);
                                        } else {
                                            Log.i(TAG, "syncAchievements wrong add value: " + String.valueOf(add));
                                        }
                                    }

                                    if (type == Achievement.TYPE_STANDARD)
                                    {
                                        if (value == 1 && achievement.getState() == Achievement.STATE_REVEALED)
                                        {
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


    private String getStringJsonParam(final String jsonStr, final String paramId)
    {
        try
        {
            JSONObject jsonObj = new JSONObject(jsonStr);
            return jsonObj.getString(paramId);
        }
        catch (JSONException e)
        {
           // Log.i(TAG, "getStringJsonParam json exception");
        }
        return "";
    }










    @Override
    public void onStart()
    {
       // mGoogleApiClient.connect();
    }

    @Override
    public void onStop()
    {
        mGoogleApiClient.disconnect();
    }

    @Override
    public void onDestroy() {
    }

    @Override
    public void onResume() {
    }

    @Override
    public void onPause() {

    }

    @Override
    public void onConnected(Bundle bundle) {

        // The player is signed in. Hide the sign-in button and allow the
        // player to proceed.
        Log.i(TAG, "onConnected");
        nativeOnSignInResult(0);
    }

    private static int RC_RESOLVE = 19002;

    private boolean mResolvingConnectionFailure = false;

    @Override
    public void onConnectionSuspended(int i) {
        Log.i(TAG, "onConnectionSuspended");
        mGoogleApiClient.connect();
    }

    @Override
    public void onConnectionFailed(ConnectionResult connectionResult) {
        Log.i(TAG, "onConnectionFailed");
        if (mResolvingConnectionFailure) {
            Log.i(TAG, "onConnectionFailed already resolving");
            return;
        }

        Log.i(TAG, "error: 1) " + connectionResult.getErrorMessage()
                + "\n 2) " + connectionResult.toString()
                + "\n 3) " + String.valueOf(connectionResult.getErrorCode()));

        if (_tryToResolveError && connectionResult.hasResolution())
        {
            mResolvingConnectionFailure = true;
            Log.i(TAG, "onConnectionFailed trying to resolve");
            try {
                connectionResult.startResolutionForResult(_activity, RC_RESOLVE);
            } catch (IntentSender.SendIntentException e) {
                Log.i(TAG, "onConnectionFailed error while resolving");
                e.printStackTrace();
            }
        }
        else
        {
            Log.i(TAG, "onConnectionFailed wont resolve");
            nativeOnSignInResult(ERR_WONT_RESOLVE);
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

    /*
            if (mGoogleApiClient != null && mGoogleApiClient.isConnected()) {
                // Call a Play Games services API method, for example:
                Achievements.unlock(mGoogleApiClient, MY_ACHIEVEMENT_ID);
            } else {
                // Alternative implementation (or warn user that they must
                // sign in to use this feature)
        }

    * */
}