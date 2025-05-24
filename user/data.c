#include "kernel/types.h"
#include "user/user.h"

struct rtcdate {
  int year;
  int month;
  int day;
  int hour;
  int minute;
  int second;
  int ms;
};

int is_leap_year(int year) {
  return (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0));
}

int days_in_month(int month, int year) {
  static const int days_per_month[] = { 31,28,31,30,31,30,31,31,30,31,30,31 };
  if (month == 2 && is_leap_year(year))
    return 29;
  return days_per_month[month - 1];
}

void timestamp_to_date(uint64 timestamp, struct rtcdate* r) {

  uint64 seconds = timestamp / 1000000000ULL;

  r->ms = (timestamp % 1000000000ULL) / 1000000ULL;

  r->second = seconds % 60;
  seconds /= 60;

  r->minute = seconds % 60;
  seconds /= 60;

  r->hour = seconds % 24;
  uint64 days = seconds / 24;

  int year = 1970;
  while (1) {
    int days_in_year = is_leap_year(year) ? 366 : 365;
    if (days >= days_in_year) {
      days -= days_in_year;
      year++;
    }
    else {
      break;
    }
  }
  r->year = year;

  int month = 1;
  while (1) {
    int dim = days_in_month(month, year);
    if (days >= dim) {
      days -= dim;
      month++;
    }
    else {
      break;
    }
  }
  r->month = month;
  r->day = days + 1;
}

int main(int argc, char* argv[]) {

  uint64 time = 0;
  rtc((char*)(&time));
  
  struct rtcdate r;
  timestamp_to_date(time, &r);

  if (r.day < 10)
    printf("0%u.", r.day);
  else
    printf("%u.", r.day);
  if (r.month < 10)
    printf("0%u.", r.month);
  else
    printf("%u.", r.month);
  printf("%u ", r.year);
  printf("%u:%u:%u.%u\n", r.hour, r.minute, r.second, r.ms);
}
