#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <ctime>
#include "display.h"

Display::Display(int width, int height, std::string title)
: m_width(width), m_height(height), m_title(title) {

    // Initialize SDL
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();

    // Create SDL window
    m_window = SDL_CreateWindow(
        m_title.c_str(),
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        m_width,
        m_height,
        SDL_WINDOW_OPENGL
    );
    //SDL_SetWindowFullscreen(m_window, SDL_WINDOW_FULLSCREEN_DESKTOP);
    // Create renderer
    m_renderer = SDL_CreateRenderer(
        m_window,
        -1,
        SDL_RENDERER_ACCELERATED
    );

    SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_BLEND);

    // Create texture used for background checkerboard
    m_backgroundTexture = SDL_CreateTexture(m_renderer,
        SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, width, height);

    // Initalise pixel buffer to hold background
    Uint32* buffer = new Uint32[width * height];

    // Draw checkerboard to buffer
    for (int x = 0; x < m_width; x++) {
        for (int y = 0; y < m_height; y++) {
            if ((y % (64) < 32)
                == (x % (64) < 32)) {
                buffer[y * width + x] = 0xFF262626;
            }
            else {
                buffer[y * width + x] = 0xFF232323;
            }
        }
    }

    // Copy buffer to texture and delete buffer
    SDL_UpdateTexture(m_backgroundTexture, NULL, buffer, width * sizeof(Uint32));
    delete[] buffer;

    // Create texture used for drawing to the screen
    m_frameTexture = SDL_CreateTexture(m_renderer, SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING, width, height);

    SDL_SetTextureBlendMode(m_frameTexture, SDL_BLENDMODE_BLEND);

    // Initialise framebuffer
    m_frameBuffer = new Uint32[width * height];
    memset(m_frameBuffer, 0x00000000, width * height * sizeof(Uint32));

    // Load font for drawing text
    m_font = TTF_OpenFont("fonts/Quicksand-Medium.ttf", 14);
}

// Draw pixel to directly to display surface at x,y (slow)
void Display::drawPixel(int x, int y, SDL_Color c) {
    SDL_SetRenderDrawColor(m_renderer, c.r, c.g, c.b, c.a);
    SDL_RenderDrawPoint(m_renderer, x, y);
}

// Draw pixel to framebuffer
void Display::drawPixelFast(int x, int y, Uint32 color) {
    if (x >= m_width || y >= m_height) {
        return;
    }
    if (x < 0 || y < 0) {
        return;
    }

    m_frameBuffer[y * m_width + x] = color;
}

// Draw background texture to display sufrace
void Display::drawBackground() {
    SDL_RenderCopy(m_renderer, m_backgroundTexture, NULL, NULL);
}

// Clear display surdace to color c
void Display::clear(SDL_Color c) {
    SDL_SetRenderDrawColor(m_renderer, c.r, c.g, c.b, c.a);
    SDL_RenderClear(m_renderer);
}

// Draw text to display surface at position x,y
void Display::drawText(std::string text, int x, int y) {
    SDL_Color color = { 128, 128, 128 };
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

// Update display
void Display::update() {
    // Copy framebuffer to texture
    SDL_UpdateTexture(m_frameTexture, NULL, m_frameBuffer,
         m_width * sizeof(Uint32));

    // Draw framebuffer texture to display surface
    SDL_RenderCopy(m_renderer, m_frameTexture, NULL, NULL);

    // Draw FPS counter
    drawText(std::to_string(getFrameRate()) + " FPS", 8, 8);

    // Present image
    SDL_RenderPresent(m_renderer);

    // Clear framebuffer
    memset(m_frameBuffer, 0x00000000, m_width * m_height * sizeof(Uint32));

    // Calculate frame time delta for FPS counter
    Uint32 t = SDL_GetTicks();
    m_frameDelta = float(t - m_lastUpdate) / 1000;
    m_lastUpdate = t;

    // Average framerate over a 0.25 second period
    m_frame_counter += m_frameDelta;
    m_frames++;

    if (m_frame_counter >= 0.25) {
        m_frameRate = m_frames / m_frame_counter;
        m_frame_counter = 0;
        m_frames = 0;
    }
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

int Display::getFrameRate() {
    return m_frameRate;
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
