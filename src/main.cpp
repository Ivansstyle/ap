#include <iostream>
#define LINUX

#ifdef __APPLE__
                        //MacIncludes
#include <SDL2/SDL.h>
#else

#include <SDL2/SDL.h>
#endif

#include "particle.h"
#include "particlesystem.h"

#define WINDOW_TITLE "SMOKE PS"

// These defines are for the initial window size (it can be changed in the resize function)
#define WIDTH 1280
#define HEIGHT 720

//Window
SDL_Window* gWindow = NULL;

//OpenGL context
SDL_GLContext gContext;

// Creating base particle system
ParticleSystem *particlesystem = NULL;




int initSDL()
{
    //Initialize SDL
    if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_TIMER ) < 0 )
    {
        printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
        return EXIT_FAILURE;
    }
    else
    {
    #ifdef __APPLE__ //Use OpenGL 2.1 core for MacOsX in Compability Mode

        SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 2 );
        SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 1 );
        SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY );
    #elif __linux__

   //Use OpenGL 3.1 core for LINUX
        SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 3 );
        SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 1 );
        SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );

    #endif
        //Create window
        gWindow = SDL_CreateWindow( WINDOW_TITLE,
                                    SDL_WINDOWPOS_UNDEFINED,
                                    SDL_WINDOWPOS_UNDEFINED,
                                    WIDTH,
                                    HEIGHT,
                                    SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
        if( gWindow == NULL )
        {
            printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
            return EXIT_FAILURE;
        }
    }
    return EXIT_SUCCESS;
}

/**
 * @brief timerCallback an SDL2 callback function which will trigger whenever the timer has hit the elapsed time.
 * @param interval The elapsed time (not used - scene uses it's own internal clock)
 * @return the elapsed time.
 */
Uint32 timerCallback(Uint32 interval, void *)
{
    if (particlesystem != NULL)
    {
      particlesystem->update();

    }
    return interval;
}


void handleInput(SDL_Event* _e)  { particlesystem->controls.TakeControls( _e); }


/**
 * @brief main The main opengl loop is managed here
 * @param argc Not used
 * @param args Not used
 * @return EXIT_SUCCESS if it went well!
 */
int main( int argc, char* args[] ) {

    //Start up SDL and create window
    if( initSDL() == EXIT_FAILURE ) return EXIT_FAILURE;

    //Create context
    gContext = SDL_GL_CreateContext( gWindow );
    if( gContext == NULL ) return EXIT_FAILURE;
    else std::cout<<"GL_CreateContext => created context context is != 0 "<<std::endl;

    //Use Vsync
    if( SDL_GL_SetSwapInterval( 1 ) < 0 ) {
        fprintf(stderr, "Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError() );
    }

    // We should now be ready to use OpenGL
    // This object holds our scene. It needs to be initialised before it can be drawn.

    particlesystem = new ParticleSystem();

    // Initialise the Particle System
    particlesystem->init();

    if (!particlesystem->isInit()) std::cout<<"particleSystem initilisation failed"<<std::endl;

    // Need an initial resize to make sure the projection matrix is initialised
    particlesystem->resize(WIDTH, HEIGHT);

    // Use a timer to update our scene. This is the best way to handle updates,
    // as the timer runs in a separate thread and is therefore not affected by the
    // rendering performance.
    SDL_TimerID timerID = SDL_AddTimer(10, /*elapsed time in milliseconds*/
                                     timerCallback, /*callback function*/
                                     (void*) NULL /*parameters (none)*/);

    //Main loop flag
    bool quit = false;

    //Event handler
    SDL_Event e;

    //Enable text input
    SDL_StartTextInput();

    //While application is running
    while( !quit )
    {
        //Handle events on queue
        while( SDL_PollEvent( &e ) != 0 )
        {
            // The window has been resized
            if ((e.type == SDL_WINDOWEVENT) &&
                    (e.window.event == SDL_WINDOWEVENT_RESIZED)) {
                SDL_SetWindowSize(gWindow, e.window.data1, e.window.data2);
                particlesystem->resize(e.window.data1, e.window.data2);
            }
            //User requests quit
            else if( e.type == SDL_QUIT ) {
                quit = true;
            }

            handleInput(&e);
        }

        //Render the scene
        //scene->draw();


        particlesystem->draw();

        //Update screen

        SDL_GL_SwapWindow( gWindow );
    }
    //MAINLOOP END

    //Disable text input
    SDL_StopTextInput();

    // Disable our timer
    SDL_RemoveTimer(timerID);

    // Delete particlesystem
    delete particlesystem;

    //Destroy window
    SDL_DestroyWindow( gWindow );

    //Quit SDL subsystems
    SDL_Quit();

    return EXIT_SUCCESS;
}


