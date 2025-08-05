
#include "inc/FluidNC_Updater.h"

// U8G2_ST7920_128X64_1_HW_SPI u8g2(U8G2_R0, 12);
AutoOTA ota(FluidNC_Controller_Ver, "thethirdcreator/FluidNC-Controller-Updates");

void ARDUINO_ISR_ATTR WiFiHandler()
{
}

wl_status_t WiFiStatus;
uint8_t wifi_idx = 0;
uint32_t prevMs = 0;
uint8_t wifiConnectedFlag = 0; // Используется чтобы показать сообщение о подключении один раз
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
        wifiConnectedFlag = 0;
    }
    else if (WiFi.status() == WL_CONNECTED && wifiConnectedFlag == 0)
    {
        _LogPrintLn("WiFi connected to: ");
        _LogPrintLn(WiFiList[wifi_idx].ssid);
        wifiConnectedFlag = 1;
    }
}


