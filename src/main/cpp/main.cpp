#include <iostream>
#include "include_all.hpp"
#include "Perlin.hpp"
#include "InvertedWorley.hpp"
#include "FBM.hpp"
#include <iostream>
#include <sstream>

#if 1

float Remap(float original_value, float original_min,
            float original_max, float new_min, float new_max)
{
    return new_min + (((original_value - original_min) /
        (original_max - original_min)) * (new_max - new_min));
}

void generate_perlin_worley(cimg_library::CImg<unsigned char> & image, float hurst, float lacunarity, float baseScale)
{
    InvertedWorley worley;
    worley.Shuffle(10);

    Perlin perlin;
    perlin.Shuffle(10);

    FBM fbmWorley(worley);
    fbmWorley.SetParameters(hurst,lacunarity,7.f);

    FBM fbmPerlin(perlin);
    fbmPerlin.SetParameters(hurst,lacunarity,7.f);

    for(int x = 0 ; x < image.width() ; x++)
        for(int y = 0 ; y < image.height() ; y++)
        {
            float worleyNoise =  fbmWorley.Get({float(x) , float(y)}, baseScale);
            worleyNoise = (worleyNoise + 1.f) / 2.f;

            float perlineNoise =  fbmPerlin.Get({float(x) , float(y)}, baseScale);
            perlineNoise = (perlineNoise + 1.f) / 2.f;
            //perlineNoise = std::abs(perlineNoise * 2 - 1);

            float value = Remap(perlineNoise, 0, 1, worleyNoise, 1);
            //value = (perlineNoise * 0.65f) / (1.0f - worleyNoise * 0.6f);;

            unsigned int greyscale = static_cast<unsigned int>(value * 255.f);

            image(x,y,0) = greyscale;
            image(x,y,1) = greyscale;
            image(x,y,2) = greyscale;
        }

    std::stringstream name;
    name << "fbm_h" << hurst << "_l" << lacunarity << "_s" << baseScale << ".bmp";
    image.save(name.str().c_str());
    std::cout<<"Saved "<< name.str() << std::endl;
}

void generate_inverted_worley(cimg_library::CImg<unsigned char> & image, WorleyFunction worleyFunc, float hurst, float lacunarity, float scale)
{
    InvertedWorley worley;
    worley.Shuffle(10);

    FBM fbmWorley(worley);
    fbmWorley.SetParameters(hurst,lacunarity,7.f);

    for(int x = 0 ; x < image.width() ; x++)
        for(int y = 0 ; y < image.height() ; y++)
        {
            float value = fbmWorley.Get({float(x) , float(y)}, scale);

            unsigned int greyscale = static_cast<unsigned int>((value + 1.f) / 2.f * 255.f);

            image(x,y,0) = greyscale;
            image(x,y,1) = greyscale;
            image(x,y,2) = greyscale;
        }

    std::stringstream name;
    name << "worley_h" << hurst << "_l" << lacunarity << "_s" << scale << ".bmp";
    image.save(name.str().c_str());
    std::cout << name.str() << std::endl;
}

int main()
{
    cimg_library::CImg<unsigned char> output(256,256,1,3,0);

#if 1
    float hurst[] = {0.2f, 0.5f, 0.8f};
    float lacunarity[] = {2.5f, 3.5f, 4.5f};
    float scale[] = {0.005f, 0.010f, 0.015f, 0.020f};
#else
    float hurst[] = {0.1f, 0.2f, 0.3f, 0.4f, 0.5f, 0.6f, 0.7f, 0.8f, 0.9f, 1.0f};
    float lacunarity[] = {2.5f, 3.0f, 3.5f, 4.0f, 4.5f, 5.0f, 5.5f};
    float scale[] = {0.005f, 0.010f, 0.015f, 0.020f, 0.025f, 0.030f};
#endif

    for (auto& h : hurst)
    {
        for (auto& l : lacunarity)
        {
            for (auto& s : scale)
            {
                generate_perlin_worley(output, h, l, s);
            }
        }
    }

    for (auto& h : hurst)
    {
        for (auto& l : lacunarity)
        {
            for (auto& s : scale)
            {
                generate_inverted_worley(output, WorleyFunction::WorleyFunction_F1, h, l, s);
            }
        }
    }
}
#else
int main()
{
    cimg_library::CImg<unsigned char> output(512,512,1,3,0);

    // Example 0 - See scr/examples/cpp/example-perlin.cpp
    generate_perlin(output);
    generate_perlin_3dslice(output);
    generate_perlin_4dslice(output);

    // Example 1 - See scr/examples/cpp/example-simplex.cpp
    generate_simplex(output);
    generate_simplex_3dslice(output);
    generate_simplex_4dslice(output);

    // Example 2 - See scr/examples/cpp/example-worley.cpp
    generate_worley(output, WorleyFunction::WorleyFunction_F1);

    // Example 3 - See scr/examples/cpp/example-fbm.cpp
    // lacunarity must be > 1, and should be > 2 to avoid scaling artifacts
    float hurst[] = {0.2f, 0.5f, 0.8f};
    float lacunarity[] = {2.5f, 3.5f, 4.5f};
    float baseScale = 0.005f;

    for(auto& h : hurst)
      for(auto& l : lacunarity)
        generate_fbm(output, h, l, baseScale);

    // Example 4 - See scr/examples/cpp/example-fbm.cpp
    for(auto& h : hurst)
      for(auto& l : lacunarity)
        generate_hmf(output, h, l, baseScale);

    return 0;
}
#endif
