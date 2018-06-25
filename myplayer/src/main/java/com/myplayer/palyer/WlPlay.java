package com.myplayer.palyer;

import android.text.TextUtils;
import android.util.Log;

import com.myplayer.listener.WlOnParparedListener;
import com.myplayer.log.Mylog;

public class WlPlay {

    static {
        System.loadLibrary("native-lib");
        System.loadLibrary("avutil-55");
        System.loadLibrary("swresample-2");
        System.loadLibrary("avcodec-57");
        System.loadLibrary("avformat-57");
        System.loadLibrary("swscale-4");
        System.loadLibrary("postproc-54");
        System.loadLibrary("avfilter-6");
        System.loadLibrary("avdevice-57");
    }

    private String source;

    private WlOnParparedListener wlOnParparedListener;
    public WlPlay (){
        Log.d("hgb","Wlplay");
    }

    public void setSource(String source) {
        this.source = source;
    }

    public void setWlOnParparedListener(WlOnParparedListener wlOnParparedListener) {
        this.wlOnParparedListener = wlOnParparedListener;
    }

    public void parpared() {
        Log.e("hgb","parpared");
        if(TextUtils.isEmpty(source)){
            Mylog.d("source be empty");
            return;
        }

        new Thread(new Runnable() {
            @Override
            public void run() {
                n_parpared(source);
            }
        }).start();
    }

    public void start(){
        Log.d("hgb","Wllay start...");
        if(TextUtils.isEmpty(source)){
            Mylog.d("source be empty");
            return;
        }

        new Thread(new Runnable() {
            @Override
            public void run() {
                Log.d("hgb","n_start();...");
                n_start();
            }
        }).start();
    }


    public void onCallParpare() {
        Log.d("hgb","C++ call java onCallparpare.......");
        if(wlOnParparedListener != null) {
            wlOnParparedListener.onParpare();
        }
    }
    public native void n_parpared(String source);

    public native void n_start();
}
