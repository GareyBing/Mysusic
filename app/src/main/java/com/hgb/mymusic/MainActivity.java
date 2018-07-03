package com.hgb.mymusic;

import android.os.Handler;
import android.os.Message;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.TextView;

import com.myplayer.Demo;
import com.myplayer.WlTimeInfoBean;
import com.myplayer.listener.WlOnParparedListener;
import com.myplayer.listener.WlOnTimeInfoListener;
import com.myplayer.listener.WlOnloadListener;
import com.myplayer.listener.WlOnpauseResumeLintener;
import com.myplayer.palyer.WlPlay;
import com.myplayer.util.WlTimeUtil;

import log.Mylog;

public class MainActivity extends AppCompatActivity {


    private WlPlay wlPlayer;
    private TextView tvTime;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        tvTime = findViewById(R.id.tv_time);


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
        wlPlayer.setWlOnTimeInfoListener(new WlOnTimeInfoListener() {
            @Override
            public void onTimeInfo(WlTimeInfoBean timeInfoBean) {
        //        Mylog.d(timeInfoBean.toString());
                Message message = Message.obtain();
                message.what = 1;
                message.obj = timeInfoBean;

                handler.sendMessage(message);
            }
        });
    }


        public void begin (View view){
            Log.d("hgb", "begin");
           final String url1 = "http://mpge.5nd.com/2015/2015-11-26/69708/1.mp3";
            final  String url = "http://win.web.rg01.sycdn.kuwo.cn/298a9ea07970fa73c0f6f99a9e255ac9/5b3aa99c/resource/n2/26/77/1173528610.mp3";
            // wlPlayer.setSource("/mnt/sdcard/other/1.mp3");
            wlPlayer.setSource(url1);
            wlPlayer.parpared();
        }

        public void pause (View view){
            wlPlayer.pause();
        }

        public void resume (View view){
            wlPlayer.resume();
        }

        Handler handler = new Handler(){
            @Override
            public void handleMessage(Message msg) {
                super.handleMessage(msg);

                if(msg.what == 1){
                    WlTimeInfoBean wlTimeInfoBean = (WlTimeInfoBean) msg.obj;
                    tvTime.setText(WlTimeUtil.secdsToDateFormat(wlTimeInfoBean.getTotalTime(),
                            wlTimeInfoBean.getTotalTime()) + "/" + WlTimeUtil.secdsToDateFormat(
                                    wlTimeInfoBean.getCurrentTime(), wlTimeInfoBean.getCurrentTime()
                            ));
                }
            }
        };

    }

