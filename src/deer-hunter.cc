#include <Arduino.h>
#include <USBAPI.h>

#include <JeeLib.h>

#include "pcm.h"

#include "sample.h"

enum
{
	MOTION_DETECTOR_TRIGGER_PIN = 2,
	SPEAKER_GND_PIN = 10,
};

class DeerHunter
{
public:
	DeerHunter() :
		m_state(STATE_INIT),
		m_time(0),
		m_roy(false)
	{
	}

	void run(unsigned int millis)
	{
		m_time += millis;

		switch(m_state)
		{
		case STATE_INIT:
			setState(STATE_IDLE);
			break;

		case STATE_IDLE:
			if (m_motion)
			{
				setState(STATE_MOTION);
			}
			break;

		case STATE_MOTION:
		{
			m_roy = !m_roy;
			if (m_roy)
				startPlayback(roy, sizeof(roy));
//			else
//				startPlayback(moa, sizeof(moa));

			m_motion = false;
			setState(STATE_WAIT);

			break;
		}
		case STATE_WAIT:
			if (!isPlaying())
			{
				stopPlayback();
				setState(STATE_IDLE);
			}
			break;
		}
	}

	void runLoop()
	{
		auto curState = m_state;
		auto state = curState;
		int time = 2000;

		do
		{
			run(time);
			time = 0;
		} while (state == curState);

		digitalWrite(LED_BUILTIN, HIGH);
		sleep(50);
		digitalWrite(LED_BUILTIN, LOW);
		sleep(1950);
	}


	void onMotion()
	{
		m_motion = true;
	}

private:
	enum states
	{
		STATE_INIT,
		STATE_IDLE,
		STATE_MOTION,
		STATE_WAIT,
	};

	void sleep(unsigned ms)
	{
		if (isPlaying())
		{
			delay(ms);
		}
		else
		{
			Sleepy::loseSomeTime(ms);
		}
	}

	const char *stateToString(enum states state)
	{
		const char *str = "unknown";

		if (state == STATE_INIT)
			str = "init";
		else if (state == STATE_IDLE)
			str = "idle";
		else if (state == STATE_MOTION)
			str = "motion";
		else if (state == STATE_WAIT)
			str = "wait";

		return str;
	}

	void setState(enum states state)
	{
		Serial.print(stateToString(m_state));
		Serial.print(" -> ");
		Serial.print(stateToString(state));
		Serial.print(" at ");
		Serial.println((long)m_time, DEC);

		m_state = state;
	}

	enum states m_state;
	uint64_t m_time;
	bool m_roy;
	volatile bool m_motion{false};
};

DeerHunter *dh;

void onMotionInterrupt()
{
	dh->onMotion();
}

void setup(void)
{
	Serial.begin(9600);
	pinMode(MOTION_DETECTOR_TRIGGER_PIN, INPUT);
	pinMode(LED_BUILTIN, OUTPUT);
	pinMode(SPEAKER_GND_PIN, OUTPUT);
	digitalWrite(SPEAKER_GND_PIN, LOW);

	dh = new DeerHunter();

	attachInterrupt(digitalPinToInterrupt(MOTION_DETECTOR_TRIGGER_PIN), onMotionInterrupt, CHANGE);
}

ISR(WDT_vect)
{
	// See https://hwstartup.wordpress.com/2013/03/11/how-to-run-an-arduino-on-a-9v-battery-for-weeks-or-months/
	Sleepy::watchdogEvent();
}

void loop(void)
{
	dh->runLoop();
}
