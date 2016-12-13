package org.oxygine.googleplay;

import android.app.Activity;

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

    Activity mActivity;
    GoogleApiClient mGoogleApiClient;
    RewardedVideoAd mRewardedVideoAd;

    public static native void nativeOnRewardVideoLoaded();
    public static native void nativeOnRewardedVideoAdClosed();
    public static native void nativeOnRewarded();
    public static native void nativeOnRewardedVideoAdFailedToLoad();


    public RewardedAdapter(Activity activity, GoogleApiClient googleApiClient, String appID)
    {
        mActivity = activity;
        mGoogleApiClient = googleApiClient;

        MobileAds.initialize(mActivity, appID);
        mRewardedVideoAd = MobileAds.getRewardedVideoAdInstance(mActivity);

        mRewardedVideoAd.setRewardedVideoAdListener(new RewardedVideoAdListener() {
            @Override
            public void onRewardedVideoAdLoaded() {
                nativeOnRewardVideoLoaded();
            }

            @Override
            public void onRewardedVideoAdOpened() {

            }

            @Override
            public void onRewardedVideoStarted() {

            }

            @Override
            public void onRewardedVideoAdClosed() {
                nativeOnRewardedVideoAdClosed();
            }

            @Override
            public void onRewarded(RewardItem rewardItem) {
                nativeOnRewarded();
            }

            @Override
            public void onRewardedVideoAdLeftApplication() {

            }

            @Override
            public void onRewardedVideoAdFailedToLoad(int i) {
                nativeOnRewardedVideoAdFailedToLoad();
            }
        });

    }

    public void show()
    {
        mActivity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                mRewardedVideoAd.show();
            }
        });

    }

    public void load(final String UNIT_ID) {

        mActivity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                AdRequest.Builder adbuilder = new AdRequest.Builder();
                mRewardedVideoAd.loadAd(UNIT_ID, adbuilder.build());
            }
        });
    }

    public boolean isLoaded()
    {
        return mRewardedVideoAd.isLoaded();
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
