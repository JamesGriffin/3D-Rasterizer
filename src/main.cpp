#include <SDL2/SDL.h>
#include <stdio.h>
#include <iostream>
#include "display.h"
#include "matrix4.h"
#include "renderer.h"
#include "mesh.h"

// Display Window width and height
const int WIDTH  = 800;
const int HEIGHT = 600;

int main(int argc, char* argv[]) {

    // Create new display
    Display display(WIDTH, HEIGHT, "3D Rasterizer");

    // Create new Renderer and pass our display
    Renderer renderer(display);

    // Load mesh
    Mesh mesh("obj/monkey_smooth.obj");

    SDL_Event event;
    Uint32 last_update = SDL_GetTicks();
    display.drawBackground();
    display.update();

    bool is_running = true;
    float rotation = 0.0f;

    // Main loop
    while (is_running) {

        // Poll SDL events
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                is_running = false;
            }
        }
        // Draw checkerboard background
        display.drawBackground();

        // Update rotation amount for animation
        rotation += float(SDL_GetTicks() - last_update) / 1000;
        last_update = SDL_GetTicks();

        // Create new transformation matrix
        Matrix4 transform =
            Matrix4::initTranslation(0, 0, 3.5)
                .mul(Matrix4::initRotation(0, rotation, 0)
                    .mul(Matrix4::initScale(1, 1, 1))
                );

        // Draw mesh
        renderer.drawMesh(mesh, transform, 0xFFFFFFFF);

        // Update display and clear zbuffer
        display.update();
        renderer.clearZBuffer();
    }

    return 0;
}
