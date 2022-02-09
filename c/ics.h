#ifndef _ICS_H_
#define _ICS_H_

#define DT_LEN 17
#define SUMMARY_LEN 80
#define LOCATION_LEN 80
#define RRULE_LEN 80
#define FREQ_LEN 80
#define REPEAT_UNTIL_LEN 80

typedef struct
{
    char dtstart[DT_LEN];
    char start_time[DT_LEN];
    char dtend[DT_LEN];
    char end_time[DT_LEN];
    char summary[SUMMARY_LEN];
    char location[LOCATION_LEN];
    char repeat_until[REPEAT_UNTIL_LEN];
    char frequency[FREQ_LEN];
} event_t;

#endif
