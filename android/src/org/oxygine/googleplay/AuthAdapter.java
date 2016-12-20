package org.oxygine.googleplay;

import android.app.Activity;
import android.app.Dialog;
import android.content.Intent;
import android.os.AsyncTask;

import com.google.android.gms.auth.GoogleAuthException;
import com.google.android.gms.auth.GoogleAuthUtil;
import com.google.android.gms.auth.GooglePlayServicesAvailabilityException;
import com.google.android.gms.auth.UserRecoverableAuthException;
import com.google.android.gms.common.GooglePlayServicesUtil;
import com.google.android.gms.common.Scopes;
import com.google.android.gms.common.api.GoogleApiClient;
import com.google.android.gms.games.Games;

import org.oxygine.lib.extension.ActivityObserver;

import java.io.IOException;

/**
 * Created by qVadro on 12.12.2016.
 */

public class AuthAdapter extends ActivityObserver {

    final static String TAG = "AuthAdapter";
    private GoogleApiClient mGoogleApiClient;
    private Activity mActivity;
    public static int AR_REQUEST_TOKEN_RECOVER = 987001;
    private static AuthAdapter mInstance;

    public static native void nativeOnDataReceived(String uid, String token);
    public static native void nativeOnFailed();

    public static AuthAdapter getInstance()
    {
        return mInstance;
    }

    public AuthAdapter(Activity activity, GoogleApiClient apiClient)
    {
        mInstance = this;
        mGoogleApiClient = apiClient;
        mActivity = activity;
    }

    public void requestData()
    {
        TokenRequestTask task = new TokenRequestTask();
        task.execute();
    }

    @Override
    public void onActivityResult(int requestCode, int resultCode, Intent data)
    {
        if (requestCode == AR_REQUEST_TOKEN_RECOVER) {
            if (resultCode == Activity.RESULT_OK) {
                requestData();
            } else {
                nativeOnFailed();
            }
        }
    }

    class TokenRequestTask extends AsyncTask<Void, Void, Void> {
        private String uid;
        private String token;
        private boolean error = false;

        @Override
        protected Void doInBackground(Void... params) {
            final String email = Games.getCurrentAccountName(mGoogleApiClient);
            final String scope = "oauth2:" + Scopes.PROFILE;

            try {
                uid = GoogleAuthUtil.getAccountId(mActivity, email);
                token = GoogleAuthUtil.getToken(mActivity, email, scope);
            } catch (UserRecoverableAuthException e) {
                handleException(e);
            } catch (GoogleAuthException e) {
                error = true;
            } catch (IOException e) {
                error = true;
            }

            return null;
        }

        @Override
        protected void onPostExecute(Void result) {
            if (error)
            {
                nativeOnFailed();
            }
            else
            {
                nativeOnDataReceived(uid, token);
            }
        }
    }

    protected void handleException(final Exception e) {
        mActivity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                if (e instanceof GooglePlayServicesAvailabilityException) {
                    int statusCode = ((GooglePlayServicesAvailabilityException) e).getConnectionStatusCode();
                    Dialog dialog = GooglePlayServicesUtil.getErrorDialog(statusCode, mActivity, AR_REQUEST_TOKEN_RECOVER);
                    dialog.show();
                } else if (e instanceof UserRecoverableAuthException) {
                    Intent intent = ((UserRecoverableAuthException) e).getIntent();
                    mActivity.startActivityForResult(intent, AR_REQUEST_TOKEN_RECOVER);
                }
            }
        });
    }
}
