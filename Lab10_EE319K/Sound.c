// Sound.c
// Runs on any TM4C123
// Sound assets based off the original Space Invaders 
// Import these constants into your SpaceInvaders.c for sounds!
// Jonathan Valvano
// 11/15/2021 
#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"
#include "Sound.h"
#include "DAC.h"
#include "Timer0.h"

#define TIMER_PRIO 4

const uint8_t Hit[2000] = {
  120, 119, 119, 119, 120, 120, 129, 130, 133, 129, 125, 119, 119, 119, 125, 128, 135, 137, 133, 123, 
  109, 99, 91, 92, 101, 116, 135, 140, 143, 130, 123, 105, 96, 89, 92, 105, 115, 116, 120, 119, 
  130, 133, 139, 149, 163, 171, 174, 173, 161, 143, 133, 115, 99, 79, 72, 75, 79, 82, 87, 103, 
  125, 143, 139, 137, 137, 149, 163, 173, 168, 153, 133, 111, 105, 106, 109, 119, 137, 149, 140, 133, 
  133, 143, 154, 168, 183, 187, 188, 184, 173, 163, 143, 113, 75, 39, 5, 0, 0, 3, 9, 19, 
  46, 82, 105, 120, 135, 139, 144, 173, 195, 208, 211, 202, 191, 181, 181, 202, 236, 255, 255, 255, 
  255, 255, 240, 212, 192, 171, 130, 85, 47, 36, 41, 37, 33, 43, 57, 51, 33, 3, 0, 0, 
  0, 0, 5, 9, 24, 67, 120, 157, 177, 201, 222, 239, 250, 246, 236, 216, 192, 159, 101, 41, 
  0, 0, 0, 0, 0, 0, 58, 103, 129, 161, 192, 218, 239, 242, 221, 184, 163, 157, 157, 159, 
  163, 161, 129, 81, 37, 17, 7, 19, 41, 61, 89, 139, 178, 195, 205, 202, 184, 161, 153, 157, 
  161, 181, 187, 177, 164, 149, 161, 197, 246, 255, 255, 255, 255, 255, 252, 226, 173, 105, 39, 0, 
  0, 0, 19, 71, 101, 133, 168, 183, 174, 163, 139, 113, 99, 77, 47, 31, 36, 41, 39, 43, 
  77, 125, 154, 149, 128, 95, 65, 57, 79, 99, 130, 161, 173, 192, 221, 246, 255, 235, 181, 115, 
  48, 0, 0, 0, 19, 58, 115, 171, 216, 216, 177, 119, 67, 43, 39, 31, 24, 17, 12, 15, 
  27, 79, 149, 226, 255, 255, 255, 255, 255, 255, 255, 240, 207, 157, 77, 3, 0, 0, 0, 0, 
  0, 0, 41, 72, 79, 67, 67, 95, 115, 125, 128, 120, 125, 147, 188, 222, 255, 255, 255, 255, 
  255, 255, 249, 255, 255, 255, 255, 255, 202, 140, 85, 29, 0, 0, 0, 0, 0, 46, 113, 129, 
  113, 79, 41, 17, 27, 61, 79, 92, 103, 103, 106, 115, 125, 154, 212, 255, 255, 255, 255, 255, 
  255, 222, 163, 115, 85, 96, 128, 163, 192, 207, 195, 143, 72, 0, 0, 0, 0, 0, 0, 0, 
  0, 53, 135, 201, 255, 255, 255, 255, 255, 222, 183, 144, 106, 63, 0, 0, 0, 0, 0, 0, 
  0, 29, 133, 192, 212, 207, 216, 245, 255, 255, 255, 249, 211, 205, 221, 250, 255, 255, 255, 255, 
  212, 140, 106, 105, 135, 167, 195, 218, 216, 192, 164, 135, 115, 105, 96, 92, 95, 103, 109, 119, 
  115, 85, 47, 15, 15, 47, 87, 115, 135, 149, 159, 188, 232, 255, 255, 255, 255, 255, 255, 255, 
  255, 255, 183, 106, 55, 17, 27, 58, 87, 129, 174, 218, 239, 215, 161, 95, 31, 0, 0, 0, 
  2, 17, 39, 87, 129, 147, 157, 168, 183, 188, 168, 115, 65, 31, 29, 33, 29, 36, 53, 61, 
  65, 70, 85, 92, 82, 82, 75, 58, 41, 48, 58, 72, 103, 149, 192, 229, 239, 226, 201, 164, 
  144, 143, 147, 137, 119, 103, 87, 85, 115, 161, 221, 255, 255, 246, 236, 218, 208, 183, 150, 128, 
  111, 101, 92, 99, 116, 135, 144, 149, 157, 163, 164, 164, 167, 159, 154, 154, 163, 173, 183, 187, 
  187, 191, 187, 177, 167, 161, 150, 137, 119, 106, 101, 99, 99, 96, 103, 105, 99, 92, 79, 70, 
  70, 71, 75, 77, 77, 82, 101, 123, 137, 144, 154, 163, 174, 177, 173, 173, 161, 153, 137, 111, 
  82, 53, 29, 22, 29, 37, 58, 87, 106, 119, 133, 149, 163, 173, 174, 164, 147, 137, 135, 133, 
  135, 139, 135, 120, 95, 72, 65, 58, 65, 75, 85, 99, 119, 130, 140, 147, 143, 135, 137, 153, 
  168, 168, 171, 161, 139, 123, 106, 113, 135, 171, 207, 235, 255, 255, 255, 255, 255, 221, 154, 77, 
  17, 0, 3, 46, 79, 91, 103, 120, 128, 119, 113, 92, 70, 51, 31, 17, 7, 12, 27, 48, 
  81, 130, 187, 212, 202, 171, 139, 113, 106, 116, 119, 129, 140, 140, 153, 171, 191, 195, 173, 130, 
  92, 57, 23, 13, 22, 36, 58, 99, 143, 177, 183, 157, 119, 85, 71, 77, 89, 91, 79, 71, 
  71, 79, 119, 171, 216, 255, 255, 249, 236, 225, 211, 202, 191, 173, 143, 82, 19, 0, 0, 0, 
  0, 0, 0, 46, 72, 79, 67, 70, 95, 113, 123, 123, 120, 123, 144, 178, 205, 240, 255, 255, 
  255, 255, 246, 236, 246, 255, 255, 255, 252, 205, 150, 101, 43, 0, 0, 0, 0, 0, 48, 109, 
  123, 105, 71, 36, 15, 24, 58, 77, 95, 103, 101, 106, 113, 128, 154, 212, 255, 255, 255, 255, 
  255, 255, 222, 163, 115, 85, 96, 128, 163, 192, 207, 195, 143, 72, 0, 0, 0, 0, 0, 0, 
  0, 0, 53, 135, 201, 255, 255, 255, 255, 255, 222, 183, 144, 106, 63, 0, 0, 0, 0, 0, 
  0, 0, 29, 133, 192, 212, 207, 216, 245, 255, 255, 255, 249, 211, 205, 221, 250, 255, 255, 255, 
  255, 212, 140, 106, 105, 135, 167, 195, 218, 216, 192, 164, 135, 115, 105, 96, 92, 95, 103, 109, 
  119, 115, 85, 43, 13, 12, 46, 85, 115, 133, 149, 161, 188, 236, 255, 255, 255, 255, 255, 255, 
  255, 255, 255, 191, 106, 51, 13, 22, 51, 82, 125, 177, 226, 250, 225, 164, 96, 22, 0, 0, 
  0, 0, 0, 24, 77, 125, 144, 157, 167, 183, 192, 168, 113, 53, 19, 17, 24, 19, 24, 47, 
  57, 57, 57, 72, 81, 72, 70, 58, 33, 12, 19, 37, 57, 99, 159, 216, 255, 255, 249, 216, 
  171, 149, 147, 147, 133, 103, 77, 55, 53, 92, 157, 240, 255, 255, 255, 255, 255, 255, 250, 208, 
  173, 149, 123, 101, 101, 115, 135, 147, 150, 153, 149, 147, 144, 149, 140, 143, 143, 144, 140, 140, 
  144, 140, 143, 143, 143, 143, 139, 140, 140, 140, 140, 137, 135, 137, 137, 137, 133, 130, 129, 130, 
  129, 129, 128, 128, 125, 125, 125, 128, 128, 128, 120, 120, 119, 123, 119, 116, 120, 116, 119, 119, 
  119, 115, 119, 116, 113, 115, 115, 115, 115, 113, 109, 106, 109, 113, 109, 109, 109, 109, 109, 109, 
  109, 109, 109, 109, 113, 113, 111, 115, 116, 116, 115, 115, 113, 96, 77, 75, 75, 67, 71, 105, 
  153, 183, 181, 154, 120, 87, 61, 48, 46, 48, 63, 87, 143, 212, 255, 255, 255, 255, 255, 235, 
  143, 77, 58, 67, 92, 92, 75, 57, 37, 24, 17, 15, 9, 0, 0, 0, 0, 0, 0, 3, 
  71, 159, 250, 255, 255, 255, 255, 240, 232, 229, 211, 171, 128, 95, 70, 55, 46, 43, 33, 3, 
  0, 33, 81, 113, 111, 96, 77, 61, 53, 53, 57, 75, 96, 120, 149, 177, 212, 255, 255, 255, 
  255, 255, 255, 255, 255, 183, 106, 43, 0, 0, 0, 0, 0, 0, 5, 67, 99, 105, 103, 96, 
  89, 82, 77, 79, 77, 79, 82, 89, 92, 99, 103, 109, 111, 115, 115, 116, 101, 55, 24, 7, 
  7, 24, 48, 75, 113, 147, 171, 202, 218, 236, 242, 255, 255, 255, 255, 255, 255, 255, 255, 250, 
  109, 9, 0, 0, 0, 0, 0, 23, 96, 130, 120, 89, 53, 19, 3, 0, 0, 0, 0, 3, 
  15, 24, 24, 0, 0, 9, 61, 109, 125, 120, 105, 101, 130, 178, 226, 245, 229, 198, 164, 139, 
  116, 103, 99, 92, 95, 99, 96, 89, 61, 36, 22, 31, 87, 159, 207, 215, 202, 174, 168, 177, 
  171, 154, 133, 129, 150, 154, 154, 157, 183, 218, 229, 225, 221, 225, 215, 178, 133, 89, 57, 29, 
  15, 9, 12, 17, 24, 31, 41, 43, 29, 37, 51, 63, 92, 133, 154, 149, 147, 135, 125, 123, 
  115, 113, 111, 109, 105, 106, 105, 89, 87, 115, 137, 149, 149, 144, 143, 137, 133, 129, 128, 125, 
  125, 123, 128, 125, 128, 128, 128, 128, 130, 128, 128, 125, 125, 129, 129, 130, 133, 129, 128, 128, 
  125, 125, 113, 113, 109, 115, 133, 143, 149, 144, 137, 135, 129, 120, 123, 113, 106, 113, 119, 119, 
  123, 120, 120, 119, 115, 119, 116, 113, 103, 75, 58, 72, 113, 149, 164, 183, 188, 181, 163, 143, 
  128, 116, 111, 103, 103, 103, 99, 103, 105, 105, 106, 106, 109, 109, 113, 111, 115, 116, 116, 116, 
  119, 116, 120, 119, 119, 119, 119, 119, 119, 113, 85, 55, 29, 33, 70, 103, 125, 135, 140, 137, 
  135, 135, 140, 171, 208, 236, 255, 255, 255, 255, 242, 202, 171, 129, 87, 71, 67, 55, 41, 48, 
  77, 113, 120, 125, 139, 144, 135, 103, 71, 43, 24, 15, 5, 0, 0, 0, 0, 19, 37, 67, 
  113, 174, 226, 245, 229, 211, 177, 149, 129, 111, 106, 95, 96, 99, 103, 109, 115, 119, 120, 99, 
  71, 77, 89, 95, 101, 105, 137, 187, 240, 255, 245, 211, 184, 178, 174, 163, 153, 163, 197, 218, 
  216, 216, 225, 225, 202, 159, 120, 95, 63, 48, 43, 39, 43, 48, 53, 61, 67, 71, 79, 85, 
  89, 91, 92, 87, 61, 36, 19, 12, 19, 33, 71, 129, 168, 192, 198, 205, 232, 255, 255, 255, 
  255, 255, 255, 222, 192, 171, 140, 92, 47, 17, 0, 0, 0, 0, 0, 0, 23, 63, 99, 111, 
  109, 92, 75, 75, 103, 154, 192, 202, 188, 178, 171, 154, 163, 178, 198, 211, 184, 139, 81, 27, 
  15, 12, 19, 53, 96, 143, 159, 154, 133, 115, 92, 67, 51, 46, 33, 23, 12, 13, 23, 39, 
  91, 163, 226, 255, 255, 255, 255, 255, 255, 242, 205, 154, 125, 103, 95, 96, 123, 150, 161, 147, 
  125, 109, 91, 85, 79, 79, 79, 85, 85, 91, 96, 101, 105, 109, 113, 115, 123, 120, 123, 128, 
  130, 119, 96, 58, 36, 19, 23, 39, 63, 89, 119, 150, 174, 198, 222, 255, 255, 255, 255, 255, 
  255, 255, 249, 168, 128, 111, 95, 101, 123, 150, 168, 154, 128, 95, 48, 2, 0, 0, 0, 0, 
  0, 13, 57, 92, 130, 150, 149, 137, 130, 135, 120, 92, 72, 53, 41, 33, 36, 33, 22, 17, 
  48, 91, 129, 144, 144, 149, 147, 147, 143, 140, 140, 139, 140, 143, 149, 149, 150, 153, 154, 157, 
  157, 159, 161, 161, 161, 161, 161, 161, 157, 161, 157, 153, 153, 150, 149, 147, 144, 147, 140, 135, 
  115, 79, 53, 31, 27, 37, 55, 77, 105, 133, 177, 232, 255, 255, 255, 255, 255, 255, 255, 212, 
  154, 101, 65, 57, 81, 128, 174, 215, 235, 231, 197, 129, 58, 13, 0, 0, 0, 0, 0, 0, 
  0, 0, 51, 135, 198, 249, 252, 229, 205, 188, 163, 128, 96, 70, 53, 46, 43, 46, 53, 61, 
  57, 51, 57, 53, 51, 53, 57, 79, 125, 181, 221, 229, 225, 211, 211, 226, 255, 255, 255, 255, 
  246, 208, 164, 130, 116, 125, 133, 133, 137, 150, 168, 168, 153, 135, 115, 105, 92, 87, 89, 89, 
  95, 101, 103, 109, 113, 116, 115, 116, 125, 133, 133, 128, 115, 103, 111, 128, 139, 149, 150, 150, 
  144, 144, 139, 143, 153, 163, 174, 184, 201, 202, 195, 178, 163, 150, 139, 120, 116, 113, 109, 101, 
  103, 113, 129, 133, 133, 137, 139, 130, 119, 106, 92, 82, 65, 41, 39, 43, 57, 70, 65, 65, 
  82, 111, 140, 168, 173, 163, 161, 159, 181, 188, 188, 191, 192, 187, 164, 137, 106, 91, 87, 87, 
  81, 82, 103, 111, 101, 96, 95, 103, 123, 139, 140, 130, 115, 101, 106, 116, 128, 154, 195, 240, 
  255, 255, 249, 242, 235, 211, 164, 119, 92, 65, 43, 17, 9, 5, 9, 9, 22, 41, 70, 96, 
  116, 115, 105, 95, 81, 71, 71, 65, 63, 72, 85, 105, 144, 164, 187, 198, 208, 231, 252, 252, 
};

