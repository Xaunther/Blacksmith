#pragma once

namespace blacksmith
{

constexpr unsigned short ROW = 6;
constexpr unsigned short COL = 6;
constexpr unsigned short MAXPOS = ROW * COL;
constexpr unsigned short MOVEPOS = 4;
constexpr unsigned short MOVEPOS_K = MOVEPOS * 2;
constexpr unsigned long MAXSTEPS = 10000000;
constexpr unsigned short N_THREADS = 8;

}
