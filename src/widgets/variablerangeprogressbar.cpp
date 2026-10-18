#include "variablerangeprogressbar.h"

// 3rdparty lib includes
#include <cpputils.h>

// local includes
#include "tftcolors.h"
#include "tftinterface.h"

namespace espgui {
VariableRangeProgressBar::VariableRangeProgressBar(
    const int x, const int y, const int width, const int height, const int min, const int max, const uint32_t color) :
      m_x{x},
      m_y{y},
      m_width{width},
      m_height{height},
      m_min{min},
      m_max{max},
      m_color{color}
{
}

void VariableRangeProgressBar::start(TftInterface &tft)
{
    m_lastValue = m_x + 1;
    tft.drawRect(m_x, m_y, m_width, m_height, TFT_WHITE);
}

void VariableRangeProgressBar::redraw(TftInterface &tft, int value)
{
    value = cpputils::mapValueClamped(value, m_min, m_max, m_x + 1, m_x + m_width - 1);

    if (value < m_lastValue)
        tft.fillRect(value, m_y + 1, m_lastValue - value, m_height - 2, TFT_BLACK);
    else if (value > m_lastValue)
        tft.fillRect(m_lastValue, m_y + 1, value - m_lastValue, m_height - 2, m_color);

    m_lastValue = value;
}
} // namespace espgui
