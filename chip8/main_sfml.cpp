#include <iostream>
#include <unistd.h>
#include "chip8.h"
#include <SFML/Graphics.hpp>

const int PIXEL_SIZE = 5;

class TextureScreen : public Chip8::Screen, public sf::Texture
{
public:
	TextureScreen()
	: Chip8::Screen()
	, sf::Texture()
	, m_needUpdate(true)
	{
		create(64,32);
		for(int i=0;i <Chip8::Screen::SCREEN_WIDTH*Chip8::Screen::SCREEN_HEIGHT;i++)
		{
			m_image[i]=255;
		}
	}
	
	void sync()
	{
		if(m_needUpdate) update((unsigned char*)m_image);
		m_needUpdate = false;
			
	}
    virtual void  clearScreen()
	{
		for(int i=0;i <Chip8::Screen::SCREEN_WIDTH*Chip8::Screen::SCREEN_HEIGHT;i++)
		{
			m_image[i]=255;
		}
		m_needUpdate = true;
	}
	
    virtual void  setPixel(int x, int y, Pixel p)
	{
		m_image[Chip8::Screen::SCREEN_WIDTH*y+x]= (p==Chip8::Screen::WHITE_PIXEL)?0xFFFFFFFF:0x000000FF;
		m_needUpdate = true;
	}
	
    virtual Pixel getPixel(int x, int y) const
	{
		return m_image[Chip8::Screen::SCREEN_WIDTH*y+x]==0xFFFFFFFF ? Chip8::Screen::WHITE_PIXEL : Chip8::Screen::BLACK_PIXEL;
	}
	
	
private:

	int m_image[32*64];
	bool m_needUpdate;
	
};


int main(int argc, char ** argv)
{
		
    sf::RenderWindow window(sf::VideoMode(640, 320), "Chip8");
	window.setVerticalSyncEnabled(true);
	
	FILE* rom = fopen("/Users/mooglwy/Desktop/MAZE.ch8", "rb");
	if(rom == NULL) return 0;
	unsigned char data[Chip8::CPU::MEMORY_SIZE] = {0};
	size_t realSize = fread(data, 1, Chip8::CPU::MEMORY_SIZE, rom);
	TextureScreen screen;
	Chip8::CPU cpu(&screen);
	cpu.loadROM(data,realSize);
	fclose(rom);

	sf::Sprite sprite(screen);
	sprite.scale(10, 10);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) window.close();
        }
		
		for (int i=0; i<10; i++) cpu.step();
		screen.sync();
		
		window.clear();
        window.draw(sprite);
        window.display();
    }
    return 0;
}

