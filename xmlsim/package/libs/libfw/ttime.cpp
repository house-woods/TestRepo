/***********************************************************************************************
* COPYRIGHT $Date:   Oct 27 1999 14:48:24  $ BY TELLABS OPERATIONS, INC 
* ALL RIGHTS RESERVED 
* NO PART OF THIS DOCUMENT MAY BE REPRODUCED WITHOUT 
* THE WRITTEN PERMISSION OF TELLABS OPERATIONS, INC., 
* 1000 REMINGTON BLVD., BOLINGBROOK, IL 60440
**********************************************************************************************/
#include "ttime.h"
#include <time.h>
#include "BaseException.h"
#include "sysErrLog.h"
#include "EmsUnixDefines.h"
#include <ospace/helper.h>


// Added by Shangping
#include "tutil.h"

using namespace ELogger;
using namespace EMS_FW;

extern const char* sourceName;
static const char* objectName = "TTime";

// this order matches that of the members in os_which_one - the set holding the day rank values
#define TZINFO_NEXT_TO_LAST osi_next_to_last
#define TZINFO_LAST osi_last
#define TZINFO_NO_DST osi_none
#define TZINFO_FIRST osi_first
#define TZINFO_SECOND osi_second
#define TZINFO_THIRD osi_third
#define TZINFO_FOURTH osi_fourth
#define TZINFO_FIFTH osi_fifth


// this order matches that of the day enumareted set defined in Objectspace date.h
#define TZINFO_INVALID 0
#define TZINFO_SUN 1
#define TZINFO_MON 2
#define TZINFO_TUE 3
#define TZINFO_WED 4
#define TZINFO_THU 5
#define TZINFO_FRI 6
#define TZINFO_SAT 7


// this order matches that of the month enumareted set defined in Objectspace date.h
#define TZINFO_INVALID 0
#define TZINFO_JAN 1
#define TZINFO_FEB 2
#define TZINFO_MAR 3
#define TZINFO_APR 4
#define TZINFO_MAY 5
#define TZINFO_JUNE 6
#define TZINFO_JULY 7
#define TZINFO_AUG 8
#define TZINFO_SEP 9
#define TZINFO_OCT 10
#define TZINFO_NOV 11
#define TZINFO_DEC 12


