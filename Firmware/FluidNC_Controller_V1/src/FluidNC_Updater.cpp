
#include "inc/FluidNC_Updater.h"

// U8G2_ST7920_128X64_1_HW_SPI u8g2(U8G2_R0, 12);
AutoOTA ota(FluidNC_Controller_Ver, "thethirdcreator/FluidNC-Controller-Updates");

void ARDUINO_ISR_ATTR WiFiHandler()
{

}

wl_status_t WiFiStatus;
uint8_t wifi_idx = 0;
uint32_t prevMs = 0;
void WiFi_Check()
{
    if (WiFi.status() != WL_CONNECTED && (millis() - prevMs >= WiFi_Check_Freq))
    {
        WiFiStatus = WiFi.status();
        _LogPrint("\r\nPrev status = ");
        _LogPrintLn(WiFiStatus);
        WiFi.disconnect();
        WiFi.begin(WiFiList[wifi_idx].ssid, WiFiList[wifi_idx].password);
        wifi_idx++;
        if (wifi_idx == 2)
            wifi_idx = 0;
        WiFiStatus = WiFi.status();
        _LogPrint(WiFiList[wifi_idx].ssid);
        _LogPrint(" = ");
        _LogPrintLn(WiFiStatus);
        // _LogPrintLn(info.wifi_sta_disconnected.reason);
        prevMs = millis();
    }
}

void FluidNC_Updater()
{

    _LogPrintLn("Checking update availability...");
    _LogPrint("Update status: ");
    _LogPrintLn(ota.checkUpdate());

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
