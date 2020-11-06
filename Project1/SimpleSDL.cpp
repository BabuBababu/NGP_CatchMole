#include "SimpleSDL.h"


//The window we'll be rendering to
SDL_Window* SimpleSDL::gWindow = nullptr;

//The window renderer
SDL_Renderer* SimpleSDL::gRenderer = nullptr;


bool SimpleSDL::init(int windowWidth, int windowHeight)
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		{
			printf("Warning: Linear texture filtering not enabled!");
		}

		//Create window
		gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, windowWidth, windowHeight, SDL_WINDOW_SHOWN);
		if (gWindow == NULL)
		{
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			//Create renderer for window
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
			if (gRenderer == NULL)
			{
				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags))
				{
					printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
					success = false;
				}
			}
			if (TTF_Init() == -1)
			{
				std::cout << "SDL_ttf could not initialize SDL_ttf Error: " << TTF_GetError() << std::endl;
				success = false;
			}
			//첫 번째 인자는 frequency이고 44100에서 앵간하면 잘 됨.
			//세 번째 인자는 채널의 개수.
			//https://www.youtube.com/watch?v=dVuf1ZKmWug -> 채널이란 무엇인가
			if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
			{
				std::cout << "SDL_mixer could not initialize SDL_mixer Error: " << Mix_GetError() << std::endl;
				success = false;
			}
		}
	}

	return success;
}

void SimpleSDL::quit()
{
	//Destroy window	
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	gRenderer = NULL;

	//SDL 시스템을 종료하기 전에 사용한 리소스를 반납한다.

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
	TTF_Quit();
	Mix_Quit();
}


SimpleSDL::Image::Image(int x, int y, int z, int w, int h, float alpha, std::string filePath)
{
	texture = nullptr;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load(filePath.c_str());
	if (loadedSurface == nullptr)
	{
		printf("Unable to load image %s! SDL_image Error: %s\n", filePath.c_str(), IMG_GetError());
	}
	else
	{
		//Create texture from surface pixels
		texture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
		if (texture == nullptr)
		{
			printf("Unable to create texture from %s! SDL Error: %s\n", filePath.c_str(), SDL_GetError());
		}

		//Get rid of old loaded surface
		SDL_FreeSurface(loadedSurface);
	}
	this->x = x;
	this->y = y;
	this->z = z;
	textureWidth = w;
	textureHeight = h;
	this->alpha = alpha;

}

void SimpleSDL::Image::free()
{
	SDL_DestroyTexture(texture);
	texture = nullptr;
}


void SimpleSDL::Image::draw()
{
	if (textureWidth == 0 && textureHeight == 0)
		SDL_QueryTexture(texture, NULL, NULL, &textureWidth, &textureHeight);
	SDL_Rect rect;
	rect.x = x;
	rect.y = y;
	rect.w = textureWidth;
	rect.h = textureHeight;
	SDL_RenderCopy(gRenderer, texture, NULL, &rect);
}

SimpleSDL::TTF::TTF(int x, int y, int fontSize, Uint8 r, Uint8 g, Uint8 b, Uint8 a, std::string fontLocation, std::string text)
{
	this->x = x;
	this->y = y;
	font = TTF_OpenFont(fontLocation.c_str(), fontSize);
	this->r = r;
	this->g = g;
	this->b = b;
	this->a = a;
	texture = nullptr;
	loadText(text, r, g, b, a);

}

void SimpleSDL::TTF::free()
{
	SDL_DestroyTexture(texture);
	texture = nullptr;
	TTF_CloseFont(font);
	font = nullptr;
	//여기 주석 해제하면 에러나는 원인 찾기
}

void SimpleSDL::TTF::draw()
{
	SDL_Rect destRect;
	destRect.x = this->x;
	destRect.y = this->y;
	destRect.w = this->textWidth;
	destRect.h = this->textHeight;

	SDL_RenderCopy(gRenderer, texture, nullptr, &destRect);
}

bool SimpleSDL::TTF::loadText(std::string text, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
	if (texture != nullptr)
	{
		SDL_DestroyTexture(texture);
		texture = nullptr;
	}
	SDL_Color color;
	color.r = r;
	color.g = g;
	color.b = b;
	color.a = a;
	SDL_Surface* textSurface = TTF_RenderText_Solid(font, text.c_str(), color);
	if (textSurface == nullptr)
	{
		std::cout << "Unable to render text surface SDL_ttf Error: " << TTF_GetError() << std::endl;
	}
	else
	{
		texture = SDL_CreateTextureFromSurface(gRenderer, textSurface);
		if (texture == nullptr)
		{
			std::cout << "Unable to create texture from rendered text SDL Error: " << SDL_GetError() << std::endl;
		}
		textWidth = textSurface->w;
		textHeight = textSurface->h;
		SDL_FreeSurface(textSurface);
	}

	return texture != nullptr;
}
