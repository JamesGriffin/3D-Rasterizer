#include <SDL2/SDL.h>
#include <stdio.h>
#include <iostream>
#include <math.h>
#include "display.h"
#include "matrix4.h"
#include "renderer.h"
#include "mesh.h"

// Display Window width and height
const int WIDTH  = 1280;
const int HEIGHT = 720;
const std::string MESH_PATH = "obj/monkey_smooth.obj";

std::string format_number(int n) {
    std::string number = std::to_string(n);
    int insertPosition = number.length() - 3;
    while (insertPosition > 0) {
        number.insert(insertPosition, ",");
        insertPosition-=3;
    }
    return number;
}

int main(int argc, char* argv[]) {

    // Create new display
    Display display(WIDTH, HEIGHT, "3D Rasterizer");

    // Create new Renderer and pass our display
    Renderer renderer(display);

    SDL_Event event;
    display.drawBackground();
    display.update();

    // Load mesh
    Mesh mesh(MESH_PATH);

    float cam_tran_x = 0;
    float cam_tran_y = 0;
    float cam_tran_z = 0;
    float cam_rot_x = 0;
    float cam_rot_y = 0;
    float cam_rot_z = 0;

    bool is_running = true;
    float rot_x = 0.0f;
    float rot_y = 0.0f;
    float tran_x = 0.0f;
    float tran_y = 0.0f;
    float zoom = 1.0f;
    int mouse_clicked = 0;
    bool animate = true;
    bool fill = true;

    // Main loop
    while (is_running) {

        float delta = display.getFrameDelta();
        
        // Poll SDL events
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                is_running = false;
            }
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                if (event.button.button == SDL_BUTTON_LEFT) {
                    mouse_clicked = 1;
                }
                if (event.button.button == SDL_BUTTON_RIGHT) {
                    mouse_clicked = 2;
                }
            }
            if (event.type == SDL_MOUSEBUTTONUP) {
                mouse_clicked = 0;
            }
            if (event.type == SDL_MOUSEWHEEL) {
                if (event.wheel.y == -1) {
                    zoom += 0.05;
                }
                if (event.wheel.y == 1) {
                    zoom -= 0.05;
                }
            }
            if (event.type == SDL_MOUSEMOTION) {
                if (mouse_clicked == 1) {
                    rot_x -= event.motion.xrel * delta;
                    rot_y -= event.motion.yrel * delta;
                }
                if (mouse_clicked == 2) {
                    tran_x += ((float)(event.motion.xrel)) * (4.0/WIDTH);
                    tran_y -= ((float)(event.motion.yrel)) * (4.0/HEIGHT);
                }

            }
            if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        is_running = false;
                    case SDLK_f:
                        fill = !fill;
                        break;
                    case SDLK_r:
                        animate = !animate;
                        break;
                    case SDLK_w:
                        cam_tran_z -= 0.1f;
                        break;
                    case SDLK_s:
                        cam_tran_z += 0.1f;
                        break;
                    case SDLK_a:
                        cam_tran_x += 0.1f;
                        break;
                    case SDLK_d:
                        cam_tran_x -= 0.1f;
                        break;
                    case SDLK_SPACE:
                        cam_tran_y -= 0.1f;
                        break;
                    case SDLK_LCTRL:
                        cam_tran_y += 0.1f;
                        break;
                    case SDLK_LEFT:
                        cam_rot_y -= 0.1f;
                        break;
                    case SDLK_RIGHT:
                        cam_rot_y += 0.1f;
                        break;
                    case SDLK_UP:
                        cam_rot_x += 0.1f;
                        break;
                    case SDLK_DOWN:
                        cam_rot_x -= 0.1f;
                }
            }
        }

        // Draw checkerboard background
        display.drawBackground();

        // Update rotation amount for animation
        if (!mouse_clicked && animate) {
            rot_x -= delta;
        }

        // Create new transformation matrix
        Matrix4 transform =
            Matrix4::initTranslation(tran_x, tran_y, 4 * zoom)
                .mul(Matrix4::initRotation(rot_y, rot_x, 0)
                    .mul(Matrix4::initScale(1, 1, 1))
                );

        transform = Matrix4::initTranslation(cam_tran_x, cam_tran_y, cam_tran_z).mul(Matrix4::initRotation(cam_rot_x, cam_rot_y, cam_rot_z).mul(transform));

        // Draw mesh
        renderer.drawMesh(mesh, transform, 0xFFFFFFFF, fill);

        // Display mesh information
        display.drawText("Triangles: " + format_number(mesh.shapes[0].mesh.indices.size()), 8, HEIGHT - 24);
        display.drawText(MESH_PATH, 8, HEIGHT - 44);

        // Update display and clear zbuffer
        display.update();
        renderer.clearZBuffer();
    }

    return 0;
}
