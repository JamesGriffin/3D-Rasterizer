#include <cstdlib>
#include "starfield.h"

//  Renders a 3D starfield effect

StarField::StarField(unsigned int numStars, float spread, float speed)
: m_numStars(numStars), m_spread(spread), m_speed(speed) {
    srand(0);
    m_stars = new float[numStars * 3];

    for (size_t i = 0; i < numStars; i++) {
        initStar(i);
    }
}

// Intialise star to new position
void StarField::initStar(int idx) {
    float r = rand() / (RAND_MAX + 1.0f);
    m_stars[3 * idx] = 2 * (r - 0.5f) * m_spread;

    r = rand() / (RAND_MAX + 1.0f);
    m_stars[3 * idx + 1] = 2 * (r - 0.5f) * m_spread;

    r = rand() / (RAND_MAX + 1.0f);
    m_stars[3 * idx + 2] = r + 0.00001f;
}

// Render to display
void StarField::render(Display &display) {
    float half_width = (float)display.getWidth() / 2.0f;
    float half_height = (float)display.getHeight() / 2.0f;

    for (size_t i = 0; i < m_numStars; i++) {
        m_stars[3 * i + 2] -= display.getFrameDelta() * m_speed;

        if (m_stars[3 * i + 2] < 0) {
            initStar(i);
        }

        int x = int((m_stars[3 * i] / m_stars[3 * i + 2]) * half_width + half_width);
        int y = int((m_stars[3 * i + 1] / m_stars[3 * i + 2]) * half_height + half_height);

        if (x <= 0 || x >= display.getWidth() || y <= 0 || y >= display.getHeight()) {
            initStar(i);
        }
        else {
            display.drawPixelFast(x, y, 0xFFFFFFFF);
        }

    }

}

// Clean up
StarField::~StarField() {
    delete[] m_stars;
};
