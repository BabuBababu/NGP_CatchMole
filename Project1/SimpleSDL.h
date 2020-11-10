#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <iostream>
#include <string>
#include <vector>

namespace SimpleSDL
{
	//The window we'll be rendering to
	extern SDL_Window* gWindow;

	//The window renderer
	extern SDL_Renderer* gRenderer;

	bool init(int windowWidth, int windowHeight);
	void quit();

	class Image
	{
	public:
		Image() = default;
		Image(int x, int y, int z, int w, int h, int alpha, std::string filePath);
		Image(const Image& other);
		~Image();

		Image& operator=(const Image& other);

		void free();

		void draw();
		void setColor(Uint8 r, Uint8 g, Uint8 b)
		{
			SDL_SetTextureColorMod(texture, r, g, b);
		}
		void setPosition(int x,int y)
		{
			this->x = x;
			this->y = y;
		}
		void setDepth(int z)
		{
			this->z = z;
		}
		void setTextureSize(int w, int h)
		{
			textureWidth = w;
			textureHeight = h;
		}
		void setAlphaValue(Uint8 alpha)
		{
			this->alpha = alpha;
			SDL_SetTextureAlphaMod(texture, alpha);
		}

		std::pair<int, int> getPosition()
		{
			return std::make_pair(x, y);
		}
		int getDepth()
		{
			return z;
		}
		std::pair<int, int>getTextureSize()
		{
			return std::make_pair(textureWidth, textureHeight);
		}
		Uint8 getAlpha()
		{
			return alpha;
		}
	private:
		SDL_Texture* texture;
		int x;
		int y;
		int z;
		int textureWidth;
		int textureHeight;
		Uint8 alpha;
		std::string imageLocation;
	};
	//이미지

	class TTF
	{
	public:
		TTF() = default;
		TTF(int x, int y, int fontSize, Uint8 r, Uint8 g, Uint8 b, Uint8 a, std::string fontLocation, std::string text);
		TTF(const TTF& other);
		TTF& operator=(const TTF& other);
		~TTF();

		void free();

		void draw();
		
		void setPosition(int x, int y)
		{
			this->x = x;
			this->y = y;
		}
		std::pair<int, int> getPosition()
		{
			return std::make_pair(x, y);
		}
		void changeText(std::string content)
		{
			loadText(content, r, g, b, a);
		}
		int getTextWidth()
		{
			int w,h;
			SDL_QueryTexture(texture, nullptr, nullptr, &w, &h);
			return w;
		}
	private:
		TTF_Font* font;
		SDL_Texture* texture;
		int x;
		int y;
		int textWidth;
		int textHeight;
		int fontSize;
		Uint8 r;
		Uint8 g;
		Uint8 b;
		Uint8 a;
		std::string filePath;
		std::string text;
		bool loadText(std::string text, Uint8 r, Uint8 g, Uint8 b, Uint8 a);
		
	};
	//폰트
	//SDL의 폰트가 작동하는 방식은 ttf와 컬러로부터 새로운 텍스처를 만드는 것.

	enum SOUNDVOLUME
	{
		SMALL = 32,
		MEDIUM = 64,
		LARGE = 128
	};

	class WAV
	{
	public:
		WAV() = default;
		WAV(std::string wavLoc, SOUNDVOLUME volume)
		{
			chunk = Mix_LoadWAV(wavLoc.c_str());
			Mix_VolumeChunk(chunk, volume);
		}
		~WAV()
		{
			Mix_FreeChunk(chunk);
		}
		void play()
		{
			Mix_PlayChannel(0, chunk, 0);
		}
		
	private:
		Mix_Chunk* chunk;
	};

	class Music
	{
	public:
		Music() = default;
		Music(std::string musicLoc, SOUNDVOLUME volume)
		{
			music = Mix_LoadMUS(musicLoc.c_str());
			setVolume(volume);
			Mix_PlayMusic(music, -1);
		}
		~Music()
		{
			Mix_FreeMusic(music);
			music = nullptr;
		}

		void pause()
		{
			Mix_PauseMusic();
		}
		void resume()
		{
			Mix_ResumeMusic();
		}
		void setVolume(int volume)
		{
			Mix_VolumeMusic(volume);
		}
	private:
		Mix_Music* music;
	};
	//Music은 Channel과 독립되어 별도로 플레이 된다.
	//★백그라운드 BGM은 Music을 사용하고 특수효과음은 WAV를 사용한다.
	//음악파일의 형식은 가급적 WAV 또는 MP3를 쓴다.
	//볼륨 설정은 enum SOUNDVOLUME을 사용한다.
	//WAV는 한 번만 실행되고 Music은 반복 실행된다고 생각하자.
	//★★★ WAV, Music은 다른 클래스와 달리 사용하고 사용이 끝났다고 판단하면 명시적으로
	//free멤버 함수를 호출해준다.

	class EditBox
	{
	public:
		EditBox() = default;
		EditBox(int x, int y, int w, int h, std::string fontLoc);
		EditBox(const EditBox& other);
		~EditBox() = default;
		void handleEvent(SDL_Event& e);
		void setFocused(bool value)
		{
			isFocused = value;
		}
		void draw();
		std::string getContent()
		{
			return content;
		}
		static std::vector<EditBox*> boxes;
	private:
		int x;
		int y;
		int boxWidth;
		int boxHeight;
		int caretX;
		std::string content;
		TTF container;
		bool isFocused;
	};
	

	//필요한 것: TTF, Sound, Button, EditBox, Spritesheet
	//타이머는 SDL_TimerID, SDL_AddTimer, SDL_RemoveTimer를 사용한다.
	//콜백함수에서 입력받은 interval을 반환하면 계속해서 호출이된다.
	//타이머가 알아서 종료되면 따로 RemoveTimer할 필요없지만, 살아있는 
	//타이머를 죽이고 싶다면 호출해주어야 한다.
	
}

//free를 왜 따로 만들었나?
//quit함수에서 시스템을 아웃하는데 그냥 소멸자에게 할당해제를 맡겨버리면
//quit이 호출된 이후 리소스 할당해제를 시도해 오류가 나기때문임.

//클래스 내부에 자기 타입의 리소스를 관리하는 벡터를 두고 quit함수 또는
//씬이 전환될 때 벡터를 순회하며 free를 호출하고 벡터를 clear함.