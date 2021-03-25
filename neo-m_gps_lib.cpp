#include "neo-m_gps_lib.h"

void gps_init()
{
    // Start the software serial port at the GPS's default baud
    gpsSerial.begin(GPSBaud);
    delay(100);
}

void gps_loop()
{
  static uint8_t  buffer[82];
  static uint8_t  i  = 0;
    // Displays information when new sentence is available.
  while (gpsSerial.available() > 0)
  {
    uint8_t tmp = gpsSerial.read();
    //Serial.write(tmp);

    if (tmp != '\n')
    {
      buffer[i] = tmp;
      i++;  
    }
    else
    {
      // end trame
      uint8_t type = buffer[3] + buffer[4] + buffer[5];


      switch (type)
      {
        case 207:   // GGA
          GP_GGA_t.update = 1;

            // *************  start get time *************
          {

            uint8_t cursor    = get_cursor(buffer, GGA_START_TIME);

            uint8_t high_bit  = cursor;
            uint8_t low_bit   = cursor + 1;

            for (uint8_t i = 0; i < 3; i++)
            {
              GP_GGA_t.time[i] = ((buffer[high_bit]-48) * 10) + (buffer[low_bit]-48);

              high_bit  += 2;
              low_bit   += 2;
            }

            //Serial.println(GP_GGA_t.time[2]);
          }
          
          
            // *************  end get time *************

          break;
        
        case 223:   // GLL
          uint8_t cursor = get_cursor(buffer, GLL_AVAILIDABLE_DATA);

          if(buffer[cursor] == 'A')
          {
            GP_GLL_t.update = 1;

            // get degreeses data, min max -90 to 90
            cursor = get_cursor(buffer, GLL_START_LATITUDE);
            GP_GLL_t.latitude_deg = ((buffer[cursor]-48) * 10) + (buffer[cursor + 1]-48);

            // example trame $GPGLL,4458.44931,N,00152.73190,E,114654.00,A,A*66
            // get minute data
            GP_GLL_t.latitude_min = ((buffer[cursor + 2]-48) * 10) + (buffer[cursor + 3]-48);
            GP_GLL_t.latitude_min += (((buffer[cursor + 5]-48) * 10) + (buffer[cursor + 6]-48)) * 0.01;
            
            // get direction (N,S,O,W) data
            cursor = get_cursor(buffer, GLL_START_LATITUDE_DIRECTION);
            GP_GLL_t.latitude_direction = buffer[cursor];
            



            // get degreeses data, min max -180 to 180
            cursor = get_cursor(buffer, GLL_START_LONGITUDE);
            GP_GLL_t.longitude_deg = (((buffer[cursor]-48) * 100) + ((buffer[cursor + 1]-48) * 10) +
                                       (buffer[cursor + 2]-48));

            // example trame $GPGLL,4458.44931,N,00152.73190,E,114654.00,A,A*66
            // get minute data
            GP_GLL_t.longitude_min = ((buffer[cursor + 3]-48) * 10) + (buffer[cursor + 4]-48);
            GP_GLL_t.longitude_min += (((buffer[cursor + 6]-48) * 10) + (buffer[cursor + 7]-48)) * 0.01;
            
            // get direction (N,S,O,W) data
            cursor = get_cursor(buffer, GLL_START_LONGITUDE_DIRECTION);
            GP_GLL_t.longitude_direction = buffer[cursor];
           
            //Serial.write( GP_GLL_t.longitude_direction);
          }

          //Serial.write( buffer[GLL_AVAILIDABLE_DATA]);
          
          break;

        case 219:   // GSA
          GP_GSA_t.update = 1;
          break;

        case 240:   // GSV
          GP_GSV_t.update = 1;
          break;

        case 241:   // VTG
          GP_VTG_t.update = 1;
          break;

        case 226:   // RMC
          GP_RMC_t.update = 1;
          break;

        default:
          break;
      }

      for (i; i > 0; i--)  // clear buffer
        buffer[i] = 0;
    }
  }
}

uint8_t get_cursor(uint8_t *ptr, uint8_t count)
{
  uint8_t cursor = 0;

  for (uint8_t i = 0; i <= count; cursor++)
    if (ptr[cursor] == ',')
      i++;

  return (cursor);
}

void setupGPSpower(uint8_t mode)
{
  if (mode == GPS_MODE_ON)
  {
    uint8_t setGPS_ON[]   =  {0xB5, 0x62, 0x06, 0x04, 0x04, 0x00, 0x00, 0x00,0x09, 0x00, 0x17, 0x76};

    gpsSerial.write(setGPS_ON, sizeof(setGPS_ON)/sizeof(uint8_t));
  }
  else if (mode == GPS_MODE_OFF)
  {
    uint8_t setGPS_OFF[]  = {0xB5, 0x62, 0x06, 0x04, 0x04, 0x00, 0x00, 0x00,0x08, 0x00, 0x16, 0x74}; 

    gpsSerial.write(setGPS_OFF , sizeof(setGPS_OFF)/sizeof(uint8_t));
  }
  
}