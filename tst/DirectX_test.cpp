/*
 * DirectX_test.cpp
 *
 *  Created on: Apr 20, 2019
 *      Author: Antero Nousiainen
 */

#include "DirectX.hpp"
#include <gtest/gtest.h>

using namespace DirectX;

namespace
{
    constexpr std::uint32_t DDSD_CAPS{0x1};
    constexpr std::uint32_t DDSD_HEIGHT{0x2};
    constexpr std::uint32_t DDSD_WIDTH{0x4};
    constexpr std::uint32_t DDSD_PITCH{0x8};
    constexpr std::uint32_t DDSD_PIXELFORMAT{0x1000};
    constexpr std::uint32_t DDSD_MIPMAPCOUNT{0x20000};
    constexpr std::uint32_t DDSD_LINEARSIZE{0x80000};
    constexpr std::uint32_t DDSD_DEPTH{0x800000};

    constexpr std::uint32_t DDSCAPS_COMPLEX{0x8};
    constexpr std::uint32_t DDSCAPS_TEXTURE{0x1000};
    constexpr std::uint32_t DDSCAPS_MIPMAP{0x400000};

    constexpr std::uint32_t DDPF_ALPHAPIXELS{0x1};
    constexpr std::uint32_t DDPF_ALPHA{0x2};
    constexpr std::uint32_t DDPF_FOURCC{0x4};
    constexpr std::uint32_t DDPF_RGB{0x40};
    constexpr std::uint32_t DDPF_YUV{0x200};
    constexpr std::uint32_t DDPF_LUMINANCE{0x20000};
}

TEST(DirectXTest, HeaderFlags)
{
    EXPECT_EQ(DDSD_PITCH, DDS_HEADER_FLAGS_PITCH);
    EXPECT_EQ(DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT, DDS_HEADER_FLAGS_TEXTURE);
    EXPECT_EQ(DDSD_MIPMAPCOUNT, DDS_HEADER_FLAGS_MIPMAP);
    EXPECT_EQ(DDSD_LINEARSIZE, DDS_HEADER_FLAGS_LINEARSIZE);
    EXPECT_EQ(DDSD_DEPTH, DDS_HEADER_FLAGS_VOLUME);
}

TEST(DirectXTest, SurfaceFlags)
{
    EXPECT_EQ(DDSCAPS_TEXTURE, DDS_SURFACE_FLAGS_TEXTURE);
    EXPECT_EQ(DDSCAPS_COMPLEX | DDSCAPS_MIPMAP, DDS_SURFACE_FLAGS_MIPMAP);
    EXPECT_EQ(DDSCAPS_COMPLEX, DDS_SURFACE_FLAGS_CUBEMAP);
}

TEST(DirectXTest, PixelFormatFlags)
{
    EXPECT_EQ(DDPF_FOURCC, DDS_FOURCC);
}
