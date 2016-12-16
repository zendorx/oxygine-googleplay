package org.oxygine.googleplay;

import android.app.Activity;
import android.util.Log;

import com.google.android.gms.ads.AdRequest;
import com.google.android.gms.ads.MobileAds;
import com.google.android.gms.ads.reward.RewardItem;
import com.google.android.gms.ads.reward.RewardedVideoAd;
import com.google.android.gms.ads.reward.RewardedVideoAdListener;
import com.google.android.gms.common.api.GoogleApiClient;

import org.oxygine.lib.extension.ActivityObserver;

/**
 * Created by qVadro on 12.12.2016.
 */
public class RewardedAdapter extends ActivityObserver{

    private static String TAG = "RewardedAdapter";

    Activity mActivity;
    GoogleApiClient mGoogleApiClient;
    RewardedVideoAd mRewardedVideoAd;
    String mUnitAd;
    public static native void nativeOnRewardVideoLoaded();
    public static native void nativeOnRewardedVideoAdClosed();
    public static native void nativeOnRewarded();
    public static native void nativeOnRewardedVideoAdFailedToLoad();


    public RewardedAdapter(Activity activity, GoogleApiClient googleApiClient, String appID, String unitAd)
    {
        Log.i(TAG, "RewardedAdapter RewardedAdapter");
        mActivity = activity;
        mGoogleApiClient = googleApiClient;
        mUnitAd = unitAd;

        MobileAds.initialize(mActivity, appID);
        mRewardedVideoAd = MobileAds.getRewardedVideoAdInstance(mActivity);

        mRewardedVideoAd.setRewardedVideoAdListener(new RewardedVideoAdListener() {
            @Override
            public void onRewardedVideoAdLoaded() {
                Log.i(TAG, "RewardedAdapter onRewardedVideoAdLoaded");
                nativeOnRewardVideoLoaded();
            }

            @Override
            public void onRewardedVideoAdOpened() {
                Log.i(TAG, "RewardedAdapter onRewardedVideoAdOpened");
            }

            @Override
            public void onRewardedVideoStarted() {
                Log.i(TAG, "RewardedAdapter onRewardedVideoStarted");
            }

            @Override
            public void onRewardedVideoAdClosed() {
                Log.i(TAG, "RewardedAdapter onRewardedVideoAdClosed");
                nativeOnRewardedVideoAdClosed();
            }

            @Override
            public void onRewarded(RewardItem rewardItem) {
                Log.i(TAG, "RewardedAdapter onRewarded");
                nativeOnRewarded();
            }

            @Override
            public void onRewardedVideoAdLeftApplication() {
                Log.i(TAG, "RewardedAdapter onRewardedVideoAdLeftApplication");
            }

            @Override
            public void onRewardedVideoAdFailedToLoad(int i) {
                Log.i(TAG, "RewardedAdapter onRewardedVideoAdFailedToLoad");
                nativeOnRewardedVideoAdFailedToLoad();
            }
        });

    }

    public void show()
    {
        mActivity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                Log.i(TAG, "RewardedAdapter show");
                mRewardedVideoAd.show();
            }
        });

    }

    public void load(final String UNIT_ID) {

        mActivity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                Log.i(TAG, "RewardedAdapter load");
                String unit_id = UNIT_ID;
                if (unit_id.isEmpty())
                    unit_id = mUnitAd;

                AdRequest.Builder adbuilder = new AdRequest.Builder();
                mRewardedVideoAd.loadAd(unit_id, adbuilder.build());
            }
        });
    }

    public boolean isLoaded()
    {
        //return mRewardedVideoAd.isLoaded();
        return true;
    }

    @Override
    public void onResume()
    {
        mRewardedVideoAd.resume(mActivity);
    }

    @Override
    public void onPause()
    {
        mRewardedVideoAd.pause(mActivity);
    }

    @Override
    public void onDestroy()
    {
        mRewardedVideoAd.destroy(mActivity);
    }


}
