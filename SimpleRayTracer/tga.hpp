#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

////////////////////////////////////////////////////////////////////////////////////

void writeTGA(const std::string& filename, int w, int h, uint8_t* pixels);
void openFileInPaint(const std::string& filename, bool showcmd = false);

////////////////////////////////////////////////////////////////////////////////////

void writeTGA(const std::string& filename, int w, int h, uint8_t* pixels) {
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
	uint8_t tgameta[18] = { {0} };
	tgameta[02] = 2; // uncompressed true-color image
	tgameta[12] = (w >> 0) & 0xff;
	tgameta[13] = (w >> 8) & 0xff;
	tgameta[14] = (h >> 0) & 0xff;
	tgameta[15] = (h >> 8) & 0xff;
	tgameta[16] = 24; // bits per pixel
	tgameta[17] = 0b00100000;

	out.write(reinterpret_cast<char*>(tgameta), sizeof(tgameta));
	out.write(reinterpret_cast<char*>(pixels), sizeof(uint8_t) * 3 * w * h);
}

void openFileInPaint(const std::string& filename, bool showcmd /*=false*/) {
	std::ostringstream cmd;
	cmd << "start paintdotnet:\"" << filename << "\"";
	if (showcmd)
		std::cout << "CMD: " << cmd.str() << "\n";
	system(cmd.str().c_str());
}
