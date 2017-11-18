#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <iostream>
#include <ctime>
#include "display.h"
#include "starfield.h"
#include "vertex.h"
#include "matrix4.h"
#include "renderer.h"
#include "../lib/OBJ_Loader/OBJ_Loader.h"

const int WIDTH  = 800;
const int HEIGHT = 600;
const bool CULL_BACKFACES = false;

int main(int argc, char* argv[]) {

    Display display(WIDTH, HEIGHT, "3D Rasterizer");
    Renderer renderer(display);
    //StarField starfield(4096, 10.0f, 0.08f);

    objl::Loader loader;
    loader.LoadFile("obj/monkey_smooth.obj");

    SDL_Event event;

    bool is_running = true;
    long count = 0;

    Matrix4 proj = Matrix4::initPerspective(70,
        (float)WIDTH / (float)HEIGHT, 0.1f, 1000.0f);

    Matrix4 transfom = Matrix4::initScreenSpaceTransform(WIDTH, HEIGHT);

    Uint32 last_update = SDL_GetTicks();

    float d = 0.0f;
    while (is_running) {

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                is_running = false;
        }

        display.drawBackground();

        d += float(SDL_GetTicks() - last_update) / 1000;
        last_update = SDL_GetTicks();

        Matrix4 tran = Matrix4::initTranslation(0, 0, 4.0).mul(
            Matrix4::initRotation(0, d, 0).mul(Matrix4::initScale(1, 1, 1)));

        tran = transfom.mul(proj).mul(tran);
        #pragma omp parallel for
        for (size_t i = 0; i < loader.LoadedIndices.size(); i+=3) {

            unsigned int i1 = loader.LoadedIndices[i];
            unsigned int i2 = loader.LoadedIndices[i + 1];
            unsigned int i3 = loader.LoadedIndices[i + 2];

            Vertex v1 = Vertex(
                loader.LoadedVertices[i1].Position.X,
                loader.LoadedVertices[i1].Position.Y,
                loader.LoadedVertices[i1].Position.Z
            ).transform(tran).perspectiveDivide();

            Vertex v2 = Vertex(
                loader.LoadedVertices[i2].Position.X,
                loader.LoadedVertices[i2].Position.Y,
                loader.LoadedVertices[i2].Position.Z
            ).transform(tran).perspectiveDivide();

            Vertex v3 = Vertex(
                loader.LoadedVertices[i3].Position.X,
                loader.LoadedVertices[i3].Position.Y,
                loader.LoadedVertices[i3].Position.Z
            ).transform(tran).perspectiveDivide();

            Vector4 s1 = v1.getPos() - v2.getPos();
            Vector4 s2 = v1.getPos() - v3.getPos();

            Vector4 norm = s2.cross(s1);
            norm.w = 0;

            if (CULL_BACKFACES && (norm.dot(Vector4(0, 0, 1.0)) <= 0)) {
                renderer.drawTriangleOutline(v1, v2, v3, 0xFF00FF32);
            }
            else {
                renderer.drawTriangleOutline(v1, v2, v3, 0xFF00FF32);
            }

        }

        //last_update = clock();
        display.update();
        count++;
    }

    return 0;
}
