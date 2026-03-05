/*
 * bmp.h
 *
 *  Created on: Mar 5, 2026
 *      Author: Abhinav
 */

#ifndef INC_BMP_H_
#define INC_BMP_H_

typedef struct
{
	char         sign[2];
	unsigned int size;
	unsigned int reserved;
	unsigned int offset;

} __attribute__((packed)) b_header_t;

typedef struct
{
	unsigned int   size;
	unsigned int   width;
	unsigned int   height;
	unsigned short num_of_planes;
	unsigned short bits_per_pixel;
	unsigned int   compression;
	unsigned int   image_size;
	unsigned int   x_res;
	unsigned int   y_res;
	unsigned int   num_of_colors;
	unsigned int   imp_colors;

} __attribute__((packed)) b_dib_t;

typedef struct
{
	unsigned char blue;
	unsigned char green;
	unsigned char red;
	unsigned char reserved;

} __attribute__((packed)) b_pixel_t;



#endif /* INC_BMP_H_ */
