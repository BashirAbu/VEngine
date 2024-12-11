#pragma once
#include <iostream>
#include <vector>
struct AssetHeader
{
    char name[256] = {};
    uint64_t size = 0;
    uint64_t offset = 0;
};


struct HeaderDesc
{
    std::vector<AssetHeader> headers;
};