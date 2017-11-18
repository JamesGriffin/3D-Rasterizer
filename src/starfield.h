#pragma once

#include "display.h"

class StarField {
private:
    unsigned int m_numStars;
    float m_spread;
    float m_speed;
    float* m_stars;
public:
    StarField (unsigned int numStars, float spread, float speed);
    void initStar(int idx);
    void render(Display &display);
    virtual ~StarField ();

};