const unsigned char 	Miss[5760] = {39,39,32,30,29,24,26,33,33,35,37,33,31,33,33,35,35,34,30,23,18,20,25,24,23,26,28,31,34,32,29,31,37,44,46,42,40,37,31,28,26,23,22,25,29,31,30,32,35,33,29,24,23,23,24,25,26,27,27,29,30,27,27,28,29,31,32,33,33,36,37,33,31,32,32,30,28,28,28,29,29,30,29,30,31,30,30,30,30,31,31,31,31,30,29,32,33,30,28,29,32,33,34,32,32,33,33,31,31,32,30,30,32,28,30,34,31,30,31,32,31,31,31,31,32,32,31,32,32,32,31,31,30,31,33,31,31,33,34,33,31,31,30,31,32,33,32,31,32,32,32,33,32,32,33,32,33,33,33,33,34,34,35,36,30,29,31,31,33,34,33,33,35,35,32,34,33,31,31,29,31,32,31,31,31,33,35,36,32,29,31,31,31,32,32,32,33,33,31,32,32,31,32,32,31,32,34,33,33,33,32,33,33,32,31,32,31,31,31,30,30,30,30,31,30,31,31,29,30,30,31,31,30,32,31,31,32,31,30,31,34,34,32,34,35,35,34,33,33,33,33,32,30,31,30,30,30,32,35,37,38,36,35,33,31,31,31,31,31,32,32,32,34,33,32,33,31,29,32,33,31,31,31,30,30,32,32,32,33,33,32,31,31,31,30,32,32,32,32,32,31,30,32,32,32,32,31,31,33,33,32,31,32,30,29,29,28,28,29,29,28,28,30,30,30,32,32,32,32,31,30,31,31,32,31,31,31,31,30,31,32,32,32,33,33,32,31,32,32,30,30,31,31,30,30,31,30,31,31,31,32,30,30,31,32,33,32,32,31,32,33,33,31,32,32,32,31,31,31,32,32,32,32,31,31,31,31,32,31,31,32,32,31,31,31,31,31,31,31,32,32,32,33,33,31,31,32,31,31,32,31,29,30,30,32,33,32,31,31,32,31,31,32,32,32,32,33,32,30,31,32,33,32,31,33,33,31,31,33,33,30,31,34,32,31,32,33,29,30,32,32,32,32,33,32,31,31,32,31,32,32,30,31,32,31,31,31,30,30,32,32,31,31,30,31,32,33,32,31,31,30,31,32,31,30,31,32,31,31,32,32,32,32,32,33,33,32,31,31,31,32,32,30,30,32,32,31,33,33,32,32,32,31,32,32,31,30,31,33,31,30,33,32,31,33,34,32,32,35,31,31,34,32,31,32,33,32,32,33,33,32,31,32,33,31,31,31,31,32,32,31,30,32,34,34,33,33,34,33,32,32,32,32,33,32,31,30,31,31,30,31,32,32,32,32,33,31,30,31,31,29,31,31,28,29,31,30,30,32,31,31,32,31,31,32,31,30,30,30,32,33,32,32,32,33,33,31,31,30,31,32,31,30,30,31,30,31,33,31,30,32,32,32,31,29,30,30,30,29,29,30,30,30,30,29,30,31,31,29,29,30,30,29,31,31,29,29,31,31,30,29,31,32,31,31,31,32,32,32,32,30,32,33,33,33,33,33,33,32,34,33,31,31,33,33,32,31,30,32,33,32,30,32,33,33,31,31,32,32,34,33,32,32,33,32,31,33,34,34,32,32,34,33,32,31,32,32,32,32,30,30,32,32,30,30,33,32,32,31,30,32,31,31,32,31,30,31,32,32,31,31,33,33,30,29,31,33,34,33,31,32,34,34,34,32,31,31,31,32,32,32,32,32,33,33,31,31,32,33,33,32,31,31,32,31,29,30,32,31,31,32,33,32,31,32,31,33,34,33,34,33,31,31,32,32,31,31,31,29,29,29,31,31,30,29,31,34,32,30,28,29,30,30,31,33,35,34,33,33,33,30,29,31,32,31,33,35,33,30,30,30,28,29,35,36,34,33,33,35,34,32,30,31,32,32,32,33,32,31,32,34,33,31,31,31,32,34,35,33,32,32,32,33,33,33,33,33,32,31,31,31,32,32,31,31,32,32,31,31,31,30,29,30,31,31,31,31,31,30,32,32,30,31,33,32,31,29,30,33,32,29,32,31,29,31,31,28,26,28,29,28,31,31,31,32,31,31,31,33,34,33,32,32,34,33,32,32,32,31,30,31,32,31,33,33,31,30,29,30,30,31,33,33,34,33,32,32,31,31,31,31,30,31,33,32,32,34,34,32,32,32,32,32,30,30,32,32,32,29,29,30,30,32,32,32,31,31,32,30,30,30,31,32,30,31,32,33,32,30,28,29,31,30,29,30,31,33,33,33,33,32,30,29,30,29,30,32,31,29,30,34,34,31,29,29,30,29,30,30,30,31,30,30,32,31,31,32,33,33,33,32,32,31,30,30,33,33,32,33,34,34,33,33,32,32,30,30,30,29,30,32,35,35,33,34,33,33,32,31,30,29,29,30,30,32,35,34,31,31,33,34,33,32,32,32,32,33,33,33,33,33,33,32,31,33,33,32,31,30,32,33,33,32,32,33,31,30,29,30,31,29,29,28,29,30,31,32,32,32,34,34,33,32,32,32,32,31,30,31,35,35,35,33,32,32,32,32,31,30,30,29,30,29,30,31,30,31,31,31,31,31,30,31,32,34,34,33,34,34,33,34,34,33,31,31,30,30,31,32,34,34,34,35,34,34,34,32,29,29,30,31,30,31,32,33,34,32,31,31,31,31,31,30,30,32,34,34,34,33,32,32,33,33,32,31,31,32,32,31,32,31,31,31,31,30,30,30,29,27,27,29,29,29,29,29,30,30,29,30,31,30,31,32,31,31,30,31,32,32,32,32,31,29,29,28,28,30,30,30,30,30,30,30,31,31,30,30,31,31,31,30,30,30,30,30,31,32,32,31,32,33,34,35,35,34,34,33,32,31,30,30,31,31,33,34,34,34,34,33,32,32,31,30,30,32,32,32,33,34,34,34,34,34,34,33,32,32,33,33,34,34,33,34,34,34,35,34,33,33,33,32,31,31,31,31,31,29,29,30,31,31,32,30,29,30,32,33,31,31,31,30,30,31,31,32,32,32,32,31,31,29,30,31,31,30,30,30,30,31,32,32,33,34,34,33,31,32,31,29,30,31,31,32,34,35,35,34,34,32,31,32,33,34,33,32,33,34,34,34,33,33,32,32,34,34,33,32,33,33,32,31,31,32,33,32,33,32,31,31,32,31,31,32,32,32,31,31,31,30,30,29,28,29,31,30,30,30,31,30,30,30,29,30,30,29,29,29,30,29,29,30,31,32,31,31,30,29,30,29,29,30,31,31,30,31,31,31,31,31,30,30,31,31,29,29,30,30,30,30,31,33,33,31,30,32,32,32,32,32,34,35,34,34,34,34,33,31,30,31,33,32,31,31,31,31,32,32,34,35,35,36,37,37,36,35,35,34,33,33,33,32,32,33,32,31,30,32,34,35,35,33,32,33,32,32,32,33,33,33,33,32,30,29,28,28,28,29,29,28,27,27,26,26,26,28,29,28,27,28,28,28,28,28,29,29,29,29,28,29,29,28,28,30,30,28,28,30,31,32,32,32,31,30,30,31,31,30,29,29,30,29,29,29,29,30,30,30,30,31,32,33,34,35,35,36,36,37,39,40,39,39,39,38,38,40,40,39,39,38,36,37,37,34,32,33,34,34,34,34,33,34,35,37,38,38,37,37,36,35,34,33,31,31,30,30,30,31,31,29,27,27,26,25,27,30,30,30,29,29,30,31,30,29,28,28,30,31,29,29,30,30,29,30,31,30,29,29,31,32,33,33,34,34,34,33,31,29,30,29,28,28,28,29,29,29,30,31,33,34,33,31,30,32,32,32,33,33,33,34,35,34,34,34,32,32,31,30,30,30,30,30,29,30,31,31,32,33,33,33,34,35,35,36,35,36,35,33,32,31,33,34,34,34,34,35,35,36,36,36,35,35,35,35,33,31,29,30,30,31,32,31,31,33,34,32,29,29,29,28,27,28,28,27,27,27,28,29,28,27,26,26,24,21,22,24,25,25,24,24,23,24,25,26,26,24,25,27,28,28,28,28,30,33,33,32,32,34,33,31,31,31,32,33,34,35,36,37,36,35,34,33,33,32,33,34,32,32,33,33,34,34,34,34,33,33,32,31,29,30,31,32,31,30,31,32,33,32,32,31,30,29,29,30,30,32,33,32,32,32,32,31,31,33,33,32,30,30,31,29,30,31,31,32,31,31,33,34,34,35,36,35,35,37,37,35,35,34,34,33,31,32,31,31,32,32,32,32,32,31,31,33,35,34,34,34,33,33,34,34,34,34,32,31,31,32,31,31,32,34,34,35,36,36,36,36,35,34,31,31,33,33,32,31,32,32,33,34,33,34,34,33,32,32,30,29,29,29,27,27,28,28,28,27,27,28,28,29,30,30,31,31,31,32,32,32,32,33,32,31,32,31,31,30,30,31,32,32,31,33,34,35,35,34,33,34,33,32,30,31,33,33,33,32,31,32,32,32,32,32,32,33,33,33,33,33,33,32,31,31,30,30,31,32,32,31,31,31,32,32,31,31,31,32,31,30,29,30,31,33,34,33,33,33,33,33,34,33,32,33,34,32,31,30,29,28,29,29,28,28,30,31,29,28,28,29,29,29,29,28,28,28,28,29,29,29,29,30,31,32,33,33,32,31,31,31,33,34,33,31,31,32,31,29,29,28,29,29,30,30,29,29,28,28,27,27,27,28,32,33,33,34,34,34,35,35,34,33,33,32,32,33,32,32,31,32,33,34,35,35,35,36,36,35,36,34,33,34,34,35,36,37,37,37,37,37,39,37,35,33,34,34,34,33,33,33,33,33,32,31,30,32,34,34,33,32,32,31,30,29,29,28,28,29,30,31,32,31,30,30,30,29,29,30,30,31,32,32,32,31,30,30,30,31,31,32,31,30,29,31,32,31,30,30,29,29,29,29,27,26,26,28,30,29,29,30,32,34,35,34,34,34,34,34,33,32,33,34,33,33,33,32,31,31,32,32,32,32,32,30,29,28,27,25,25,26,28,30,31,30,30,31,32,32,31,30,30,31,31,29,29,28,27,27,27,28,28,28,28,28,27,27,27,28,27,27,27,29,31,31,31,31,31,31,31,31,30,29,29,29,30,31,30,30,32,33,32,31,31,32,34,35,33,31,32,33,32,31,30,33,36,36,35,35,37,38,38,37,35,36,37,37,37,36,37,38,37,37,36,38,38,37,36,36,37,37,37,37,35,35,35,34,33,33,33,34,36,34,34,34,31,32,34,34,33,34,34,37,38,35,32,32,33,34,33,30,30,33,33,28,28,30,29,26,26,27,28,30,29,26,30,33,29,25,24,22,28,30,24,23,27,24,22,23,23,27,28,25,24,29,32,29,29,30,32,33,31,31,30,33,34,36,35,32,32,32,34,33,31,32,31,31,31,33,32,31,32,32,32,33,33,30,28,29,31,34,34,34,34,34,33,32,32,31,32,31,31,32,29,29,30,30,30,30,31,33,35,33,32,33,33,34,34,34,34,31,30,32,35,34,31,32,34,34,33,34,33,31,32,34,35,33,31,32,34,34,32,30,28,29,31,31,30,30,30,30,32,32,28,27,26,28,29,26,25,25,28,29,28,27,28,31,28,27,27,26,28,28,28,28,28,30,31,36,34,31,32,35,36,33,31,33,33,31,29,31,33,32,31,29,29,32,37,38,35,33,32,35,38,38,37,35,35,35,35,37,36,35,34,35,36,35,33,32,33,33,33,33,31,32,32,31,30,30,29,29,30,31,30,28,31,36,36,34,34,36,35,32,30,28,30,30,31,32,30,33,37,39,39,35,32,32,34,35,33,30,28,30,32,30,29,30,30,29,31,32,32,32,32,32,32,32,33,35,35,35,36,35,35,38,39,38,36,33,32,31,30,31,32,30,27,29,30,28,27,25,27,28,26,26,27,25,23,24,25,24,27,23,25,33,38,40,35,35,36,34,32,23,21,21,23,21,19,21,22,29,33,31,28,30,34,33,33,30,29,32,31,29,28,30,33,32,32,33,38,43,39,34,32,31,30,31,28,26,27,28,30,31,32,33,35,35,35,37,36,36,37,38,40,41,40,36,34,35,34,32,30,30,32,31,31,30,31,32,32,33,31,32,31,31,34,34,31,30,30,32,32,31,29,30,33,33,31,29,32,33,32,33,32,31,33,32,33,33,34,33,32,33,33,33,31,31,33,33,33,31,30,29,29,30,30,30,31,31,32,32,33,33,33,34,34,35,35,35,35,36,36,34,32,31,30,30,28,27,29,28,26,28,30,29,29,29,30,32,34,34,34,35,35,36,33,32,30,29,31,32,33,33,33,33,33,33,31,30,31,30,30,32,35,34,34,33,33,34,34,34,30,29,31,32,34,34,34,32,30,34,35,34,33,30,30,30,31,31,29,27,26,29,31,31,29,26,27,28,29,30,27,25,25,28,28,27,27,27,28,29,29,28,27,28,27,28,28,28,28,30,32,32,32,31,31,31,30,30,31,31,31,31,31,31,31,31,30,31,33,32,33,33,33,33,34,34,33,32,32,33,33,32,32,31,30,31,33,34,33,34,37,37,35,32,31,30,30,31,31,29,30,32,32,31,31,33,33,34,35,33,34,33,33,32,28,26,26,28,31,33,34,34,34,36,36,33,31,30,29,29,32,35,35,34,34,34,32,33,33,32,33,32,31,33,33,32,31,29,30,30,31,32,33,33,33,32,33,35,34,33,34,35,34,33,34,35,34,33,33,33,33,32,31,31,33,34,33,30,28,28,28,28,29,28,30,30,31,30,29,31,31,29,30,29,29,30,30,30,31,31,29,29,30,29,30,30,30,30,31,34,33,36,38,35,37,38,39,38,31,28,28,26,23,23,24,20,18,21,23,22,22,26,29,31,32,32,33,36,36,34,32,35,39,42,42,42,39,39,41,38,30,25,27,27,23,21,21,24,25,27,28,28,32,35,40,44,43,43,43,40,38,36,32,33,35,35,32,31,30,31,31,34,36,35,34,34,35,34,32,33,33,31,31,32,31,31,36,38,36,34,34,35,35,35,33,30,31,33,32,31,31,32,32,32,31,32,33,32,30,28,30,34,34,32,31,31,31,32,29,25,25,26,27,27,28,28,29,32,32,31,31,27,27,30,29,29,27,24,26,27,29,33,37,38,36,36,34,35,37,34,30,27,28,30,26,23,23,26,24,25,28,29,31,35,36,37,40,40,36,35,34,31,31,33,30,27,27,29,30,30,29,31,35,36,36,37,38,39,36,32,32,34,33,30,28,26,25,27,28,26,25,26,26,26,26,25,28,31,27,26,32,35,34,35,37,38,38,39,38,37,36,35,31,28,29,28,27,26,27,30,32,34,36,36,37,38,38,36,36,38,36,31,30,30,32,32,30,31,31,32,34,33,32,32,31,30,28,24,21,22,23,23,25,26,29,30,32,33,32,32,31,31,33,32,28,26,28,29,31,32,30,30,31,33,32,34,34,31,31,33,31,30,30,29,27,26,27,29,30,29,31,34,35,37,37,36,37,36,35,36,35,34,34,32,33,33,31,33,36,35,35,36,36,35,37,38,37,36,35,36,37,37,37,35,36,37,36,36,35,35,35,34,32,31,30,30,31,30,29,30,32,32,31,32,33,33,34,32,33,33,31,32,33,32,31,30,28,27,29,28,25,26,27,29,30,29,28,30,32,32,31,31,30,30,30,28,27,28,29,29,28,27,29,31,33,32,32,33,33,33,32,30,31,34,34,33,33,33,32,31,31,32,33,35,35,35,35,34,34,32,32,30,28,30,30,30,32,31,30,29,29,29,28,28,29,29,30,30,30,29,30,31,33,33,31,31,33,34,32,30,30,31,32,30,29,30,31,31,31,32,31,30,31,31,29,31,32,31,31,33,34,33,33,35,37,35,34,32,30,30,30,29,29,30,30,31,30,30,30,30,29,28,27,28,29,29,29,29,32,31,29,29,30,32,32,31,32,33,32,33,31,27,27,27,27,27,27,28,29,30,31,32,34,33,32,33,33,32,31,31,30,31,33,34,34,35,35,36,36,35,34,32,34,35,34,32,31,32,31,30,30,31,31,32,34,34,34,34,34,33,33,33,32,31,31,31,32,31,30,32,32,31,32,33,33,34,35,35,35,34,34,33,32,31,32,34,32,32,34,34,35,36,35,34,34,34,34,33,32,32,32,31,31,31,31,32,33,34,34,34,33,32,32,31,31,30,30,31,31,32,31,30,31,31,31,32,32,30,31,31,32,32,32,31,31,31,29,28,29,31,31,31,33,34,34,35,34,32,31,30,30,28,30,30,30,31,30,30,30,30,31,31,31,31,33,32,31,31,31,31,32,31,29,29,29,29,28,26,27,29,30,31,32,32,32,33,33,32,31,30,30,31,31,31,31,32,32,33,34,33,32,33,32,32,33,31,30,30,30,30,29,27,25,26,28,28,28,27,29,31,30,29,28,28,28,29,30,29,29,30,30,30,30,31,29,30,31,30,30,31,30,29,29,30,31,32,30,29,31,31,30,29,28,27,27,29,29,29,30,32,32,31,30,30,31,31,32,32,32,33,33,32,32,33,33,33,34,32,32,33,33,32,32,32,31,32,32,31,30,30,31,32,32,33,32,33,34,34,34,34,33,32,31,29,30,30,29,29,29,30,32,33,33,33,33,32,33,33,32,32,33,34,34,35,37,37,36,36,36,37,37,38,38,36,35,36,35,33,33,33,33,33,33,34,34,36,35,33,35,35,34,33,33,33,33,34,33,32,34,36,36,35,35,37,37,37,38,38,37,36,36,35,34,33,34,34,33,33,34,34,33,32,32,32,31,31,30,30,31,30,30,29,29,29,29,29,28,28,27,25,24,24,23,24,24,24,28,29,28,29,31,32,32,32,32,33,33,35,35,35,36,37,38,38,37,35,35,37,38,36,34,34,34,35,36,34,34,36,37,37,39,38,37,38,40,41,40,38,39,39,37,39,39,38,39,40,38,37,39,39,39,38,37,37,36,36,34,34,35,35,34,34,35,35,33,32,32,31,31,32,31,29,27,28,28,25,25,26,26,27,29,28,27,27,26,25,24,23,23,25,25,24,25,25,24,26,25,25,25,24,24,24,23,21,23,25,24,23,23,22,21,23,24,24,24,22,22,23,24,25,23,23,24,24,24,24,22,22,23,24,24,24,25,25,24,24,24,24,26,26,24,23,24,24,26,26,25,26,27,27,28,28,30,30,29,29,31,31,29,29,31,32,30,29,30,31,31,31,31,31,31,31,33,33,30,30,30,30,31,30,30,30,31,32,33,32,31,32,32,32,33,32,31,32,32,32,32,32,31,31,32,33,34,33,33,33,33,32,32,32,31,32,33,32,31,32,33,31,32,33,32,31,32,34,34,34,34,34,34,34,33,35,35,35,37,37,37,37,37,37,36,37,36,35,36,36,36,36,35,34,35,35,35,35,36,38,39,41,42,43,43,44,44,42,42,41,40,40,39,38,40,40,39,38,39,38,38,40,40,37,36,37,35,34,35,34,33,35,35,34,34,35,35,36,36,36,35,35,34,34,34,33,32,32,34,35,34,35,34,35,35,35,36,36,37,36,36,34,31,30,31,31,31,32,31,29,29,29,28,28,30,32,32,30,29,30,30,30,30,31,31,32,32,32,32,32,33,33,34,33,33,34,33,33,32,31,32,33,34,34,35,36,35,36,37,36,34,33,33,32,32,32,31,29,28,30,30,30,29,30,31,31,31,29,28,29,30,32,32,34,34,33,32,33,34,34,32,33,33,32,31,31,29,28,28,29,29,28,28,28,28,28,28,28,27,27,27,28,28,28,28,27,27,27,27,26,26,28,28,27,29,31,33,33,32,33,34,35,35,33,33,33,32,31,32,31,30,29,29,29,30,31,32,32,32,32,31,32,31,30,31,31,30,31,32,31,32,33,34,34,34,35,36,35,34,34,34,33,33,33,33,32,32,31,29,29,31,32,31,29,28,29,29,30,31,30,29,30,29,27,27,28,29,29,29,28,27,28,27,26,25,27,29,29,27,29,30,30,30,31,32,31,30,29,29,29,28,27,25,28,28,26,27,28,28,28,28,29,29,28,28,30,31,30,30,32,35,35,35,34,33,34,35,32,30,31,31,30,28,27,25,25,27,27,28,27,27,29,31,30,30,31,31,30,31,30,29,30,29,27,28,29,31,31,33,34,32,29,27,26,25,24,25,25,25,25,25,25,26,28,29,29,30,32,32,33,33,32,34,35,35,34,35,34,33,34,33,35,35,31,30,31,30,29,28,28,30,31,30,32,32,31,31,31,32,31,31,33,33,32,33,34,34,34,37,36,34,35,35,32,31,33,33,33,33,34,36,35,33,34,33,31,31,31,31,31,31,31,31,32,30,29,30,31,33,35,35,35,36,36,33,32,32,32,31,29,30,33,32,32,31,31,29,30,32,32,33,34,34,34,33,32,33,34,34,33,33,30,28,29,29,30,32,33,32,32,35,36,36,38,37,35,34,33,32,33,32,31,33,32,30,31,31,33,35,35,36,37,36,34,32,32,34,34,35,35,33,35,37,36,35,35,35,34,34,33,34,33,31,31,31,32,33,32,31,31,32,34,34,32,32,35,38,40,39,38,37,37,36,35,35,34,33,31,30,28,30,32,33,33,34,34,33,30,30,32,31,31,31,30,31,31,28,27,29,29,29,31,31,33,35,36,35,34,36,37,36,35,36,37,35,35,36,34,34,34,32,31,30,29,29,29,31,31,30,32,31,28,28,28,29,31,32,34,34,35,35,35,35,37,37,37,36,36,35,34,33,32,31,31,31,30,27,28,28,27,27,27,27,25,26,30,30,30,32,32,31,31,32,30,28,29,29,30,31,32,31,31,32,30,31,30,26,25,24,22,21,20,20,20,22,24,25,29,28,28,30,30,29,29,28,27,28,29,28,27,29,27,27,29,28,27,25,26,26,28,28,28,31,30,32,34,32,31,34,35,35,36,36,37,35,34,36,37,36,36,36,36,36,36,34,32,31,30,28,27,28,27,27,29,29,28,30,33,34,34,34,35,35,34,34,33,34,34,35,35,36,36,35,34,34,32,30,29,28,28,30,31,31,30,29,29,30,30,29,27,28,29,28,27,29,29,29,30,30,30,31,31,31,30,31,32,31,31,31,31,31,30,30,30,29,30,32,32,28,30,32,31,32,32,33,34,33,34,33,35,36,35,34,34,37,38,39,38,38,41,42,39,36,35,35,33,32,31,30,31,31,32,32,34,36,35,35,35,35,33,31,27,27,28,28,27,26,27,27,28,28,27,27,26,29,30,28,29,30,30,32,33,34,34,35,36,35,34,35,35,34,31,30,29,28,29,29,30,31,30,30,31,31,31,31,31,32,33,32,32,33,33,33,33,35,36,36,37,38,37,35,35,35,35,34,33,32,31,29,27,25,24,24,25,26,25,27,27,26,27,27,27,28,28,28,28,27,27,28,30,30,28,29,31,32,33,34,33,31,33,34,35,34,32,34,34,33,33,31,31,31,31,30,31,32,33,33,32,34,35,32,32,32,30,30,30,27,25,27,29,29,31,33,33,35,37,40,41,39,36,34,34,33,31,30,30,32,33,31,29,29,29,30,31,30,29,30,31,30,29,28,30,32,31,33,35,35,35,34,33,34,35,34,32,31,32,34,35,35,35,36,36,37,35,34,36,35,34,33,33,32,32,32,31,32,33,33,32,32,32,32,33,32,31,32,32,33,34,33,32,32,33,33,33,34,32,33,35,35,35,33,33,32,31,32,32,33,32,31,32,32,30,29,28,28,28,29,28,28,28,29,30,31,30,30,28,28,29,29,29,29,29,31,31,30,30,30,30,30,30,30,31,29,28,29,29,28,26,26,28,28,27,28,29,31,33,31,30,32,32,29,28,28,25,23,26,27,26,28,31,33,35,36,35,36,37,35,34,34,35,35,31,31,33,32,30,30,28,28,31,31,30,30,30,29,30,32,30,29,31,32,33,33,33,32,31,32,33,32,32,33,33,34,35,36,34,31,30,33,33,32,31,31,30,31,32,32,32,32,33,34,33,31,30,31,34,34,35,35,36,36,33,34,33,31,32,32,31,30,29,30,29,29,29,31,34,33,33,34,33,32,31,32,32,31,30,30,30,30,32,33,33,35,35,33,33,34,35,35,32,29,30,30,28,28,28,28,29,30,32,34,33,33,35,33,31,30,30,31,32,32,32,33,32,32,34,33,33,33,31,31,30,29,28,30,30,29,29,29,29,30,31,31,32,34,35,35,34,32,31,33,33,32,31,29,29,30,30,29,29,30,31,31,31,33,32,31,30,30,30,29,29,31,34,35,35,35,36,37,38,38,35,32,32,32,30,30,31,32,33,34,33,33,33,30,30,32,31,30,29,29,29,29,31,30,28,30,29,28,28,29,28,28,30,29,29,31,31,31,32,34,34,34,36,36,37,38,36,34,34,37,36,34,34,34,31,30,30,29,30,30,32,32,31,30,30,30,32,32,31,31,32,33,32,31,29,30,29,27,29,32,33,33,35,37,37,38,38,36,34,33,34,32,31,31,31,30,30,31,31,32,31,29,30,29,30,32,31,32,33,31,30,29,29,30,30,29,30,31,31,29,29,31,31,31,33,34,33,32,32,32,32,32,32,32,33,33,33,31,30,31,30,29,28,28,30,32,33,31,30,31,30,29,30,32,32,30,31,32,31,31,33,32,31,28,28,30,30,30,30,29,30,32,33,31,30,30,31,32,33,32,33,35,35,34,32,31,32,33,33,31,30,30,29,29,29,29,29,31,32,34,34,32,33,34,33,31,31,33,34,34,33,33,33,34,36,35,35,35,34,32,31,31,30,28,29,28,28,28,26,26,26,27,28,29,30,29,28,30,32,33,34,33,31,31,32,32,32,31,32,31,31,33,33,32,32,32,32,29,27,27,27,28,28,29,29,28,28,29,29,29,29,32,33,32,30,30,31,31,30,29,28,27,27,26,27,28,28,26};
	

const uint8_t *sound;
uint32_t length;
void Sound_Init(void){
	Timer0_Init(80000000/11000, TIMER_PRIO); // 11kHz
	Timer0_Stop();
	DAC_Init();
	length = 0;
};

void Timer0A_Handler(void){ // called at 11 kHz
  TIMER0_ICR_R = 0x01;   // acknowledge TIMER0A timeout
	DAC_Out(*sound);
	sound++;
	length--;
	if (length <= 0) {
		Timer0_Stop();
	}
}

void Play_Hit_Sound(void) {
	sound = Hit;
	length = 2000;
	Timer0_Start();
}

void Play_Miss_Sound(void) {
	sound = Miss;
	length = 5760;
	Timer0_Start();
}


