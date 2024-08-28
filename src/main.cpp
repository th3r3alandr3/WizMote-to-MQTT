#include <wiz_mote.h>

void setup()
{
  WizMote::instance().setup();
}

void loop()
{
  WizMote::instance().loop();
}