//
// Created by yangw on 2018-3-6.
//

#ifndef MYMUSIC_WLPLAYSTATUS_H
#define MYMUSIC_WLPLAYSTATUS_H


class Playstatus {

public:
    bool exit = false;
    bool load = false;
    bool seek = false;
public:
    Playstatus();
    ~Playstatus();
};


#endif //MYMUSIC_WLPLAYSTATUS_H
