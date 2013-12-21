#include <Arduino.h>
#include <USBAPI.h>

enum
{
	SPEAKER_PIN = 12,
	MOTION_DETECTOR_TRIGGER_PIN = 13,
};

class DeerHunter
{
public:
	DeerHunter() :
		m_state(STATE_INIT),
		m_time(0),
		m_motionStart(0)
	{
	}

	void run(unsigned int millis, bool motion)
	{
		m_time += millis;

		switch(m_state)
		{
		case STATE_INIT:
			if (m_time > 10000)
				setState(STATE_IDLE);
			break;

		case STATE_IDLE:
			noTone(SPEAKER_PIN);

			if (motion) {
				m_motionStart = m_time;
				setState(STATE_MOTION);
			}
			break;

		case STATE_MOTION:
			tone(SPEAKER_PIN, 262, 100);

			if (!motion)
				setState(STATE_IDLE);
			else
				setState(STATE_WAIT);

			break;

		case STATE_WAIT:
			noTone(SPEAKER_PIN);

			if (m_time - m_motionStart > 8000)
				setState(STATE_IDLE);
			break;
		}
	}

private:
	enum states
	{
		STATE_INIT,
		STATE_IDLE,
		STATE_MOTION,
		STATE_WAIT,
	};

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
};

DeerHunter *dh;

void setup(void)
{
	Serial.begin(9600);
	pinMode(SPEAKER_PIN, OUTPUT);
	pinMode(MOTION_DETECTOR_TRIGGER_PIN, INPUT);

	dh = new DeerHunter();
}


void loop(void)
{
	int motion = digitalRead(MOTION_DETECTOR_TRIGGER_PIN);

	dh->run(100, motion);

	delay(100);
}
