#pragma once

namespace espgui {

class BackInterface
{
public:
    virtual ~BackInterface() = default;
    virtual void back() = 0;
};

template<typename T>
class BackActionInterface : public virtual BackInterface
{
public:
    void back() override { T{}.triggered(); }
};

class DummyBack : public virtual BackInterface
{
public:
    void back() override {}
};

} // namespace espgui
