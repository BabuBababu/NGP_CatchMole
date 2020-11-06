#include "SimpleSDL.h"


int main(int argc, char* args[])
{
	if (!SimpleSDL::init(800,800))
	{
		std::cout << "SDL initiation failed" << std::endl;
	}

	
	//SimpleSDL::TTF text = SimpleSDL::TTF(100, 100, 100, 255, 0, 0, 255, "test1.ttf", "Hello World");

	//SimpleSDL::Image jaeo = SimpleSDL::Image(100, 100, 0, 0, 0, 255, "test.png");
	
	//SimpleSDL::EditBox(100, 100, 200, 50, "test2.ttf");
	//SimpleSDL::EditBox(100, 200, 200, 50, "test2.ttf");
	//SimpleSDL::EditBox(100, 300, 200, 50, "test2.ttf");
	
	SimpleSDL::WAV effect = SimpleSDL::WAV("test.wav", SimpleSDL::SOUNDVOLUME::LARGE);
	SimpleSDL::Music bgMusic = SimpleSDL::Music("test.mp3", SimpleSDL::SOUNDVOLUME::LARGE);
	effect.play();
	bool quit = false;
	SDL_Event e;
	//Handle events on queue
	int number = 3;

	int i = 0;
	while (!quit)
	{
		SDL_StartTextInput();
		while (SDL_PollEvent(&e) != 0)
		{
			//User requests quit
			if (e.type == SDL_QUIT)
			{
				quit = true;
			}
			
		}

		SDL_RenderClear(SimpleSDL::gRenderer);


		SDL_RenderPresent(SimpleSDL::gRenderer);
	}
	
	//effect.free();
	//bgMusic.free();
	SimpleSDL::quit();
	return 0;
}