static TimeZoneInfo timezones[] =
    {
        /*Time zone string                  DST Offset hh, mm              day rank,    weekday,  month,    hh,mm*/
        // 1
        {"Eniwetok-Kwajalein [GMT-12:00]", 12, 0, TZINFO_NO_DST, TZINFO_SUN, TZINFO_JAN, 0, 0,
         /* CT_TimeZone_GMT_M12_ENIWETOK_KWAJALEIN */                      TZINFO_NO_DST,    TZINFO_SUN,    TZINFO_JAN,    0,    0},

        // 2
        {"Midway Island-Samoa [GMT-11:00]", 11, 0, TZINFO_NO_DST, TZINFO_SUN, TZINFO_JAN, 0, 0,
         /* CT_TimeZone_GMT_M11_MIDWAY_SAMOA */                            TZINFO_NO_DST,    TZINFO_SUN,    TZINFO_JAN,    0,    0},

        // 3
        {"Hawaii [GMT-10:00]", 10, 0, TZINFO_NO_DST, TZINFO_SUN, TZINFO_JAN, 0, 0,
         /* CT_TimeZone_GMT_M10_HAWAII */                                  TZINFO_NO_DST,    TZINFO_SUN,    TZINFO_JAN,    0,    0},

        // 4
        {"Alaska [GMT-09:00]", 9, 0, TZINFO_FIRST, TZINFO_SUN, TZINFO_APR, 2, 0,
         /* CT_TimeZone_GMT_M09_ALASKA */                                  TZINFO_LAST,      TZINFO_SUN,    TZINFO_OCT,    2,    0},

        // 5
        {"Pacific Time[US & Canada] [GMT-08:00]", 8 , 0, TZINFO_FIRST, TZINFO_SUN, TZINFO_APR, 2, 0,
         /* CT_TimeZone_GMT_M08_PACTIME_USCAN */                           TZINFO_LAST,      TZINFO_SUN,    TZINFO_OCT,    2,    0},

        // 6
        {"Arizona [GMT-07:00]", 7, 0, TZINFO_NO_DST, TZINFO_SUN, TZINFO_JAN, 0, 0,
         /* CT_TimeZone_GMT_M07_ARIZONA */                                 TZINFO_NO_DST,    TZINFO_SUN,    TZINFO_JAN,    0,    0},

        // 7
        {"Mountain Time [US & Canada] [GMT-07:00]", 7, 0, TZINFO_FIRST, TZINFO_SUN, TZINFO_APR, 2, 0,
         /* CT_TimeZone_GMT_M07_MOUNTAINTIME_USCAN */                      TZINFO_LAST,      TZINFO_SUN,    TZINFO_OCT,    2,    0},

        // 8
        {"CentralAmerica [GMT-06:00]", 6, 0, TZINFO_NO_DST, TZINFO_SUN, TZINFO_JAN, 0, 0,
         /* CT_TimeZone_GMT_M06_CENTRALAMERICA */                          TZINFO_NO_DST,    TZINFO_SUN,    TZINFO_JAN,    0,    0},

        // 9
        {"Central Time [US & Canada] [GMT-06:00]", 6, 0, TZINFO_FIRST, TZINFO_SUN, TZINFO_APR, 2, 0,
         /* CT_TimeZone_GMT_M06_CENTRACTIME_USCAN */                       TZINFO_LAST,      TZINFO_SUN,    TZINFO_OCT,    2,    0},

        // 10
        {"Mexico City-Tegucigalpa [GMT-06:00]", 6, 0, TZINFO_FIRST, TZINFO_SUN, TZINFO_APR, 2, 0,
         /* CT_TimeZone_GMT_M06_MEXICO_TEGUCIGALPA */                      TZINFO_LAST,      TZINFO_SUN,    TZINFO_OCT,    2,    0},

        // 11
        {"Saskatchewan [GMT-06:00]", 6, 0, TZINFO_NO_DST, TZINFO_SUN, TZINFO_JAN, 0, 0,
         /* CT_TimeZone_GMT_M06_SASKATCHEWAN */                            TZINFO_NO_DST,    TZINFO_SUN,    TZINFO_JAN,    0,    0},

        // 12
        {"Bagota-Lima-Quito [GMT-05:00]", 5, 0, TZINFO_NO_DST, TZINFO_SUN, TZINFO_JAN, 0, 0,
         /* CT_TimeZone_GMT_M05_BAGOTA_LIMA_QUITO */                       TZINFO_NO_DST,    TZINFO_SUN,    TZINFO_JAN,    0,    0},

        // 13
        {"Eastern Time [US & Canada] [GMT-05:00]", 5, 0, TZINFO_FIRST, TZINFO_SUN, TZINFO_APR, 2, 0,
         /* CT_TimeZone_GMT_M05_ EASTERNTIME_USCAN */                       TZINFO_LAST,      TZINFO_SUN,    TZINFO_OCT,    2,    0},

        // 14
        {"Indiana[East] [GMT-05:00]", 5, 0, TZINFO_NO_DST, TZINFO_SUN, TZINFO_JAN, 0, 0,
         /* CT_TimeZone_GMT_M05_INDIANA_EAST */                            TZINFO_NO_DST,    TZINFO_SUN,    TZINFO_JAN,    0,    0},

        // 15
        {"Atlantic Time [Canada] [GMT-04:00]", 4, 0, TZINFO_FIRST, TZINFO_SUN, TZINFO_APR, 2, 0,
         /* CT_TimeZone_GMT_M04_ATLANTIC_CANADA */                         TZINFO_LAST,      TZINFO_SUN,    TZINFO_OCT,    2,    0},

        // 16
        {"Caracas-La Paz [GMT-04:00]", 4, 0, TZINFO_NO_DST, TZINFO_SUN, TZINFO_JAN, 0, 0,
         /* CT_TimeZone_GMT_M04_CARACAS_LAPAZ */                           TZINFO_NO_DST,    TZINFO_SUN,    TZINFO_JAN,    0,    0},

        // 17
        {"Santiago [GMT-04:00]", 4, 0, TZINFO_SECOND, TZINFO_SAT, TZINFO_OCT, 0, 0,
         /* CT_TimeZone_GMT_M04_SANTIAGO */                                TZINFO_SECOND,     TZINFO_SAT,    TZINFO_MAR,      0,    0},

        // 18
        {"Newfoundland [GMT-03:30]", 3, 30, TZINFO_FIRST, TZINFO_SUN, TZINFO_APR, 2, 0,
         /* CT_TimeZone_GMT_M03H30_NEWFOUNDLAND */                         TZINFO_LAST,       TZINFO_SUN,    TZINFO_OCT,    2,    0},

        // 19
        {"Brasilia [GMT-03:00]", 3, 0, TZINFO_THIRD, TZINFO_SUN, TZINFO_OCT, 2, 0,
         /* CT_TimeZone_GMT_M03_BRASILIA */                                TZINFO_SECOND,     TZINFO_SUN,    TZINFO_FEB,    2,    0},

        // 20
        {"Buenos Aires, Georgetown [GMT-03:00]", 3, 0, TZINFO_NO_DST, TZINFO_SUN, TZINFO_JAN, 0, 0,
         /* CT_TimeZone_GMT_M03_BUENOS_AIRES_GEORGETOWN */                 TZINFO_NO_DST,    TZINFO_SUN,    TZINFO_JAN,    0,    0},

        // 21
        {"Greenland [GMT-03:00]", 3, 0, TZINFO_FIRST, TZINFO_SUN, TZINFO_APR, 2, 0,
         /* CT_TimeZone_GMT_M03_GREENLAND */                               TZINFO_LAST,      TZINFO_SUN,    TZINFO_OCT,    2,    0},

        // 22
        {"Mid-Atlantic [GMT-02:00]", 2, 0, TZINFO_LAST, TZINFO_SUN, TZINFO_MAR, 2, 0,
         /* CT_TimeZone_GMT_M02_MID_ATLANTIC */                            TZINFO_LAST,      TZINFO_SUN,    TZINFO_SEP,    2,    0},

        // 23
        {"Azores [GMT-01:00]", 1, 0, TZINFO_LAST, TZINFO_SUN, TZINFO_MAR, 2, 0,
         /* CT_TimeZone_GMT_M01_AZORES */                                 TZINFO_LAST,      TZINFO_SUN,    TZINFO_OCT,    3,    0},

        // 24
        {"Cape Verde Is. [GMT-01:00]", 1, 0, TZINFO_NO_DST, TZINFO_SUN, TZINFO_JAN, 0, 0,
         /* CT_TimeZone_GMT_M01_CAPE_VERDE */                              TZINFO_NO_DST,    TZINFO_SUN,    TZINFO_JAN,    0,    0},

        // 25
        {"Casablanca-Monrovia [GMT]", 0, 0, TZINFO_NO_DST, TZINFO_SUN, TZINFO_JAN, 0, 0,
         /* CT_TimeZone_GMT_CASABLANCA_MONROVIA */                         TZINFO_NO_DST,    TZINFO_SUN,    TZINFO_JAN,    0,    0},

        // 26
        {"Greenwich Mean Time: Dublin-Edinburgh-Lisbon-London [GMT]", 0, 0, TZINFO_LAST, TZINFO_SUN, TZINFO_MAR, 1, 0,
         /* CT_TimeZone_GMT_LONDON */                                     TZINFO_LAST,    TZINFO_SUN,    TZINFO_OCT,    2,    0},

        // 27
        {"Amsterdam-Copenhagen-Madrid-ParisVilnius [GMT+01:00]", -1, 0, TZINFO_LAST, TZINFO_SUN, TZINFO_MAR, 2, 0,
         /* CT_TimeZone_GMT_P01_PARIS */                                   TZINFO_LAST,    TZINFO_SUN,    TZINFO_OCT,    3,    0},

        // 28
        {"Belgrade-Sarajevo-Skopje-Sofija-Zargreb[GMT+01:00]", -1, 0, TZINFO_LAST, TZINFO_SUN, TZINFO_MAR, 2, 0,
         /* CT_TimeZone_GMT_P01_BELGRADE */                                TZINFO_LAST,    TZINFO_SUN,    TZINFO_OCT,    3,    0},

        // 29
        {"Bratislava-Budapest-Ljublijana-Prague-Wasaw [GMT+01:00]", -1, 0, TZINFO_LAST, TZINFO_SUN, TZINFO_MAR, 2, 0,
         /* CT_TimeZone_GMT_P01_BRATISLAVA */                              TZINFO_LAST,    TZINFO_SUN,    TZINFO_OCT,    3,    0},

        // 30
        {"Brussels-Berlin-Bern-Rome-Stockholm-Vienna [GMT+01:00]", -1, 0, TZINFO_LAST, TZINFO_SUN, TZINFO_MAR, 2, 0,
         /* CT_TimeZone_GMT_P01_BRUSSELS */                                TZINFO_LAST,    TZINFO_SUN,    TZINFO_OCT,    3,    0},

        // 31
        {"West Central Africa [GMT+01:00]", -1, 0, TZINFO_NO_DST, TZINFO_SUN, TZINFO_JAN, 0, 0,
         /* CT_TimeZone_GMT_P01_WEST_CENTRAL_AFRICA */                     TZINFO_NO_DST,    TZINFO_SUN,    TZINFO_JAN,    0,    0},

        // 32
        {"Athens-Istanbul-Minsk [GMT+02:00]", -2, 0, TZINFO_LAST, TZINFO_SUN, TZINFO_MAR, 2, 0,
         /* CT_TimeZone_GMT_P02_ATHENS */                                  TZINFO_LAST,    TZINFO_SUN,    TZINFO_OCT,    3,    0},

        // 33
        {"Bucharest [GMT+02:00]", -2, 0, TZINFO_LAST, TZINFO_SUN, TZINFO_MAR, 0, 0,
         /* CT_TimeZone_GMT_P02_BUCHAREST */                               TZINFO_LAST,    TZINFO_SUN,    TZINFO_SEP,    1,    0},

        // 34
        {"Cairo [GMT+02:00]", -2, 0, TZINFO_FIRST, TZINFO_FRI, TZINFO_MAY, 2, 0,
         /* CT_TimeZone_GMT_P02_CAIRO */                                   TZINFO_LAST,    TZINFO_WED,    TZINFO_SEP,    2,    0},

        // 35
        {"Harare-Pretoria [GMT+02:00]", -2, 0, TZINFO_NO_DST, TZINFO_SUN, TZINFO_JAN, 0, 0,
         /* CT_TimeZone_GMT_P02_PRETORIA */                                TZINFO_NO_DST,    TZINFO_SUN,    TZINFO_JAN,    0,    0},

        // 36
        {"Helsinki-Riga-Tallinn [GMT+02:00]", -2, 0, TZINFO_LAST, TZINFO_SUN, TZINFO_MAR, 3, 0,
         /* CT_TimeZone_GMT_P02_HELSINKI */                                TZINFO_LAST,    TZINFO_SUN,    TZINFO_OCT,    4,    0},

        // 37
        {"Jerusalem [GMT+02:00]", -2, 0, TZINFO_NO_DST, TZINFO_SUN, TZINFO_JAN, 0, 0,
         /* CT_TimeZone_GMT_P02_JERUSALEM */                               TZINFO_NO_DST,    TZINFO_SUN,    TZINFO_JAN,    0,    0},

        // 38
        {"Israel [GMT+02:00]", -2, 0, TZINFO_NO_DST, TZINFO_SUN, TZINFO_JAN, 0, 0,
         /* CT_TimeZone_GMT_P02_ISRAEL */                                  TZINFO_NO_DST,    TZINFO_SUN,    TZINFO_JAN,    0,    0},

        // 39
        {"Baghdad [GMT+03:00]", -3, 0, TZINFO_FIRST, TZINFO_SUN, TZINFO_APR, 3, 0,
         /* CT_TimeZone_GMT_P03_BAGHDAD */                                 TZINFO_FIRST,    TZINFO_SUN,    TZINFO_OCT,    4,    0},

        // 40
        {"Kuwait-Riyadh [GMT+03:00]", -3, 0, TZINFO_NO_DST, TZINFO_SUN, TZINFO_JAN, 0, 0,
         /* CT_TimeZone_GMT_P03_KUWAIT */                                  TZINFO_NO_DST,    TZINFO_SUN,    TZINFO_JAN,    0,    0},

        // 41
        {"Moscow-St.Petersburg-Volgograd[GMT+03:00]", -3, 0, TZINFO_LAST, TZINFO_SUN, TZINFO_MAR, 2, 0,
         /* CT_TimeZone_GMT_P03_MOSKOW */                                  TZINFO_LAST,    TZINFO_SUN,    TZINFO_OCT,    3,    0},

        // 42
        {"Nairobi [GMT+03:00]", -3, 0, TZINFO_NO_DST, TZINFO_SUN, TZINFO_JAN, 0, 0,
         /* CT_TimeZone_GMT_P03_NAIROBI */                                 TZINFO_NO_DST,    TZINFO_SUN,    TZINFO_JAN,    0,    0},

        // 43
        {"Tehran [GMT+03:30]", -3, 30, TZINFO_FIRST, TZINFO_SUN, TZINFO_MAR, 2, 0,
         /* CT_TimeZone_GMT_P03H30_TEHRAN */                               TZINFO_FOURTH,    TZINFO_TUE,    TZINFO_SEP,    2,    0},

        // 44
        {"Abu Dhabi-Muscat [GMT+04:00]", -4, 0, TZINFO_NO_DST, TZINFO_SUN, TZINFO_JAN, 0, 0,
         /* CT_TimeZone_GMT_P04_ABU_DHABI */                               TZINFO_NO_DST,    TZINFO_SUN,    TZINFO_JAN,    0,    0},

        // 45
        {"Baku- [GMT+04:00]", -4, 0, TZINFO_LAST, TZINFO_SUN, TZINFO_MAR, 2, 0,
         /* CT_TimeZone_GMT_P04_BAKU */                                    TZINFO_LAST,    TZINFO_SUN,    TZINFO_OCT,    3,    0},

        // 46
        {"Tbilisi [GMT+04:00]", -4, 0, TZINFO_NO_DST, TZINFO_SUN, TZINFO_JAN, 0, 0,
         /* CT_TimeZone_GMT_P04_TBILISI */                                 TZINFO_NO_DST,    TZINFO_SUN,    TZINFO_JAN,    0,    0},

        // 47
        {"Kabul [GMT+04:30]", -4, -30, TZINFO_NO_DST, TZINFO_SUN, TZINFO_JAN, 0, 0,
         /* CT_TimeZone_GMT_P04H30_KABUL */                                TZINFO_NO_DST,    TZINFO_SUN,    TZINFO_JAN,    0,    0},

        // 48
        {"Ekaterinburg [GMT+05:00]", -5, 0, TZINFO_LAST, TZINFO_SUN, TZINFO_MAR, 2, 0,
         /* CT_TimeZone_GMT_P05_EKATERINBURG */                            TZINFO_LAST,    TZINFO_SUN,    TZINFO_OCT,    3,    0},

        // 49
        {"Islamabad-Karachi-Tashkent [GMT+05:00]", -5, 0, TZINFO_NO_DST, TZINFO_SUN, TZINFO_JAN, 0, 0,
         /* CT_TimeZone_GMT_P05_ISLAMABAD */                               TZINFO_NO_DST,    TZINFO_SUN,    TZINFO_JAN,    0,    0},

        // 50
        {"Mumbai-Calcutta-Chennai-New Delhi[GMT+05:30]", -5, -30, TZINFO_NO_DST, TZINFO_SUN, TZINFO_JAN, 0, 0,
         /* CT_TimeZone_GMT_P05H30_MUMBAI */                              TZINFO_NO_DST,    TZINFO_SUN,    TZINFO_JAN,    0,    0},

        // 51
        {"Kathmandu [GMT+05:45]", -5, -45, TZINFO_NO_DST, TZINFO_SUN, TZINFO_JAN, 0, 0,
         /* CT_TimeZone_GMT_P05H45_KATHMANDU */                           TZINFO_NO_DST,    TZINFO_SUN,    TZINFO_JAN,    0,    0},

        // 52
        {"Dhaka [GMT+06:00]", -6, 0, TZINFO_NO_DST, TZINFO_SUN, TZINFO_JAN, 0, 0,
         /* CT_TimeZone_GMT_P06_DHAKA */                                   TZINFO_NO_DST,    TZINFO_SUN,    TZINFO_JAN,    0,    0},

        // 53
        {"Almaty [GMT+06:00]", -6, 0, TZINFO_NO_DST, TZINFO_SUN, TZINFO_JAN, 0, 0,
         /* CT_TimeZone_GMT_P06_ALMATY */                                  TZINFO_NO_DST,    TZINFO_SUN,    TZINFO_JAN,    0,    0},

        // 54
        {"Colombo [GMT+06:00]", -6, 0, TZINFO_NO_DST, TZINFO_SUN, TZINFO_JAN, 0, 0,
         /* CT_TimeZone_GMT_P06_COLOMBO */                 TZINFO_NO_DST,    TZINFO_SUN,    TZINFO_JAN,    0,    0},

        // 55
        {"Rangoon [GMT+06:30]", -6, -30, TZINFO_NO_DST, TZINFO_SUN, TZINFO_JAN, 0, 0,
         /* CT_TimeZone_GMT_P06H30_RANGOON */                              TZINFO_NO_DST,    TZINFO_SUN,    TZINFO_JAN,    0,    0},

        // 56
        {"Bangkok-Hanoi-Jakarta [GMT+07:00]", -7, 0, TZINFO_NO_DST, TZINFO_SUN, TZINFO_JAN, 0, 0,
         /* CT_TimeZone_GMT_P07_BANGKOK */                                 TZINFO_NO_DST,    TZINFO_SUN,    TZINFO_JAN,    0,    0},

        // 57
        {"Beijing-Chongqing-Hong Kong-Urumqi[GMT+08:00]", -8, 0, TZINFO_NO_DST, TZINFO_SUN, TZINFO_JAN, 0, 0,
         /* CT_TimeZone_GMT_P08_BEIJING */                                 TZINFO_NO_DST,    TZINFO_SUN,    TZINFO_JAN,    0,    0},

        // 58
        {"Perth [GMT+08:00]", -8, 0, TZINFO_NO_DST, TZINFO_SUN, TZINFO_JAN, 0, 0,
         /* CT_TimeZone_GMT_P08_PERTH */                                   TZINFO_NO_DST,    TZINFO_SUN,    TZINFO_JAN,    0,    0},

        // 59
        {"Singapore-Kuala Lumpur [GMT+08:00]", -8, 0, TZINFO_NO_DST, TZINFO_SUN, TZINFO_JAN, 0, 0,
         /* CT_TimeZone_GMT_P08_SINGAPORE */                                TZINFO_NO_DST,    TZINFO_SUN,    TZINFO_JAN,    0,    0},

        // 60
        {"Taipei [GMT+08:00]", -8, 0, TZINFO_NO_DST, TZINFO_SUN, TZINFO_JAN, 0, 0,
         /* CT_TimeZone_GMT_P08_TAIPEI */                                  TZINFO_NO_DST,    TZINFO_SUN,    TZINFO_JAN,    0,    0},

        // 61
        {"Osaka-Sapporo-Tokyo [GMT+09:00]", -9, 0, TZINFO_NO_DST, TZINFO_SUN, TZINFO_JAN, 0, 0,
         /* CT_TimeZone_GMT_P09_OSAKA */                                   TZINFO_NO_DST,    TZINFO_SUN,    TZINFO_JAN,    0,    0},

        // 62
        {"Seoul [GMT+09:00]", -9, 0, TZINFO_NO_DST, TZINFO_SUN, TZINFO_JAN, 0, 0,
         /* CT_TimeZone_GMT_P09_SEOUL */                                   TZINFO_NO_DST,    TZINFO_SUN,    TZINFO_JAN,    0,    0},

        // 63
        {"Yakutsk [GMT+09:00]", -9, 0, TZINFO_LAST, TZINFO_SUN, TZINFO_MAR, 2, 0,
         /* CT_TimeZone_GMT_P09_YAKUTSK */                                 TZINFO_LAST,    TZINFO_SUN,    TZINFO_OCT,    3,    0},

        // 64
        {"Adelaide [GMT+09:30]", -9, -30, TZINFO_LAST, TZINFO_SUN, TZINFO_OCT, 2, 0,
         /* CT_TimeZone_GMT_P09H30_ADELAIDE */                             TZINFO_LAST,    TZINFO_SUN,    TZINFO_MAR,    2,    0},

        // 65
        {"Darwin [GMT+09:30]", -9, -30, TZINFO_NO_DST, TZINFO_SUN, TZINFO_JAN, 0, 0,
         /* CT_TimeZone_GMT_P09H30_DARWIN */                               TZINFO_NO_DST,    TZINFO_SUN,    TZINFO_JAN,    0,    0},

        // 66
        {"Brisbane [GMT+10:00]", -10, 0, TZINFO_NO_DST, TZINFO_SUN, TZINFO_JAN, 0, 0,
         /* CT_TimeZone_GMT_P10_BRISBANE */                                TZINFO_NO_DST,    TZINFO_SUN,    TZINFO_JAN,    0,    0},

        // 67
        {"Canberra-Melbourne-Sydney [GMT+10:00]", -10, 0, TZINFO_LAST, TZINFO_SUN, TZINFO_OCT, 2, 0,
         /* CT_TimeZone_GMT_P10_SYDNEY */                                  TZINFO_LAST,    TZINFO_SUN,    TZINFO_MAR,    2,    0},

        // 68
        {"Guam-Port Moresby [GMT+10:00]", -10, 0, TZINFO_NO_DST, TZINFO_SUN, TZINFO_JAN, 0, 0,
         /* CT_TimeZone_GMT_P10_GUAM */                                    TZINFO_NO_DST,    TZINFO_SUN,    TZINFO_JAN,    0,    0},

        // 69
        {"Hobart [GMT+10:00]", -10, 0, TZINFO_FIRST, TZINFO_SUN, TZINFO_OCT, 2, 0,
         /* CT_TimeZone_GMT_P10_HOBART */                                  TZINFO_LAST,    TZINFO_SUN,    TZINFO_MAR,    2,    0},

        // 70
        {"Vladivostok [GMT+10:00]", -10, 0, TZINFO_LAST, TZINFO_SUN, TZINFO_MAR, 2, 0,
         /* CT_TimeZone_GMT_P10_VLADIVOSTOK */                             TZINFO_LAST,    TZINFO_SUN,    TZINFO_OCT,    3,    0},

        // 71
        {"Magadan-Solomon Is.-New Caledonia[GMT+11:00]", -11, 0, TZINFO_NO_DST, TZINFO_SUN, TZINFO_JAN, 0, 0,
         /* CT_TimeZone_GMT_P11_MAGADAN */                                 TZINFO_NO_DST,    TZINFO_SUN,    TZINFO_JAN,    0,    0},

        // 72
        {"Auckland-Wellington [GMT+12:00]", -12, 0, TZINFO_FIRST, TZINFO_SUN, TZINFO_OCT, 2, 0,
         /* CT_TimeZone_GMT_P12_AUCKLAND */                                TZINFO_THIRD,    TZINFO_SUN,    TZINFO_MAR,    2,    0},

        // 73
        {"Fiji-Kamchatka-Marshall Is. [GMT+12:00]", -12, 0, TZINFO_NO_DST, TZINFO_SUN, TZINFO_JAN, 0, 0,
         /* CT_TimeZone_GMT_P12_FIJI */                                    TZINFO_NO_DST,    TZINFO_SUN,    TZINFO_JAN,    0,    0},

        // 74
        {"Nuku'alofa [GMT+13:00]", -13, 0, TZINFO_NO_DST, TZINFO_SUN, TZINFO_JAN, 0, 0,
         /* CT_TimeZone_GMT_P13_NUKUALOFA */                               TZINFO_NO_DST,    TZINFO_SUN,    TZINFO_JAN,    0,    0},

        // 75
        {"Kiritimati [GMT+14:00]", -14, 0, TZINFO_NO_DST, TZINFO_SUN, TZINFO_JAN, 0, 0,
         /* CT_TimeZone_GMT_P14_KIRITIMATI */                              TZINFO_NO_DST,    TZINFO_SUN,    TZINFO_JAN,    0,    0}
    };



