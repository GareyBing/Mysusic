package com.myplayer;

public class WlTimeInfoBean {
    private int currentTime;
    private int totalTime;

    public void setCurrentTime(int currentTime) {
        this.currentTime = currentTime;
    }

    public void setTotalTime(int totalTime) {
        this.totalTime = totalTime;
    }

    @Override
    public String toString() {
        return "WlTimeInfoBean{" +
                "currentTime=" + currentTime +
                ", totalTime=" + totalTime +
                '}';
    }

    public int  getCurrentTime(){
        return currentTime;
    }

    public int getTotalTime(){
        return totalTime;
    }
}
