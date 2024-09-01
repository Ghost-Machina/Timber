// This is where our game starts from
#include <SFML/Graphics.hpp>
// "using namespace sf;" eliminates the need to add:"sf::" before classes
using namespace sf;
int main()
{
	//VideoMode class allows you to set parameters for display size and RenderWindow will allow the creation of windows for 2D drawing
	VideoMode vm(1920, 1080);
	RenderWindow window(vm, "Timber!!!", Style::Fullscreen);
	
	//Texture and Sprite class work together to put a sprite on screen. A sprite needs a texture for it to be "visible" and in this case we made a background.
	Texture textureBackground;
		textureBackground.loadFromFile("graphics/background.png");
	
	// Here we set the spriteBackground texture to be the textureBackground object above.
	Sprite spriteBackground;
		spriteBackground.setTexture(textureBackground);
		spriteBackground.setPosition(0, 0);
	
	//A while loop continues to execute everything inside {} over and over. The If statement inside allows us to close the loop.
	while (window.isOpen())
	{
		//window.clear will clear everything from the last frame window
		window.clear();
		//window.draw will draw the game scene using spriteBackground object
		window.draw(spriteBackground);
		//window.display will show everything we just drew
		window.display();
		// This checks to see if a key is being pressed and we specify the key esc.
		if (Keyboard::isKeyPressed(Keyboard::Escape))
		{
			window.close();
		}
	}

	return 0;
}