const int CHECK_FOR_AGENT_TIME_LEN = 15;

TTime::TTime()
{}

const char*
TTime::CompareTime(const char* eventTime, const char* clearTime, const char* ackTime)
{
    // At this point we want to compare apples to apples
    // The eventTime format for Universal Time is different from the UT for cleartime
    // The agent time is in the form of YYYYMMDDHHMMSS followed by + or - offset, 4 digit form
    // UT
    // e.g. 20000717160547+0100, translates to
    // local date July 17, 2000
    // local time 04:05:27 PM
    // UT date July 17, 2000
    // UT time 03:05:27 PM
    // The clearTime and ackTime is in the format of YYYYMMDDHHMMSS.UUZ
    // e.g. 20000717150557.61Z
    // UT date July 17, 2000
    // UT time 03:05:57 PM
    // We need to convert the event time to the clear time / ack time format before we
    // compare.
    char szUnivTime[32];
    memset(szUnivTime, 0, sizeof(szUnivTime));
    // THIS IS A TEMPORARY FIX.  WE NEED TO RE_VISIT THIS ISSUE AND DECIDE ON A SINGLE FORMAT
    if (strlen(eventTime) > CHECK_FOR_AGENT_TIME_LEN)  // Needs conversion
    {
        if (ConvertFromAgentTimeToSimplifiedGMT(eventTime, szUnivTime) == false)
        {
            // No time was returned.  Copy the old time back.
            strcpy(szUnivTime, eventTime);
        }
    }
    else
    {
        strcpy(szUnivTime, eventTime);
    }
    const char* maxTime;
    const char* returnTime;
    if ((maxTime = CompareTime(szUnivTime, clearTime) ) == NULL)
        maxTime = eventTime; //i.e. both Time are equal
    if ((returnTime = CompareTime(maxTime, ackTime) ) == NULL)
        return eventTime; //i.e. All three Time are equal
    else
        return returnTime;
}

const char*
TTime::CompareTime(const char* eventTime, const char* clearTime)
{
    if ((strcmp(eventTime, "") == 0) && (strcmp(clearTime, "") == 0))
        return NULL;
    if (strcmp(clearTime, "") == 0)
        return eventTime;
    if (strcmp(eventTime, "") == 0)
        return clearTime;
    char eventYear[5], clearYear[5];
    char eventMonth[3], clearMonth[3];
    char eventDay[3], clearDay[3];
    char eventHour[3], clearHour[3];
    char eventMinute[3], clearMinute[3];
    char eventSecond[3], clearSecond[3];
    char eventUsecond[3], clearUsecond[3];

    // Initializing the micro seconds, so always 00
    memset(eventUsecond, 0, sizeof(eventUsecond));
    memset(clearUsecond, 0, sizeof(clearUsecond));


    strncpy(eventYear, eventTime, 4);
    strncpy(eventMonth, eventTime + 4, 2);
    strncpy(eventDay, eventTime + 6, 2);
    strncpy(eventHour, eventTime + 8, 2);
    strncpy(eventMinute, eventTime + 10, 2);
    strncpy(eventSecond, eventTime + 12, 2);
    // Are there any micro seconds?
    if (strlen(eventTime) > CHECK_FOR_AGENT_TIME_LEN)  // YYYYMMDDHHMMSS.UUZ
    {
        strncpy(eventUsecond, eventTime + 15, 2);
    }

    strncpy(clearYear, clearTime, 4);
    strncpy(clearMonth, clearTime + 4, 2);
    strncpy(clearDay, clearTime + 6, 2);
    strncpy(clearHour, clearTime + 8, 2);
    strncpy(clearMinute, clearTime + 10, 2);
    strncpy(clearSecond, clearTime + 12, 2);
    // Are there any micro seconds?
    if (strlen(clearTime) > CHECK_FOR_AGENT_TIME_LEN)  // YYYYMMDDHHMMSS.UUZ
    {
        strncpy(clearUsecond, clearTime + 15, 2);
    }

    eventYear[4] = '\0';
    eventMonth[2] = '\0';
    eventDay[2] = '\0';
    eventHour[2] = '\0';
    eventMinute[2] = '\0';
    eventSecond[2] = '\0';
    eventUsecond[2] = '\0';

    clearYear[4] = '\0';
    clearMonth[2] = '\0';
    clearDay[2] = '\0';
    clearHour[2] = '\0';
    clearMinute[2] = '\0';
    clearSecond[2] = '\0';
    clearUsecond[2] = '\0';

    try
    {

        osi_date osEventDate = osi_date( (osi_month_t)atoi(eventMonth), (osi_day_t)atoi(eventDay),
                                         (osi_year_t)atoi(eventYear) );
        osi_time osEventTime = osi_time( (osi_hour_t)atoi(eventHour), (osi_minute_t)atoi(eventMinute),
                                         (osi_second_t)atoi(eventSecond), (osi_usecond_t)atoi(eventUsecond) );
        osi_date osClearDate = osi_date( (osi_month_t)atoi(clearMonth), (osi_day_t)atoi(clearDay),
                                         (osi_year_t)atoi(clearYear) );
        osi_time osClearTime = osi_time( (osi_hour_t)atoi(clearHour), (osi_minute_t)atoi(clearMinute),
                                         (osi_second_t)atoi(clearSecond), (osi_usecond_t)atoi(clearUsecond) );

        if (osEventDate > osClearDate)
        {
            return eventTime;
        }
        if (osEventDate < osClearDate)
        {
            return clearTime;
        }
        if (osEventTime > osClearTime)
        {
            return eventTime;
        }
        if (osEventTime < osClearTime)
        {
            return clearTime;
        }
    }
    catch (osi_time_toolkit_error err)
    {

        ErrorLogger::logVarError(sourceName, objectName, MAJOR,
                                 "TTime::CompareTime Caught an exception while constructing date and time: error is : %d %s", err.code(),
                                 osi_time_toolkit_error::description(err.code()));
        return 0;
    }
    catch (...)
    {

        ErrorLogger::logError(sourceName, objectName, MAJOR,
                              "TTime::CompareTime Unknown exception while constructing date and time");
        return 0;
    }

    return NULL;
}

bool TTime::isExpiredDate(const char* date)
{

    char year[5];
    char month[3];
    char day[3];        

    // yyyyMMdd
    strncpy(year, date, 4);
    strncpy(month, date + 4, 2);
    strncpy(day, date + 6, 2);

    year[4] = '\0';
    month[2] = '\0';
    day[2] = '\0';

    osi_date osDate = osi_date( (osi_month_t)atoi(month), (osi_day_t)atoi(day),
                                         (osi_year_t)atoi(year) );

    osi_time_and_date now = osi_time_and_date::now();
    osi_date today = now.utc_date();

    // Check if expired (not same day but on next day)
    if (today > osDate)
    {
        return true;
    }

    return false;
}

