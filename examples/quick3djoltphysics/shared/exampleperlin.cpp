#include "exampleperlin.h"

// stb_perlin.h — public domain (Sean Barrett), via Jolt Physics TestFramework

static unsigned char stb_perlin_randtab[512] = {
    23, 125, 161, 52, 103, 117, 70, 37, 247, 101, 203, 169, 124, 126, 44, 123,
    152, 238, 145, 45, 171, 114, 253, 10, 192, 136, 4, 157, 249, 30, 35, 72,
    175, 63, 77, 90, 181, 16, 96, 111, 133, 104, 75, 162, 93, 56, 66, 240,
    8, 50, 84, 229, 49, 210, 173, 239, 141, 1, 87, 18, 2, 198, 143, 57,
    225, 160, 58, 217, 168, 206, 245, 204, 199, 6, 73, 60, 20, 230, 211, 233,
    94, 200, 88, 9, 74, 155, 33, 15, 219, 130, 226, 202, 83, 236, 42, 172,
    165, 218, 55, 222, 46, 107, 98, 154, 109, 67, 196, 178, 127, 158, 13, 243,
    65, 79, 166, 248, 25, 224, 115, 80, 68, 51, 184, 128, 232, 208, 151, 122,
    26, 212, 105, 43, 179, 213, 235, 148, 146, 89, 14, 195, 28, 78, 112, 76,
    250, 47, 24, 251, 140, 108, 186, 190, 228, 170, 183, 139, 39, 188, 244, 246,
    132, 48, 119, 144, 180, 138, 134, 193, 82, 182, 120, 121, 86, 220, 209, 3,
    91, 241, 149, 85, 205, 150, 113, 216, 31, 100, 41, 164, 177, 214, 153, 231,
    38, 71, 185, 174, 97, 201, 29, 95, 7, 92, 54, 254, 191, 118, 34, 221,
    131, 11, 163, 99, 234, 81, 227, 147, 156, 176, 17, 142, 69, 12, 110, 62,
    27, 255, 0, 194, 59, 116, 242, 252, 19, 21, 187, 53, 207, 129, 64, 135,
    61, 40, 167, 237, 102, 223, 106, 159, 197, 189, 215, 137, 36, 32, 22, 5,
    23, 125, 161, 52, 103, 117, 70, 37, 247, 101, 203, 169, 124, 126, 44, 123,
    152, 238, 145, 45, 171, 114, 253, 10, 192, 136, 4, 157, 249, 30, 35, 72,
    175, 63, 77, 90, 181, 16, 96, 111, 133, 104, 75, 162, 93, 56, 66, 240,
    8, 50, 84, 229, 49, 210, 173, 239, 141, 1, 87, 18, 2, 198, 143, 57,
    225, 160, 58, 217, 168, 206, 245, 204, 199, 6, 73, 60, 20, 230, 211, 233,
    94, 200, 88, 9, 74, 155, 33, 15, 219, 130, 226, 202, 83, 236, 42, 172,
    165, 218, 55, 222, 46, 107, 98, 154, 109, 67, 196, 178, 127, 158, 13, 243,
    65, 79, 166, 248, 25, 224, 115, 80, 68, 51, 184, 128, 232, 208, 151, 122,
    26, 212, 105, 43, 179, 213, 235, 148, 146, 89, 14, 195, 28, 78, 112, 76,
    250, 47, 24, 251, 140, 108, 186, 190, 228, 170, 183, 139, 39, 188, 244, 246,
    132, 48, 119, 144, 180, 138, 134, 193, 82, 182, 120, 121, 86, 220, 209, 3,
    91, 241, 149, 85, 205, 150, 113, 216, 31, 100, 41, 164, 177, 214, 153, 231,
    38, 71, 185, 174, 97, 201, 29, 95, 7, 92, 54, 254, 191, 118, 34, 221,
    131, 11, 163, 99, 234, 81, 227, 147, 156, 176, 17, 142, 69, 12, 110, 62,
    27, 255, 0, 194, 59, 116, 242, 252, 19, 21, 187, 53, 207, 129, 64, 135,
    61, 40, 167, 237, 102, 223, 106, 159, 197, 189, 215, 137, 36, 32, 22, 5,
};

