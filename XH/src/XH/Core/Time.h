#pragma once

namespace XH
{
    class Time
    {
    public:
        static float deltatime;

        static void SetDeltaTime(float t)
        {
            deltatime = t;
        }

        static float GetSeconds() { return deltatime; }
        static float GetMilliseconds() { return deltatime * 1000; }

    };
}
