#include "SDLGraphicsProgram.hpp"

// Constructor
// Returns a true or false value based on successful completion of setup.
// Takes in dimensions of window.
SDLGraphicsProgram::SDLGraphicsProgram(int w, int h) {
	m_window = NULL; // the window we'll be rendering to
    // Window's width and height
    m_width = w;
    m_height = h;
	// Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::cerr << "SDL could not initialize! SDL Error: " << SDL_GetError() << std::endl;
        exit(EXIT_FAILURE);
	}
    // Use OpenGL 3.3 core
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    // We want to request a double buffer for smooth updating
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    // Create window
    m_window = SDL_CreateWindow("Ray Tracer",
                                SDL_WINDOWPOS_UNDEFINED,
                                SDL_WINDOWPOS_UNDEFINED,
                                m_width,
                                m_height,
                                SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    // Check if window did not create
    if (m_window == NULL) {
        std::cerr << "Window could not be created! SDL Error: " << SDL_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }
    // Create an OpenGL graphics context
    m_openGLContext = SDL_GL_CreateContext(m_window);
    if (m_openGLContext == NULL) {
        std::cerr << "OpenGL context could not be created! SDL Error: " << SDL_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }
    // Initialize GLAD library
    if (!gladLoadGLLoader(SDL_GL_GetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        exit(EXIT_FAILURE);
    }
    // If initialization doesn't succeed, then print out a list of errors in the constructor.
    SDL_Log("SDLGraphicsProgram::SDLGraphicsProgram - No SDL, GLAD, or OpenGL errors detected during initialization\n\n");
	// SDL_LogSetAllPriority(SDL_LOG_PRIORITY_WARN); // Uncomment to enable extra debug support
    getOpenGLVersionInfo();
}

// Destructor
// Proper shutdown of SDL and destroy initialized objects
SDLGraphicsProgram::~SDLGraphicsProgram() {
	SDL_DestroyWindow(m_window); // destroy window
	m_window = nullptr; // point m_window to NULL to ensure it points to nothing
	SDL_Quit(); // quit SDL subsystems
}

// Loops forever
void SDLGraphicsProgram::loop() {
    std::shared_ptr<Renderer> renderer = std::make_shared<Renderer>(m_width, m_height); // create a renderer
    renderer -> getCamera() -> setCameraEyePosition(0.0f, 0.0f, 10.0f); // set a default position for our camera
    // Main loop flag
    bool quit = false; // if this is quit = 'true', then the program terminates
    SDL_Event e; // event handler that handles various events in SDL that are related to input and output
    SDL_StartTextInput(); // enable text input
    float cameraSpeed = 5.0f; // set the camera speed for how fast we move
    SDL_WarpMouseInWindow(m_window, m_width / 2, m_height / 2); // center our mouse
    const Uint8* keyboardState = SDL_GetKeyboardState(NULL); // get a pointer to the keyboard state
    // Set the frame per seconds
    const int fps = 55;
    const int frameDelay = 1000 / fps;
    auto startTime = std::chrono::high_resolution_clock::now();
    // while application is running
    while (!quit) {
        auto startTick = SDL_GetTicks();
        // Get the elapsed time in seconds
        auto currentTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsedTime = currentTime - startTime;
        // Handle events on queue
        while(SDL_PollEvent(&e) != 0) {
            // User posts an event to quit
            if (e.type == SDL_QUIT || e.type == SDL_KEYDOWN && e.key.keysym.scancode == SDL_SCANCODE_ESCAPE) { // hit 'x' in the window's corner or press the 'esc' key
                quit = true;
            }
            // Handle keyboard input for the camera class
//            if (e.type == SDL_MOUSEMOTION) {
//                // Handle mouse movements
//                int mouseX = e.motion.x;
//                int mouseY = e.motion.y;
//                renderer -> getCamera() -> mouseLook(mouseX, mouseY);
//            }
        } // End SDL_PollEvent loop.
        // Move leftward or rightward
        if (keyboardState[SDL_SCANCODE_LEFT]) {
            renderer -> getCamera() -> moveLeft(cameraSpeed);
        } else if (keyboardState[SDL_SCANCODE_RIGHT]) {
            renderer -> getCamera() -> moveRight(cameraSpeed);
        }
        // Move forward or backward
        if (keyboardState[SDL_SCANCODE_UP]) {
//            renderer -> getCamera() -> moveForward(cameraSpeed);
        } else if (keyboardState[SDL_SCANCODE_DOWN]) {
            renderer -> getCamera() -> moveBackward(cameraSpeed);
        }
        // Move upward or downward
//        if (keyboardState[SDL_SCANCODE_LSHIFT] || keyboardState[SDL_SCANCODE_RSHIFT]) {
//            renderer -> getCamera() -> moveUp(cameraSpeed);
//        } else if (keyboardState[SDL_SCANCODE_LCTRL] || keyboardState[SDL_SCANCODE_RCTRL]) {
//            renderer -> getCamera() -> moveDown(cameraSpeed);
//        }
        renderer -> render(elapsedTime.count()); // render our ray tracer
      	SDL_GL_SwapWindow(getSDLWindow()); // Update screen of our specified window
        // Keep the frame per seconds constant
        auto endTick = SDL_GetTicks();
        int elapsedTick = endTick - startTick;
        if (elapsedTick < frameDelay) {
            SDL_Delay(frameDelay - elapsedTick);
        }
	}
    SDL_StopTextInput(); // disable text input
}

// Queries OpenGL information
void SDLGraphicsProgram::getOpenGLVersionInfo(){
	SDL_Log("(NOTE: if you have two GPUs, make sure the correct one is selected)");
	SDL_Log("Vendor: %s", (const char*)glGetString(GL_VENDOR));
	SDL_Log("Renderer: %s", (const char*)glGetString(GL_RENDERER));
	SDL_Log("Version: %s", (const char*)glGetString(GL_VERSION));
	SDL_Log("Shading language: %s", (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION));
}
