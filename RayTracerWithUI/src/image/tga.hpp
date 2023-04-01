#pragma once

#include "../core/core.hpp"

void writeTGA(const std::string& filename, i32 w, i32 h, const u8* pixelsRGB) {
	// Write output as TGA image format
	// https://en.wikipedia.org/wiki/Truevision_TGA
	// https://stackoverflow.com/questions/16636311/what-is-the-simplest-rgb-image-format
	std::ofstream out(filename, std::ios::binary);

	/*
	Field no.   Length     Field name                Description
	1           1 byte     ID length                 Length of the image ID field
	2           1 byte     Color map type            Whether a color map is included
	3           1 byte     Image type                Compression and color types
	4           5 bytes    Color map specification   Describes the color map
	5           10 bytes   Image specification       Image dimensions and format
	*/
	u8 tgameta[18] = { {0} };
	tgameta[02] = 2; // uncompressed true-color image
	tgameta[12] = (w >> 0) & 0xff;
	tgameta[13] = (w >> 8) & 0xff;
	tgameta[14] = (h >> 0) & 0xff;
	tgameta[15] = (h >> 8) & 0xff;
	tgameta[16] = 24; // bits per pixel
	tgameta[17] = 0b00100000;

	// Meta
	out.write((const char*)(tgameta), sizeof(tgameta));

	// write rgb reversed (bgr)
	// since TGA is LITTLE-ENDIAN
	for (i32 y = 0; y < h; ++y)
		for (i32 x = 0; x < w; ++x) {
			i32 index = 3 * (y * w + x);
			const u8 bgr[] = {
				pixelsRGB[index + 2],
				pixelsRGB[index + 1],
				pixelsRGB[index + 0],
			};
			out.write(reinterpret_cast<const char*>(bgr), sizeof(bgr));
		}
}