bool TTime::isGreater(const char* eventTime, const char* clearTime)
{
    if ((strcmp(eventTime, "") == 0) && (strcmp(clearTime, "") == 0))
        return false;
    if (strcmp(clearTime, "") == 0)
        return true;
    if (strcmp(eventTime, "") == 0)
        return false;

    char eventYear[5], clearYear[5];
    char eventMonth[3], clearMonth[3];
    char eventDay[3], clearDay[3];
    char eventHour[3], clearHour[3];
    char eventMinute[3], clearMinute[3];
    char eventSecond[3], clearSecond[3];
    char eventUsecond[3], clearUsecond[3];
    // Initializing the micro seconds, so always 00
    memset(eventUsecond, 0, sizeof(eventUsecond));
    memset(clearUsecond, 0, sizeof(clearUsecond));

    strncpy(eventYear, eventTime, 4);
    strncpy(eventMonth, eventTime + 4, 2);
    strncpy(eventDay, eventTime + 6, 2);
    strncpy(eventHour, eventTime + 8, 2);
    strncpy(eventMinute, eventTime + 10, 2);
    strncpy(eventSecond, eventTime + 12, 2);
    // Are there any micro seconds?
    if (strlen(eventTime) > CHECK_FOR_AGENT_TIME_LEN)  // YYYYMMDDHHMMSS.UUZ
    {
        strncpy(eventUsecond, eventTime + 15, 2);
    }

    strncpy(clearYear, clearTime, 4);
    strncpy(clearMonth, clearTime + 4, 2);
    strncpy(clearDay, clearTime + 6, 2);
    strncpy(clearHour, clearTime + 8, 2);
    strncpy(clearMinute, clearTime + 10, 2);
    strncpy(clearSecond, clearTime + 12, 2);
    // Are there any micro seconds?
    if (strlen(clearTime) > CHECK_FOR_AGENT_TIME_LEN)  // YYYYMMDDHHMMSS.UUZ
    {
        strncpy(clearUsecond, clearTime + 15, 2);
    }

    eventYear[4] = '\0';
    eventMonth[2] = '\0';
    eventDay[2] = '\0';
    eventHour[2] = '\0';
    eventMinute[2] = '\0';
    eventSecond[2] = '\0';
    eventUsecond[2] = '\0';

    clearYear[4] = '\0';
    clearMonth[2] = '\0';
    clearDay[2] = '\0';
    clearHour[2] = '\0';
    clearMinute[2] = '\0';
    clearSecond[2] = '\0';
    clearUsecond[2] = '\0';
    try
    {

        osi_date osEventDate = osi_date( (osi_month_t)atoi(eventMonth), (osi_day_t)atoi(eventDay),
                                         (osi_year_t)atoi(eventYear) );
        osi_time osEventTime = osi_time( (osi_hour_t)atoi(eventHour), (osi_minute_t)atoi(eventMinute),
                                         (osi_second_t)atoi(eventSecond), (osi_usecond_t)atoi(eventUsecond) );
        osi_date osClearDate = osi_date( (osi_month_t)atoi(clearMonth), (osi_day_t)atoi(clearDay),
                                         (osi_year_t)atoi(clearYear) );
        osi_time osClearTime = osi_time( (osi_hour_t)atoi(clearHour), (osi_minute_t)atoi(clearMinute),
                                         (osi_second_t)atoi(clearSecond), (osi_usecond_t)atoi(clearUsecond) );

        if (osEventDate > osClearDate)
        {
            return true;
        }
        if (osEventDate < osClearDate)
        {
            return false;
        }
        if (osEventTime > osClearTime)
        {
            return true;
        }
        if (osEventTime < osClearTime)
        {
            return false;
        }
        if (osEventDate == osClearDate)
        {
            return true;
        }
        if (osEventTime == osClearTime)
        {
            return true;
        }
    }
    catch (osi_time_toolkit_error err)
    {

        ErrorLogger::logVarError(sourceName, objectName, MAJOR,
                                 "TTime::isGreater Caught an exception while constructing date and time: error is : %d %s", err.code(),
                                 osi_time_toolkit_error::description(err.code()));
        return false;
    }
    catch (...)
    {

        ErrorLogger::logError(sourceName, objectName, MAJOR,
                              "TTime::isGreater Unknown exception while constructing date and time");
        return false;
    }


    return true;
}

bool TTime::isGreaterYear(const char* eventTime, const char* clearTime)
{
    if ((strcmp(eventTime, "") == 0) && (strcmp(clearTime, "") == 0))
        return false;
    if (strcmp(clearTime, "") == 0)
        return true;
    if (strcmp(eventTime, "") == 0)
        return false;

    char eventYear[5], clearYear[5];

    strncpy(eventYear, eventTime, 4);
    strncpy(clearYear, clearTime, 4);

    eventYear[4] = '\0';
    clearYear[4] = '\0';
    try
    {

        osi_date osEventYear = osi_date( (osi_year_t)atoi(eventYear) );
        osi_date osClearYear = osi_date( (osi_year_t)atoi(clearYear) );

        if (osEventYear > osClearYear)
        {
            return true;
        }
        if (osEventYear < osClearYear)
        {
            return false;
        }
        if (osEventYear == osClearYear)
        {
            return true;
        }

    }
    catch (...)
    {

        ErrorLogger::logError(sourceName, objectName, MAJOR,
                              "TTime::isGreaterYear Unknown exception while constructing Year");
        return false;
    }


    return true;
}

bool TTime::isValid(const char* cTime)
{
    bool valid = true;
    char eventMonth[3], eventDay[3], eventYear[5];
    char eventHour[3], eventMinute[3], eventSecond[3];
    char eventUsecond[3];

    // For MR39001
    int loopCount = 0;
    char offset[4];
    string tmpTime;
    bool minus, plus, hasZ, hasDot;
    minus = plus = hasZ = hasDot = false;

    // Initializing the micro seconds, so always 00
    memset(eventUsecond, 0, sizeof(eventUsecond));

    // date
    strncpy(eventYear, cTime, 4);
    eventYear[4] = '\0';
    strncpy(eventMonth, cTime + 4, 2);
    eventMonth[2] = '\0';
    strncpy(eventDay, cTime + 6, 2);
    eventDay[2] = '\0';

    // time
    strncpy(eventHour, cTime + 8, 2);
    eventHour[2] = '\0';
    strncpy(eventMinute, cTime + 10, 2);
    eventMinute[2] = '\0';
    strncpy(eventSecond, cTime + 12, 2);
    eventSecond[2] = '\0';

    // Are there any micro seconds?
    // if (strlen(cTime) > CHECK_FOR_AGENT_TIME_LEN)  // YYYYMMDDHHMMSS.UUZ
    // {
    //strncpy(eventUsecond, cTime + 15, 2);
    // }

    // hwang's fix for MR39001
    hasDot = extractKeyValue(".", cTime, tmpTime, tmpTime);
    hasZ = extractKeyValue("Z", cTime, tmpTime, tmpTime);
    minus = extractKeyValue("-", cTime, tmpTime, tmpTime);
    plus = extractKeyValue("+", cTime, tmpTime, tmpTime);

    // Are there any non-digit in hhss after +/-?
    if ( minus || plus )
    {
        if ( hasDot )
            strncpy(offset, cTime + 17, 4);
        else
            strncpy(offset, cTime + 15, 4);

        while ( loopCount < 4 )
        {
            if ( ! isdigit(offset[loopCount]) )
            {
                valid = false;
                return valid;
            }
            loopCount++;
        }
    }

    int len = 0;
    if ( hasDot )
    {
        if ( hasZ )   // yyyymmddhhmmss.uZ or yyyymmddhhmmss.uuZ
            len = strlen(cTime) - CHECK_FOR_AGENT_TIME_LEN - 1;
        else    // yyyymmddhhmmss.u OR yyyymmddhhmmss.u+(-)hhss
            len = 1;

        strncpy(eventUsecond, cTime + 15, len);

        if ( len == 1 )
            eventUsecond[1] = '\0';
        else
            eventUsecond[2] = '\0';
    }
    else     // There are no microseconds
        eventUsecond[0] = '\0';

    // End of the fix

    try
    {
        unsigned short month = (osi_month_t)atoi(eventMonth);
        unsigned short day = (osi_day_t)atoi(eventDay);
        short year = (osi_year_t)atoi(eventYear);

        unsigned short hour = (osi_hour_t)atoi(eventHour);
        unsigned short minute = (osi_minute_t)atoi(eventMinute);
        unsigned short second = (osi_second_t)atoi(eventSecond);
        unsigned long usecond = (osi_usecond_t)atoi(eventUsecond);

        // hwang's fix for MR39001
        if ( month > 12 || day > 31 || hour > 23 || minute > 59 || second > 59 )
        {
            valid = false;
            return valid;
        }
        // End of the fix

        osi_date osEventDate;
        osi_time osEventTime;

        osEventDate = osi_date( month, day, year );
        osEventTime = osi_time( hour, minute, second, usecond );

        if (!osEventDate.is_year(year))
            valid = false;
        if (!osEventDate.is_month(month))
            valid = false;
        if (!osEventDate.is_day_in_month(month, day, year))
            valid = false;
        if (!osEventTime.is_hour(hour))
            valid = false;
        if (!osEventTime.is_minute(minute))
            valid = false;
        if (!osEventTime.is_second(second))
            valid = false;
        if (!osEventTime.is_microsecond(usecond))
            valid = false;

        return valid;
    }
    catch (osi_time_toolkit_error error)
    {
        ErrorLogger::logVarError(sourceName, objectName, MAJOR,
                                 "osi_time_toolkit_error: %s", error.description(error.code()));
        valid = false;
        return valid;
    }
    catch (...)
    {
        ErrorLogger::logVarError(sourceName, objectName, MAJOR,
                                 "Unknown exception caught");
        valid = false;
        return valid;
    }

}

int
TTime::GetWeek(const char* eventTime)
{
    char month[3], day[3], year[5], eventTimeSGMT[32];

    //  The FM's must agree on a time format.  When FML receives an alarm, he
    //  stores it in the database with the original event time, which contains
    //  an offset to GMT.  e.g., 20020805190231-0600.  When he gives the alarm
    //  to FMR to publish to the clients, FMR converts the time (applies the
    //  offset) before publishing (e.g., 20020806010231).  In the year 2002, when
    //  GetWeek() is called with the above 2 times, GetWeek() returns 30 and 31
    //  respectively - different weeks!  When the client tries to clear/ack an alarm,
    //  he sends the alarm ID and (converted) event time to FMW, who then
    //  attempts to get the week of the alarm and query the appropriate ostore
    //  dictionary.  GetWeek() returns a week different from what FML used to
    //  to store the alarm, and the clear/ack fails.  Here we'll convert the
    //  time to simplified GMT for all FM's using GetWeek().  In this way, FML
    //  will store the alarm in the dictionary (week) that FMR and FMW will
    //  attempt to find it in.  If the event time passed in is not in the offset
    //  format, that's ok.  ConvertFromAgentTimeToSimplifiedGMT() will return the
    //  original time.

    if ( ConvertFromAgentTimeToSimplifiedGMT( eventTime, eventTimeSGMT ) == false )
    {
        //  Somehow the conversion failed.  Copy in the original time.
        strcpy( eventTimeSGMT, eventTime );
    }

    strncpy(year, eventTimeSGMT, 4);
    year[4] = '\0';
    strncpy(month, eventTimeSGMT + 4, 2);
    month[2] = '\0';
    strncpy(day, eventTimeSGMT + 6, 2);
    day[2] = '\0';
    osi_date date((osi_month_t)atoi(month), (osi_day_t)atoi(day), (osi_year_t)atoi(year));
    int week = (date.day_of_year()) / 7;
    if (week > 51)
        week = 51;  //31 st December day is taken in previous week
    return week;
}

//******************************************************************************
bool TTime::GetOsiTimeAndDate(const char* cTime, osi_time_and_date& timeDate)
{
    if (strlen(cTime) < 14)  // If input string not long enough,
        return false;

    char eventMonth[3], eventDay[3], eventYear[5];
    char eventHour[3], eventMinute[3], eventSecond[3];
    char eventUsecond[3];
    // Initializing the micro seconds, so always 00
    memset(eventUsecond, 0, sizeof(eventUsecond));

    // date
    strncpy(eventYear, cTime, 4);
    eventYear[4] = '\0';
    strncpy(eventMonth, cTime + 4, 2);
    eventMonth[2] = '\0';
    strncpy(eventDay, cTime + 6, 2);
    eventDay[2] = '\0';

    // time
    strncpy(eventHour, cTime + 8, 2);
    eventHour[2] = '\0';
    strncpy(eventMinute, cTime + 10, 2);
    eventMinute[2] = '\0';
    strncpy(eventSecond, cTime + 12, 2);
    eventSecond[2] = '\0';

    // Are there any micro seconds?
    if (strlen(cTime) > CHECK_FOR_AGENT_TIME_LEN)  // YYYYMMDDHHMMSS.UUZ
    {
        strncpy(eventUsecond, cTime + 15, 2);
    }
    eventUsecond[2] = '\0';

    try
    {
        osi_date osEventDate = osi_date( (osi_month_t)atoi(eventMonth), (osi_day_t)atoi(eventDay),
                                         (osi_year_t)atoi(eventYear) );
        osi_time osEventTime = osi_time( (osi_hour_t)atoi(eventHour), (osi_minute_t)atoi(eventMinute),
                                         (osi_second_t)atoi(eventSecond), (osi_usecond_t)atoi(eventUsecond) );
        timeDate.date(osEventDate);
        timeDate.time(osEventTime);
        timeDate.time_zone(osi_time_zone::greenwich());
        return true;
    }
    catch (osi_time_toolkit_error& err)
    {
        ErrorLogger::logVarError(sourceName, objectName, "GetOsiTimeAndDate", MAJOR,
                                 "Caught an exception while constructing date and time: error is : %d %s", err.code(),
                                 osi_time_toolkit_error::description(err.code()));
        return false;
    }
    catch (...)
    {
        ErrorLogger::logError(sourceName, objectName, "GetOsiTimeAndDate", MAJOR,
                              "Caught Unknown exception while constructing date and time");
        return false;
    }
}

