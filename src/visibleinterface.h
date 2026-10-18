#pragma once

namespace espgui {
class VisibleInterface
{
public:
    virtual ~VisibleInterface() = default;
    [[nodiscard]] virtual bool visible() const { return true; }
};

template<bool TVisible>
class StaticVisibility : public virtual VisibleInterface
{
public:
    [[nodiscard]] bool visible() const override { return TVisible; }
};

class ChangeableVisibility : public virtual VisibleInterface
{
public:
    void setVisible(const bool visible) { m_visible = visible; }
    [[nodiscard]] bool visible() const override { return m_visible; }

private:
    bool m_visible = true;
};
} // namespace espgui
