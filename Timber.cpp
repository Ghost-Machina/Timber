// This is where our game starts from
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <sstream>


// "using namespace sf;" eliminates the need to add:"sf::" before classes
using namespace sf;

//Function declaration
void updateBranches(int seed);

const int NUM_BRANCHES = 6;
Sprite branches[NUM_BRANCHES];

//Where is the player/branch?
//Left or Right
enum class side {LEFT, RIGHT, NONE};
side branchPositions[NUM_BRANCHES];

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
	bool beeGrounded = false;
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
		
	bool cloud1Active = false;
	bool cloud2Active = false;
	bool cloud3Active = false;
	
	float cloud1Speed = 0.0f;
	float cloud2Speed = 0.0f;
	float cloud3Speed = 0.0f;
		
	//Create an object called clock to calculate the time elapsed
		
	Clock clock;

	//Time bar
	RectangleShape timeBar;
	float timeBarStartWidth = 400;
	float timeBarHeight = 80;
	timeBar.setSize(Vector2f(timeBarStartWidth, timeBarHeight));
	timeBar.setFillColor(Color::Red);
	timeBar.setPosition((1920 / 2) - timeBarStartWidth / 2, 980);

	Time gameTimeTotal;
	float timeRemaining = 6.0f;
	float timeBarWidthPerSecond = timeBarStartWidth / timeRemaining;

	// Track whether the game is running
	bool paused = true;
	
	//Draw some text
	int score = 0;

	//Text class and objects
	Text messageText;
	Text scoreText;

	//Choose a font
	Font font;
	font.loadFromFile("fonts/KOMIKAP_.ttf");

	//Set the font to message
	messageText.setFont(font);
	scoreText.setFont(font);

	//Assign message
	messageText.setString("Press Enter to start!");
	scoreText.setString("Score = 0");

	//Font size
	messageText.setCharacterSize(75);
	scoreText.setCharacterSize(100);

	//Font Color
	messageText.setFillColor(Color::White);
	scoreText.setFillColor(Color::White);

	//Position the text
	FloatRect textRect = messageText.getLocalBounds();
	messageText.setOrigin(textRect.left +
		textRect.width / 2.0f,
		textRect.top +
		textRect.height / 2.0f);

	messageText.setPosition(1920 / 2.0, 1080 / 2.0f);
	scoreText.setPosition(20, 20);

	//Prepare 5 branches
	Texture textureBranch;
	textureBranch.loadFromFile("graphics/branch.png");

	//int i becomes a container that represents each position in the array
	//Set the texture for each branch sprite
	for (int i = 0; i < NUM_BRANCHES; i++)
	{
		branches[i].setTexture(textureBranch);
		branches[i].setPosition(-2000, -2000);

		branches[i].setOrigin(220, 20);
	}

	Texture texturePlayer;
	texturePlayer.loadFromFile("graphics/player.png");
	Sprite spritePlayer;
	spritePlayer.setTexture(texturePlayer);
	spritePlayer.setPosition(580, 720);

	//The player starts on the left
	side playerSide = side::LEFT;

	//Prepare the gravestone
	Texture textureRIP;
	textureRIP.loadFromFile("graphics/rip.png");
	Sprite spriteRIP;
	spriteRIP.setTexture(textureRIP);
	spriteRIP.setPosition(600, 860);

	//Prepare the axe
	Texture textureAxe;
	textureAxe.loadFromFile("graphics/axe.png");
	Sprite spriteAxe;
	spriteAxe.setTexture(textureAxe);
	spriteAxe.setPosition(700, 830);

	//Line the axe up with the tree
	const float AXE_POSITION_LEFT = 700;
	const float AXE_POSITION_RIGHT = 1075;

	//Prepare the flying log
	Texture textureLog;
	textureLog.loadFromFile("graphics/log.png");
	Sprite spriteLog;
	spriteLog.setTexture(textureLog);
	spriteLog.setPosition(810, 720);

	//useful log related variables
	bool logActive = false;
	float logSpeedX = 1000;
	float logSpeedY = -1500;

	bool acceptInput = false;

	//prepare the sound
	SoundBuffer chopBuffer;
	chopBuffer.loadFromFile("sound/chop.wav");
	Sound chop;
	chop.setBuffer(chopBuffer);

	SoundBuffer deathBuffer;
	deathBuffer.loadFromFile("sound/death.wav");
	Sound death;
	death.setBuffer(deathBuffer);

	//Out of time
	SoundBuffer ootBuffer;
	ootBuffer.loadFromFile("sound/out_of_time.wav");
	Sound outOfTime;
	outOfTime.setBuffer(ootBuffer);

	//A while loop continues to execute everything inside {} over and over every frame. The If statement inside allows us to close the loop.
	while (window.isOpen())
	{
		Event event;
			while (window.pollEvent(event))
			{
				if (event.type == Event::KeyReleased && !paused)
				{
					//listen for key presses again
					acceptInput = true;

					//hide the axe
					spriteAxe.setPosition(2000,
						spriteAxe.getPosition().y);
				}
			}
		
		// This checks to see if a key is being pressed and we specify the key esc.
		if (Keyboard::isKeyPressed(Keyboard::Escape))
		{
			window.close();
		}

		//track if game is running
		if (Keyboard::isKeyPressed(Keyboard::Return))
		{
			paused = false;
		
			//Reset the time and the score
			score = 0;
			timeRemaining = 6;
		
			//Make all the branches disappear
			for (int i = 1; i < NUM_BRANCHES; i++)
			{
				branchPositions[i] = side::NONE;
			}

			//make sure the gravestone is hidden
			spriteRIP.setPosition(675, 2000);

			//move player into position
			spritePlayer.setPosition(580, 720);

			acceptInput = true;

		}

		if (acceptInput)
		{
			if (Keyboard::isKeyPressed(Keyboard::Right))
			{
				playerSide = side::RIGHT;
				score++;

				//add to amount of time remaining
				timeRemaining += (2 / score) + .15;

				spriteAxe.setPosition(AXE_POSITION_RIGHT,
					spriteAxe.getPosition().y);
				spritePlayer.setPosition(1200, 720);

				//update the branches
				updateBranches(score);

				//set the log flying to the left
				spriteLog.setPosition(810, 720);
				logSpeedX = -5000;
				logActive = true;

				acceptInput = false;

				//play chop sound
				chop.play();
			}
			if (Keyboard::isKeyPressed(Keyboard::Left))
			{
				playerSide = side::LEFT;
				score++;

				//add to amount of time remaining
				timeRemaining += (2 / score) + .15;

				spriteAxe.setPosition(AXE_POSITION_LEFT,
					spriteAxe.getPosition().y);
				spritePlayer.setPosition(580, 720);

				//update branches
				updateBranches(score);

				//set the log flying to the right
				spriteLog.setPosition(810, 720);
				logSpeedX = 5000;
				logActive = true;

				acceptInput = false;
				
				//play chop sound
				chop.play();
			}

		}

		if (!paused)
		{

			// Remember that "while" creates a loop and clock.restart calculates the time between each loop and dt holds that value.
			Time dt = clock.restart();


			//Subtract from the amount of time remaining
			timeRemaining -= dt.asSeconds();

			//Size up the time bar
			timeBar.setSize(Vector2f(timeBarWidthPerSecond *
				timeRemaining, timeBarHeight));

			if (timeRemaining <= 0.0f)
			{
				paused = true;
				messageText.setString("Out of time!!");

				FloatRect textRect = messageText.getLocalBounds();
				messageText.setOrigin(textRect.left +
					textRect.width / 2.0f,
					textRect.top +
					textRect.height / 2.0f);

				messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);

				//play the out of time sound
				outOfTime.play();

			}

			// Bee movement
			if (!beeActive)
			{
					srand((int)time(0));
					beeSpeed = (rand() % 200) + 100;

					srand((int)time(0) * 10);
					float height = (rand() % 500) + 400;
					spriteBee.setPosition(2000, height);
					beeActive = true;
			}
			else
			{
				srand((int)time(0));
				float height = (rand() % 500) + 400;

				spriteBee.setPosition(
					spriteBee.getPosition().x -
					(beeSpeed * dt.asSeconds()),
					spriteBee.getPosition().y);

				if (spriteBee.getPosition().x < -100)
				{
					beeActive = false;
				}
			}

			//Bee going up and down hehe
			if (!beeGrounded)
			{
				spriteBee.setPosition(
					spriteBee.getPosition().x,
					spriteBee.getPosition().y +
					(beeSpeed * dt.asSeconds()));
				if (spriteBee.getPosition().y >= 899)
				{
					beeGrounded = true;
				}
			}
			else
			{
				spriteBee.setPosition(
					spriteBee.getPosition().x,
					spriteBee.getPosition().y -
					(beeSpeed * dt.asSeconds()));
				if (spriteBee.getPosition().y <= 499)
				{
					beeGrounded = false;
				}

			}
			
			//Cloud movement
			if (!cloud1Active)
			{
				srand((int)time(0) * 10);
				cloud1Speed = (rand() % 200);

				srand((int)time(0) * 10);
				float height = (rand() % 150);
				spriteCloud1.setPosition(-200, height);
				cloud1Active = true;
			}
			else
			{
				spriteCloud1.setPosition(
					spriteCloud1.getPosition().x +
					(cloud1Speed * dt.asSeconds()),
					spriteCloud1.getPosition().y);
				if (spriteCloud1.getPosition().x > 1920)
				{
					cloud1Active = false;
				}
			}

			if (!cloud2Active)
			{
				srand((int)time(0) * 20);
				cloud2Speed = (rand() % 200);

				srand((int)time(0) * 20);
				float height = (rand() % 300) - 150;
				spriteCloud2.setPosition(-200, height);
				cloud2Active = true;
			}
			else
			{
				spriteCloud2.setPosition(
					spriteCloud2.getPosition().x +
					(cloud2Speed * dt.asSeconds()),
					spriteCloud2.getPosition().y);
				if (spriteCloud2.getPosition().x > 1920)
				{
					cloud2Active = false;
				}
			}

			if (!cloud3Active)
			{
				srand((int)time(0) * 30);
				cloud3Speed = (rand() % 200);

				srand((int)time(0) * 30);
				float height = (rand() % 450) - 150;
				spriteCloud3.setPosition(-200, height);
				cloud3Active = true;
			}
			else				
			{
				spriteCloud3.setPosition(
					spriteCloud3.getPosition().x +
					(cloud3Speed * dt.asSeconds()),
					spriteCloud3.getPosition().y);
				if (spriteCloud3.getPosition().x > 1920)
				{
					cloud3Active = false;
				}
			}
			//Update the score text
			std::stringstream ss;
			ss << "Score = " << score;
			scoreText.setString(ss.str());

			//Update the branch sprites
			for (int i = 0; i < NUM_BRANCHES; i++)
			{
				float height = i * 150;
				if (branchPositions[i] == side::LEFT)
				{
					branches[i].setPosition(610, height);

					branches[i].setRotation(180);

				}
				else if (branchPositions[i] == side::RIGHT)
				{
					branches[i].setPosition(1330, height);

					branches[i].setRotation(0);
				}
				else
				{
					branches[i].setPosition(3000, height);
				}
			}
			//handle a flying log
			if (logActive)
			{
				spriteLog.setPosition(
					spriteLog.getPosition().x +
					(logSpeedX * dt.asSeconds()),
					spriteLog.getPosition().y +
					(logSpeedY * dt.asSeconds()));
				//has the log reached the right or left hand edge?
				if (spriteLog.getPosition().x < -100 ||
					spriteLog.getPosition().x > 2000)
				{
					logActive = false;
					spriteLog.setPosition(810, 720);
				}
			}

			if (branchPositions[5] == playerSide)
			{
				paused = true;
				acceptInput = false;

				//draw the gravestone
				spriteRIP.setPosition(525, 760);

				//hide the player
				spritePlayer.setPosition(2000, 660);

				//change the text of the message
				messageText.setString("SQUISHED!!");

				//center it on the screen
				FloatRect textRect = messageText.getLocalBounds();
				messageText.setOrigin(textRect.left +
					textRect.width / 2.0f,
					textRect.top + textRect.height / 2.0f);

				messageText.setPosition(1920 / 2.0f,
					1080 / 2.0f);

				//play the death sound
				death.play();
			}

		}
		//window.draw will draw the game scene or "staging area". They also work in layers so be mindful of the order images are drawn.
		window.draw(spriteBackground);
		window.draw(spriteCloud1);
		window.draw(spriteCloud2);
		window.draw(spriteCloud3);
		for (int i = 0; i < NUM_BRANCHES; i++)
		{
			window.draw(branches[i]);
		}
		window.draw(spriteTree);
		window.draw(spriteBee);
		window.draw(scoreText);
		window.draw(timeBar);
		if (paused)
		{
			window.draw(messageText);
		}
		window.draw(spritePlayer);
		window.draw(spriteAxe);
		window.draw(spriteLog);
		window.draw(spriteRIP);

		//window.display will show everything we just drew
		window.display();

		//window.clear will clear everything from the last frame window
		window.clear();
	}

	return 0;
}
//Function definition
void updateBranches(int seed)
{
	for (int j = NUM_BRANCHES - 1; j > 0; j--)
	{
		branchPositions[j] = branchPositions[j - 1];
	}

	//Spawn a new branch at position 0
	//LEFT, RIGHT, or NONE
	srand((int)time(0) + seed);
	int r = (rand() % 5);

	switch (r) 
	{
		case 0:
			branchPositions[0] = side::LEFT;
			break;

		case 1:
			branchPositions[0] = side::RIGHT;
			break;

		default:
			branchPositions[0] = side::NONE;
			break;
	}
}