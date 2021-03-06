#include "Timer.h"

using namespace std;

// === Static Declarations ===

vector<Timer*> Timer::listTimers;

void Timer::updateTimers(float dt)
{
	for_each(listTimers.begin(), listTimers.end(), [dt](Timer*& timer) {
		if (timer->getToDelete())
		{
			delete timer;
			timer = NULL;
		}
		else
		{
			timer->update(dt);
		}
	});
	listTimers.erase(remove(listTimers.begin(), listTimers.end(), static_cast<Timer*>(NULL)), listTimers.end());
}

Timer::Timer(float interval, float duration)
{
	this->tickInterval = interval;
	this->tickDuration = durationLeft = duration;
	listTimers.push_back(this);
}

Timer::~Timer()
{
	//printf("timer deleted \n");
}

void Timer::destroy()
{
	toDelete = 1;
}

void Timer::update(float dt)
{
	if (!isRunning) return;

	// Manage time in seconds.
	currentTime += dt;

	// If tick inverval is elasped, set tick.
	if (currentTime > tickInterval)
	{
		// If timer is set to run over a duration.
		if (tickDuration != 0.0f)
		{
			durationLeft -= currentTime;
		}

		currentTime = 0.0f;
		ticked = true;
	}

	// Stop timer if it has ran its duration
	if (tickDuration != 0.0f && durationLeft < 0.0f)
	{
		isRunning = false;
	}
}

bool Timer::hasTicked()
{
	if (ticked)
	{
		ticked = false;
		return true;
	}

	return false;
}

void Timer::start()
{
	isRunning = true;
}

void Timer::stop()
{
	isRunning = false;
}

void Timer::reset()
{
	durationLeft = tickDuration;
}

float Timer::getTickInterval() const
{
	return tickInterval;
}

float Timer::getDuration() const
{
	return tickDuration;
}

bool Timer::getIsRunning() const
{
	return isRunning;
}

bool Timer::getToDelete() const
{
	return toDelete;
}

void Timer::setTickInterval(float interval)
{
	tickInterval = interval;
}
