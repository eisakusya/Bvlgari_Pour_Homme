#pragma once
void init(unsigned int exihash[1 << 16], unsigned int slihash[1 << 16], unsigned int slibhash[1 << 16], unsigned int canhash[1 << 16]);
int ai2048(std::array<std::array<int, 4>, 4>data, unsigned int exihash[1 << 16], unsigned int slihash[1 << 16], unsigned int slibhash[1 << 16], unsigned int canhash[1 << 16]);