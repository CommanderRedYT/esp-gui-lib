#pragma once

// system includes
#include <cstdint>
#include <string>

namespace espgui {

enum RotaryEvent
{
    Clockwise,
    CounterClockwise,
    ButtonPressed,
    ButtonReleased,
    ShortButtonPress,
    LongButtonPress,
    RotaryMax = LongButtonPress
};

inline std::string toString(const RotaryEvent rotaryEvent)
{
    switch (rotaryEvent)
    {
        case RotaryEvent::Clockwise:
            return "Clockwise";
        case RotaryEvent::CounterClockwise:
            return "CounterClockwise";
        case RotaryEvent::ButtonPressed:
            return "ButtonPressed";
        case RotaryEvent::ButtonReleased:
            return "ButtonReleased";
        case RotaryEvent::ShortButtonPress:
            return "ShortButtonPress";
        case RotaryEvent::LongButtonPress:
            return "LongButtonPress";
        default:
            return "Unknown";
    }
}

class RotaryInterface
{
public:
    virtual ~RotaryInterface() = default;
    virtual void rotaryEvent(RotaryEvent rotaryEvent) = 0;
};

} // namespace espgui
