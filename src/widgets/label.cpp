#include "label.h"

// 3rdparty lib includes
#include <fontrenderer.h>

// local includes
#include "richtextrenderer.h"
#include "tftinterface.h"

namespace espgui {
Label::Label(int x, int y) : m_x{x}, m_y{y}
{
}

void Label::start(TftInterface &)
{
    m_lastStr.clear();
    m_lastFont = -1;
    m_lastColor = -1;

    m_lastWidth = 0;
    m_lastHeight = 0;
}

void Label::redraw(TftInterface &tft,
                   const std::string_view str,
                   const uint16_t color,
                   const uint16_t bgcolor,
                   const uint8_t font,
                   const bool forceRedraw)
{
    espgui::FontRenderer fontRenderer{tft};
    redraw(tft, fontRenderer, str, color, bgcolor, font, forceRedraw);
}

void Label::redraw(TftInterface &tft,
                   FontRenderer &fontRenderer,
                   const std::string_view str,
                   const uint16_t color,
                   const uint16_t bgcolor,
                   const uint8_t font,
                   const bool forceRedraw)
{
    if (m_lastStr == str && m_lastColor == color && m_lastFont == font && !forceRedraw) return;

    const auto renderedWidth = renderRichText(tft, fontRenderer, str, m_x, m_y, color, bgcolor, font);
    const auto renderedHeight = fontRenderer.fontHeight(font);

    if (renderedWidth < m_lastWidth)
        tft.fillRect(m_x + renderedWidth, m_y, m_lastWidth - renderedWidth, m_lastHeight, bgcolor);

    if (renderedHeight < m_lastHeight)
        tft.fillRect(m_x, m_y + renderedHeight, renderedWidth, m_lastHeight - renderedHeight, bgcolor);

    m_lastStr = str;
    m_lastColor = color;
    m_lastFont = font;

    m_lastWidth = renderedWidth;
    m_lastHeight = renderedHeight;
}

void Label::clear(TftInterface &tft, const uint16_t bgcolor)
{
    if (m_lastWidth || m_lastHeight) tft.fillRect(m_x, m_y, m_lastWidth, m_lastHeight, bgcolor);

    start(tft);
}
} // namespace espgui
