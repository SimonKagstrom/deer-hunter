#include <Arduino.h>
#include <USBAPI.h>

#include <JeeLib.h>

#include "pcm.h"

#include "sample.h"

enum
{
	MOTION_DETECTOR_TRIGGER_PIN = 2,
};

class DeerHunter
{
public:
	DeerHunter() :
		m_state(STATE_INIT),
		m_time(0),
		m_motionStart(0),
		m_roy(false)
	{
	}

	void run(unsigned int millis, bool motion)
	{
		m_time += millis;

		switch(m_state)
		{
		case STATE_INIT:
			if (m_time > 1000)
				setState(STATE_IDLE);
			break;

		case STATE_IDLE:
			if (motion) {
				m_motionStart = m_time;
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
		int motion = digitalRead(MOTION_DETECTOR_TRIGGER_PIN);

		run(160, motion);
		sleep(160);
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
	uint64_t m_motionStart;
	bool m_roy;
};

DeerHunter *dh;

void setup(void)
{
	Serial.begin(9600);
	pinMode(MOTION_DETECTOR_TRIGGER_PIN, INPUT);

	dh = new DeerHunter();
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
