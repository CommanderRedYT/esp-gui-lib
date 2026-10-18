#include "display.h"

// local includes
#include "tftcolors.h"
#include "tftinterface.h"

namespace espgui {

void Display::initScreen(TftInterface &tft)
{
    tft.fillScreen(TFT_BLACK);
}

} // namespace espgui
