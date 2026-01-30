// -- WIFI & OTA --
#define WIFI_AP_NAME "XIAO-SETUP"
#define HOSTNAME "XIAO-C6"

// -- OLED ---
#define SCR_WIDTH 128
#define SCR_HEIGHT 64
#define SCR_SDA    D9
#define SCR_SCL    D10

// -- DEBUG --
#define DEBUG_MODE 1
#if DEBUG_MODE
    #define LOG(x) Serial.println(x)
    #define LOGF(...) Serial.printf(__VA_ARGS__)
#else
    #define LOG(x)
    #define LOGF(...)
#endif
