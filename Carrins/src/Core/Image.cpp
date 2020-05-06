#include "Image.h"
#include "Core.h"

#include <png.h>

#include "Instrumentation/Profile.h"

Image::Image(const std::string &filepath)
{
	NIC_PROFILE_FUNCTION();

	FILE *fp = fopen(filepath.c_str(), "rb");
	unsigned char header[8];

	NIC_ASSERT(fp, "File %s could not be opened for reading" << filepath);
	NIC_ASSERT(fread(header, 1, std::size(header), fp) == std::size(header), "fread falied");
	NIC_ASSERT(png_check_sig(header, std::size(header)), "File %s is not recognized as a PNG file");

	png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	NIC_ASSERT(png, "png_create_read_struct failed");
	png_infop info = png_create_info_struct(png);
	NIC_ASSERT(png, "png_create_info_struct failed");

	png_init_io(png, fp);
	png_set_sig_bytes(png, 8);

	png_read_info(png, info);

	m_Width = png_get_image_width(png, info);
	m_Height = png_get_image_height(png, info);
	m_BitDepth = png_get_bit_depth(png, info);

	unsigned char colorType = png_get_color_type(png, info);
	NIC_ASSERT(
	colorType == PNG_COLOR_TYPE_GRAY || 
	colorType == PNG_COLOR_TYPE_GRAY_ALPHA || 
	colorType == PNG_COLOR_TYPE_RGBA,"Color type not suported");

	m_Channels = (colorType == PNG_COLOR_TYPE_RGBA ? 4 : 1);

	png_set_interlace_handling(png);
	if (colorType == PNG_COLOR_TYPE_GRAY_ALPHA)
		png_set_strip_alpha(png);
	if (m_BitDepth == 16)
		png_set_swap(png);
	png_read_update_info(png, info);

	png_bytep *row_pointers = (png_bytep *)malloc(sizeof(png_bytep) * m_Height);
	for (size_t i = 0; i < m_Height; i++)
		row_pointers[i] = (png_byte *)malloc(png_get_rowbytes(png, info));

	png_read_image(png, row_pointers);

	m_ImgBuffer = new unsigned char[m_Width * m_BitDepth / 8 * m_Channels * m_Height];
	for (size_t i = 0; i < m_Height; i++)
		std::memcpy(m_ImgBuffer + i * m_Width * m_BitDepth / 8 * m_Channels, row_pointers[m_Height - 1 - i], m_Width * m_BitDepth / 8 * m_Channels);

	fclose(fp);

	png_destroy_read_struct(&png, &info, NULL);

	for (size_t i = 0; i < m_Height; i++)
		free(row_pointers[i]);
	free(row_pointers);
}

Image::Image(const unsigned width, const unsigned height, const unsigned char channels, const unsigned char bitDepth, const unsigned char *imgBuffer)
		: m_Width(width), m_Height(height), m_Channels(channels), m_BitDepth(bitDepth)
{
	NIC_PROFILE_FUNCTION();

	NIC_ASSERT(!(bitDepth % 8),"bitDepth must be 8 or above multiples");
	NIC_ASSERT(channels == 1 || channels == 4,"Only images with 1 or 4 channels suported");
	m_ImgBuffer = new unsigned char[width * bitDepth / 8 * channels * height];
	std::memcpy(m_ImgBuffer, imgBuffer, width * bitDepth / 8 * channels * height);
}

Image::~Image()
{
	delete[] m_ImgBuffer;
}
