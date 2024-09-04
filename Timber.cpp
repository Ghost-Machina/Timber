// This is where our game starts from

#include <SFML/Graphics.hpp>

// "using namespace sf;" eliminates the need to add:"sf::" before classes

using namespace sf;
int main()
{
	//VideoMode class allows you to set parameters for display size and RenderWindow will allow the creation of windows for 2D drawing
		
		VideoMode vm(1920, 1080);
		RenderWindow window(vm, "Timber!!!", Style::Fullscreen);
	
	//TEXTURES AND SPRITES
	//Texture and Sprite class work together to put a sprite on screen. A sprite needs a texture for it to be "visible" and in this case we made a background.
	
		Texture textureBackground;
		textureBackground.loadFromFile("graphics/background.png");
		Sprite spriteBackground;
		spriteBackground.setTexture(textureBackground);
		spriteBackground.setPosition(0, 0);

	const float TREE_HORIZONTAL_POSITION = 810;
	const float TREE_VERTICAL_POSITION = 0;
	
	//Tree
	
	Texture textureTree;
	textureTree.loadFromFile("graphics/tree.png");
	
	Sprite spriteTree;
	spriteTree.setTexture(textureTree);
	spriteTree.setPosition(TREE_HORIZONTAL_POSITION, TREE_VERTICAL_POSITION);
	
	//Bee
	
	Texture textureBee;
	textureBee.loadFromFile("graphics/bee.png");
	
	Sprite spriteBee;
	spriteBee.setTexture(textureBee);
	spriteBee.setPosition(0, 800);
	bool beeActive = false;
	float beeSpeed = 0.0f;
	
	//Clouds
	
	Texture	textureCloud;
	textureCloud.loadFromFile("graphics/cloud.png");
	Sprite spriteCloud1;
	spriteCloud1.setTexture(textureCloud);
	spriteCloud1.setPosition(0, 0);
	Sprite spriteCloud2;
	spriteCloud2.setTexture(textureCloud);
	spriteCloud2.setPosition(0, 250);
	Sprite spriteCloud3;
	spriteCloud3.setTexture(textureCloud);
	spriteCloud3.setPosition(0, 500);
		
	bool cloud1active = false;
	bool cloud2active = false;
	bool cloud3active = false;
	
	float cloud1speed = 0.0f;
	float cloud2speed = 0.0f;
	float cloud3speed = 0.0f;
		
	//Create an object called clock to calculate the time elapsed
		
	Clock clock;

	//A while loop continues to execute everything inside {} over and over every frame. The If statement inside allows us to close the loop.
	
	while (window.isOpen())
	{
		if (Keyboard::isKeyPressed(Keyboard::Escape))
		{
			window.close();
		}
		
		//window.clear will clear everything from the last frame window
		
		window.clear();
		
		// Remember that "while" creates a loop and clock.restart calculates the time between each loop and dt holds that value.
		
		Time dt = clock.restart();

		// ! (NOT) = false
		if (!beeActive)
		{
			srand((int)time(0));
			beeSpeed = (rand() % 200) + 200;

			srand((int)time(0) * 10);
			float height = (rand() % 500) + 500;
			spriteBee.setPosition(2000, height);
			beeActive = true;
		}
		else
		{
			spriteBee.setPosition(
				spriteBee.getPosition().x -
				(beeSpeed * dt.asSeconds()),
				spriteBee.getPosition().y);

			if (spriteBee.getPosition().x < -100)
			{
				beeActive = false;
			}
		}
		//window.draw will draw the game scene or "staging area" using spriteBackground object
		
		window.draw(spriteBackground);
		window.draw(spriteCloud1);
		window.draw(spriteCloud2);
		window.draw(spriteCloud3);
		window.draw(spriteTree);
		window.draw(spriteBee);
		
		//window.display will show everything we just drew
		
		window.display();
		
		// This checks to see if a key is being pressed and we specify the key esc.
		
	}

	return 0;
}