//********************************************************************
void
TTime::GetTimeForDisplay(const char* cTime, char* str)
{
    if (strcmp(cTime, "") == 0)
        return ;

    char eventMonth[3], eventDay[3], eventYear[5];
    char eventHour[3], eventMinute[3], eventSecond[3];
    char eventUsecond[3];
    // Initializing the micro seconds, so always 00
    memset(eventUsecond, 0, sizeof(eventUsecond));

    // date
    strncpy(eventYear, cTime, 4);
    eventYear[4] = '\0';
    strncpy(eventMonth, cTime + 4, 2);
    eventMonth[2] = '\0';
    strncpy(eventDay, cTime + 6, 2);
    eventDay[2] = '\0';

    // time
    strncpy(eventHour, cTime + 8, 2);
    eventHour[2] = '\0';
    strncpy(eventMinute, cTime + 10, 2);
    eventMinute[2] = '\0';
    strncpy(eventSecond, cTime + 12, 2);
    eventSecond[2] = '\0';

    // Are there any micro seconds?
    if (strlen(cTime) > CHECK_FOR_AGENT_TIME_LEN)  // YYYYMMDDHHMMSS.UUZ
    {
        strncpy(eventUsecond, cTime + 15, 2);
    }
    eventUsecond[2] = '\0';

    try
    {
        osi_date osEventDate = osi_date( (osi_month_t)atoi(eventMonth), (osi_day_t)atoi(eventDay),
                                         (osi_year_t)atoi(eventYear) );
        osi_time osEventTime = osi_time( (osi_hour_t)atoi(eventHour), (osi_minute_t)atoi(eventMinute),
                                         (osi_second_t)atoi(eventSecond), (osi_usecond_t)atoi(eventUsecond) );
        osi_time_and_date timeDate;
        timeDate = osi_time_and_date(osEventTime, osEventDate, osi_time_zone::greenwich ());

        osi_time_and_date now = osi_time_and_date::now();
        timeDate.time_zone(now.time_zone());
        osi_date Date = timeDate.date(); //Date and Time converted from Universal Time Coordinates
        osi_time Time = timeDate.time(); //to local Time (wall clock) coordinates (well not really, see below).

        // For some reason ObjectSpace (2.1.6) was not returning time() with the daylight
        // savings adjustment accounted for. Using "now()" appeared to account for the adjustment
        // but when create a osi_time_and_date object and setting a time/date/time zone it appeared
        // not to work. Therefore the following code is exactly what ObjectSpace does in its "time()"
        // function. TLIaa67191.
        const osi_time_zone& zone = timeDate.time_zone();
        if ( zone.is_dst( Time, Date ) )
            Time += zone.dst_adjustment( Time, Date );

        char month[3], day[3], year[5];
        char hour[3], minute[3], second[3];
        char usecond[3];
        char microsecond[7];

        _itoa(Date.month(), month, 10);
        if (Date.month() >= 0 && Date.month() < 10)
        {
            strcpy(str, "0");
            strcat(str, month);
        }
        else
            strcpy(str, month) ;
        strcat(str, "/");
        _itoa(Date.day(), day, 10);
        if (Date.day() >= 0 && Date.day() < 10)
            strcat(str, "0");
        strcat(str, day) ;
        strcat(str, "/");
        _itoa(Date.year(), year, 10);
        if (Date.year() >= 0 && Date.year() < 10)
            strcat(str, "0");
        strcat(str, year) ;
        strcat(str, " ");
        _itoa(Time.hours(), hour, 10);
        if (Time.hours() >= 0 && Time.hours() < 10)
            strcat(str, "0");
        strcat(str, hour) ;
        strcat(str, ":");
        _itoa(Time.minutes(), minute, 10);
        if (Time.minutes() >= 0 && Time.minutes() < 10)
            strcat(str, "0");
        strcat(str, minute) ;
        strcat(str, ":");
        _itoa(Time.seconds(), second, 10);
        if (Time.seconds() >= 0 && Time.seconds() < 10)
            strcat(str, "0");
        strcat(str, second) ;
        strcat(str, ":") ;
        _itoa(Time.microseconds(), microsecond, 10);
        strncpy(usecond, microsecond, 2);
        usecond[2] = '\0';
        if (atoi(usecond) >= 0 && atoi(usecond) < 10)
            strcat(str, "0");
        strcat(str, usecond) ;
    }
    catch (osi_time_toolkit_error err)
    {

        ErrorLogger::logVarError(sourceName, objectName, MAJOR,
                                 "TTime::GetTimeForDisplay Caught an exception while constructing date and time: error is : %d %s", err.code(),
                                 osi_time_toolkit_error::description(err.code()));
        strcpy(str, "");
    }
    catch (...)
    {

        ErrorLogger::logError(sourceName, objectName, MAJOR,
                              "TTime::GetTimeForDisplay Unknown exception while constructing date and time");
        strcpy(str, "");
    }

}

void
TTime::GetTimeInAgentFormat(osi_date today, osi_time Timenow, char* str)
{
    char month[3], day[3], year[5];
    char hour[3], minute[3], second[3];
    char usecond[3];
    char microsecond[7]; //Used for converting 6byte string returned by microseconds to usecond

    _itoa(today.year(), year, 10);
    if (today.year() >= 0 && today.year() < 10)
    {
        strcpy(str, "0");
        strcat(str, year);
    }
    else
        strcpy(str, year) ;
    _itoa(today.month(), month, 10);
    if (today.month() >= 0 && today.month() < 10)
        strcat(str, "0");
    strcat(str, month) ;
    _itoa(today.day(), day, 10);
    if (today.day() >= 0 && today.day() < 10)
        strcat(str, "0");
    strcat(str, day) ;

    _itoa(Timenow.hours(), hour, 10);
    if (Timenow.hours() >= 0 && Timenow.hours() < 10)
        strcat(str, "0");
    strcat(str, hour) ;
    _itoa(Timenow.minutes(), minute, 10);
    if (Timenow.minutes() >= 0 && Timenow.minutes() < 10)
        strcat(str, "0");
    strcat(str, minute) ;
    _itoa(Timenow.seconds(), second, 10);
    if (Timenow.seconds() >= 0 && Timenow.seconds() < 10)
        strcat(str, "0");
    strcat(str, second) ;
    strcat(str, ".") ;
    _itoa(Timenow.microseconds(), microsecond, 10);
    strncpy(usecond, microsecond, 2);
    usecond[2] = '\0';
    if (atoi(usecond) >= 0 && atoi(usecond) < 10)
        strcat(str, "0");
    strcat(str, usecond) ;
    strcat(str, "Z") ;
}

void
TTime::ConvertTimeFromDisplayToAgentFormat(const char* displayTime, char* str)
{
    char eventMonth[3], eventDay[3], eventYear[5];
    char eventHour[3], eventMinute[3], eventSecond[3];
    char eventUsecond[3];
    // Initializing the micro seconds, so always 00
    memset(eventUsecond, 0, sizeof(eventUsecond));

    // date
    strncpy(eventMonth, displayTime, 2);
    eventMonth[2] = '\0';
    strncpy(eventDay, displayTime + 3, 2);
    eventDay[2] = '\0';
    strncpy(eventYear, displayTime + 6, 4);
    eventYear[4] = '\0';

    // time
    strncpy(eventHour, displayTime + 11, 2);
    eventHour[2] = '\0';
    strncpy(eventMinute, displayTime + 14, 2);
    eventMinute[2] = '\0';
    strncpy(eventSecond, displayTime + 17, 2);
    eventSecond[2] = '\0';
    if (strlen(displayTime) > 20)
    {
        strncpy(eventUsecond, displayTime + 20, 2);
    }
    eventUsecond[2] = '\0';
    try
    {

        osi_date osEventDate = osi_date( (osi_month_t)atoi(eventMonth), (osi_day_t)atoi(eventDay),
                                         (osi_year_t)atoi(eventYear) );
        osi_time osEventTime = osi_time( (osi_hour_t)atoi(eventHour), (osi_minute_t)atoi(eventMinute),
                                         (osi_second_t)atoi(eventSecond), (osi_usecond_t)atoi(eventUsecond) );

        osi_time_and_date localTimeDate = osi_time_and_date(osEventTime, osEventDate);
        osi_date localDay = localTimeDate.utc_date(); //Date and Time is in Universal Time Coordinates
        osi_time localTime = localTimeDate.utc_time();
        GetTimeInAgentFormat(localDay, localTime, str);
    }
    catch (osi_time_toolkit_error err)
    {
        ErrorLogger::logVarError(sourceName, objectName, MAJOR,
                                 "TTime::ConvertTimeFromDisplayToAgentFormat Caught an exception while constructing date and time: error is : %d %s", err.code(),
                                 osi_time_toolkit_error::description(err.code()));
        strcpy(str, "");
    }
    catch (...)
    {

        ErrorLogger::logError(sourceName, objectName, MAJOR,
                              "TTime::GetTimeForDisplay Unknown exception while constructing date and time");
        strcpy(str, "");
    }


}
//Convert the local time to utc time and agent format.
void TTime::ConvertLocalTimeToAgentFormat(const char* displayTime, char* str)
{
    char eventMonth[3], eventDay[3], eventYear[5];
    char eventHour[3], eventMinute[3], eventSecond[3];
    char eventUsecond[3];

    // Initializing the micro seconds, so always 00
    memset(eventUsecond, 0, sizeof(eventUsecond));

    strncpy(eventYear, displayTime, 4);
    strncpy(eventMonth, displayTime + 4, 2);
    strncpy(eventDay, displayTime + 6, 2);
    strncpy(eventHour, displayTime + 8, 2);
    strncpy(eventMinute, displayTime + 10, 2);
    strncpy(eventSecond, displayTime + 12, 2);

    eventYear[4] = '\0';
    eventMonth[2] = '\0';
    eventDay[2] = '\0';
    eventHour[2] = '\0';
    eventMinute[2] = '\0';
    eventSecond[2] = '\0';
    eventUsecond[2] = '\0';

    try
    {

        osi_date osEventDate = osi_date( (osi_month_t)atoi(eventMonth), (osi_day_t)atoi(eventDay),
                                         (osi_year_t)atoi(eventYear) );
        osi_time osEventTime = osi_time( (osi_hour_t)atoi(eventHour), (osi_minute_t)atoi(eventMinute),
                                         (osi_second_t)atoi(eventSecond), (osi_usecond_t)atoi(eventUsecond) );

        osi_time_and_date localTimeDate = osi_time_and_date(osEventTime, osEventDate);
        osi_date localDay = localTimeDate.utc_date();
        osi_time localTime = localTimeDate.utc_time();
        GetTimeInAgentFormat(localDay, localTime, str);
    }
    catch (osi_time_toolkit_error err)
    {
        ErrorLogger::logVarError(sourceName, objectName, MAJOR,
                                 "TTime::ConvertLocalTimeToAgentFormat Exception while constructing date and time: error is : %d %s", err.code(),
                                 osi_time_toolkit_error::description(err.code()));
        strcpy(str, "");
    }
    catch (...)
    {

        ErrorLogger::logError(sourceName, objectName, MAJOR,
                              "TTime::ConvertLocalTimeToAgentFormat Unknown exception while constructing date and time");
        strcpy(str, "");
    }


}

