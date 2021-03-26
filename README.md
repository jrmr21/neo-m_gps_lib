# neo-m_gps_lib
my_uart lib to control gps module (with arduino framwork)


# example cod use this library :
# we get GPS GLL trame every 5s whit GPS set ON 300ms

#include <neo-m_gps_lib.h>

void setup()
{
  // Start the Arduino hardware serial port at 9600 baud
  Serial.begin(9600);
  gps_init();
  
}

void loop()
{
  static unsigned long time = 0;

  // tab to get GLL data frame
  static char tab[30]; 

  // "GPS thread func, don't touch this calling"
  gps_loop();



  if ((millis() - time) > 5500) // at 5,5s we set GPS power OFF
  {
    setupGPSpower(GPS_MODE_OFF);
    time = millis();

    get_GLL_str(tab);

    for (uint8_t i = 0; i < 30; i++)
      Serial.print(tab[i]);

    Serial.println();
  }
  else if ((millis() - time) > 5200)  // at 5,2s we set GPS power ON
  {
    if (get_gps_status() == 0)
      setupGPSpower(GPS_MODE_ON);
  }

}

# end example
