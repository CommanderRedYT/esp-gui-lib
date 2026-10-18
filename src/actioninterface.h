#pragma once

namespace espgui {
class ActionInterface
{
public:
    virtual ~ActionInterface() = default;
    virtual void triggered() = 0;
};
} // namespace espgui
