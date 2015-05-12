#include <SDL_image.h>

#include "sdltexture.hpp"

#include "main.hpp"

SdlTexture::SdlTexture(SDL_Renderer* renderer, std::string& path):
    m_renderer{renderer}, m_path{path}, m_width{0}, m_height{0}, m_texture{nullptr}
{

    //Load image at specified path
    SDL_Surface* loadedSurface = IMG_Load( m_path.c_str() );
    if( loadedSurface == nullptr ) { throw SdlImgError("Unable to load image " + m_path); }

    //Get image dimensions
    m_width  = loadedSurface->w;
    m_height = loadedSurface->h;

    //Color key image
    SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 0, 0xFF, 0xFF ) );

    //The final texture
    //Create texture from surface pixels
    m_texture = SDL_CreateTextureFromSurface( renderer, loadedSurface );
    if( m_texture == nullptr ) { throw SdlError("Unable to create texture from " + m_path); }

    //Get rid of old loaded surface
    SDL_FreeSurface( loadedSurface );
}

SdlTexture::~SdlTexture()
{
    if( m_texture != nullptr )
	{
		SDL_DestroyTexture( m_texture );
		m_texture = nullptr;
	}
}

void SdlTexture::render( int x, int y,  SDL_Rect* clip )
{
	//Set rendering space and render to screen
	SDL_Rect renderQuad = { x, y, m_width, m_height };

	 //Set clip rendering dimensions
    if( clip != nullptr )
    {
        renderQuad.w = clip->w;
        renderQuad.h = clip->h;
    }

	SDL_RenderCopy( m_renderer, m_texture, clip, &renderQuad );
}

void SdlTexture::setColor(uint8_t red, uint8_t green, uint8_t blue){
    //Modulate texture
	SDL_SetTextureColorMod( m_texture, red, green, blue );
}

void SdlTexture::setAlpha(uint8_t alpha){
   //Modulate texture alpha
   SDL_SetTextureAlphaMod( m_texture, alpha);
}

void SdlTexture::setBlendMode( SDL_BlendMode mode){
    //Set blending function
    SDL_SetTextureBlendMode(m_texture, mode);
}
