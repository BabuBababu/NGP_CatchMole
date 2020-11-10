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
	//�̹���

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
	//��Ʈ
	//SDL�� ��Ʈ�� �۵��ϴ� ����� ttf�� �÷��κ��� ���ο� �ؽ�ó�� ����� ��.

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
	//Music�� Channel�� �����Ǿ� ������ �÷��� �ȴ�.
	//�ڹ�׶��� BGM�� Music�� ����ϰ� Ư��ȿ������ WAV�� ����Ѵ�.
	//���������� ������ ������ WAV �Ǵ� MP3�� ����.
	//���� ������ enum SOUNDVOLUME�� ����Ѵ�.
	//WAV�� �� ���� ����ǰ� Music�� �ݺ� ����ȴٰ� ��������.
	//�ڡڡ� WAV, Music�� �ٸ� Ŭ������ �޸� ����ϰ� ����� �����ٰ� �Ǵ��ϸ� ���������
	//free��� �Լ��� ȣ�����ش�.

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
	

	//�ʿ��� ��: TTF, Sound, Button, EditBox, Spritesheet
	//Ÿ�̸Ӵ� SDL_TimerID, SDL_AddTimer, SDL_RemoveTimer�� ����Ѵ�.
	//�ݹ��Լ����� �Է¹��� interval�� ��ȯ�ϸ� ����ؼ� ȣ���̵ȴ�.
	//Ÿ�̸Ӱ� �˾Ƽ� ����Ǹ� ���� RemoveTimer�� �ʿ������, ����ִ� 
	//Ÿ�̸Ӹ� ���̰� �ʹٸ� ȣ�����־�� �Ѵ�.
	
}

//free�� �� ���� �������?
//quit�Լ����� �ý����� �ƿ��ϴµ� �׳� �Ҹ��ڿ��� �Ҵ������� �ðܹ�����
//quit�� ȣ��� ���� ���ҽ� �Ҵ������� �õ��� ������ ���⶧����.

//Ŭ���� ���ο� �ڱ� Ÿ���� ���ҽ��� �����ϴ� ���͸� �ΰ� quit�Լ� �Ǵ�
//���� ��ȯ�� �� ���͸� ��ȸ�ϸ� free�� ȣ���ϰ� ���͸� clear��.