#include "centeredlabel.h"

// 3rdparty lib includes
#include <fontrenderer.h>

// local includes
#include "tftinterface.h"

namespace espgui {
CenteredLabel::CenteredLabel(const int x, const int y) : m_x{x}, m_y{y}
{
}

void CenteredLabel::start(TftInterface &)
{
    m_lastStr.clear();
    m_lastFont = -1;
    m_lastColor = -1;

    m_lastWidth = 0;
    m_lastHeight = 0;
}

void CenteredLabel::redraw(TftInterface &tft,
                           const std::string_view str,
                           const uint16_t color,
                           const uint16_t bgcolor,
                           const uint8_t font,
                           const bool forceRedraw)
{
    espgui::FontRenderer fontRenderer{tft};
    redraw(tft, fontRenderer, str, color, bgcolor, font, forceRedraw);
}

void CenteredLabel::redraw(TftInterface &tft,
                           FontRenderer &fontRenderer,
                           const std::string_view str,
                           const uint16_t color,
                           const uint16_t bgcolor,
                           const uint8_t font,
                           const bool forceRedraw)
{
    if (m_lastStr == str && m_lastColor == color && m_lastFont == font && !forceRedraw) return;

    const auto renderedWidth = fontRenderer.drawCentreString(str, m_x, m_y, color, bgcolor, font);
    const auto renderedHeight = fontRenderer.fontHeight(font);

    if (renderedWidth < m_lastWidth)
    {
        const auto oldLeftX = m_x - (m_lastWidth / 2);
        const auto newLeftX = m_x - (renderedWidth / 2);

        tft.fillRect(oldLeftX, m_y, newLeftX - oldLeftX, renderedHeight, bgcolor);

        const auto oldRightX = m_x + (m_lastWidth / 2);
        const auto newRightX = m_x + (renderedWidth / 2);

        tft.fillRect(newRightX, m_y, oldRightX - newRightX, renderedHeight, bgcolor);
    }

    //    if (renderedHeight < m_lastHeight)
    //        tft.fillRect(m_x, m_y + renderedHeight,
    //                     renderedWidth, m_lastHeight - renderedHeight,
    //                     bgcolor);

    m_lastStr = str;
    m_lastColor = color;
    m_lastFont = font;

    m_lastWidth = renderedWidth;
    m_lastHeight = renderedHeight;
}

void CenteredLabel::clear(TftInterface &tft, const uint16_t bgcolor)
{
    if (m_lastWidth || m_lastHeight) tft.fillRect(m_x - (m_lastWidth / 2), m_y, m_lastWidth, m_lastHeight, bgcolor);

    start(tft);
}
} // namespace espgui
