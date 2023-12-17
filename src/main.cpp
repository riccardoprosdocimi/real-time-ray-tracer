// Support Code written by Michael D. Shah
#include <iostream>
#include "SDLGraphicsProgram.hpp"

int main(int argc, char** argv) {
	SDLGraphicsProgram mySDLGraphicsProgram(1280, 720); // create an instance of an object for an SDLGraphicsProgram
    std::cout << "Press the right arrow key to spin rightward" << std::endl;
    std::cout << "Press the left arrow key to spin leftward" << std::endl;
    std::cout << "Press the down arrow key to reset and stand still" << std::endl;
    std::cout << "Press the 'w' key to toggle wireframe mode" << std::endl;
    std::cout << "Press the 'esc' key or hit the 'x' on the top left corner to exit" << std::endl;
	mySDLGraphicsProgram.loop(); // run our program forever
	return 0;
}
