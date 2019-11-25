///////////////////////////////////////////////////////////////////////////////
///
///         IRT Antoine de Saint-Exupéry - (http://http://www.irt-saintexupery.com/)
///         (c) All Rights Reserved. 2016
///
///         No authorization to modify or use this file for
///         commercial purposes without prior written consentement
///         of its author(s)
///
///////////////////////////////////////////////////////////////////////////////
#ifndef APPLICATION_DEFINITIONS_H
#define APPLICATION_DEFINITIONS_H

#define SIMULATION_TIME_LOW 0x0
#define SIMULATION_TIME_HIGH 0x4

#define FIFO_LENGTH 2

// Original height of the image (OBS.: if this value is modified, make sure to modify buffer sizes accordingly (scaled height % buffer number of lines == 0))
#define IHEIGHT 240
// 558
// Original width of the image (OBS.: if this value is modified, make sure to modify buffer sizes accordingly (scaled height % buffer number of lines == 0))
#define IWIDTH 320
//840
/*
 * To ensure the correctness of the resize image algorithm the resize scale has to respect the following conditions :
 * WIDTH % RESIZING_SCALE == 0 AND HEIGHT % RESIZING_SCALE == 0
 * */
// Scale parameter for the resize block (OBS.: if this value is modified, make sure to modify buffer sizes accordingly (scaled height % buffer number of lines == 0))
#define RESIZING_SCALE 2


// Size of internal buffer used to cache memory values  [(WIDTH/2)  is the width of resized image when scaled by 2]
#define INTERNAL_BUFFER_SIZE 4

//Input: 2 lines of original image | Output: 1 lines of scaled image
#define LINE_DETECTION_INPUT_BUFFER_SIZE (320 * 240)
#define LINE_DETECTION_OUTPUT_BUFFER_SIZE (160*120)

/* Canny detector */
 // Low threshold
#define  LOW_THRESH 30
 // High threshold
#define  HIGH_THRESH 55

/*  Histogram */
// Range of intensity value of a pixel
#define HIST_WIDTH 256

/* Hough*/
// Threshold for hough (number of votes (pixels) required to be considered as a line)
#define  HOUGH_THRESHOLD 27
// Width of the hough space (the higher is the value, the best is the precision but the longer is the processing)
#define ACCUMULATOR_WIDTH 180
#define MAXIMUM_NB_LINES 60

//Addresses
#define DDR_BASE_ADDRESS  0x1B2A0000

// PerfMonitor
#define FRAME_DONE	0x4FFF8
#define START 0xFFFF
#define END 0x0

#define SIMULATION_TIME_LOW 0x0
#define SIMULATION_TIME_HIGH 0x4

struct line_points{
  unsigned int x0;
  unsigned int y0;
  unsigned int x1;
  unsigned int y1;
};

template <int SIZE>
struct buffer_pixel{
  int memory_offset;
  unsigned char array[SIZE];
};

#endif //APPLICATION_DEFINITIONS_H
