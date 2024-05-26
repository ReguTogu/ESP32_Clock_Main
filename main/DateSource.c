#include "Date.h"
#include <stdbool.h> // Include the <stdbool.h> header for the bool type

void CDate_init(CDate* date, int day, int month, int year) {
    date->day = day;
    date->month = month;
    date->year = year;
}

bool CDate_NamNhuan(const CDate* date) {
    if ((date->year % 100 == 0 && date->year % 400 == 0) || date->year % 4 == 0) {
        return true;
    } else {
        return false;
    }
}

bool CDate_Thang30(const CDate* date) {
    return (date->month == 4 || date->month == 6 || date->month == 9 || date->month == 11);
}

bool CDate_Thang31(const CDate* date) {
    return (date->month == 1 || date->month == 3 || date->month == 5 || date->month == 7 ||
            date->month == 8 || date->month == 10 || date->month == 12);
}

bool CDate_Thang2(const CDate* date) {
    return (date->month == 2);
}

int CDate_DaysOfLastMonths(const CDate* date) {
    int s = 0;
    for (int nMonth = date->month - 1; nMonth >= 0; nMonth--) {
        if (CDate_NamNhuan(date)) {
            if (nMonth > 2) {
                if (date->month == 4 || date->month == 6 || date->month == 9 || date->month == 11) {
                    s += 30;
                } else {
                    s += 31;
                }
            } else { // Thang 1,2
                s += 31 + 29;
            }
        } else {
            if (date->month == 4 || date->month == 6 || date->month == 9 || date->month == 11) {
                s += 30;
            } else if (nMonth == 2) {
                s += 28;
            } else {
                s += 31;
            }
        }
    }
    return s;
}

int CDate_TotalTime(const CDate* date) {
    int nYear = date->year - 1;
    int DayMonth1 = nYear / 4 - nYear / 100 + nYear / 400;
    int DayMonth2 = nYear * 365 - DayMonth1;
    int DayNow = DayMonth1 + DayMonth2 + CDate_DaysOfLastMonths(date) + date->day;
    return DayNow;
}

void CDate_Increment(CDate* date) {
    if (CDate_Thang31(date)) {
        if (1 <= date->day && date->day < 31) {
            date->day++;
        } else {
            if (date->month == 12) {
                date->day = 1;
                date->month = 1;
                date->year++;
            } else {
                date->day = 1;
                date->month++;
            }
        }
    } else if (CDate_Thang30(date)) {
        if (1 <= date->day && date->day < 30) {
            date->day++;
        } else {
            date->day = 1;
            date->month++;
        }
    } else {
        if (CDate_NamNhuan(date)) {
            if (1 <= date->day && date->day < 29) {
                date->day++;
            } else {
                date->day = 1;
                date->month++;
            }
        } else {
            if (1 <= date->day && date->day < 28) {
                date->day++;
            } else {
                date->day = 1;
                date->month++;
            }
        }
    }
}

