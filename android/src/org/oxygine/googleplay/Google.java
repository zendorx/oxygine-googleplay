package org.oxygine.googleplay;

import android.app.Activity;
import android.util.Log;

import com.google.android.gms.common.api.GoogleApiClient;

/**
 * Created by qVadro on 12.12.2016.
 */
public class Google {

    private static String TAG = "Oxygine - Google";

    private AuthAdapter mAuthAdapter;
    private GooglePlayAdapter mGooglePlayAdapter;
    private InterstitialAdapter mInterstitialAdapter;
    private RewardedAdapter mRewardedAdapter;
    private FirebaseAdapter mFirebaseAdapter;

    private Activity mActivity;
    private GoogleApiClient mGoogleApiClient;

/*
        mGoogleApiClient = new GoogleApiClient.Builder(activity)
                .addApi(AppInvite.API)
                .addConnectionCallbacks(this)
                .addOnConnectionFailedListener(this)
                .addApi(Games.API).addScope(Games.SCOPE_GAMES)
                .build();
* */
    public Google(Activity activity, GoogleApiClient googleApiClient)
    {
        if (activity == null)
        {
            Log.e(TAG, "Google: Activity is null!");
            return;
        }

        mActivity = activity;
        mGoogleApiClient = googleApiClient;
    }

    public GooglePlayAdapter googlePlayAdapterInit()
    {
        if (mGoogleApiClient == null)
        {
            Log.e(TAG, "googlePlayAdapterInit: mGoogleApiClient is null!");
            return null;
        }

        mGooglePlayAdapter = new GooglePlayAdapter(mActivity, mGoogleApiClient);
        return mGooglePlayAdapter;
    }

    public AuthAdapter authAdapterInit()
    {
        if (mGoogleApiClient == null)
        {
            Log.e(TAG, "authAdapterInit: mGoogleApiClient is null!");
            return null;
        }

        mAuthAdapter = new AuthAdapter(mActivity, mGoogleApiClient);
        return mAuthAdapter;
    }

    public InterstitialAdapter interstitialAdapterInit(String AD_UNIT)
    {
        mInterstitialAdapter = new InterstitialAdapter(mActivity, AD_UNIT);
        return mInterstitialAdapter;
    }

    public RewardedAdapter rewardedAdapterInit(String APP_ID, String UNIT_AD)
    {
        if (mGoogleApiClient == null)
        {
            Log.e(TAG, "rewardedAdapterInit: mGoogleApiClient is null!");
            return null;
        }

        mRewardedAdapter = new RewardedAdapter(mActivity, mGoogleApiClient, APP_ID, UNIT_AD);
        return mRewardedAdapter;
    }

    public FirebaseAdapter firebaseAdapterInit()
    {

        if (mGoogleApiClient == null)
        {
            Log.e(TAG, "FirebaseAdapterInit: mGoogleApiClient is null!");
            return null;
        }

        mFirebaseAdapter = new FirebaseAdapter(mActivity, mGoogleApiClient);
        return mFirebaseAdapter;
    }
}