// The following method is added by Shangping
bool TTime::ConvertFromAgentTimeToSimplifiedGMT(const char* agentTime, char* sGMTTime)
{
    const char *methodName = "ConvertFromAgentTimeToSimplifiedGMT";
    bool retVal = true;

    // ErrorLogger::logError( sourceName, objectName, methodName, TRACE4, "Entering..." );

    /* ErrorLogger::logVarError( sourceName, objectName, methodName, __LINE__, TRACE1,
      "Converting time [%s] to simplified GMT.", agentTime );*/

    /* The agent time format can be the following three formats:
     (1) yyyymmddhhmmss.u, for example 19851106210627.3 (local time) 
     (2) yyyymmddhhmmss.uZ, for example 19851106210627.3Z (GMT)
     (3) yyyymmddhhmmss.u - hhss, for example 19851106210627.3-0500 (local time, and offset from GMT)
       
     We also assume that the variable agentTime cannot be empty string, or null
    */
    if (agentTime == NULL)
        return false;

    try
    {

        const string SGMT = "YYYYMMDDHHMMSS";
        string agentTimeStr = string(agentTime);

        if (agentTimeStr.size() < SGMT.size()) // The agentTime should be at least of the length YYYYMMDDHHMMSS.
        {
            ErrorLogger::logVarError( sourceName, objectName, methodName, __LINE__, MAJOR,
                                      "The length [%d] of input time [%s] is too short.  Cannot convert.  Returning...",
                                      agentTimeStr.size(), agentTime );

            return false;
        }

        if (agentTimeStr.size() == SGMT.size())
        {
            // The agentTime has the same format as SGMT format, no conversion needed.
            ErrorLogger::logError( sourceName, objectName, methodName, __LINE__, TRACE5,
                                   "Time already in YYYYMMDDHHMMSS format - no conversion necessary.  Returning original time." );
            strcpy(sGMTTime, agentTime);
            return true;
        }

        string localTime, offset, ossGMTTimeDateStr;
        string ignorePart, tmpStr;

        bool minus, plus, hasZ;
        minus = plus = hasZ = false;

        char sGMTMonth[3], sGMTDay[3], sGMTYear[5];
        char sGMTHour[3], sGMTMinute[3], sGMTSecond[3];
        char sGMTUsecond[3];

        char offsetHour[3], offsetMinute[3];

        minus = extractKeyValue("-", agentTime, localTime, offset);
        plus = extractKeyValue("+", agentTime, localTime, offset);
        extractKeyValue(".", agentTime, ossGMTTimeDateStr, ignorePart);
        hasZ = extractKeyValue("Z", ignorePart, tmpStr, tmpStr);


        if ( minus || plus )
        {
            // It is the format (3).  We need to adjust the offset.

            // date
            strncpy(sGMTYear, agentTime, 4);
            sGMTYear[4] = '\0';

            strncpy(sGMTMonth, agentTime + 4, 2);
            sGMTMonth[2] = '\0';

            strncpy(sGMTDay, agentTime + 6, 2);
            sGMTDay[2] = '\0';

            strncpy(sGMTHour, agentTime + 8, 2);
            sGMTHour[2] = '\0';

            strncpy(sGMTMinute, agentTime + 10, 2);
            sGMTMinute[2] = '\0';

            strncpy(sGMTSecond, agentTime + 12, 2);
            sGMTSecond[2] = '\0';

            sGMTUsecond[0] = '\0';  //note: we don't use the Usecond bit right now, just innore it and set it to zero

            osi_date ossGMTDate = osi_date( (osi_month_t)atoi(sGMTMonth), (osi_day_t)atoi(sGMTDay),
                                            (osi_year_t)atoi(sGMTYear) );
            osi_time ossGMTTime = osi_time( (osi_hour_t)atoi(sGMTHour), (osi_minute_t)atoi(sGMTMinute),
                                            (osi_second_t)atoi(sGMTSecond), (osi_usecond_t)atoi(sGMTUsecond) );
            osi_time_and_date osLocalTime = osi_time_and_date(ossGMTTime, ossGMTDate);

            // Find the offset
            strncpy(offsetHour, offset.c_str(), 2);
            offsetHour[2] = '\0';

            strncpy(offsetMinute, offset.c_str() + 2, 2);
            offsetMinute[2] = '\0';

            osi_time_period osOffset = osi_time_period(0, atol(offsetHour), atol(offsetMinute), 0, 0);

            // Find if it is "-" or "+"
            osi_time_and_date ossGMTTimeDate;

            if (minus)
            {
                ossGMTTimeDate = osLocalTime + osOffset;
            }
            else
            {
                ossGMTTimeDate = osLocalTime - osOffset;
            }

            ossGMTTimeDateStr = ossGMTTimeDate.to_string();
            string year, month, date, hour, minute, second, tmp1, tmp2;
            extractKeyValue("/", ossGMTTimeDateStr, month, tmp1);
            extractKeyValue("/", tmp1, date, tmp2);
            extractKeyValue(" ", tmp2, year, tmp1);
            year = "20" + year;
            extractKeyValue(":", tmp1, hour, tmp2);
            extractKeyValue(":", tmp2, minute, tmp1);
            extractKeyValue(":", tmp1, second, tmp2);

            ossGMTTimeDateStr = year + month + date + hour + minute + second;

            ErrorLogger::logVarError( sourceName, objectName, methodName, __LINE__, TRACE3,
                                      "Converted time = [%s]", ossGMTTimeDateStr.c_str() );

            strcpy(sGMTTime, ossGMTTimeDateStr.c_str());
            sGMTTime[14] = '\0';
        }
        else
        {
            // It is either the format (1), or (2).
            if (hasZ) // It is format (2), truncate the .xZ
            {
                strncpy(sGMTTime, ossGMTTimeDateStr.c_str(), ossGMTTimeDateStr.length());
                sGMTTime[14] = '\0';
            }
            else // It is format (1), local time and need to convert to sGMT time.
            {
                ErrorLogger::logVarError( sourceName, objectName, methodName, __LINE__, MINOR,
                                          "Cannot convert [apparently] local time [%s], returning original time.",
                                          agentTime );

                strcpy( sGMTTime, agentTime );
                retVal = false;
            }
        }

        // ErrorLogger::logError( sourceName, objectName, methodName, TRACE4, "Returning..." );
        return retVal;

    }
    catch (osi_time_toolkit_error err)
    {

        ErrorLogger::logVarError(sourceName, objectName, MAJOR,
                                 "TTime::CompareTime Caught an exception while constructing date and time: error is : %d %s", err.code(),
                                 osi_time_toolkit_error::description(err.code()));
        return false;

    }
    catch (...)
    {
        ErrorLogger::logError(sourceName, objectName, MAJOR, "TTime::CompareTime Unknown exception while constructing date and time");
        return false;
    }
}

void
TTime::GetCurrentAgentTime(char* str)
{
    osi_time_and_date now = osi_time_and_date::now();
    osi_date today = now.utc_date(); //Date and Time is in Universal Time Coordinates
    osi_time Timenow = now.utc_time();
    GetTimeInAgentFormat(today, Timenow, str);
}


bool TTime::extractKeyValue(const char* delim, const string& line,
                            string& key, string& value)
{
    //if(ErrorLogger::isTraceListOn())
    // ErrorLogger::logVarError(sourceName,getClassName().c_str(),"extractKeyValue",__LINE__,TRACE3,"Method Begin line=%s",line.c_str());
    if (delim == NULL)
    { // invalid delimeter
        // ErrorLogger::logError(sourceName,getClassName().c_str(),"extractKeyValue",
        //__LINE__,CRITICAL,
        //"NULL Delimeter ???");
        return false;
    }
    if (line.size() == 0)
    { // line with zero size
        //ErrorLogger::logError(sourceName,getClassName().c_str(),"extractKeyValue",
        //__LINE__,CRITICAL,
        //"Zero size Line ???");
        return false;
    }
    int pos = line.find(delim);
    if (pos < 1) // Invalid Line
    {  // line is not valid unable to find delimeter should be Key=Value
        //ErrorLogger::logVarError(sourceName,getClassName().c_str(),"extractKeyValue",
        //__LINE__,CRITICAL,
        //"Invalid Line no delimeter %s",line.c_str());
        return false;
    }
    key = line.substr(0, pos);
    if (((unsigned)(pos + 1)) >= line.size())
    { // empty value
        value = "";
    }
    else
    {
        value = line.substr(pos + 1);
    }
    //if(ErrorLogger::isTraceListOn())
    // ErrorLogger::logVarError(sourceName,getClassName().c_str(),"extractKeyValue",__LINE__,TRACE3,"Method finished key=%s value=%s",key.c_str(),value.c_str());
    return true;
}

void
TTime::periodAddedDate(const char* eventTime, int period, char* str, PeriodType periodType )
{

    int m_days;
    int m_hours;
    int m_mins;
    int m_secs;
    int m_millisecs;
    m_days = m_hours = m_mins = m_secs = m_millisecs = 0;
    if (periodType == days)
        m_days = period;
    else if (periodType == hours)
        m_hours = period;
    else if (periodType == mints)
        m_mins = period;
    else if (periodType == secs)
        m_secs = period;
    else if (periodType == millisecs)
        m_millisecs = period;



    if (strcmp(eventTime, "") == 0)
    { // return 0;
        throw;
    }
    char eventYear[5];
    char eventMonth[3];
    char eventDay[3];
    char eventHour[3];
    char eventMinute[3];
    char eventSecond[3];
    char eventUsecond[3];

    // Initializing the micro seconds, so always 00
    memset(eventUsecond, 0, sizeof(eventUsecond));


    strncpy(eventYear, eventTime, 4);
    strncpy(eventMonth, eventTime + 4, 2);
    strncpy(eventDay, eventTime + 6, 2);
    strncpy(eventHour, eventTime + 8, 2);
    strncpy(eventMinute, eventTime + 10, 2);
    strncpy(eventSecond, eventTime + 12, 2);
    // Are there any micro seconds?
    if (strlen(eventTime) > CHECK_FOR_AGENT_TIME_LEN)  // YYYYMMDDHHMMSS.UUZ
    {
        strncpy(eventUsecond, eventTime + 15, 2);
    }


    eventYear[4] = '\0';
    eventMonth[2] = '\0';
    eventDay[2] = '\0';
    eventHour[2] = '\0';
    eventMinute[2] = '\0';
    eventSecond[2] = '\0';
    eventUsecond[2] = '\0';



    try
    {

        osi_date osEventDate = osi_date( (osi_month_t)atoi(eventMonth), (osi_day_t)atoi(eventDay),
                                         (osi_year_t)atoi(eventYear) );
        osi_time osEventTime = osi_time( (osi_hour_t)atoi(eventHour), (osi_minute_t)atoi(eventMinute),
                                         (osi_second_t)atoi(eventSecond), (osi_usecond_t)atoi(eventUsecond) );

        osi_time_and_date timeDate = osi_time_and_date(osEventTime, osEventDate);

        timeDate = timeDate + osi_time_period( m_days, m_hours, m_mins, m_secs, m_millisecs); //period, 0, 0, 0, 0 );
        osi_time addedTime = timeDate.time();
        osi_date addedDate = timeDate.date();

        GetTimeInAgentFormat(addedDate, addedTime, str);


    }
    catch (osi_time_toolkit_error err)
    {

        ErrorLogger::logVarError(sourceName, objectName, MAJOR,
                                 "TTime::CompareTime Caught an exception while constructing date and time: error is : %d %s", err.code(),
                                 osi_time_toolkit_error::description(err.code()));
        throw;
    }
    catch (...)
    {

        ErrorLogger::logError(sourceName, objectName, MAJOR,
                              "TTime::CompareTime Unknown exception while constructing date and time");
        throw;
    }

    //return NULL;


}


