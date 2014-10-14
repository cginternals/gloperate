#pragma once

#include <memory>

#include <time.h>

#include <gloperate/gloperate_api.h>


namespace gloperate
{

using longf = long double;

class ChronoTimer;

class GLOPERATE_API CyclicTime
{
	enum class Status
	{
		Cycling
    ,   Pausing
	};

public:
	static long systemUtcOffset();

	CyclicTime(longf time = 0.0, longf secondsPerCycle = 0.0);
	CyclicTime(
		const time_t & time
    ,   const time_t & utcOffset ///< In seconds (UTC+01:00 is m_utcOffset = +3600).
    ,   longf secondsPerCycle = 0.0);

	virtual ~CyclicTime();

	/** Increments time appropriate to secondsPerCycle.
    */
	void update();

	// Cycling manipulation - does not tamper the time.

	void start(bool update = false);
	void pause(bool update = false);
	void reset(bool update = false); ///< resets the time to initial value (secondsPerCycle remain unchanged).
	void stop (bool update = false); ///< stops and resets the time.

    longf secondsPerCycle() const;
	void setSecondsPerCycle(longf secondsPerCycle);

	/** Float time in the intervall [0;1]
    */
    longf timef() const;
	longf timef(bool updateFirst);

	// Sets only time, date remains unchanged.
	void setTimef(longf time, bool update = false);

	// Elapsed float time from initialized time.
	longf nonModTimef(bool update = false);

	/** Time in seconds from initial time.
    */
    time_t time() const;
	time_t time(bool update);
	void setTime(const time_t & time, bool update = false);

    time_t utcOffset() const;
	void setUtcOffset(const time_t & utcOffset /* in seconds */);

    // 

	bool isRunning() const;

protected:
	static longf secondsTof(const time_t & time);
	static time_t fToSeconds(longf time);

	void initialize();

    longf elapsed() const;

protected:
    std::unique_ptr<ChronoTimer> m_timer;

    longf m_secondsPerCycle;
    Status m_mode;
    longf m_offset;
    longf m_lastModeChangeTime;
    time_t m_utcOffset;

    time_t m_time[3]; // [2] is for stop
    longf m_timef[3]; // [2] is for stop
};

} // namespace gloperate
