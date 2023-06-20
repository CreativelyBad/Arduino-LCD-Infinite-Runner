#include <LiquidCrystal.h>
#include <math.h>

LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

int buttonPin = 3;
bool buttonReleased = true;

int playerY = 1;

int obstacle1X = 15;
int obstacle1Y = 1;

int obstacle2X = 7;
int obstacle2Y = 1;

bool gameOver = false;
bool started = false;

int scoreX = 15;
int distScore = 0;

byte playerChar[8] = {
	0b01110,
	0b01110,
	0b01110,
	0b00100,
	0b01110,
	0b10101,
	0b00100,
	0b01010
};

byte cactusChar[8] = {
	0b00100,
	0b00101,
	0b00101,
	0b10111,
	0b10100,
	0b11100,
	0b00100,
	0b00100
};

void jump()
{
  lcd.setCursor(2, 0);
  lcd.write(byte(0));
  playerY = 0;
}

void ground()
{
  lcd.setCursor(2, 1);
  lcd.write(byte(0));
  playerY = 1;
}

void obstacle(int x, int y)
{
  if (obstacle1X < 0)
  {
    obstacle1X = 15;
  }

  if (obstacle2X < 0)
  {
    obstacle2X = 15;
  }

  lcd.setCursor(x, y);
  lcd.write(byte(1));
}

void checkCollision()
{
  if (obstacle1X == 2 && playerY == 1 || obstacle2X == 2 && playerY == 1)
  {
    gameOver = true;
  }
}

void score()
{
  lcd.setCursor(scoreX, 0);
  lcd.print(distScore);
  distScore++;
  double d_distScore = (double)distScore;
  double log10_distScore = log10(d_distScore);
  if ((log10_distScore - floor(log10_distScore)) == 0.0 && distScore >= 10)
  {
    scoreX--;
  }
}

void setup() {
  lcd.begin(16, 2);
  pinMode(buttonPin, INPUT_PULLUP);

  // Create new characters:
  lcd.createChar(0, playerChar);
  lcd.createChar(1, cactusChar);

  lcd.setCursor(0, 0);
  lcd.print("Press to Start");
}

void loop() {

  if (!started)
  {
    if (digitalRead(buttonPin) == LOW)
    {
      started = true;
    }
    
    return;
  }

  if (gameOver)
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Game Over!");
    lcd.setCursor(0, 1);
    lcd.print("Press to Restart");

    if (digitalRead(buttonPin) == LOW)
    {
      gameOver = false;
      distScore = 0;
      scoreX = 15;
      obstacle1X = 15;
      obstacle2X = 7;
    }

    return;
  }

  if (digitalRead(buttonPin) == LOW && buttonReleased) // jump
  {
    jump();
    buttonReleased = false;
  }
  else if (digitalRead(buttonPin) == HIGH) // ground
  {
    ground();
    buttonReleased = true;
  }
  else
  {
    ground();
  }

  obstacle(obstacle1X, obstacle1Y);
  obstacle(obstacle2X, obstacle2Y);

  checkCollision();

  score();

  delay(500);
  lcd.clear();
  obstacle1X--;
  obstacle2X--;
}
