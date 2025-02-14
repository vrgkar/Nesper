#pragma once

#define INIT_SEGMENTS static int __segment_number = 0;
#define START_SEGMENT(n) if (__segment_number < n) { __segment_number = n;
#define END_SEGMENT return false; }
#define NEXT_SEGMENT(n) END_SEGMENT else START_SEGMENT(n)
#define BREAK_SEGMENT(n) START_SEGMENT(n) END_SEGMENT
#define FREE_SEGMENTS __segment_number = 0;

