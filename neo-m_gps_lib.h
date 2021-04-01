#ifndef NEO6M_LIB_H_
#define NEO6M_LIB_H_


#include <Arduino.h>

#if defined(__AVR_ATmega168__) || defined(__AVR_ATmega328P__)

    #include <SoftwareSerial.h>
    // Choose two Arduino pins to use for software serial
    #define RXPin  2
    #define TXPin  3

    // Create a software serial port called "gpsSerial"
    static SoftwareSerial gpsSerial(RXPin, TXPin);

#else

    // Create a software serial port called "gpsSerial"
    //static Uart gpsSerial = Serial;
    #define gpsSerial Serial
#endif



// GPS mode :
#define GPS_MODE_ON     1
#define GPS_MODE_OFF    0

#define GPS_MAX_PERF    0
#define GPS_SAVE_PERF   1
#define GPS_ECO_PERF    2

//Default baud of NEO-6M is 9600
#define GPSBaud 9600


#define GGA_START_TIME 0
//  add other GGA define..

#define GLL_START_LATITUDE              0
#define GLL_START_LATITUDE_DIRECTION    1
#define GLL_START_LONGITUDE             2
#define GLL_START_LONGITUDE_DIRECTION   3
#define GLL_AVAILIDABLE_DATA            5
//  add other GLL define..

#define GRMC_AVAILIDABLE_DATA            1
#define GRMC_START_LATITUDE              2
#define GRMC_START_LATITUDE_DIRECTION    3
#define GRMC_START_LONGITUDE             4
#define GRMC_START_LONGITUDE_DIRECTION   5
//  add other GRMC define..

static uint8_t gps_status = 1;

/*
'$' suivi par un groupe de 2 lettres pour l'identifiant du récepteur. (Non limitatif):
    •GP pour Global Positioning System.
    •LC Loran-C receiver.
    •OM Omega Navigation receiver.
    •II Integrated Instrumentation (eg. AutoHelm Seatalk system).    

Puis un groupe de 3 lettres pour l'identifiant de la trame.
    •GGA : pour GPS Fix et Date.
    •GLL : pour Positionnement Géographique Longitude - Latitude.
    •GSA : pour DOP et satellites actifs.
    •GSV : pour Satellites visibles.
    •VTG : pour Direction (cap) et vitesse de déplacement (en noeuds et Km/h).
    •RMC: pour données minimales exploitables spécifiques.
*/
static struct GP_GGA
{
    uint8_t     update  = 0;
    uint8_t     time[3] = {0};    // H (1 bytes)| M (1 bytes)| S (1 bytes)|
    /* data */
}GP_GGA_t;

static struct GP_GLL
{
    uint8_t update = 0;

    int8_t  latitude_deg        = 0;     // latitude value
    float   latitude_min        = 0;     // time 
    uint8_t latitude_direction  = 0;     // orientation (N,S,W,e)

    int8_t  longitude_deg       = 0;
    float   longitude_min       = 0;
    uint8_t longitude_direction = 0;

    char    str_frame[30]       = {0};
    /* data */
}GP_GLL_t;

static struct GP_GSA
{
    uint8_t update = 0;
    /* data */
}GP_GSA_t;

static struct GP_GSV
{
    uint8_t update = 0;
    /* data */
}GP_GSV_t;

static struct GP_VTG
{
    uint8_t update = 0;
    /* data */
}GP_VTG_t;

static struct GP_RMC
{
    uint8_t update = 0;

    int8_t  latitude_deg        = 0;     // latitude value
    float   latitude_min        = 0;     // time 
    uint8_t latitude_direction  = 0;     // orientation (N,S,W,e)

    int8_t  longitude_deg       = 0;
    float   longitude_min       = 0;
    uint8_t longitude_direction = 0;

    char    str_frame[30]       = {0};
    /* data */
}GP_RMC_t;

void gps_init();
void gps_loop();

// refer p 186 on/of mode in UBLOX doc
void    setupGPSpower(uint8_t mode);
void    setupGPSmode(uint8_t mode);

    // func to get position in GPS trame
uint8_t get_cursor(uint8_t *ptr, uint8_t count);
uint8_t get_gps_status();
uint8_t get_GLL_struct(GP_GLL *gll_data);
uint8_t get_GLL_str(char *str);
uint8_t get_GRMC_struct(GP_RMC *gll_data);
uint8_t get_GRMC_str(char *str);


#endif