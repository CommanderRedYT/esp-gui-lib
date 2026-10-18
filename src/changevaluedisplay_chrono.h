#pragma once

// system includes
#include <string>

// 3rdparty lib includes
#include <espchrono.h>
#include <fontrenderer.h>

// local includes
#include "backinterface.h"
#include "changevaluedisplay.h"
#include "confirminterface.h"
#include "displaywithtitle.h"
#include "errorhandlerinterface.h"
#include "widgets/label.h"

namespace espgui {

namespace detail {
template<typename T>
class ChangeValueDisplayChrono : public DisplayWithTitle,
                                 public virtual AccessorInterface<T>,
                                 public virtual ConfirmInterface,
                                 public virtual BackInterface,
                                 public virtual ErrorHandlerInterface,
                                 public virtual ChangeValueDisplaySettingsInterface<T>
{
    using Base = DisplayWithTitle;

public:
    void start() override;
    void initScreen(TftInterface &tft) override;
    void update() override;
    void redraw(TftInterface &tft) override;

    void buttonPressed(Button button) override;
    void buttonReleased(Button button) override;

private:
    T m_value;

    int m_rotateOffset{};
    bool m_pressed{};

    Label m_valueLabel{36, 71}; // 188, 53

    struct ButtonHeldInfo
    {
        espchrono::millis_clock::time_point nextTimestamp;
        int counter{};
    };

    std::optional<ButtonHeldInfo> m_upHeld;
    std::optional<ButtonHeldInfo> m_downHeld;
};

template<typename T>
void ChangeValueDisplayChrono<T>::buttonReleased(const Button button)
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

} // namespace detail

template<>
class ChangeValueDisplay<espchrono::hours32> : public detail::ChangeValueDisplayChrono<espchrono::hours32>
{
};
template<>
class ChangeValueDisplay<espchrono::minutes32> : public detail::ChangeValueDisplayChrono<espchrono::minutes32>
{
};
template<>
class ChangeValueDisplay<espchrono::seconds32> : public detail::ChangeValueDisplayChrono<espchrono::seconds32>
{
};
template<>
class ChangeValueDisplay<espchrono::milliseconds32> : public detail::ChangeValueDisplayChrono<espchrono::milliseconds32>
{
};

} // namespace espgui
