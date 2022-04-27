// images.h
// contains image buffers for simple space invaders
// Jonathan Valvano, March 5, 2018
// Capture image dimensions from BMP files

#ifndef __images_h
#define __images_h
#include <stdint.h>

const unsigned short title[] = {
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xF7DD, 0xFFDD, 0xFFDD, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0xEFDD, 0x001F, 0x001F, 0xFFDD, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xEFDD, 0x001F, 0x001F, 0x001F, 0xFFDD, 0xFFDD, 0xFFDD, 0xFFDD,
 0xFFDD, 0xFFDD, 0xFFDD, 0xFFDD, 0xFFDD, 0xFFDD, 0xFFDD, 0xFFDD, 0xFFDD, 0xFFDD, 0xFFDD, 0xFFDD, 0xFFDD, 0xFFDD, 0xFFDD, 0xFFDD,
 0xFFDD, 0xFFDD, 0xFFDD, 0xFFDD, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xEFDD,
 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0xFFDD, 0xFD86, 0xFD86, 0xFD86,
 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFFDD, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xEFDD, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F,
 0x001F, 0x001F, 0x001F, 0xFFDD, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86,
 0xFD86, 0xFD86, 0xFD86, 0xFFDD, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xF7DD, 0x001F, 0x001F,
 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0xFFDD, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86,
 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFFDD, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0xF7DD, 0x001F, 0x001F, 0xF797, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F,
 0x001F, 0xFFDD, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86,
 0xFD86, 0xFD86, 0xFFDD, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xF7DD, 0x001F, 0x001F, 0xF797, 0xF797,
 0xF797, 0xF797, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0xFFDD, 0xFD86, 0xFD86, 0xFD86, 0xF797, 0xF797, 0xF797, 0xF797,
 0xF797, 0xF797, 0xF797, 0xF797, 0xF797, 0xF797, 0xF797, 0xFD86, 0xFD86, 0xFFDD, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0xFFDD, 0x001F, 0x001F, 0xF797, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0xFFDD,
 0xFD86, 0xFD86, 0xFD86, 0xF797, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFF75, 0xFD86, 0xFD86,
 0xFFDD, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFFDD, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F,
 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0xFFDD, 0xFD86, 0xFD86, 0xFD86, 0xF797, 0xFD86, 0xF797, 0xF797, 0xF797, 0xFD86,
 0xF797, 0xF797, 0xF797, 0xFD86, 0xFF54, 0xFD86, 0xFD86, 0xFFDD, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0xFFDD, 0x001F, 0x001F, 0xF797, 0xF797, 0xF797, 0xF797, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0xFFDD, 0xFD86, 0xFD86,
 0xFD86, 0xF797, 0xFD86, 0xF797, 0xF797, 0xF797, 0xFD86, 0xF797, 0xF797, 0xF797, 0xFD86, 0xFF74, 0xFD86, 0xFD86, 0xFFDD, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFFDD, 0x001F, 0x001F, 0xF797, 0x001F, 0xF797, 0x001F, 0x001F, 0x001F,
 0x001F, 0x001F, 0x001F, 0x001F, 0xFFDD, 0xFD86, 0xFD86, 0xFD86, 0xF797, 0xFD86, 0xFD86, 0xF797, 0xFD86, 0xFD86, 0xFD86, 0xF797,
 0xFD86, 0xFD86, 0xFF76, 0xFD86, 0xFD86, 0xFFDD, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFFDD, 0x001F,
 0x001F, 0x001F, 0xF797, 0xF797, 0xF797, 0x001F, 0xF797, 0xF797, 0xF797, 0xF797, 0x001F, 0xFFDD, 0xFD86, 0xFD86, 0xFD86, 0xFD86,
 0xF797, 0xFD86, 0xFD86, 0xFD86, 0xF797, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xF776, 0xFD86, 0xFD86, 0xFFDD, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFFDD, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0xF797, 0x001F, 0xF797,
 0x001F, 0x001F, 0xFFDD, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xF797, 0xF797, 0xF797, 0xFD86, 0xF797, 0xF797, 0xF797, 0xF797,
 0xFD86, 0xFD86, 0xFD86, 0xFFDD, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFFDD, 0x001F, 0x001F, 0xF797,
 0xF797, 0xF797, 0xF797, 0x001F, 0xF797, 0xF797, 0xF797, 0xF797, 0x001F, 0xFFDD, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86,
 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFFDD, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0xFFDD, 0x001F, 0x001F, 0xF797, 0x001F, 0x001F, 0xF797, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F,
 0xFFDD, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xF797, 0xF797, 0xF797, 0xF797, 0xF797, 0xF797, 0xF797, 0xF797, 0xFD86,
 0xFD86, 0xFD86, 0xFFDD, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFFDD, 0x001F, 0x001F, 0xF797, 0xF797, 0xF797,
 0xF797, 0x001F, 0xF797, 0xF797, 0xF797, 0xF797, 0x001F, 0xFFDD, 0xFD86, 0xFD86, 0xFD86, 0xF797, 0xF797, 0xF797, 0xF797, 0xFD86,
 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xF797, 0xF797, 0xFD86, 0xFD86, 0xFD86, 0xFFDD, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0xFFDD, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0xF797, 0xF797, 0x001F, 0x001F, 0xFFDD, 0xFD86,
 0xFD86, 0xFD86, 0xF797, 0xFD86, 0xFD86, 0xFD86, 0xF797, 0xF797, 0xF797, 0xF797, 0xF797, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86,
 0xFFDD, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFFDD, 0x001F, 0x001F, 0xF797, 0xF797, 0x001F, 0x001F, 0x001F,
 0xF797, 0x001F, 0x001F, 0xF797, 0x001F, 0xFFDD, 0xFD86, 0xFD86, 0xFD86, 0xF797, 0xFD86, 0xFD86, 0xFD86, 0xF797, 0xF797, 0xF797,
 0xF797, 0xF797, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFFDD, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFFDD,
 0x001F, 0x001F, 0x001F, 0xF797, 0xF797, 0xF797, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0xFFDD, 0xFD86, 0xFD86, 0xFD86,
 0xF797, 0xF797, 0xF797, 0xF797, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xF797, 0xF797, 0xFD86, 0xFD86, 0xFD86, 0xFFDD, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFFDD, 0x001F, 0x001F, 0xF797, 0xF797, 0x001F, 0x001F, 0x001F, 0xF797, 0xF797,
 0xF797, 0xF797, 0x001F, 0xFFDD, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xF797, 0xF797, 0xF797, 0xF797, 0xF797,
 0xF797, 0xF797, 0xFD86, 0xFD86, 0xFD86, 0xFFDD, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFFDD, 0x001F, 0x001F,
 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0xF797, 0x001F, 0x001F, 0xFFDD, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86,
 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFFDD, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0xFFDD, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0xF797, 0xF797, 0xF797, 0xF797,
 0x001F, 0xFFDD, 0xFD86, 0xFD86, 0xFD86, 0xF797, 0xF797, 0xF797, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86,
 0xFD86, 0xFD86, 0xFD86, 0xFFDD, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFFDD, 0x001F, 0x001F, 0x001F, 0x001F,
 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0xFFDD, 0xFD86, 0xFD86, 0xFD86, 0xF797, 0xFD86, 0xF797, 0xF797,
 0xF797, 0xF797, 0xF797, 0xF797, 0xF797, 0xF797, 0xF797, 0xFD86, 0xFD86, 0xFD86, 0xFFDD, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0xFFDD, 0x001F, 0x001F, 0xF797, 0xF797, 0xF797, 0xF797, 0x001F, 0xF797, 0x001F, 0x001F, 0xF797, 0x001F, 0xFFDD,
 0xFD86, 0xFD86, 0xFD86, 0xF797, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xF797, 0xFD86, 0xFD86,
 0xFD86, 0xFD86, 0xFFDD, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFFDD, 0x001F, 0x001F, 0xF797, 0x001F, 0xF797, 0x001F,
 0x001F, 0xF797, 0xF797, 0xF797, 0xF797, 0x001F, 0xFFDD, 0xFD86, 0xFD86, 0xFD86, 0xF797, 0xFD86, 0xF797, 0xF797, 0xF797, 0xF797,
 0xF797, 0xF797, 0xF797, 0xF797, 0xF797, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFFDD, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0xFFDD, 0x001F, 0x001F, 0xF797, 0xF797, 0xF797, 0xF797, 0x001F, 0xF797, 0x001F, 0x001F, 0xF797, 0x001F, 0xFFDD, 0xFD86, 0xFD86,
 0xFD86, 0xF797, 0xF797, 0xF797, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86,
 0xFFDD, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFFDD, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F,
 0x001F, 0x001F, 0x001F, 0x001F, 0xFFDD, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86,
 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFFDD, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFFDD, 0x001F,
 0x001F, 0xF797, 0xF797, 0xF797, 0xF797, 0x001F, 0xF797, 0xF797, 0xF797, 0xF797, 0x001F, 0xFFDD, 0xFD86, 0xFD86, 0xFD86, 0xF797,
 0xF797, 0xF797, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFFDD, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFFDD, 0x001F, 0x001F, 0x001F, 0xF797, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F,
 0xF797, 0x001F, 0xFFDD, 0xFD86, 0xFD86, 0xFD86, 0xF797, 0xFD86, 0xF797, 0xF797, 0xF797, 0xF797, 0xF797, 0xF797, 0xF797, 0xF797,
 0xF797, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFFDD, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFFDD, 0x001F, 0x001F, 0x001F,
 0x001F, 0xF797, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0xFFDD, 0xFD86, 0xFD86, 0xFD86, 0xF797, 0xFD86, 0xFD86,
 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xF797, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFFDD, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0xFFDD, 0x001F, 0x001F, 0xF797, 0xF797, 0xF797, 0xF797, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F,
 0xFFDD, 0xFD86, 0xFD86, 0xFD86, 0xF797, 0xFD86, 0xF797, 0xF797, 0xF797, 0xF797, 0xF797, 0xF797, 0xF797, 0xF797, 0xF797, 0xFD86,
 0xFD86, 0xFD86, 0xFD86, 0xFFDD, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFFDD, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F,
 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0xFFDD, 0xFD86, 0xFD86, 0xFD86, 0xF797, 0xF797, 0xF797, 0xFD86, 0xFD86,
 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFFDD, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0xFFDD, 0x001F, 0x001F, 0xF797, 0xF797, 0xF797, 0xF797, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0xFFDD, 0xFD86,
 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86,
 0xFD86, 0xFFDD, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFFDD, 0x001F, 0x001F, 0xF797, 0x001F, 0x001F, 0xF797, 0x001F,
 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0xFFDD, 0xFD86, 0xFD86, 0xFD86, 0xF797, 0xF797, 0xF797, 0xF797, 0xF797, 0xF797, 0xF797,
 0xF797, 0xF797, 0xF797, 0xF797, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFFDD, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFFDD,
 0x001F, 0x001F, 0x001F, 0xF797, 0xF797, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0xFFDD, 0xFD86, 0xFD86, 0xFD86,
 0xF797, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xF797, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFFDD,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFFDD, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F,
 0x001F, 0x001F, 0x001F, 0xFFDD, 0xFD86, 0xFD86, 0xFD86, 0xF797, 0xF797, 0xF797, 0xF797, 0xF797, 0xF797, 0xF797, 0xF797, 0xF797,
 0xFD86, 0xF797, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFFDD, 0xFFDD, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFFDD, 0x001F, 0x001F,
 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0xFFDD, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86,
 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xF797, 0xFD86, 0xF797, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFFDD, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0xFFDD, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F,
 0x001F, 0xFFDD, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xF797, 0xF797, 0xF797,
 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFFDD, 0x0000, 0x0000, 0x0000, 0x0000, 0xFFDD, 0x001F, 0x001F, 0x001F, 0x001F,
 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0xFFDD, 0xFD86, 0xFD86, 0xFD86, 0xF797, 0xF797, 0xF797, 0xF797,
 0xF797, 0xF797, 0xF797, 0xF797, 0xF797, 0xF797, 0xF797, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFFDD, 0x0000, 0x0000,
 0x0000, 0x0000, 0xFFDD, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0xFFDD,
 0xFD86, 0xFD86, 0xFD86, 0xF797, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xF797, 0xFD86, 0xFD86,
 0xFD86, 0xFD86, 0xFD86, 0xFFDD, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFFDD, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F,
 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0xFFDD, 0xFD86, 0xFD86, 0xFD86, 0xF797, 0xFD86, 0xF797, 0xF797, 0xF797, 0xFD86,
 0xF797, 0xF797, 0xF797, 0xFD86, 0xF797, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFFDD, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0xFFDD, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0xFFDD, 0xFD86, 0xFD86,
 0xFD86, 0xF797, 0xFD86, 0xF797, 0xFD86, 0xF797, 0xFD86, 0xF797, 0xFD86, 0xFF95, 0xFD86, 0xF797, 0xFD86, 0xFD86, 0xFD86, 0xFD86,
 0xFFDD, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFFDD, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F,
 0x001F, 0x001F, 0x001F, 0x001F, 0xFFDD, 0xFD86, 0xFD86, 0xFD86, 0xF797, 0xFD86, 0xF797, 0xFD86, 0xF797, 0xF797, 0xF797, 0xFD86,
 0xFF95, 0xFD86, 0xF797, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFFDD, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFFDD, 0x001F,
 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0xFFDD, 0xFD86, 0xFD86, 0xFD86, 0xF797,
 0xF797, 0xF797, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xF795, 0xF795, 0xF797, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFFDD, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFFDD, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x0000, 0x001F,
 0x001F, 0x001F, 0xFFDD, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xF797, 0xF797, 0xFD86, 0xF5C4, 0xFD86, 0xFD86, 0xF797, 0xF797,
 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFFDD, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFFDD, 0x001F, 0x001F, 0x001F,
 0x001F, 0x001F, 0x001F, 0x001F, 0x0000, 0x0000, 0x001F, 0x001F, 0x001F, 0xFFDD, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xF797, 0xFD86,
 0xFD86, 0xF797, 0xF797, 0xFD86, 0xFD86, 0xF797, 0xF797, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFFDD, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0xFFDD, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x0000, 0x0000, 0x001F, 0x001F, 0x001F,
 0xFFDD, 0xFD86, 0xFD86, 0xFD86, 0xF797, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xF797, 0xF797, 0xF797, 0xFD86, 0xF797, 0xFD86,
 0xFD86, 0xFD86, 0xFD86, 0xFFDD, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFFDD, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F,
 0x001F, 0x001F, 0x0000, 0x0000, 0x001F, 0x001F, 0x001F, 0xFFDD, 0xFD86, 0xFD86, 0xFD86, 0xF797, 0xFD86, 0xFD86, 0xF797, 0xF797,
 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xF797, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFFDD, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0xFFDD, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x0000, 0x0000, 0x001F, 0x001F, 0x001F, 0xFFDD, 0xFD86,
 0xFD86, 0xFD86, 0xF797, 0xFD86, 0xFD86, 0xF797, 0xFD86, 0xF797, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xF797, 0xFD86, 0xFD86, 0xFD86,
 0xFD86, 0xFFDD, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFFDD, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F,
 0x0000, 0x0000, 0x001F, 0x001F, 0x001F, 0xFFDD, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xF797, 0xFD86, 0xFD86, 0xF797, 0xFD86, 0xF797,
 0xFD86, 0xFD86, 0xF797, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFFDD, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFFDD,
 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x0000, 0x0000, 0x001F, 0x001F, 0x001F, 0xFFDD, 0xFD86, 0xFD86, 0xFD86,
 0xFD86, 0xFD86, 0xF797, 0xF797, 0xFD86, 0xFD86, 0xFD86, 0xF797, 0xF797, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFFDD,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFFDD, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x0000, 0x0000,
 0x001F, 0x001F, 0x001F, 0xFFDD, 0xFD86, 0xFD86, 0xFD86, 0xF797, 0xF797, 0xF797, 0xF797, 0xF797, 0xF797, 0xF797, 0xF797, 0xF797,
 0xF797, 0xF797, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFFDD, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFFDD, 0x001F, 0x001F,
 0x001F, 0x001F, 0x001F, 0x001F, 0x0000, 0x0000, 0x0000, 0x001F, 0x001F, 0x001F, 0xFFDD, 0xFD86, 0xFD86, 0xFD86, 0xF797, 0xFD86,
 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xF797, 0xFD86, 0xFD86, 0xFD86, 0xFFDD, 0xFFDD, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0xFFDD, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x0000, 0x0000, 0x0000, 0x001F, 0x001F,
 0x001F, 0xFFDD, 0xFD86, 0xFD86, 0xFD86, 0xF797, 0xF797, 0xF797, 0xF797, 0xF797, 0xFD86, 0xF797, 0xF797, 0xF797, 0xF797, 0xF797,
 0xFD86, 0xFD86, 0xFD86, 0xFFDD, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFFDD, 0x001F, 0x001F, 0x001F, 0x001F,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x001F, 0x001F, 0x001F, 0xFFDD, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86,
 0xF797, 0xFD86, 0xF797, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFFDD, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0xFFDD, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x0000, 0x0000, 0x0000, 0x0000, 0x001F, 0x001F, 0x001F, 0xFFDD,
 0xFD86, 0xFD86, 0xFD86, 0xF797, 0xF797, 0xF797, 0xF797, 0xF797, 0xFD86, 0xF797, 0xF797, 0xF797, 0xF797, 0xF797, 0xFD86, 0xFD86,
 0xFD86, 0xFFDD, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFFDD, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F,
 0x0000, 0x0000, 0x0000, 0x001F, 0x001F, 0x001F, 0xFFDD, 0xFD86, 0xFD86, 0xFD86, 0xF797, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86,
 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xF797, 0xFD86, 0xFD86, 0xFD86, 0xFFDD, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0xFFDD, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x0000, 0x0000, 0x0000, 0x001F, 0x001F, 0x001F, 0xFFDD, 0xFD86, 0xFD86,
 0xFD86, 0xF797, 0xF797, 0xF797, 0xF797, 0xF797, 0xF797, 0xF797, 0xF797, 0xF797, 0xF797, 0xF797, 0xFD86, 0xFD86, 0xFD86, 0xFFDD,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFFDD, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x0000, 0x0000, 0x0000,
 0x0000, 0x001F, 0x001F, 0x001F, 0xFFDD, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86,
 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFFDD, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFFDD, 0x001F,
 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x0000, 0x0000, 0x0000, 0x001F, 0x001F, 0x001F, 0xFFDD, 0xFD86, 0xFD86, 0xFD86, 0xF797,
 0xF797, 0xF797, 0xF797, 0xF797, 0xF797, 0xF797, 0xF797, 0xF797, 0xF797, 0xF797, 0xFD86, 0xFD86, 0xFD86, 0xFFDD, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFFDD, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x0000, 0x0000, 0x0000, 0x001F,
 0x001F, 0x001F, 0xFFDD, 0xFD86, 0xFD86, 0xFD86, 0xF797, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86,
 0xF797, 0xFD86, 0xFD86, 0xFD86, 0xFFDD, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFFDD, 0x001F, 0x001F, 0x001F,
 0x001F, 0x001F, 0x001F, 0x001F, 0x0000, 0x0000, 0x001F, 0x001F, 0x001F, 0xFFDD, 0xFD86, 0xFD86, 0xFD86, 0xF797, 0xF797, 0xF797,
 0xF797, 0xF797, 0xF797, 0xF797, 0xF797, 0xF797, 0xF797, 0xF797, 0xFD86, 0xFD86, 0xFD86, 0xFFDD, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0xFFDD, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x0000, 0x0000, 0x001F, 0x001F, 0x001F,
 0xFFDD, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86,
 0xFD86, 0xFFDD, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFFDD, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F,
 0x001F, 0x001F, 0x0000, 0x0000, 0x001F, 0x001F, 0x001F, 0xFFDD, 0xFD86, 0xFD86, 0xFD86, 0xF797, 0xF797, 0xF797, 0xF797, 0xF797,
 0xF797, 0xF797, 0xF797, 0xF797, 0xF797, 0xF797, 0xFD86, 0xFD86, 0xFFDD, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0xFFDD, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x0000, 0x0000, 0x001F, 0x001F, 0x001F, 0xFFDD, 0xFD86,
 0xFD86, 0xFD86, 0xF797, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xF797, 0xFD86, 0xFD86, 0xFFDD,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFFDD, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F,
 0x0000, 0x0000, 0x001F, 0x001F, 0x001F, 0xFFDD, 0xFD86, 0xFD86, 0xFD86, 0xF797, 0xFD86, 0xF797, 0xF797, 0xF797, 0xFD86, 0xF797,
 0xF797, 0xF797, 0xF797, 0xF797, 0xFD86, 0xFD86, 0xFFDD, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFFDD,
 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x0000, 0x0000, 0x001F, 0x001F, 0x001F, 0xFFDD, 0xFD86, 0xFD86, 0xFD86,
 0xF797, 0xFD86, 0xF797, 0xF797, 0xF797, 0xFD86, 0xF797, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFFDD, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFFDD, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x0000, 0x0000, 0x0000,
 0x001F, 0x001F, 0x001F, 0xFFDD, 0xFD86, 0xFD86, 0xFD86, 0xF797, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xF797, 0xFD86, 0xFD86, 0xFD86,
 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFFDD, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFFDD, 0x001F, 0x001F,
 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x0000, 0x0000, 0x001F, 0x001F, 0x001F, 0xFFDD, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xF797,
 0xF797, 0xF797, 0xF797, 0xF797, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFFDD, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0xFFDD, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x0000, 0x001F, 0x001F,
 0x001F, 0xFFDD, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86,
 0xFD86, 0xFD86, 0xFFDD, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFFDD, 0x001F, 0x001F, 0x001F, 0x001F,
 0x001F, 0x001F, 0x001F, 0x001F, 0x0000, 0x001F, 0x001F, 0x001F, 0xFFDD, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86,
 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFFDD, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0xFFDD, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0xFFDD,
 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFFDD,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFFDD, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F,
 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0xFFDD, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86,
 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFD86, 0xFFDD, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0xFFDD, 0x001F, 0x001F, 0x001F, 0xFFDD, 0xFFDD, 0xFFDD, 0xFFDD, 0xFFDD, 0xFFDD, 0xFFDD, 0xFFDD, 0xFFDD, 0xFFDD, 0xFFDD, 0xFFDD,
 0xFFDD, 0xFFDD, 0xFFDD, 0xFFDD, 0xFFDD, 0xFFDD, 0xFFDD, 0xFFDD, 0xFFDD, 0xFFDD, 0xFFDD, 0xFFDD, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFFDD, 0x001F, 0x001F, 0xFFDD, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFFDD, 0xFFDD,
 0xFFDD, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
};

#endif /* __images_h */
