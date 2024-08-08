#pragma once

#include <SDL.h>
#include <stdexcept>
#include <string>
#include <iostream>

namespace sdl
{
	class Init
	{
	public:
		~Init();
		Init(Uint32 flags = SDL_INIT_VIDEO);
	};

	class Texture
	{
		SDL_Texture* ptr{ nullptr };
	public:
		Texture(const Texture&) = delete;
		Texture& operator=(const Texture&) = delete;

		~Texture();
		Texture() = delete;
		explicit Texture(SDL_Texture* r);
		operator SDL_Texture* ();

		Texture& operator=(Texture&&) noexcept;
	};

	class Renderer
	{
		SDL_Renderer* ptr{ nullptr };
	public:
		Renderer() = delete;
		Renderer(const Renderer&) = delete;
		Renderer& operator=(const Renderer&) = delete;

		~Renderer(); 
		Renderer(SDL_Window* window, int index = -1, Uint32 flags = 0);
		explicit Renderer(SDL_Renderer* r);
		operator SDL_Renderer* ();

		Texture CreateTexture(Uint32 format, int access, int w, int h) const;
		Texture CreateTextureFromSurface(SDL_Surface* surface) const;
		Texture LoadTextureFromBMP(const char* file) const;
		void RenderCopy(SDL_Texture* texture, const SDL_Rect* srcrect=nullptr, const SDL_Rect* dstrect=nullptr);
		void RenderPresent();
		void RenderClear();
		void SetRenderDrawColor(const SDL_Color& color);
		void RenderDrawLine(const SDL_Point& p1, const SDL_Point& p2);
		void RenderDrawLineF(const SDL_FPoint& p1, const SDL_FPoint& p2);
	};

	class Window
	{
		SDL_Window* ptr{ nullptr };
	public:
		Window() = delete;
		Window(const Window&) = delete;
		Window& operator=(const Window&) = delete;

		~Window();
		/// Parameter order changed from SDL_CreateWindow in order to be lazy with default parameters.
		Window(
			const char* title,
			int w,
			int h,
			Uint32 flags = 0,
			int x = SDL_WINDOWPOS_UNDEFINED,
			int y = SDL_WINDOWPOS_UNDEFINED
		);
		explicit Window(SDL_Window* w);
		operator SDL_Window* ();
		Renderer CreateRenderer(int index = -1, Uint32 flags = 0);
	};

	class Surface
	{
		SDL_Surface* ptr{ nullptr };
	public:
		Surface(const Surface&) = delete;
		Surface& operator=(const Surface&) = delete;

		~Surface();
		explicit Surface(SDL_Surface* r);
		operator SDL_Surface* ();
	};
	
	Surface LoadBMP(const char* file);
}

