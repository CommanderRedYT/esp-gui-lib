#include "changevaluedisplay_chrono.h"

// 3rdparty lib includes
#include <tftinterface.h>

namespace espgui::detail {
template<typename T>
void ChangeValueDisplayChrono<T>::start()
{
    Base::start();

    m_value = this->getValue();

    m_pressed = false;
}

template<typename T>
void ChangeValueDisplayChrono<T>::initScreen(TftInterface &tft)
{
    Base::initScreen(tft);

    FontRenderer fontRenderer{tft};

    tft.drawRoundRect(32, 65, 190, 34, 8, TFT_WHITE);
    m_valueLabel.start(tft);

    if (espgui::isLandscape(tft))
    {
        fontRenderer.drawString("Change value and press", 10, 152, TFT_WHITE, TFT_BLACK, 4);
        fontRenderer.drawString("button to confirm and", 10, 177, TFT_WHITE, TFT_BLACK, 4);
        fontRenderer.drawString("go back", 10, 202, TFT_WHITE, TFT_BLACK, 4);
    }
    else
    {
        fontRenderer.drawString("Change value and", 10, 160, TFT_WHITE, TFT_BLACK, 4);
        fontRenderer.drawString("press button to", 10, 185, TFT_WHITE, TFT_BLACK, 4);
        fontRenderer.drawString("confirm and go", 10, 210, TFT_WHITE, TFT_BLACK, 4);
        fontRenderer.drawString("back.", 10, 235, TFT_WHITE, TFT_BLACK, 4);
    }
}

template<typename T>
void ChangeValueDisplayChrono<T>::update()
{
    Base::update();

    const auto now = espchrono::millis_clock::now();
    if (m_upHeld && now >= m_upHeld->nextTimestamp)
    {
        using namespace std::chrono_literals;
        m_upHeld->nextTimestamp += m_upHeld->counter > 10 ? 50ms : (m_upHeld->counter > 3 ? 100ms : 200ms);
        ++m_upHeld->counter;
        m_rotateOffset--;
    }
    if (m_downHeld && now >= m_downHeld->nextTimestamp)
    {
        using namespace std::chrono_literals;
        m_downHeld->nextTimestamp += m_downHeld->counter > 10 ? 50ms : (m_downHeld->counter > 3 ? 100ms : 200ms);
        ++m_downHeld->counter;
        m_rotateOffset++;
    }

    {
        const auto rotateOffset = m_rotateOffset;
        m_rotateOffset = 0;

        m_value -= rotateOffset * this->step();
    }

    if (m_pressed)
    {
        m_pressed = false;
        if (auto result = this->setValue(m_value); result)
            confirm();
        else
            errorOccurred(std::move(result).error());
    }
}

template<typename T>
void ChangeValueDisplayChrono<T>::redraw(TftInterface &tft)
{
    Base::redraw(tft);

    m_valueLabel.redraw(tft, espchrono::toString(m_value), TFT_WHITE, TFT_BLACK, 4);
}

template<typename T>
void ChangeValueDisplayChrono<T>::buttonPressed(const Button button)
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
            using namespace std::chrono_literals;
        case Button::Up:
            m_rotateOffset--;
            m_upHeld = ButtonHeldInfo{.nextTimestamp = espchrono::millis_clock::now() + 300ms};
            break;
        case Button::Down:
            m_rotateOffset++;
            m_downHeld = ButtonHeldInfo{.nextTimestamp = espchrono::millis_clock::now() + 300ms};
            break;
        default:;
    }
}
} // namespace espgui::detail
