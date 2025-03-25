#ifndef __FLUID_UPDATER_H__
#define __FLUID_UPDATER_H__

#include <Arduino.h>
#include <U8g2lib.h>
#include <AutoOTA.h>

#define FluidNC_Controller_Ver "1.0"

U8G2_ST7920_128X64_1_HW_SPI u8g2(U8G2_R0, 12);

AutoOTA ota(FluidNC_Controller_Ver, "thethirdcreator/FluidNC-Controller-Updates");

void FluidNC_Updater()
{

  Serial.println("Checking update availability...");
  Serial.print("Update status: ");
  Serial.println(ota.checkUpdate());
  

  u8g2.firstPage();
  do
  {
    u8g2.setFont(u8g2_font_unifont_t_cyrillic); // Set Russian font
    u8g2.setCursor(0, 32);
    u8g2.print("Поиск обновлений");
  } while (u8g2.nextPage());

  delay(1000);

  if (ota.checkUpdate())
  {

    Serial.println("New version found!");
    Serial.println("Updating in ...");
    Serial.println("3");
    delay(1000);
    Serial.println("2");
    delay(1000);
    Serial.println("1");
    delay(1000);

    u8g2.firstPage();
    do
    {
      u8g2.setFont(u8g2_font_unifont_t_cyrillic); // Set Russian font
      u8g2.setCursor(0, 32);
      u8g2.print("Обновление");
    } while (u8g2.nextPage());

    Serial.println("Performing an update...");
    if (!ota.updateNow())
      Serial.println("\n\nUpdate failed!");
    AutoOTA::Error otaError = ota.getError();
    Serial.println("Error: ");
    Serial.println((uint8_t)otaError);
  }
  else
  {

    Serial.print("Update error: ");
    AutoOTA::Error otaError = ota.getError();
    Serial.println((uint8_t)otaError);

    if (otaError == AutoOTA::Error::NoUpdates)
    {
      u8g2.firstPage();
      do
      {
        u8g2.setFont(u8g2_font_unifont_t_cyrillic); // Set Russian font
        u8g2.setCursor(0, 32);
        u8g2.print("Обновлений нет");
      } while (u8g2.nextPage());
      delay(1000);
    }
    else
    {
      u8g2.firstPage();
      do
      {
        u8g2.setFont(u8g2_font_unifont_t_cyrillic); // Set Russian font
        u8g2.setCursor(0, 32);
        u8g2.print("Ошибка обновления");
      } while (u8g2.nextPage());
      delay(1000);
    }
  }
}

#endif