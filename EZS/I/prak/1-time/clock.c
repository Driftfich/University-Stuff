#include <stdio.h>
#include <threads.h>
#include <time.h>

int month_to_days(int month, int year) {
    if (month == 2 && (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0))) {
        return 29;
    } else if (month == 2) {
        return 28;
    } else if (month == 4 || month == 6 || month == 9 || month == 11) {
        return 30;
    } else {
        return 31;
    }
}

void to_total_months_and_days(long seconds, long* months_res, long* days_res) {
    long days = seconds / 86400;
    // printf("days: %ld\n", days);
    long total_months = 0;
    long total_days = 0;
    int month = 1;
    int year = 1970;
    int days_in_month = month_to_days(month, year);
    while (days >= days_in_month) {
        total_months++;
        month++;
        days -= days_in_month;
        days_in_month = month_to_days(month, year);
        total_days += days_in_month;
        if (month == 13) {
            month = 1;
            year++;
        }
    }
    *months_res = total_months;
    *days_res = total_days;
}

int to_month(long total_months) {
    return total_months % 12;
}

int to_year(long total_months) {
    return total_months / 12  + 1970;
}

int to_day(long seconds, long total_days) {
    return seconds / 86400 - total_days;
}

int to_hour(long seconds) {
    return seconds / 3600;
}

int main() {
    time_t now;

    now = (long) time(NULL);

    printf("Seconds since epoch (1970): %ld\n", now);

    long months_res, days_res;
    to_total_months_and_days(now, &months_res, &days_res);
    int month = to_month(months_res) + 1;
    long year = to_year(months_res);
    int day = to_day(now, days_res) + 1;
    int hours = to_hour(now - days_res * 86400);
    int hour = hours % 24 + 1;
    int minute = (now - days_res * 86400 - hours * 3600) / 60;
    int second = (now - days_res * 86400 - hours * 3600 - minute * 60) % 60;

    printf("Time: %ld/%d/%d %d:%02d:%02d\n", year, month, day, hour, minute, second);

    return 0;
}