const int
TTime::getDays(const char* eventTime, const char* clearTime)
{
    if (strcmp(clearTime, "") == 0)
        return 0;
    if (strcmp(eventTime, "") == 0)
        return 0;
    char eventYear[5], clearYear[5];
    char eventMonth[3], clearMonth[3];
    char eventDay[3], clearDay[3];
    char eventHour[3], clearHour[3];
    char eventMinute[3], clearMinute[3];
    char eventSecond[3], clearSecond[3];
    char eventUsecond[3], clearUsecond[3];

    // Initializing the micro seconds, so always 00
    memset(eventUsecond, 0, sizeof(eventUsecond));
    memset(clearUsecond, 0, sizeof(clearUsecond));


    strncpy(eventYear, eventTime, 4);
    strncpy(eventMonth, eventTime + 4, 2);
    strncpy(eventDay, eventTime + 6, 2);
    strncpy(eventHour, eventTime + 8, 2);
    strncpy(eventMinute, eventTime + 10, 2);
    strncpy(eventSecond, eventTime + 12, 2);
    // Are there any micro seconds?
    if (strlen(eventTime) > CHECK_FOR_AGENT_TIME_LEN)  // YYYYMMDDHHMMSS.UUZ
    {
        strncpy(eventUsecond, eventTime + 15, 2);
    }

    strncpy(clearYear, clearTime, 4);
    strncpy(clearMonth, clearTime + 4, 2);
    strncpy(clearDay, clearTime + 6, 2);
    strncpy(clearHour, clearTime + 8, 2);
    strncpy(clearMinute, clearTime + 10, 2);
    strncpy(clearSecond, clearTime + 12, 2);
    // Are there any micro seconds?
    if (strlen(clearTime) > CHECK_FOR_AGENT_TIME_LEN)  // YYYYMMDDHHMMSS.UUZ
    {
        strncpy(clearUsecond, clearTime + 15, 2);
    }

    eventYear[4] = '\0';
    eventMonth[2] = '\0';
    eventDay[2] = '\0';
    eventHour[2] = '\0';
    eventMinute[2] = '\0';
    eventSecond[2] = '\0';
    eventUsecond[2] = '\0';

    clearYear[4] = '\0';
    clearMonth[2] = '\0';
    clearDay[2] = '\0';
    clearHour[2] = '\0';
    clearMinute[2] = '\0';
    clearSecond[2] = '\0';
    clearUsecond[2] = '\0';

    try
    {

        osi_date osEventDate = osi_date( (osi_month_t)atoi(eventMonth), (osi_day_t)atoi(eventDay),
                                         (osi_year_t)atoi(eventYear) );
        osi_time osEventTime = osi_time( (osi_hour_t)atoi(eventHour), (osi_minute_t)atoi(eventMinute),
                                         (osi_second_t)atoi(eventSecond), (osi_usecond_t)atoi(eventUsecond) );
        osi_date osClearDate = osi_date( (osi_month_t)atoi(clearMonth), (osi_day_t)atoi(clearDay),
                                         (osi_year_t)atoi(clearYear) );
        osi_time osClearTime = osi_time( (osi_hour_t)atoi(clearHour), (osi_minute_t)atoi(clearMinute),
                                         (osi_second_t)atoi(clearSecond), (osi_usecond_t)atoi(clearUsecond) );

        osi_time_period noOfDays = (osEventDate - osClearDate).to_days();

        string daysRemain = noOfDays.to_string();

        int no_days = atoi(daysRemain.c_str());

        return no_days;


    }
    catch (osi_time_toolkit_error err)
    {

        ErrorLogger::logVarError(sourceName, objectName, MAJOR,
                                 "TTime::CompareTime Caught an exception while constructing date and time: error is : %d %s", err.code(),
                                 osi_time_toolkit_error::description(err.code()));
        return 0;
    }
    catch (...)
    {

        ErrorLogger::logError(sourceName, objectName, MAJOR,
                              "TTime::CompareTime Unknown exception while constructing date and time");
        return 0;
    }

}

const int TTime::getSeconds(const char* startTime)
{
    if (strcmp(startTime, "") == 0)
        return 0;
    char eventYear[5];
    char eventMonth[3];
    char eventDay[3];
    char eventHour[3];
    char eventMinute[3];
    char eventSecond[3];

    strncpy(eventYear, startTime, 4);
    strncpy(eventMonth, startTime + 4, 2);
    strncpy(eventDay, startTime + 6, 2);
    strncpy(eventHour, startTime + 8, 2);
    strncpy(eventMinute, startTime + 10, 2);
    strncpy(eventSecond, startTime + 12, 2);

    eventYear[4] = '\0';
    eventMonth[2] = '\0';
    eventDay[2] = '\0';
    eventHour[2] = '\0';
    eventMinute[2] = '\0';
    eventSecond[2] = '\0';

    try
    {

        osi_date osStartDate = osi_date( (osi_month_t)atoi(eventMonth), (osi_day_t)atoi(eventDay),
                                         (osi_year_t)atoi(eventYear) );
        osi_time osStartTime = osi_time( (osi_hour_t)atoi(eventHour), (osi_minute_t)atoi(eventMinute),
                                         (osi_second_t)atoi(eventSecond), (osi_usecond_t)0 );

        osi_time_and_date osStartTimeDate = osi_time_and_date(osStartTime, osStartDate);
        osi_time_and_date now = osi_time_and_date::now();

        osi_time_period noOfSecs = (osStartTimeDate - now).to_seconds();

        string secRemain = noOfSecs.to_string();

        int no_secs = atoi(secRemain.c_str());

        return no_secs;

    }
    catch (osi_time_toolkit_error err)
    {

        ErrorLogger::logVarError(sourceName, objectName, MAJOR, "Caught an exception while constructing date and time: error is : %d %s", err.code(), osi_time_toolkit_error::description(err.code()));
        return 0;
    }
    catch (...)
    {

        ErrorLogger::logError(sourceName, objectName, MAJOR, "Unknown exception while constructing date and time");
        return 0;
    }
}



const int
TTime::getSeconds(const char* eventTime, const char* clearTime)
{
    if (strcmp(clearTime, "") == 0)
        return 0;
    if (strcmp(eventTime, "") == 0)
        return 0;
    char eventYear[5], clearYear[5];
    char eventMonth[3], clearMonth[3];
    char eventDay[3], clearDay[3];
    char eventHour[3], clearHour[3];
    char eventMinute[3], clearMinute[3];
    char eventSecond[3], clearSecond[3];
    char eventUsecond[3], clearUsecond[3];

    // Initializing the micro seconds, so always 00
    memset(eventUsecond, 0, sizeof(eventUsecond));
    memset(clearUsecond, 0, sizeof(clearUsecond));


    strncpy(eventYear, eventTime, 4);
    strncpy(eventMonth, eventTime + 4, 2);
    strncpy(eventDay, eventTime + 6, 2);
    strncpy(eventHour, eventTime + 8, 2);
    strncpy(eventMinute, eventTime + 10, 2);
    strncpy(eventSecond, eventTime + 12, 2);
    // Are there any micro seconds?
    if (strlen(eventTime) > CHECK_FOR_AGENT_TIME_LEN)  // YYYYMMDDHHMMSS.UUZ
    {
        strncpy(eventUsecond, eventTime + 15, 2);
    }

    strncpy(clearYear, clearTime, 4);
    strncpy(clearMonth, clearTime + 4, 2);
    strncpy(clearDay, clearTime + 6, 2);
    strncpy(clearHour, clearTime + 8, 2);
    strncpy(clearMinute, clearTime + 10, 2);
    strncpy(clearSecond, clearTime + 12, 2);
    // Are there any micro seconds?
    if (strlen(clearTime) > CHECK_FOR_AGENT_TIME_LEN)  // YYYYMMDDHHMMSS.UUZ
    {
        strncpy(clearUsecond, clearTime + 15, 2);
    }

    eventYear[4] = '\0';
    eventMonth[2] = '\0';
    eventDay[2] = '\0';
    eventHour[2] = '\0';
    eventMinute[2] = '\0';
    eventSecond[2] = '\0';
    eventUsecond[2] = '\0';

    clearYear[4] = '\0';
    clearMonth[2] = '\0';
    clearDay[2] = '\0';
    clearHour[2] = '\0';
    clearMinute[2] = '\0';
    clearSecond[2] = '\0';
    clearUsecond[2] = '\0';

    try
    {

        osi_date osEventDate = osi_date( (osi_month_t)atoi(eventMonth), (osi_day_t)atoi(eventDay),
                                         (osi_year_t)atoi(eventYear) );
        osi_time osEventTime = osi_time( (osi_hour_t)atoi(eventHour), (osi_minute_t)atoi(eventMinute),
                                         (osi_second_t)atoi(eventSecond), (osi_usecond_t)atoi(eventUsecond) );
        osi_date osClearDate = osi_date( (osi_month_t)atoi(clearMonth), (osi_day_t)atoi(clearDay),
                                         (osi_year_t)atoi(clearYear) );
        osi_time osClearTime = osi_time( (osi_hour_t)atoi(clearHour), (osi_minute_t)atoi(clearMinute),
                                         (osi_second_t)atoi(clearSecond), (osi_usecond_t)atoi(clearUsecond) );

        osi_time_and_date osEventTimeDate = osi_time_and_date(osEventTime, osEventDate);
        osi_time_and_date osClearTimeDate = osi_time_and_date(osClearTime, osClearDate);

        osi_time_period noOfSecs = (osEventTimeDate - osClearTimeDate).to_seconds();

        string secRemain = noOfSecs.to_string();

        int no_secs = atoi(secRemain.c_str());

        return no_secs;


    }
    catch (osi_time_toolkit_error err)
    {

        ErrorLogger::logVarError(sourceName, objectName, MAJOR,
                                 "TTime::CompareTime Caught an exception while constructing date and time: error is : %d %s", err.code(),
                                 osi_time_toolkit_error::description(err.code()));
        return 0;
    }
    catch (...)
    {

        ErrorLogger::logError(sourceName, objectName, MAJOR,
                              "TTime::CompareTime Unknown exception while constructing date and time");
        return 0;
    }

}


// returns the differrence in years of the two time inputs
const int
TTime::GetYears(const char* endTime, const char* startTime)
{
    if (strcmp(startTime, "") == 0)
        return 0;
    if (strcmp(endTime, "") == 0)
        return 0;

    char endYear[5], startYear[5];

    strncpy(endYear, endTime, 4);
    strncpy(startYear, startTime, 4);

    endYear[4] = '\0';
    startYear[4] = '\0';

    int years = atoi(endYear) - atoi(startYear);

    return years;
}


