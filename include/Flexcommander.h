#pragma once

#include <stdio.h>
#include <blkid/blkid.h>
#include "HFSPlus.h"

#if defined(_MSC_VER)
//  Microsoft
    #define EXPORT __declspec(dllexport)
    #define IMPORT __declspec(dllimport)
#elif defined(__GNUC__)
//  GCC
#define EXPORT __attribute__((visibility("default")))
#define IMPORT
#else
//  do nothing and hope for the best?
    #define EXPORT
    #define IMPORT
    #pragma warning Unknown dynamic link import/export semantics.
#endif

typedef struct FlexCommanderProbeInfo {
    blkid_cache blkCache;
} FlexCommanderProbeInfo;

typedef struct FlexCommanderFS {
    FILE *file;
    uint32_t blockSize;
    uint64_t catalogFileBlock;
    HFSPlusVolumeHeader volumeHeader;
} FlexCommanderFS;

//Functions for block device probing
int EXPORT Init(FlexCommanderProbeInfo* info);
int EXPORT ProbeDevices(FlexCommanderProbeInfo* info);
int EXPORT IterateDevices(FlexCommanderProbeInfo* info);

//Functions for filesystem interaction
int FlexOpen(const char * path, FlexCommanderFS* fs);
int FlexListDirContent(const char* path, FlexCommanderFS* fs);
int FlexSetCurrentDir(const char* path, FlexCommanderFS* fs);
int FlexCopy(const char* path, const char* currentDir, FlexCommanderFS* fs);