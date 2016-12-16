package org.oxygine.googleplay;

import android.app.Activity;
import android.util.Log;

import com.chartboost.sdk.Chartboost;
import com.google.android.gms.ads.AdListener;
import com.google.android.gms.ads.AdRequest;
import com.google.android.gms.ads.InterstitialAd;

import org.oxygine.lib.extension.ActivityObserver;

/**
 * Created by qVadro on 12.12.2016.
 */
public class InterstitialAdapter extends ActivityObserver {

    public static String TAG = "InterstitialAdapter";
    public static native void nativeOnAdLoaded();
    public static native void nativeonAdFailedToLoad();
    public static native void nativeonOnAdClosed();

    InterstitialAd mInterstitialAd;
    Activity mActivity;
    static InterstitialAdapter mInstance;

    public static InterstitialAdapter getInstance()
    {
        return mInstance;
    }

    public InterstitialAdapter(Activity activity, String adUnityID)
    {
        Log.i(TAG, "InterstitialAdapter InterstitialAdapter");
        mInstance = this;
        mActivity = activity;

        mInterstitialAd = new InterstitialAd(activity);
        mInterstitialAd.setAdUnitId(adUnityID);

        mInterstitialAd.setAdListener(new AdListener() {
            @Override
            public void onAdLoaded() {
                Log.i(TAG, "InterstitialAdapter onAdLoaded");
                nativeOnAdLoaded();
            }

            @Override
            public void onAdFailedToLoad(int errorCode) {
                Log.i(TAG, "InterstitialAdapter onAdFailedToLoad");
                nativeonAdFailedToLoad();
            }

            @Override
            public void onAdClosed() {
                Log.i(TAG, "InterstitialAdapter onAdClosed");
                nativeonOnAdClosed();
            }
        });

    }


    public void load()
    {
        if (mActivity == null)
        {
            Log.e(TAG, "load: mActivity is null!");
        }

        mActivity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                Log.i(TAG, "InterstitialAdapter load");
                AdRequest.Builder adbuilder = new AdRequest.Builder();
                mInterstitialAd.loadAd(adbuilder.build());
            }
        });
    }

    public void show()
    {
        mActivity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                Log.i(TAG, "InterstitialAdapter show");
                mInterstitialAd.show();
            }
        });
    }

    public boolean isLoaded()
    {
        return mInterstitialAd.isLoaded();
    }

    public boolean isLoading()
    {
        return mInterstitialAd.isLoading();
    }

    @Override
    public void onStart()
    {
        Chartboost.onStart(_activity);
    }

    @Override
    public void onResume()
    {
        Chartboost.onResume(_activity);
    }

    @Override
    public void onPause() {
        Chartboost.onPause(_activity);
    }

    @Override
    public void onStop() {
        Chartboost.onStop(_activity);
    }

    @Override
    public void onDestroy() {
        Chartboost.onDestroy(_activity);
    }

    @Override
    public boolean onBackPressed()
    {
        if (Chartboost.onBackPressed())
            return false;

        return true;
    }

}
