#include "Image.h"
#include "Core.h"

#include <png.h>

Image::Image(const std::string &filepath)
{
	FILE *fp = fopen(filepath.c_str(), "rb");
	unsigned char header[8];

	NIC_ASSERT(fp, "[read_png_file] File %s could not be opened for reading" << filepath);
	fread(header, 1, 8, fp);
	NIC_ASSERT(png_check_sig(header, 8), "[read_png_file] File %s is not recognized as a PNG file");

	png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	NIC_ASSERT(png, "[read_png_file] png_create_read_struct failed");
	png_infop info = png_create_info_struct(png);
	NIC_ASSERT(png, "[read_png_file] png_create_info_struct failed");

	png_init_io(png, fp);
	png_set_sig_bytes(png, 8);

	png_read_info(png, info);

	m_Width = png_get_image_width(png, info);
	m_Height = png_get_image_height(png, info);
	m_BitDepth = png_get_bit_depth(png, info);

	m_Channels = (png_get_color_type(png, info) == PNG_COLOR_TYPE_GRAY ? 1 : 4);

	png_set_interlace_handling(png);
	if (m_BitDepth == 16)
		png_set_swap(png);
	png_read_update_info(png, info);

	png_bytep *row_pointers = (png_bytep *)malloc(sizeof(png_bytep) * m_Height);
	for (size_t i = 0; i < m_Height; i++)
		row_pointers[i] = (png_byte *)malloc(png_get_rowbytes(png, info));

	png_read_image(png, row_pointers);

	m_ImgBuffer = new unsigned char[m_Width * m_BitDepth / 8 * m_Channels * m_Height];
	for (size_t i = 0; i < m_Height; i++)
		std::memcpy(m_ImgBuffer + i * m_Width * m_BitDepth / 8 * m_Channels, row_pointers[i], m_Width * m_BitDepth / 8 * m_Channels);

	fclose(fp);

	png_destroy_read_struct(&png, &info, NULL);

	for (size_t i = 0; i < m_Height; i++)
		free(row_pointers[i]);
	free(row_pointers);
}

Image::Image(const unsigned width, const unsigned height, const unsigned char channels, const unsigned char bitDepth, const unsigned char *imgBuffer)
		: m_Width(width), m_Height(height), m_Channels(channels), m_BitDepth(bitDepth)
{
	m_ImgBuffer = new unsigned char[width * bitDepth / 8 * channels * height];
	std::memcpy(m_ImgBuffer, imgBuffer, width * bitDepth / 8 * channels * height);
}

Image::~Image()
{
	delete[] m_ImgBuffer;
}
