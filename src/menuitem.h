#pragma once

// local includes
#include "actioninterface.h"
#include "colorinterface.h"
#include "fontinterface.h"
#include "iconinterface.h"
#include "scrollinterface.h"
#include "textinterface.h"
#include "visibleinterface.h"

namespace espgui {

using MenuItemIcon = Icon<24, 24>;

template<uint16_t width, uint16_t height>
class SelectableIconInterface
{
public:
    virtual ~SelectableIconInterface() = default;
    [[nodiscard]] virtual const Icon<width, height> *icon(bool selected) const { return nullptr; }
};

template<uint16_t width, uint16_t height, const Icon<width, height> *T, const Icon<width, height> *Tselected>
class StaticSelectableIcon : public virtual SelectableIconInterface<width, height>
{
public:
    const Icon<width, height> *icon(bool selected) const override { return selected ? Tselected : T; }
};

template<uint16_t width, uint16_t height>
class SelectedIconInterface
{
public:
    virtual ~SelectedIconInterface() = default;
    [[nodiscard]] virtual const Icon<width, height> *selectedIcon() const { return nullptr; }
};

template<uint16_t width, uint16_t height, const Icon<width, height> *T>
class StaticSelectedIcon : public virtual SelectedIconInterface<width, height>
{
public:
    const Icon<width, height> *selectedIcon() const override { return T; }
};

template<uint16_t width, uint16_t height>
class ChangeableSelectableIcon : public virtual SelectableIconInterface<width, height>
{
public:
    const Icon<width, height> *icon(bool selected) const override { return selected ? m_selectedIcon : m_icon; }

    void setIcon(const Icon<width, height> *icon, const Icon<width, height> *selectedIcon)
    {
        m_icon = icon;
        m_selectedIcon = selectedIcon;
    }

    void setIcon(const Icon<width, height> *icon) { m_icon = icon; }
    void setSelectedIcon(const Icon<width, height> *selectedIcon) { m_selectedIcon = selectedIcon; }

private:
    const Icon<width, height> *m_icon = nullptr;
    const Icon<width, height> *m_selectedIcon = nullptr;
};

template<uint16_t width, uint16_t height>
class ChangeableSelectedIcon : public virtual SelectedIconInterface<width, height>
{
public:
    const Icon<width, height> *selectedIcon() const override { return m_selectedIcon; }

    void setSelectedIcon(const Icon<width, height> *selectedIcon) { m_selectedIcon = selectedIcon; }

private:
    const Icon<width, height> *m_selectedIcon = nullptr;
};

using MenuItemIconInterface = SelectableIconInterface<24, 24>;

using MenuItemSelectedIconInterface = SelectedIconInterface<24, 24>;

template<const MenuItemIcon *T, const MenuItemIcon *Tselected>
using StaticMenuItemIcon = StaticSelectableIcon<24, 24, T, Tselected>;

template<const MenuItemIcon *T>
using StaticMenuItemSelectedIcon = StaticSelectedIcon<24, 24, T>;

using ChangeableMenuItemIcon = ChangeableSelectableIcon<24, 24>;

using ChangeableMenuItemSelectedIcon = ChangeableSelectedIcon<24, 24>;

class MenuItem : public virtual ActionInterface,
                 public virtual TextInterface,
                 public virtual FontInterface,
                 public virtual ColorInterface,
                 public virtual MenuItemIconInterface,
                 public virtual MenuItemSelectedIconInterface,
                 public virtual VisibleInterface,
                 public virtual ScrollInterface
{
public:
    ~MenuItem() override = default;
    virtual void start() {}
    virtual void update() {}
    virtual void stop() {}
};

template<typename T>
class MenuItemWithMenuRef : public MenuItem
{
public:
    explicit MenuItemWithMenuRef(T &menu) : m_menu{menu} {}

protected:
    T &m_menu;
};

class EmptyMenuItem : public MenuItem, public EmptyText
{
public:
    void triggered() override {}
};
} // namespace espgui
