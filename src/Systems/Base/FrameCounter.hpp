#ifndef __FrameCounter_hpp__
#define __FrameCounter_hpp__

class EventSystem;

/**
 * Frame counter used by RealLive code to ensure events happen at a
 * constant speed. Constant to all System implementations since it is
 * implemented in terms of operations on System classes.a
 *
 * See section "5.13.3 Frame counters" of the RLdev specification for
 * more details.
 */
class FrameCounter
{
public:
  FrameCounter(EventSystem& es, int frameMin, int frameMax, int milliseconds);

  virtual ~FrameCounter();

  /** 
   * Returns the current value of this frame counter, a value between
   * 
   * @return 
   */
  virtual int readFrame(EventSystem& eventSystem) = 0;

  /** 
   * When a timer starts, we need to tell the EventSystem that we now
   * have a near realtime event going on and to stop being nice to the
   * operating system.
   *
   * @see endTimer
   */
  void beginTimer(EventSystem& eventSystem);

  /** 
   * When a timer ends, there's no need to be so harsh on the
   * system. Tell the eventSystem that we no longer require near
   * realtime event handeling.
   * 
   * @see beginTimer
   */
  void endTimer(EventSystem& eventSystem);

  bool isActive() const { return m_isActive; }

// Give these accessors later?
protected:
  EventSystem& m_eventSystem;

  int m_value;
  int m_minValue;
  int m_maxValue;
  bool m_isActive;

  unsigned int m_totalTime;
};

// -----------------------------------------------------------------------

/**
 * Simple frame counter that counts from frameMin to frameMax.
 */
class SimpleFrameCounter : public FrameCounter
{
public:
  SimpleFrameCounter(EventSystem& es, int frameMin, int frameMax, int milliseconds);

  virtual int readFrame(EventSystem& eventSystem);

private:
  unsigned int m_changeInterval;
  unsigned int m_timeAtLastCheck;
};

// -----------------------------------------------------------------------

/**
 * Loop frame counter that counts from frameMin to frameMax.
 */
class LoopFrameCounter : public FrameCounter
{
public:
  LoopFrameCounter(EventSystem& es, int frameMin, int frameMax, int milliseconds);

  virtual int readFrame(EventSystem& eventSystem);

private:
  unsigned int m_changeInterval;
  unsigned int m_timeAtLastCheck;
};

// -----------------------------------------------------------------------

/**
 * Turn frame counter that counts from frameMin to frameMax.
 */
class TurnFrameCounter : public FrameCounter
{
public:
  TurnFrameCounter(EventSystem& es, int frameMin, int frameMax, int milliseconds);

  virtual int readFrame(EventSystem& eventSystem);

private:
  bool m_goingForward;
  unsigned int m_changeInterval;
  unsigned int m_timeAtLastCheck;
};

// -----------------------------------------------------------------------

/**
 * Simple frame counter that counts from frameMin to frameMax.
 */
class AcceleratingFrameCounter : public FrameCounter
{
public:
  AcceleratingFrameCounter(EventSystem& es, int frameMin, int frameMax, int milliseconds);

  virtual int readFrame(EventSystem& eventSystem);

private:
  unsigned int m_timeAtStart;
};

// -----------------------------------------------------------------------


/**
 * Simple frame counter that counts from frameMin to frameMax.
 */
class DeceleratingFrameCounter : public FrameCounter
{
public:
  DeceleratingFrameCounter(EventSystem& es, int frameMin, int frameMax, int milliseconds);

  virtual int readFrame(EventSystem& eventSystem);

private:
  unsigned int m_timeAtStart;
};


#endif