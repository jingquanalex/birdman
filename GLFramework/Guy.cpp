#include "Guy.h"

using namespace std;
using namespace glm;

Guy::Guy(vec3 position) : Character("media\\img\\char.png", position, vec2(64, 64))
{
	// Timer for damage taken sprite flash.
	tHurtFlash = new Timer(0.07f, 1.5f);
	projectile = new Projectile();

	jumpSpeed = 1000.0f;
	moveSpeed = 550.0f;
	setupAnimation(vec2(64, 64), 0.1f, 0, 2);
	startAnimation();
	setupCollision(vec2(38, 50), vec3(0, -5, 0));
	//setBoundingRectVisible(1);
}

Guy::~Guy()
{
	tHurtFlash->destroy();
	//delete projectile;
}

void Guy::update(float dt)
{
	// Flip sprite left or right, depending on movement.
	if (!(isMovingLeft && isMovingRight))
	{
		if (isMovingLeft)
		{
			isFlippedX = 1;
		}
		else if (isMovingRight)
		{
			isFlippedX = 0;
		}
	}

	if (tHurtFlash->getIsRunning())
	{
		// Sprite flashing during invuln period.
		if (tHurtFlash->hasTicked())
		{
			if (alpha == 1.0f)
			{
				alpha = 0.3f;
			}
			else
			{
				alpha = 1.0f;
			}
		}
	}
	else
	{
		alpha = 1.0f;
		isInvuln = 0;
	}

	if (isOnPlatform && velocity.y == 0.0f)
	{
		kickCharges = 1;
	}

	if (statePunching)
	{
		if (currentFrame == 28)
		{
			statePunching = 0;
			moveSpeed = oldMoveSpeed;
		}
	}
	else if (stateKicking)
	{
		if (currentFrame == 38)
		{
			stateKicking = 0;
			moveSpeed = oldMoveSpeed;
		}
	}
	else if (stateKnockedBack)
	{
		setFrameRange(4, 4);
	}
	else if (collidingX && (isMovingLeft || isMovingRight) && !(isMovingLeft && isMovingRight))
	{
		setFrameRange(48, 48);
	}
	else if (!isOnPlatform && velocity.y >= 0.0f)
	{
		// Jumping up
		setFrameRange(15, 15);
	}
	else if (!isOnPlatform && velocity.y <= 0.0f)
	{
		// Falling down
		setFrameRange(15, 16);
		setFrameInterval(0.1f);
	}
	else if ((isMovingLeft || isMovingRight) && !(isMovingLeft && isMovingRight))
	{
		setFrameRange(12, 14);
		setFrameInterval(0.1f);
	}
	else
	{
		setFrameRange(0, 0);
	}

	Character::update(dt);
	Projectile::updateProjectiles(dt);
}

void Guy::damageTaken()
{
	if (!isInvuln)
	{
		stateKnockedBack = 1;
		isInvuln = 1;
		tHurtFlash->reset();
		tHurtFlash->start();
	}
}

void Guy::jump()
{
	if (stateJumping == 0 && isOnPlatform)
	{
		stateJumping = 1;
	}
}

void Guy::bounce()
{
	position = oldPosition;
	velocity.y = jumpSpeed / 2;
}

void Guy::punch()
{
	if (punchPressed == 0 && statePunching == 0 && stateKicking == 0)
	{
		punchPressed = 1;
		statePunching = 1;
		setFrameRange(24, 28);
		setFrameInterval(0.1f);
		oldMoveSpeed = moveSpeed;
		moveSpeed /= 4;

		// Create projectile
		new Projectile(projectile, tilemap, position, isFlippedX);
	}
}

void Guy::kick()
{
	if (kickPressed == 0 && statePunching == 0 && stateKicking == 0 && kickCharges)
	{
		kickCharges = 0;
		kickPressed = 1;
		stateKicking = 1;
		setFrameRange(36, 38);
		setFrameInterval(0.2f);
		oldMoveSpeed = moveSpeed;
		moveSpeed *= 2;
		bounce();
	}
}

void Guy::keyboardSpecial(int key)
{
	switch (key)
	{
	case GLUT_KEY_LEFT:
		isMovingLeft = 1;
		break;
	case GLUT_KEY_RIGHT:
		isMovingRight = 1;
		break;
	}
}

void Guy::keyboardSpecialUp(int key)
{
	switch (key)
	{
	case GLUT_KEY_LEFT:
		isMovingLeft = 0;
		break;
	case GLUT_KEY_RIGHT:
		isMovingRight = 0;
		break;
	}
}

void Guy::keyboard(unsigned char key)
{
	switch (key)
	{
	case KEY_SPACE:
		if (jumpPressed == 0)
		{
			jump();
			jumpPressed = 1;
		}
		break;
	case 'd':
		punch();
		break;
	case 'f':
		kick();
		break;
	}
}

void Guy::keyboardUp(unsigned char key)
{
	switch (key)
	{
	case KEY_SPACE:
		jumpPressed = 0;
		break;
	case 'd':
		punchPressed = 0;
		break;
	case 'f':
		kickPressed = 0;
		break;
	}
}