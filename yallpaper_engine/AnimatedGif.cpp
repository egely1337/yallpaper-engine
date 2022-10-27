#include "AnimatedGif.h"

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_GIF
#include "stb_image.h"

AnimatedGIF::AnimatedGIF(const char* filename)
{
    FILE* f = stbi__fopen(filename, "rb");
    assert(f);
    stbi__context s;
    stbi__start_file(&s, f);

    int* delays = 0;
    int z = 0, comp = 0;
    void* pixels = stbi__load_gif_main(&s, &delays, &size.x, &size.y, &z, &comp, STBI_rgb_alpha);
    sf::Image image;
    int step = size.x * size.y * 4;

    for (int i = 0; i < z; i++)
    {
        image.create(size.x, size.y, (const sf::Uint8*)pixels + step * i);

        sf::Texture texture;
        texture.loadFromImage(image);

        frames.push_back(std::tuple<int, sf::Texture>(delays[i], texture));
    }

    frameIter = frames.begin();

    stbi_image_free(pixels);
    if (delays)
        stbi_image_free(delays);
    fclose(f);

    totalDelay = sf::Time::Zero;
    startTime = clock.getElapsedTime();
}

const sf::Vector2i& AnimatedGIF::getSize()
{
    return size;
}

void AnimatedGIF::update(sf::Sprite& sprite)
{
    sf::Time delay = sf::milliseconds(std::get<0>(*frameIter));

    while (startTime + totalDelay + delay < clock.getElapsedTime())
    {
        frameIter++;
        if (frameIter == frames.end()) frameIter = frames.begin();
        totalDelay += delay;
        delay = sf::milliseconds(std::get<0>(*frameIter));
    }

    sf::Texture& texture = std::get<1>(*frameIter);
    sprite.setTexture(texture);
}