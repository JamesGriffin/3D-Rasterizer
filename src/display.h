#pragma once

#include <string>
#include <ctime>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>


class Display {
private:
    int m_width;
    int m_height;
    std::string m_title;
    SDL_Window *m_window;
    SDL_Renderer *m_renderer;
    SDL_Texture* m_backgroundTexture;
    SDL_Texture* m_frameTexture;
    TTF_Font * m_font;
    Uint32* m_frameBuffer;
    Uint32 m_lastUpdate = 0;
    float m_frameDelta  = 0;
    int m_frames = 0;
    int m_frameRate = 0;
    float m_frame_counter = 0;    
public:
    Display (int width, int height, std::string title);
    void drawBackground();
    void clear(SDL_Color c);
    void drawPixel(int x, int y, SDL_Color c);
    void drawPixelFast(int x, int y, Uint32);
    void drawText(std::string text, int x, int y);
    void update();
    int getWidth();
    int getHeight();
    float getFrameDelta();
    int getFrameRate();
    virtual ~Display ();

};
