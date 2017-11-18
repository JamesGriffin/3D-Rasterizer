#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <ctime>
#include "display.h"

Display::Display(int width, int height, std::string title)
: m_width(width), m_height(height), m_title(title) {

    // Initialize SDL
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();

    // Create SDL window and renderer
    m_window = SDL_CreateWindow(
        m_title.c_str(),
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        m_width,
        m_height,
        SDL_WINDOW_OPENGL
  );

    m_renderer = SDL_CreateRenderer(
        m_window,
        -1,
        SDL_RENDERER_ACCELERATED
    );

    SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_BLEND);

    m_backgroundTexture = SDL_CreateTexture(m_renderer,
        SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, width, height);

    Uint32* buffer = new Uint32[width * height];

    for (int x = 0; x < m_width; x++) {
        for (int y = 0; y < m_height; y++) {
            if ((y % (64) < 32)
                == (x % (64) < 32)) {
                buffer[y * width + x] = 0xFF242424;
            }
            else {
                buffer[y * width + x] = 0xFF2A2A2A;
            }
        }
    }

    SDL_UpdateTexture(m_backgroundTexture, NULL, buffer, width * sizeof(Uint32));
    delete[] buffer;

    m_frameTexture = SDL_CreateTexture(m_renderer, SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING, width, height);
    SDL_SetTextureBlendMode(m_frameTexture, SDL_BLENDMODE_BLEND);
    m_frameBuffer = new Uint32[width * height];
    memset(m_frameBuffer, 0x00000000, width * height * sizeof(Uint32));

    m_font = TTF_OpenFont("fonts/FreeSans.ttf", 14);
}

int Display::getWidth() {
    return m_width;
}

int Display::getHeight() {
    return m_height;
}

float Display::getFrameDelta() {
    return m_frameDelta;
}

// Draw pixel to display at x,y
void Display::drawPixel(int x, int y, SDL_Color c) {
    SDL_SetRenderDrawColor(m_renderer, c.r, c.g, c.b, c.a);
    SDL_RenderDrawPoint(m_renderer, x, y);
}

void Display::drawPixelFast(int x, int y, Uint32 color) {
    if (x >= m_width || y >= m_height) {
        return;
    }
    if (x < 0 || y < 0) {
        return;
    }

    m_frameBuffer[y * m_width + x] = color;
}

void Display::drawBackground() {
    SDL_RenderCopy(m_renderer, m_backgroundTexture, NULL, NULL);
}

void Display::clear(SDL_Color c) {
    SDL_SetRenderDrawColor(m_renderer, c.r, c.g, c.b, c.a);
    SDL_RenderClear(m_renderer);
    // memset(m_frameBuffer, 0, m_width * m_height * sizeof(Uint32));
}

void Display::drawText(std::string text, int x, int y) {
    SDL_Color color = { 255, 255, 255 };
    SDL_Surface * surface = TTF_RenderText_Blended(m_font, text.c_str(), color);
    SDL_Texture * texture = SDL_CreateTextureFromSurface(m_renderer, surface);
    int texW = 0;
    int texH = 0;
    SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);
    SDL_Rect dstrect = { x, y, texW, texH };
    SDL_RenderCopy(m_renderer, texture, NULL, &dstrect);
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
}

void Display::update() {
    SDL_UpdateTexture(m_frameTexture, NULL, m_frameBuffer,
         m_width * sizeof(Uint32));

    SDL_RenderCopy(m_renderer, m_frameTexture, NULL, NULL);

    drawText(std::to_string((int)(1.0f/getFrameDelta())) + " FPS", 8, 8);
    SDL_RenderPresent(m_renderer);

    memset(m_frameBuffer, 0x00000000, m_width * m_height * sizeof(Uint32));

    Uint32 t = SDL_GetTicks();
    m_frameDelta = float(t - m_lastUpdate) / 1000;
    m_lastUpdate = t;
}

Display::~Display() {
    TTF_CloseFont(m_font);
    SDL_DestroyTexture(m_backgroundTexture);
    SDL_DestroyTexture(m_frameTexture);
    SDL_DestroyWindow(m_window);
    SDL_DestroyRenderer(m_renderer);
    delete[] m_frameBuffer;
    TTF_Quit();
    SDL_Quit();
}
