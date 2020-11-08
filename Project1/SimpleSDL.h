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
		Image() = delete;
		Image(int x, int y, int z, int w, int h, int alpha, std::string filePath);
		Image(const Image& other);
		~Image();

		void free();

		void draw();
		void setColor(Uint8 r, Uint8 g, Uint8 b)
		{
			SDL_SetTextureColorMod(texture, r, g, b);
		}
		void enableAlphaBlending()
		{
			SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
			SDL_SetTextureAlphaMod(texture, alpha);
		}
		void disableAlphaBlending()
		{
			SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_NONE);
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
	};
	//�̹���

	class TTF
	{
	public:
		TTF() = default;
		TTF(int x, int y, int fontSize, Uint8 r, Uint8 g, Uint8 b, Uint8 a, std::string fontLocation, std::string text);
		TTF(const TTF& other);
		
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
		EditBox(int x,int y, int w,int h, std::string fontLoc):container(x, y + h / 8, h / 2, 0, 0, 0, 255, fontLoc.c_str(), "")
		{
			this->x = x;
			this->y = y;
			boxWidth = w;
			boxHeight = h;
			caretX = x;
			content = "";
			isFocused = false;
			boxes.push_back(*this);
		}
		EditBox(const EditBox& other)
		{
			x = other.x;
			y = other.y;
			boxWidth = other.boxWidth;
			boxHeight = other.boxHeight;
			caretX = other.caretX;
			content = other.content;
			isFocused = other.isFocused;
			this->container = other.container;
		}
		~EditBox() = default;
		void handleEvent(SDL_Event&e)
		{
			if (e.type == SDL_MOUSEBUTTONDOWN)
			{
				int mx, my;
				SDL_GetMouseState(&mx, &my);
				if ((mx > x && mx < x + boxWidth) && (my > y && my < y + boxHeight))
				{
					for (auto& box : boxes)
					{
						box.setFocused(false);
					}
					isFocused = true;
				}
			}
			else if (isFocused&&e.type==SDL_KEYDOWN&&e.key.keysym.sym == SDLK_BACKSPACE && content.length() > 0)
			{
				content.pop_back();
				container.changeText(content);
				caretX=container.getTextWidth()+x;
				if (content.length() == 0)
					caretX = x;
			}
			else if (isFocused&&e.type == SDL_TEXTINPUT)
			{
				content += e.text.text;
				container.changeText(content);
				caretX = container.getTextWidth() + x;
			}
		}
		void setFocused(bool value)
		{
			isFocused = value;
		}
		void draw()
		{
			SDL_Rect myRect;
			myRect.x = x;
			myRect.y = y;
			myRect.w = boxWidth;
			myRect.h = boxHeight;
			SDL_SetRenderDrawColor(SimpleSDL::gRenderer, 0, 0, 0, 255);
			SDL_RenderDrawRect(SimpleSDL::gRenderer, &myRect);
			if (isFocused)
			{
				myRect.x = caretX;
				myRect.y += myRect.h/4;
				myRect.w = 5;
				myRect.h /= 2;
				SDL_RenderFillRect(SimpleSDL::gRenderer, &myRect);
			}
			SDL_SetRenderDrawColor(SimpleSDL::gRenderer, 255, 255, 255, 255);
			
			container.draw();
		}
		std::string getContent()
		{
			return content;
		}
		static std::vector<EditBox> boxes;
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