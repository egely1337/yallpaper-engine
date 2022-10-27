#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <Windows.h>
#include "AnimatedGif.h"
#include <csignal>


BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam) {
    HWND process = FindWindowEx(hwnd, NULL, L"SHELLDLL_DefView", NULL);
    HWND* ret = (HWND*)lParam; if (process) *ret = FindWindowEx(NULL, hwnd, L"WorkerW", NULL);
    return true;
}

HWND GetWallpaperWindow() {
    HWND __progman = FindWindow(L"ProgMan", NULL);
    SendMessageTimeout(__progman, 0x052C, 0, 0, SMTO_NORMAL, 1000, nullptr);
    HWND wallpaper_ptr = nullptr;
    EnumWindows(EnumWindowsProc, (LPARAM)&wallpaper_ptr); return wallpaper_ptr;
}

int main(int argc, char** argv)
{
    sf::WindowHandle handle = GetWallpaperWindow();
    sf::RenderWindow window(handle);
    sf::Sprite sp;
    AnimatedGIF gif(argv[1]);
    window.setFramerateLimit(75);
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
        }
        window.clear(sf::Color::Black);
        gif.update(sp); window.draw(sp);
        window.display();
    }
    return 0;
}
