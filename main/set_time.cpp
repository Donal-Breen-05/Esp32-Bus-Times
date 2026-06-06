#include <time.h>
#include <Arduino.h>

void setup_time() {

    // ireland timezone
    setenv("TZ", "IST-1GMT0,M10.5.0,M3.5.0/1", 1);
    tzset();

    configTime(
        0,      // GMT offset
        3600,   // daylight savings offset (Ireland summer)
        "pool.ntp.org",
        "time.nist.gov"
    );

    Serial.print("Waiting for NTP");

    time_t now = time(nullptr);

    while(now < 100000)
    {
        delay(500);
        Serial.print(".");
        now = time(nullptr);
    }

    Serial.println("\nTime synced");
}

String get_time()
{
    struct tm timeinfo;

    if(!getLocalTime(&timeinfo))
    {
        return "";
    }

    char buffer[32];

    strftime(
        buffer,
        sizeof(buffer),
        "%Y-%m-%dT%H:%M:%SZ",
        &timeinfo
    );

    return String(buffer);
}

String get_time_hhmm() {

    struct tm timeinfo;

    if (!getLocalTime(&timeinfo)) {
        return "--:--";
    }

    char buffer[6];
    strftime(buffer, sizeof(buffer), "%H:%M", &timeinfo);

    return String(buffer);
}//get time hhmmm
