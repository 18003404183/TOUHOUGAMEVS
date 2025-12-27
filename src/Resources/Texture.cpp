#include "Texture.h"

#include <SDL.h>
#include <SDL_image.h>

void Texture::free_data() {
	if (data) {
		SDL_DestroyTexture(data);
		data = nullptr;
		w = h = 0;
	}
}

bool Texture::load(const std::string& path, ImageData type, SDL_Renderer* renderer)
{
	free_data();

	if (type == ImageData::ImageE) {
		// EasyX no longer supported in this build
		std::cerr << "[Texture] EasyX support removed." << std::endl;
		return false;
	}

	if (type == ImageData::ImageS) {
		if (!renderer) {
			std::cerr << "[Texture] SDL_Renderer required to load texture: " << path << std::endl;
			return false;
		}

		SDL_Surface* surf = IMG_Load(path.c_str());
		if (!surf) {
			std::cerr << "[Texture] IMG_Load failed: " << IMG_GetError() << " path=" << path << std::endl;
			return false;
		}

		SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer,surf);
		if (!tex) {
			std::cerr << "[Texture] SDL_CreateTextureFromSurface failed: " << SDL_GetError() << " path=" << path << std::endl;
			SDL_FreeSurface(surf);
			return false;
		}

		w = surf->w;
		h = surf->h;
		SDL_FreeSurface(surf);

		data = tex;
		return true;
	}

	return false;
}

