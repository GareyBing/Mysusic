package com.hgb.mymusic;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.TextView;

import com.myplayer.Demo;
import com.myplayer.listener.WlOnParparedListener;
import com.myplayer.palyer.WlPlay;

import log.Mylog;

public class MainActivity extends AppCompatActivity {


    private WlPlay wlPlayer;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        wlPlayer = new WlPlay();
        wlPlayer.setWlOnParparedListener(new WlOnParparedListener(){

            //解码
            @Override
            public void onParpare() {
                Mylog.d("MainActivity Parpared..............start play");
                wlPlayer.start();
            }

        });

    }

    public void begin(View view) {
        Log.d("hgb", "begin");
        wlPlayer.setSource("http://mpge.5nd.com/2015/2015-11-26/69708/1.mp3");
        wlPlayer.parpared();
    }
}
