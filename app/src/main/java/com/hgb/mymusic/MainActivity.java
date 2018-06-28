package com.hgb.mymusic;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.TextView;

import com.myplayer.Demo;
import com.myplayer.listener.WlOnParparedListener;
import com.myplayer.listener.WlOnloadListener;
import com.myplayer.listener.WlOnpauseResumeLintener;
import com.myplayer.palyer.WlPlay;

import log.Mylog;

public class MainActivity extends AppCompatActivity {


    private WlPlay wlPlayer;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        wlPlayer = new WlPlay();
        wlPlayer.setWlOnParparedListener(new WlOnParparedListener() {

            //解码
            @Override
            public void onParpare() {
                Mylog.d("MainActivity Parpared..............start play");
                wlPlayer.start();
            }

        });

        wlPlayer.setWlOnloadListener(new WlOnloadListener() {
            @Override
            public void onLoad(Boolean load) {
                if (load) {
                    Mylog.d("Loading...");
                } else {
                    Mylog.d("Playing...");
                }
            }
        });

        wlPlayer.setWlOnpauseResumeLintener(new WlOnpauseResumeLintener() {
            @Override
            public void onPause(boolean pause) {
                if (pause) {
                    Mylog.d("pause....");
                }else {
                    Mylog.d("Playback....");
                }
            }
        });
    }


        public void begin (View view){
            Log.d("hgb", "begin");
            wlPlayer.setSource("http://mpge.5nd.com/2015/2015-11-26/69708/1.mp3");
            // wlPlayer.setSource("/mnt/sdcard/other/1.mp3");
            wlPlayer.parpared();
        }

        public void pause (View view){
            wlPlayer.pause();
        }

        public void resume (View view){
            wlPlayer.resume();
        }
    }

