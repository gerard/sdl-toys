#include <stdio.h>
#include <stdlib.h>
#include "SDL/SDL.h"

#define MAX_SPEED 16

uint32_t refresh_callback(uint32_t interval, void *param)
{
    SDL_UserEvent userevent = {
        .type = SDL_USEREVENT,
        .code = 0,
    };

    SDL_Event event = {
        .type = SDL_USEREVENT,
        .user = userevent,
    };

    SDL_PushEvent(&event);
    return interval;
}

int main(int argc, char *argv[])
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0) {
        fprintf(stderr, "Failed to initialize SDL: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    uint8_t *kstate = SDL_GetKeyState(NULL);
    SDL_AddTimer(30, refresh_callback, NULL);
    SDL_Surface *screen = SDL_SetVideoMode(640, 480, 16, SDL_DOUBLEBUF |
                                                         SDL_HWSURFACE);

    SDL_Rect r = { .w = 32, .h = 32, .x = 0, .y = 0, };
    uint32_t color = SDL_MapRGB(screen->format, 0xFF, 0xFF, 0xFF);

    SDL_Event event;
    int xspeed = 0;
    int yspeed = 0;

    while (SDL_WaitEvent(&event)) {
        //printf("%d: %d\n", SDL_GetTicks(), n_queued_events);
        if (event.type == SDL_QUIT) break;
        if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_q) break;
        if (event.type != SDL_USEREVENT) continue;

        if (r.y < 480 - 32 && kstate[SDLK_DOWN] && !kstate[SDLK_UP]) {
            yspeed = yspeed < MAX_SPEED ? yspeed + 1 : MAX_SPEED;
        } else if (yspeed > 0) {
            yspeed -= 1;
        }

        if (r.y != 0 && kstate[SDLK_UP] && !kstate[SDLK_DOWN]) {
            yspeed = yspeed > -MAX_SPEED ? yspeed - 1 : -MAX_SPEED;
        } else if (yspeed < 0) {
            yspeed += 1;
        }

        r.y += yspeed;
        if (r.y < 0) {
            r.y = -r.y;
            yspeed = -(yspeed + 1);
        } else if (r.y > 480 - 32) {
            r.y = (480 - 32) - (r.y - (480 - 32));
            yspeed = -(yspeed - 1);
        }

        if (r.x != 640 - 32 && kstate[SDLK_RIGHT] && !kstate[SDLK_LEFT]) {
            xspeed = xspeed < MAX_SPEED ? xspeed + 1 : MAX_SPEED;
        } else if (xspeed > 0) {
            xspeed -= 1;
        }

        if (r.x != 0 && kstate[SDLK_LEFT] && !kstate[SDLK_RIGHT]) {
            xspeed = xspeed > -MAX_SPEED ? xspeed - 1 : -MAX_SPEED;
        } else if (xspeed < 0) {
            xspeed += 1;
        }

        r.x += xspeed;
        if (r.x < 0) {
            r.x = 0;
            xspeed = -(xspeed + 1);
        } else if (r.x > 640 - 32) {
            r.x = (640 - 32) - (r.x - (640 - 32));
            xspeed = -(xspeed - 1);
        }

        SDL_FillRect(screen, NULL, 0);
        SDL_FillRect(screen, &r, color);
        SDL_Flip(screen);
    }

    SDL_Quit();
    return EXIT_SUCCESS;
}
