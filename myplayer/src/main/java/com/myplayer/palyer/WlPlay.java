package com.myplayer.palyer;

import android.support.v4.app.NavUtils;
import android.text.TextUtils;
import android.util.Log;

import com.myplayer.WlTimeInfoBean;
import com.myplayer.listener.WlOnParparedListener;
import com.myplayer.listener.WlOnTimeInfoListener;
import com.myplayer.listener.WlOnloadListener;
import com.myplayer.listener.WlOnpauseResumeLintener;
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
    private WlOnloadListener wlOnloadListener;
    public WlOnpauseResumeLintener wlOnpauseResumeLintener;
    private WlOnTimeInfoListener wlOnTimeInfoListener;
    private static WlTimeInfoBean wlTimeInfoBean;

    public void setWlOnpauseResumeLintener(WlOnpauseResumeLintener wlOnpauseResumeLintener) {
        this.wlOnpauseResumeLintener = wlOnpauseResumeLintener;
    }

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

    public void setWlOnloadListener(WlOnloadListener wlOnloadListener) {
        this.wlOnloadListener = wlOnloadListener;
    }

    public void setWlOnTimeInfoListener(WlOnTimeInfoListener wlOnTimeInfoListener) {
        this.wlOnTimeInfoListener = wlOnTimeInfoListener;
    }

    public void parpared() {
        Log.e("hgb","parpared");
        if(TextUtils.isEmpty(source)){
            Mylog.d("source be empty");
            return;
        }

        onCallLoad(true);
        new Thread(new Runnable() {
            @Override
            public void run() {
                Log.e("hgb","source = "+source);
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

    public void pause(){
        n_pause();
        if(wlOnpauseResumeLintener != null){
            wlOnpauseResumeLintener.onPause(true);
        }
    }

    public void resume(){
        n_resume();
        if(wlOnpauseResumeLintener != null){
            wlOnpauseResumeLintener.onPause(false);
        }
    }



    public void onCallParpare() {
        Log.d("hgb","C++ call java onCallparpare.......");
        if(wlOnParparedListener != null) {
            wlOnParparedListener.onParpare();
        }
    }

    public void onCallLoad(Boolean load){
        if(wlOnloadListener != null) {
            wlOnloadListener.onLoad(load);
        }
    }

    public void onCallTimeInfo(int currentTime, int totalTime) {
        if(wlOnTimeInfoListener != null) {
            if(wlTimeInfoBean == null){
                wlTimeInfoBean = new WlTimeInfoBean();
            }
            wlTimeInfoBean.setCurrentTime(currentTime);
            wlTimeInfoBean.setTotalTime(totalTime);
            wlOnTimeInfoListener.onTimeInfo(wlTimeInfoBean);
        }
    }


    private native void n_parpared(String source);

    private native void n_start();
    private native void n_pause();
    private native void n_resume();
}
