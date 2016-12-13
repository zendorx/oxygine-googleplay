package org.oxygine.googleplay;

import android.app.Activity;

import com.google.android.gms.common.api.GoogleApiClient;
import com.google.firebase.analytics.FirebaseAnalytics;

/**
 * Created by qVadro on 12.12.2016.
 */
public class FirebaseAdapter {

    private static String TAG = "FirebaseAdapter";

    private Activity mActivity;
    private FirebaseAnalytics mFirebaseAnalytics;
    private GoogleApiClient mGoogleApiClient;

    private static FirebaseAdapter mInstance;

    public static FirebaseAdapter getInstance()
    {
        return mInstance;
    }

    public FirebaseAdapter()
    {

    }
}
