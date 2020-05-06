#pragma once

void InitBumpToNormalMap(const unsigned int width, const unsigned int height);
int KernelBumpToNormalMap(const unsigned short *h_in_img, unsigned char *h_out_img);
void ShutdownBumpToNormalMap();