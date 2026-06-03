#include <time.h>

void setup_time()
{
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
