#include "SDL2_RAII.h"

namespace sdl
{

	Init::Init(Uint32 flags)
	{
		if ( SDL_Init(flags) != 0 )
		{
			throw std::runtime_error{
				std::string{"Couldn't Init SDL:\nSDL Error:\n"} + SDL_GetError()
			};
		}
	}
	Init::~Init()
	{
		SDL_Quit();
#ifndef NDEBUG
		std::clog << "SDL_Quit() called\n";
#endif
	}



	Window::Window(const char* title, int w, int h, Uint32 flags, int x, int y)
		: ptr(SDL_CreateWindow(title, x, y, w, h, flags))
	{
		if ( !ptr )
			throw std::runtime_error{
				std::string{"Couldn't create Window:\nSDL Error:\n"} + SDL_GetError()
		};
	}

	Window::Window(SDL_Window* w) : ptr{ w } {}

	Window::~Window()
	{
		if ( ptr )
		{
			SDL_DestroyWindow(ptr);
#ifndef NDEBUG
			std::clog << "Window destroyed\n";
#endif
		}
	}

	Window::operator SDL_Window* () { return ptr; }

	Renderer Window::CreateRenderer(int index, Uint32 flags)
	{
		return Renderer{ this->ptr, index, flags };
	}



	Renderer::Renderer(SDL_Window* window, int index, Uint32 flags)
		:ptr{ SDL_CreateRenderer(window,index,flags) }
	{
		if ( !ptr )
			throw std::runtime_error{
				std::string{"Couldn't create Renderer:\nSDL Error:\n"} + SDL_GetError()
		};
	}

	Renderer::Renderer(SDL_Renderer* r) : ptr{ r } {}

	Renderer::operator SDL_Renderer* () { return ptr; }

	Renderer::~Renderer()
	{
		if ( ptr )
		{
			SDL_DestroyRenderer(ptr);
#ifndef NDEBUG
			std::clog << "Renderer deleted\n";
#endif
		}
	}

	Texture Renderer::CreateTexture(Uint32 format, int access, int w, int h) const
	{
		SDL_Texture* temp{ SDL_CreateTexture(this->ptr, format, access, w, h) };
		if ( !temp )
		{
			throw std::runtime_error{
				std::string{"Couldn't create Texture:\nSDL Error:\n"} + SDL_GetError()
			};
		}
		return Texture{ temp };
	}

	Texture Renderer::CreateTextureFromSurface(SDL_Surface* surface) const
	{
		SDL_Texture* temp{ SDL_CreateTextureFromSurface(this->ptr, surface) };
		if ( !temp )
		{
			throw std::runtime_error{
				std::string{"Couldn't create Texture From Surface:\nSDL Error:\n"} + SDL_GetError()
			};
		}
		return Texture{ temp };
	}

	Texture Renderer::LoadTextureFromBMP(const char* file) const
	{
		sdl::Surface temp{ LoadBMP(file) };
		if ( !temp )
		{
			throw std::runtime_error{
				std::string{"Couldn't LoadBMP:\nSDL Error:\n"} + SDL_GetError()
			};
		}
		return CreateTextureFromSurface(temp);
	}

	void Renderer::RenderCopy(SDL_Texture* texture, const SDL_Rect* srcrect, const SDL_Rect* dstrect)
	{
		int result{ SDL_RenderCopy(this->ptr, texture, srcrect, dstrect) };
		if ( result != 0 )
			throw std::runtime_error{
				std::string{"Couldn't copy texture to renderer:\nSDL Error:\n"} + SDL_GetError()
		};
	}

	void Renderer::RenderPresent()
	{
		SDL_RenderPresent(this->ptr);
	}

	void Renderer::RenderClear()
	{
		int result{ SDL_RenderClear(this->ptr) };
		if ( result != 0 )
			throw std::runtime_error{
				std::string{"Couldn't Clear renderer:\nSDL Error:\n"} + SDL_GetError()
		};
	}

	void Renderer::SetRenderDrawColor(const SDL_Color& color)
	{
		int result{ SDL_SetRenderDrawColor(this->ptr, color.r, color.g, color.b, color.a) };
		if ( result != 0 )
			throw std::runtime_error{
				std::string{"Couldn't Set renderer draw color:\nSDL Error:\n"} + SDL_GetError()
		};
	}

	void Renderer::RenderDrawLine(const SDL_Point& p1, const SDL_Point& p2)
	{
		int result{ SDL_RenderDrawLine(this->ptr, p1.x,p1.y,p2.x,p2.y) };
		if ( result != 0 )
			throw std::runtime_error{
				std::string{"Couldn't RenderDrawLine:\nSDL Error:\n"} + SDL_GetError()
		};
	}

	void Renderer::RenderDrawLineF(const SDL_FPoint& p1, const SDL_FPoint& p2)
	{
		int result{ SDL_RenderDrawLineF(this->ptr, p1.x,p1.y,p2.x,p2.y) };
		if ( result != 0 )
			throw std::runtime_error{
				std::string{"Couldn't RenderDrawLineF:\nSDL Error:\n"} + SDL_GetError()
		};
	}



	Surface::Surface(SDL_Surface* r) : ptr{ r } {}

	Surface::operator SDL_Surface* () { return ptr; }

	Surface::~Surface()
	{
		if ( ptr )
		{
			SDL_FreeSurface(ptr);
#ifndef NDEBUG
			std::clog << "Surface freed\n";
#endif
		}
	}

	Surface LoadBMP(const char* file)
	{
		SDL_Surface* temp{ SDL_LoadBMP(file) };
		if ( !temp )
			throw std::runtime_error{
				std::string{"Couldn't LoadBMP:\nSDL Error:\n"} + SDL_GetError()
		};
		return Surface{ temp };
	}



	Texture::Texture(SDL_Texture* r) : ptr{ r } {}

	Texture::operator SDL_Texture* () { return ptr; }

	Texture& Texture::operator=(Texture&& other) noexcept
	{
		if ( &other == this ) return *this;

		auto temp = other.ptr;
		other.ptr = ptr;
		ptr = temp;

#ifndef NDEBUG
		std::clog << "Texture move-assigned\n";
#endif

		return *this;
	}

	Texture::~Texture()
	{
		if ( ptr )
		{
			SDL_DestroyTexture(ptr);
#ifndef NDEBUG
			std::clog << "Texture destroyed\n";
#endif
		}
	}

}
