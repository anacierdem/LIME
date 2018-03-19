#pragma once

#include <Awesomium/Surface.h>
#include <SFML/Graphics.hpp>

// A custom Awesomium surface that we composite onto the gameview
// to render a HTML/JS based UI
class CustomSurface : public  Awesomium::Surface
{
public:
	sf::Texture textureToUpdate;
	sf::Uint16 width;
	sf::Uint16 height;

	sf::Sprite tmpSprite;
	sf::RenderTexture renderBuffer;
	sf::RenderTexture renderBuffer2;

	sf::Shader shader;

	///
	/// This event is called whenever the WebView wants to paint a certain section
	/// of the Surface with a block of pixels. It is your responsibility to copy
	/// src_buffer to the location in this Surface specified by dest_rect.
	///
	/// @param src_buffer  A pointer to a block of pixels in 32-bit BGRA format.
	///                    Size of the buffer is `src_row_span * src_rect.height`.
	///                    Beware that src_buffer points to the beginning of the
	///                    transport buffer, you should use src_rect to determine
	///                    the offset to begin copying pixels from.
	///
	/// @param src_row_span  The number of bytes of each row.
	///                      (Usually `src_rect.width * 4`)
	///
	/// @param src_rect  The dimensions of the region of src_buffer to copy from.
	///                  May have a non-zero origin.
	///
	/// @param dest_rect The location to copy src_buffer to. Always has same
	///                  dimensions as src_rect but may have different origin
	///                  (which specifies the offset of the section to copy to).
	///
	void Paint(unsigned char* src_buffer,
		int src_row_span,
		const Awesomium::Rect& src_rect,
		const Awesomium::Rect& dest_rect) override;

	///
	/// This event is called whenever the WebView wants to 'scroll' an existing
	/// section of the Surface by a certain offset. It your responsibility to
	/// translate the pixels within the specified clipping rectangle by the
	/// specified offset.
	///
	/// @param dx  The amount of pixels to offset vertically.
	/// @param dy  The amount of pixels to offset vertically.
	///
	/// @param clip_rect  The rectangle that this operation should be clipped to.
	///
	void Scroll(int dx,
		int dy,
		const Awesomium::Rect& clip_rect) override;

	CustomSurface(sf::Uint16 width, sf::Uint16 height);
};

