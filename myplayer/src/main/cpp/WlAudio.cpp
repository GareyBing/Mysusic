//
// Created by hgb on 18-6-10.
//

#include "WlAudio.h"

WlAudio::WlAudio(WlPlaystatus *playstatus) {
    this->playstatus = playstatus;
    queue = new WLQueue(playstatus);

}

WlAudio::~WlAudio() {

}