static float stb_perlin_lerp(float a, float b, float t)
{
    return a + (b - a) * t;
}

static int stb_perlin_fastfloor(float a)
{
    const int ai = int(a);
    return (a < ai) ? ai - 1 : ai;
}

static float stb_perlin_grad(int hash, float x, float y, float z)
{
    static float basis[12][3] = {
        { 1, 1, 0 },  { -1, 1, 0 },  { 1, -1, 0 },  { -1, -1, 0 },
        { 1, 0, 1 },  { -1, 0, 1 },  { 1, 0, -1 }, { -1, 0, -1 },
        { 0, 1, 1 },  { 0, -1, 1 }, { 0, 1, -1 }, { 0, -1, -1 },
    };

    static unsigned char indices[64] = {
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11,
        0, 9, 1, 11,
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11,
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11,
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11,
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11,
    };

    float *grad = basis[indices[hash & 63]];
    return grad[0] * x + grad[1] * y + grad[2] * z;
}

float examplePerlinNoise3(float x, float y, float z, int xWrap, int yWrap, int zWrap)
{
    const unsigned int xMask = (xWrap - 1) & 255;
    const unsigned int yMask = (yWrap - 1) & 255;
    const unsigned int zMask = (zWrap - 1) & 255;

    const int px = stb_perlin_fastfloor(x);
    const int py = stb_perlin_fastfloor(y);
    const int pz = stb_perlin_fastfloor(z);
    const int x0 = px & xMask;
    const int x1 = (px + 1) & xMask;
    const int y0 = py & yMask;
    const int y1 = (py + 1) & yMask;
    const int z0 = pz & zMask;
    const int z1 = (pz + 1) & zMask;

    float xf = x - px;
    float yf = y - py;
    float zf = z - pz;
    const float u = ((xf * 6 - 15) * xf + 10) * xf * xf * xf;
    const float v = ((yf * 6 - 15) * yf + 10) * yf * yf * yf;
    const float w = ((zf * 6 - 15) * zf + 10) * zf * zf * zf;

    const int r0 = stb_perlin_randtab[x0];
    const int r1 = stb_perlin_randtab[x1];
    const int r00 = stb_perlin_randtab[r0 + y0];
    const int r01 = stb_perlin_randtab[r0 + y1];
    const int r10 = stb_perlin_randtab[r1 + y0];
    const int r11 = stb_perlin_randtab[r1 + y1];

    const float n000 = stb_perlin_grad(stb_perlin_randtab[r00 + z0], xf, yf, zf);
    const float n001 = stb_perlin_grad(stb_perlin_randtab[r00 + z1], xf, yf, zf - 1);
    const float n010 = stb_perlin_grad(stb_perlin_randtab[r01 + z0], xf, yf - 1, zf);
    const float n011 = stb_perlin_grad(stb_perlin_randtab[r01 + z1], xf, yf - 1, zf - 1);
    const float n100 = stb_perlin_grad(stb_perlin_randtab[r10 + z0], xf - 1, yf, zf);
    const float n101 = stb_perlin_grad(stb_perlin_randtab[r10 + z1], xf - 1, yf, zf - 1);
    const float n110 = stb_perlin_grad(stb_perlin_randtab[r11 + z0], xf - 1, yf - 1, zf);
    const float n111 = stb_perlin_grad(stb_perlin_randtab[r11 + z1], xf - 1, yf - 1, zf - 1);

    const float n00 = stb_perlin_lerp(n000, n001, w);
    const float n01 = stb_perlin_lerp(n010, n011, w);
    const float n10 = stb_perlin_lerp(n100, n101, w);
    const float n11 = stb_perlin_lerp(n110, n111, w);
    const float n0 = stb_perlin_lerp(n00, n01, v);
    const float n1 = stb_perlin_lerp(n10, n11, v);
    return stb_perlin_lerp(n0, n1, u);
}
