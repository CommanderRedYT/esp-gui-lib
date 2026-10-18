#include "menudisplay.h"

// esp-idf includes
#include <esp_log.h>

// local includes
#include "tftcolors.h"
#include "tftinterface.h"

using namespace std::chrono_literals;

constexpr const char *const TAG = "menudisplay";

namespace espgui {
void MenuDisplay::start()
{
    Base::start();

    //    m_selectedIndex = 0;
    //    m_scrollOffset = 0;

    m_rotateOffset = 0;
    m_pressed = false;

    m_upHeld = std::nullopt;
    m_downHeld = std::nullopt;
}

void MenuDisplay::initScreen(TftInterface &tft)
{
    Base::initScreen(tft);

    for (auto &label: m_labels) label.start(tft);

    runForEveryMenuItem([](MenuItem &item) { item.start(); });

    m_icons.fill(nullptr);

    m_highlightedIndex = -1;
}

void MenuDisplay::update()
{
    Base::update();

    const auto now = espchrono::millis_clock::now();
    if (m_upHeld && now >= m_upHeld->nextTimestamp)
    {
        m_upHeld->nextTimestamp += m_upHeld->counter > 3 ? 100ms : 200ms;
        m_upHeld->counter++;
        m_rotateOffset--;
    }
    if (m_downHeld && now >= m_downHeld->nextTimestamp)
    {
        m_downHeld->nextTimestamp += m_downHeld->counter > 3 ? 100ms : 200ms;
        m_downHeld->counter++;
        m_rotateOffset++;
    }

    {
        const auto offset = m_rotateOffset;
        m_rotateOffset = 0;

        if (const auto itemCount = menuItemCount())
        {
            if (m_selectedIndex == -1) m_selectedIndex = 0;

            m_selectedIndex += offset;

            if (m_selectedIndex < 0) m_selectedIndex += itemCount;
            if (m_selectedIndex >= itemCount) m_selectedIndex -= itemCount;

            if (getMenuItem(m_selectedIndex).skipScroll())
            {
                m_selectedIndex = offset >= 0 ? getNextAccessibleMenuItemIndex(m_selectedIndex)
                                              : getPreviousAccessibleMenuItemIndex(m_selectedIndex);
            }

            if (m_selectedIndex < m_scrollOffset) m_scrollOffset = m_selectedIndex;
            if (m_selectedIndex >= m_scrollOffset + m_labels.size())
                m_scrollOffset = m_selectedIndex - m_labels.size() + 1;
        }
        else
        {
            m_selectedIndex = -1;
            m_scrollOffset = 0;
        }

        runForEveryMenuItem([&](MenuItem &item) { item.update(); });
    }

    if (m_pressed)
    {
        m_pressed = false;
        if (m_selectedIndex >= 0 && m_selectedIndex < m_menuItems.size())
        {
            getMenuItem(m_selectedIndex).triggered();
        }
    }
}

void MenuDisplay::redraw(TftInterface &tft)
{
    Base::redraw(tft);

    int i{0};

    auto labelsIter = std::begin(m_labels);

    auto iconsIter = std::begin(m_icons);

    size_t newHighlightedIndex{0};

    const auto drawItemRect = [&tft](const auto &label, const auto color) {
        tft.fillRect(5, label.y() - 1, tft.width() - 10, lineHeight + 1, color);
        // tft.drawRoundRect(5,
        //                   label.y()-1,
        //                   tft.width() - 10,
        //                   lineHeight+1,
        //                   5,
        //                   color);
    };

    runForEveryMenuItemConst([&](const MenuItem &item) {
        const auto index = i++;

        if (!item.visible()) return;

        if (index < m_scrollOffset) return;

        if (labelsIter == std::end(m_labels)) return;

        const auto relativeIndex = index - m_scrollOffset;
        const auto selected = index == m_selectedIndex;

        if (selected)
        {
            newHighlightedIndex = relativeIndex;

            if (relativeIndex != m_highlightedIndex)
            {
                drawItemRect(*labelsIter, CONFIG_ESPGUI_MENUITEM_BACKGROUND_COLOR);
                *iconsIter = nullptr;
                labelsIter->start(tft);

                if (const auto icon = item.selectedIcon())
                {
                    tft.pushImage(tft.width() - 6 - MenuItemIcon::WIDTH, labelsIter->y() + 1, *icon);
                }
            }
        }
        else if (relativeIndex == m_highlightedIndex)
        {
            drawItemRect(*labelsIter, TFT_BLACK);
            *iconsIter = nullptr;
            labelsIter->start(tft);
        }

        labelsIter->redraw(tft, item.text(), item.color(),
                           selected ? CONFIG_ESPGUI_MENUITEM_BACKGROUND_COLOR : TFT_BLACK, item.font());

        if (const auto icon = item.icon(selected); icon != *iconsIter)
        {
            if (icon)
                tft.pushImage(6, labelsIter->y() + 1, *icon);
            else if (*iconsIter)
                tft.fillRect(6, labelsIter->y() + 1, 24, 24,
                             selected ? CONFIG_ESPGUI_MENUITEM_BACKGROUND_COLOR : TFT_BLACK);
            *iconsIter = icon;
        }

        //        if (selected && (relativeIndex != m_highlightedIndex))
        //        {
        //            drawItemRect(*labelsIter, CONFIG_ESPGUI_MENUITEM_BACKGROUND_COLOR);
        //        }

        labelsIter++;
        iconsIter++;
    });

    for (; labelsIter != std::end(m_labels); labelsIter++, iconsIter++)
    {

        if (const auto relativeIndex = std::distance(std::begin(m_labels), labelsIter);
            relativeIndex == m_highlightedIndex)
            drawItemRect(*labelsIter, TFT_BLACK);

        labelsIter->clear(tft, TFT_BLACK);

        if (*iconsIter)
        {
            tft.fillRect(5, labelsIter->y() + 1, 24, 24, TFT_BLACK);
            *iconsIter = nullptr;
        }
    }

    m_highlightedIndex = newHighlightedIndex;
}

void MenuDisplay::stop()
{
    Base::stop();

    runForEveryMenuItem([](MenuItem &item) { item.stop(); });
}

void MenuDisplay::buttonPressed(const Button button)
{
    // Base::buttonPressed(button);

    switch (button)
    {
        case Button::Left:
            this->back();
            break;
        case Button::Right:
            m_pressed = true;
            break;
        case Button::Up:
            m_rotateOffset--;
            m_upHeld = ButtonHeldInfo{.nextTimestamp = espchrono::millis_clock::now() + 300ms};
            break;
        case Button::Down:
            m_rotateOffset++;
            m_downHeld = ButtonHeldInfo{.nextTimestamp = espchrono::millis_clock::now() + 300ms};
            break;
    }
}

void MenuDisplay::buttonReleased(const Button button)
{
    // Base::buttonPressed(button);

    switch (button)
    {
        case Button::Up:
            m_upHeld = std::nullopt;
            break;
        case Button::Down:
            m_downHeld = std::nullopt;
            break;
        default:;
    }
}

MenuItem &MenuDisplay::getMenuItem(const std::size_t index)
{
    assert(index < m_menuItems.size());
    // Ensure the stored unique_ptr isn't null. If it is, avoid deref and return a stable empty object.
    if (!m_menuItems[index])
    {
        static EmptyMenuItem s_empty;
        ESP_LOGE(TAG, "menudisplay %s wanted to access menu item at index %i, but ptr == nullptr", title().c_str(),
                 index);
        // print all menuitems
        ESP_LOGE(TAG, "m_menuItems.size(): %i", m_menuItems.size());
        for (int i = 0; i < m_menuItems.size(); i++)
        {
            ESP_LOGE(TAG, "[%i]: %p", i, &m_menuItems[i]);
        }
        assert(false && "MenuDisplay: getMenuItem() encountered null unique_ptr");
        return s_empty;
    }
    return *m_menuItems[index];
}

const MenuItem &MenuDisplay::getMenuItem(const std::size_t index) const
{
    assert(index < m_menuItems.size());
    // Ensure the stored unique_ptr isn't null. If it is, avoid deref and return a stable empty object.
    if (!m_menuItems[index])
    {
        static EmptyMenuItem s_empty;
        ESP_LOGE(TAG, "menudisplay %s wanted to access menu item at index %i, but ptr == nullptr", title().c_str(),
                 index);
        // print all menuitems
        ESP_LOGE(TAG, "m_menuItems.size(): %i", m_menuItems.size());
        for (int i = 0; i < m_menuItems.size(); i++)
        {
            ESP_LOGE(TAG, "[%i]: %p", i, &m_menuItems[i]);
        }
        assert(false && "MenuDisplay: getMenuItem() encountered null unique_ptr");
        return s_empty;
    }
    return *m_menuItems[index];
}

// ReSharper disable once CppMemberFunctionMayBeConst
void MenuDisplay::runForEveryMenuItem(const std::function<void(MenuItem &)> &callback)
{
    if (!callback)
    {
        ESP_LOGE(TAG, "Invalid callback was passed!");
        return;
    }

    for (const auto &ptr: m_menuItems)
        if (ptr) callback(*ptr);
}

void MenuDisplay::runForEveryMenuItemConst(const std::function<void(const MenuItem &)> &callback) const
{
    if (!callback)
    {
        ESP_LOGE(TAG, "Invalid callback was passed!");
        return;
    }

    for (const auto &ptr: m_menuItems)
        if (ptr) callback(*ptr);
}

void MenuDisplay::emplaceMenuItem(std::unique_ptr<MenuItem> &&ptr)
{
    // Don't allow inserting a null pointer.
    assert(ptr && "MenuDisplay::emplaceMenuItem called with null ptr");
    if (!ptr) return;
    m_menuItems.emplace_back(std::move(ptr));
}

} // namespace espgui