// get the date of the time input formating as mm/dd/yyyy
const string
TTime::GetDate(const char* eventTime)
{
    string date;
    char month[3], day[3], year[5];

    if (strcmp(eventTime, "") == 0)
        return date;

    strncpy(year, eventTime, 4);
    year[4] = '\0';

    strncpy(month, eventTime + 4, 2);
    month[2] = '\0';

    strncpy(day, eventTime + 6, 2);
    day[2] = '\0';

    date = month;
    date = date + "/" + day;
    date = date + "/" + year;

    return date;
}
string TTime::ConvertTL1DateTimeToAgentTimeFormat (const string &date, const string &time)
{
    const char* methodName = "ConvertTL1DateTimeToAgentTimeFormat";
    ErrorLogger::logError(sourceName, objectName, methodName, TRACE4, "Entering...");
    string retStr;
    try
    {
        // calculate month and day
        string month, day;
        if (date != "")
        {
            osi_tokenizer dateTokenizer ("-:", false, "", " \n", "", false);
            vector< string > tokens = dateTokenizer.tokenize( date.c_str() );
            if (tokens.size() != 2)
            {
                string message = "Invalid date format " + date;
                throw BaseException (message.c_str());
            }
            vector< string >::const_iterator i = tokens.begin();
            month = *i++;
            day = *i;
        }

        // calculate hours, minutes and seconds
        string hour, min, sec;
        if (time != "")
        {
            osi_tokenizer timeTokenizer ("-:", false, "", " \n", "", false);
            vector< string > tokens = timeTokenizer.tokenize( time.c_str() );
            if (tokens.size() != 3)
            {
                string message = "Invalid time format " + time;
                throw BaseException (message.c_str());
            }
            vector< string >::const_iterator i = tokens.begin();
            hour = *i++;
            min = *i++;
            sec = *i;
        }
        // calculate year
        osi_date today = osi_time_and_date::now().utc_date();
        int currentMonth = today.month();
        int eventMonth = atoi (month.c_str());
        // the TL1 date does not provide the year so we assume the current year except
        // if the month in the event is later than month today then it must be an year back
        int yearvalue = today.year();
        if (eventMonth > currentMonth)
            yearvalue--;
        char yearBuffer[5];
        _itoa(yearvalue, yearBuffer, 10);
        string year = string(yearBuffer);
        retStr = year.append(month).append(day).append(hour).append(min).append(sec).append(".00Z");

    }
    catch (osi_time_toolkit_error &err)
    {
        ErrorLogger::logVarError(sourceName, objectName, methodName, MAJOR, "Caught an exception while constructing date/time information: error is : %d %s", err.code(), osi_time_toolkit_error::description(err.code()));
        string message = "Caught an exception while constructing date/time information: error is  " + string ( osi_time_toolkit_error::description(err.code()));
        throw BaseException (message.c_str());
    }
    catch (BaseException& ex)
    {
        ErrorLogger::logVarError(sourceName, objectName, methodName, MAJOR, "Caught exception while constucting date/time information; ", ex.getDescription().c_str());
        string message = "Caught an exception while constructing date/time information: " + ex.getDescription();
        throw BaseException (message.c_str());
    }
    catch (...)
    {
        ErrorLogger::logVarError(sourceName, objectName, methodName, MAJOR, "Caught an unknown exception while constructing date/time information");
        string message = "Caught an unknown exception while constructing date/time information";
        throw BaseException (message.c_str());
    }
    return retStr;

}
string TTime::GetUTCDateTime (const string &date, const string &time, const string &timeZoneIndex, const string &DSPSetting, const string &format)
{
    const char* methodName = "GetUTCDateTime";
    ErrorLogger::logError(sourceName, objectName, methodName, TRACE4, "Entering...");
    try
    {
        string utcDateTime;
        if (format == "TL1")
        {
            // calculate month and day
            string month, day;
            if (date != "")
            {
                osi_tokenizer dateTokenizer ("-:", false, "", " \n", "", false);
                vector< string > tokens = dateTokenizer.tokenize( date.c_str() );
                if (tokens.size() != 2)
                {
                    string message = "Invalid date format " + date;
                    throw BaseException (message.c_str());
                }
                vector< string >::const_iterator i = tokens.begin();
                month = *i++;
                day = *i;
            }

            // calculate hours, minutes and seconds
            string hour, min, sec;
            if (time != "")
            {
                osi_tokenizer timeTokenizer ("-:", false, "", " \n", "", false);
                vector< string > tokens = timeTokenizer.tokenize( time.c_str() );
                if (tokens.size() != 3)
                {
                    string message = "Invalid time format " + time;
                    throw BaseException (message.c_str());
                }
                vector< string >::const_iterator i = tokens.begin();
                hour = *i++;
                min = *i++;
                sec = *i;
            }

            // calculate year
            osi_date today = osi_time_and_date::now().utc_date();
            int currentMonth = today.month();
            int eventMonth = atoi (month.c_str());
            // the TL1 date does not provide the year so we assume the current year except
            // if the month in the event is later than month today then it must be an year back
            int yearvalue = today.year();
            if (eventMonth > currentMonth)
                yearvalue--;
            char yearBuffer[5];
            _itoa(yearvalue, yearBuffer, 10);
            string year = string(yearBuffer);

            // convert numbers to Objectspace date and time formats
            osi_date osEventDate = osi_date( (osi_month_t)atoi(month.c_str()), (osi_day_t)atoi(day.c_str()), (osi_year_t)atoi(year.c_str()) );
            osi_time osEventTime = osi_time( (osi_hour_t)atoi(hour.c_str()), (osi_minute_t)atoi(min.c_str()), (osi_second_t)atoi(sec.c_str()), 0 );

            // computer time and date local to the NE based on its timezone if available
            osi_time_and_date localTimeDate;
            string localTimeString = "";

            if ( timeZoneIndex.empty())
            {
                ErrorLogger::logVarError(sourceName, objectName, methodName, TRACE1, "No timezone info available from the NE; adjustment will not be possible");
                localTimeDate = osi_time_and_date(osEventTime, osEventDate);
                localTimeString = localTimeDate.to_string();
            }
            else
            {
                int tzIndex = atoi (timeZoneIndex.c_str()) - 1 ;
                // obtain the name and offset values for this timezone from the array

                // only the range 0-74 is valid for the timzone array
                if ((tzIndex >= 0) && (tzIndex <= 74))
                {
                    string timezoneName = timezones[tzIndex].name;

                    ErrorLogger::logVarError(sourceName, objectName, methodName, TRACE3, "Timezone for NE is %s; time adjustment will be done accordingly", timezoneName.c_str());

                    int hoursOffset = timezones[tzIndex].hourOffsetFromGMT;
                    int minsOffset = timezones[tzIndex].minOffsetFromGMT;

                    // construct timezone based on the above values
                    osi_time_period offsetPeriod(0, hoursOffset, minsOffset, 0);
                    osi_time_zone zone(offsetPeriod, timezoneName, timezoneName);

                    // calculate DSP-based adjustment if DSP is set to on
                    if (DSPSetting == "ON")
                    {
                        if ( (timezones[tzIndex].startDayRank) == TZINFO_NO_DST)
                        {
                            // note that this case should not occur as the NE disallows DSP to be set to ON
                            // for non-supporting timezones...we still do this to be safe
                            ErrorLogger::logVarError(sourceName, objectName, methodName, TRACE4, "DSP is ON for the NE but does not apply for this timezone; DST adjustment will NOT be applied");
                            localTimeDate = osi_time_and_date(osEventTime, osEventDate, zone);
                            localTimeString = localTimeDate.to_string();
                        }
                        else
                        {
                            ErrorLogger::logVarError(sourceName, objectName, methodName, TRACE4, "DSP is ON for NE; DST adjustment will be applied");
                            // determine the beginning and end date/times of the DSP adjustment period

                            osi_time_period startOffset (0, timezones[tzIndex].startHours, timezones[tzIndex].startMinutes, 0, 0);
                            osi_time_period endOffset (0, timezones[tzIndex].endHours, timezones[tzIndex].endMinutes, 0, 0);
                            osi_calendar_date startDateTime (startOffset, timezones[tzIndex].startMonth, (osi_which_one) (timezones[tzIndex].startDayRank), timezones[tzIndex].startWeekDay);
                            osi_calendar_date endDateTime (endOffset, timezones[tzIndex].endMonth, (osi_which_one) (timezones[tzIndex].endDayRank), timezones[tzIndex].endWeekDay);

                            // create the daylight savings offset
                            osi_time_period DSPOffset (0, 1, 0, 0);

                            // construct the DSP rule
                            osi_simple_time_zone_rule DSPRule (startDateTime, endDateTime, 2000, 2099, DSPOffset);

                            // add the rule to the time zone created above
                            zone.insert (DSPRule);

                            // calculate the adjusted time based on the created time zone
                            localTimeDate = osi_time_and_date(osEventTime, osEventDate, zone);
                            localTimeString = localTimeDate.to_string();
                        }
                    }
                    else
                    {
                        ErrorLogger::logVarError(sourceName, objectName, methodName, TRACE4, "DSP is not ON for NE; DST adjustment will NOT be applied");
                        // calculate the adjusted time based on the created time zone
                        localTimeDate = osi_time_and_date(osEventTime, osEventDate, zone);
                        localTimeString = localTimeDate.to_string();
                    }
                }
                else
                {
                    ErrorLogger::logVarError(sourceName, objectName, methodName, TRACE1, "TimeZone index returned from NE is out of range; time adjustment will not be possible ");
                    localTimeDate = osi_time_and_date(osEventTime, osEventDate);
                    localTimeString = localTimeDate.to_string();
                }
            }

            ErrorLogger::logVarError(sourceName, objectName, methodName, TRACE3, "The local alarm date/time is %s", localTimeString.c_str());

            // convert to utc time
            osi_date localDay = localTimeDate.utc_date();
            osi_time localTime = localTimeDate.utc_time();

            // convert to the CMIP-specific agent date/time format
            char str[40];
            GetTimeInAgentFormat(localDay, localTime, str);
            utcDateTime = str;
        }
        else
            utcDateTime = date + " " + time;

        if ( !isValid((const char*) utcDateTime.c_str()) )
        {
            // handle invalid date/time by changing to current EMS time
            char tempStr[40];
            TTime::GetCurrentAgentTime(tempStr);
            ErrorLogger::logVarError(sourceName, objectName, methodName, __LINE__, MAJOR, "Original UTC alarm time [%s] is INVALID, resetting to current EMS time [%s]!", utcDateTime.c_str(), tempStr );
            utcDateTime = tempStr;
        }

        ErrorLogger::logError(sourceName, objectName, methodName, TRACE4, "Leaving...");
        return (utcDateTime);
    }
    catch (osi_time_toolkit_error &err)
    {
        ErrorLogger::logVarError(sourceName, objectName, methodName, MAJOR, "Caught an exception while constructing date/time information: error is : %d %s", err.code(), osi_time_toolkit_error::description(err.code()));
        string message = "Caught an exception while constructing date/time information: error is  " + string ( osi_time_toolkit_error::description(err.code()));
        throw BaseException (message.c_str());
    }
    catch (BaseException& ex)
    {
        ErrorLogger::logVarError(sourceName, objectName, methodName, MAJOR, "Caught exception while constucting date/time information; ", ex.getDescription().c_str());
        string message = "Caught an exception while constructing date/time information: " + ex.getDescription();
        throw BaseException (message.c_str());
    }
    catch (...)
    {
        ErrorLogger::logVarError(sourceName, objectName, methodName, MAJOR, "Caught an unknown exception while constructing date/time information");
        string message = "Caught an unknown exception while constructing date/time information";
        throw BaseException (message.c_str());
    }
}

/************************************************************************************
*
* Name        : getCurrentTimeGMTString
*
* Description : 
*
* Arguments   : 
*
* Returns     : string
*
/************************************************************************************/

string TTime::getCurrentTimeGMTString ()
{
    return getCurrentTimeGMT().to_string ("%Y%m%d%H%M%S");
}

/************************************************************************************
*
* Name        : getCurrentTimeGMT
*
* Description : 
*
* Arguments   : 
*
* Returns     : osi_time_and_date
*
/************************************************************************************/

osi_time_and_date TTime::getCurrentTimeGMT ()
{
    osi_date datenow = osi_date (osi_time_and_date::now().utc_date());
    osi_time timenow = osi_time (osi_time_and_date::now().utc_time());
    osi_time_and_date now (timenow, datenow);

    return now;
}



/************************************************************************************
*
* Name        : FormatTime
*
* Description : Method to convert time from the format of 20010601102034 into the format of 
*    06/01/2001 10:20:34 
*    
* Arguments   : inputTime
*
* Returns     : string
*
/************************************************************************************/
string TTime::FormatTime(string& inputTime)
{
    const char* methodName = "FormatTime";
    ErrorLogger::logVarError(sourceName, objectName, methodName, TRACE6, "The time to be formatted is: %s", inputTime.c_str());

    if ((inputTime.find("/", 0)) != string::npos )
    {
        string outputTime = inputTime;
        ErrorLogger::logVarError(sourceName, objectName, methodName, TRACE6, "The time does not need further formatting, returning the same:%s.", inputTime.c_str());
        return outputTime;
    }

    try
    {

        char Year [5];
        memset(Year, 0, 5);
        char Month [3];
        memset(Month, 0, 3);
        char Day [3];
        memset(Day, 0, 3);
        char Hour [3];
        memset(Hour, 0, 3);
        char Minute [3];
        memset(Minute, 0, 3);
        char Second [3];
        memset(Second, 0, 3);
        char Usecond [3];
        memset(Usecond, 0, 3);

        // Initializing the micro seconds, so always 00
        memset(Usecond, 0, sizeof(Usecond));

        // Convert input time string into multiple variables
        strncpy(Year, inputTime.c_str(), 4);
        strncpy(Month, inputTime.c_str() + 4, 2);
        strncpy(Day, inputTime.c_str() + 6, 2);
        strncpy(Hour, inputTime.c_str() + 8, 2);
        strncpy(Minute, inputTime.c_str() + 10, 2);
        strncpy(Second, inputTime.c_str() + 12, 2);

        osi_time_and_date dateTime;
        string dateStr, timeStr;

        // Set time zone rule for GMT time zone to standard GMT time, not daylight saving time
        // because the input time is always the standard GMT time
        // added by Xiaobing Lu at 08/03/01
        osi_time_zone GMTzone( 0, "GMT", "BST" );
        osi_simple_time_zone_rule rule
        (
            osi_calendar_date( 7200, osi_date::march, osi_last, osi_date::sunday),     // Start.
            osi_calendar_date( 3600, osi_date::october, osi_fourth, osi_date::sunday),     // End.
            1991,     // Start year.
            0,     // End year 0 = undefined.
            osi_time_period( 0, 0, 0, 0) // No adjustment for daylight saving time.
        );
        GMTzone.insert( rule );

        //First get the date/time in GMT
        dateTime = osi_time_and_date (osi_time(atoi(Hour), atoi(Minute), atoi(Second), 0),
                                      osi_date (atoi(Month), atoi(Day), atoi(Year)),
                                      GMTzone);

        //Convert time to local time
        dateTime.time_zone (osi_time_and_date::default_time_zone());

        //for the string in the form "MM/DD/YYYY HH:MM:SS TIMEZone"
        string outputTime = dateTime.to_string("%m/%d/%Y %H:%M:%S %Z");

        ErrorLogger::logVarError(sourceName, objectName, methodName, TRACE6, "The formatted time now is:%s", outputTime.c_str());
        return outputTime;
    }

    catch (...)
    {
        ErrorLogger::logVarError(sourceName, objectName, methodName, MAJOR, "Caught an unknown exception while formatting the date/time information");
        return inputTime;
    }
}
