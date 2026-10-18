#pragma once

namespace espgui {
class ScrollInterface
{
public:
    virtual ~ScrollInterface() = default;
    [[nodiscard]] virtual bool skipScroll() const { return false; }
};

template<bool TScroll>
class StaticScrollBehaviour : public virtual ScrollInterface
{
public:
    [[nodiscard]] bool skipScroll() const override { return TScroll; }
};

using SkipScroll = StaticScrollBehaviour<true>;

} // namespace espgui
