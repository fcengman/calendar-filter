/*
 * process_cal3.c
 *
 * Starter file provided to students for Assignment #3, SENG 265,
 * Fall 2021.
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include "emalloc.h"
#include "ics.h"
#include "listy.h"

/*
 * struct event_bounds:
 *
 * Based on commands given on the command line. Stores the beginning 
 * and end bounds of what should be filters form the calendar. 
 * 
 * Also stores the from and to dates as a string for comparision with
 * the dates given in the ics file. 
 *
 */

typedef struct
{
    int from_yy;
    int from_mm;
    int from_dd;
    int to_yy;
    int to_mm;
    int to_dd;
    char from_date[DT_LEN];
    char to_date[DT_LEN];
} Event_bounds;

// Program control functions.
node_t *read_file(char *filename, node_t *event_list);
event_t *parse_line(char *label, char *description, event_t *current_event);
node_t *generate_reoccuring_events(node_t *event_list);
void evaluate_events(node_t *event_list, Event_bounds *bounds);
void print_event(node_t *event, bool visited, const int count);

// Helper functions.
Event_bounds *create_bounds(int fy, int fm, int fd, int ty, int tm, int td);
event_t *copy_event(event_t *old_event);
void concat_date(int year, int month, int day, char *date);
void dt_format(char *formatted_time, const char *dt_time, const int len);
void time_format(char *formatted_time, const char *time, const int len);
void dt_increment(char *after, const char *before, int const num_days);
void generate_dashedline(char *formatted_date);
void free_list(node_t *list);

int main(int argc, char *argv[])
{
    int from_y = 0, from_m = 0, from_d = 0;
    int to_y = 0, to_m = 0, to_d = 0;
    char *filename = NULL;
    int i;

    for (i = 0; i < argc; i++)
    {
        if (strncmp(argv[i], "--start=", 8) == 0)
        {
            sscanf(argv[i], "--start=%d/%d/%d", &from_y, &from_m, &from_d);
        }
        else if (strncmp(argv[i], "--end=", 6) == 0)
        {
            sscanf(argv[i], "--end=%d/%d/%d", &to_y, &to_m, &to_d);
        }
        else if (strncmp(argv[i], "--file=", 7) == 0)
        {
            filename = argv[i] + 7;
        }
    }

    if (from_y == 0 || to_y == 0 || filename == NULL)
    {
        fprintf(stderr,
                "usage: %s --start=yyyy/mm/dd --end=yyyy/mm/dd --file=icsfile\n",
                argv[0]);
        exit(1);
    }

    /* Starting calling your own code from this point. */
    //Create structs that will be used to store unique dates, events, and date bounds.
    Event_bounds *bounds = create_bounds(from_y, from_m, from_d, to_y, to_m, to_d);
    node_t *event_list = NULL;

    event_list = read_file(filename, event_list);
    event_list = generate_reoccuring_events(event_list);
    evaluate_events(event_list, bounds);

    free_list(event_list);
    free(bounds);
    exit(0);
}

/*
 * Function parse_line:
 *
 * Reads an ical file and populates a events array struct.
 * 
 * Returns a node_t pointer.
 * 
 */
node_t *read_file(char *filename, node_t *event_list)
{
    FILE *cal = fopen(filename, "r");
    event_t *current_event = (event_t *)emalloc(sizeof(event_t));
    //Create char arrays to store contents of the calendar to be evaluated.
    char *line = (char *)emalloc(sizeof(char) * SUMMARY_LEN);
    char *label = (char *)emalloc(sizeof(char) * SUMMARY_LEN);
    char *description = (char *)emalloc(sizeof(char) * SUMMARY_LEN);

    while (fgets(line, SUMMARY_LEN, cal))
    {
        //Reset label and description.
        sscanf(line, "%[A-Z]:%[^\n]", label, description);
        //When an END:VEVENT tag is reached, add the event to the events array and clear the current event struct.
        if (strcmp(label, "END") == 0 && strcmp(description, "VEVENT") == 0)
        {
            event_t *new_event = copy_event(current_event);
            event_list = add_inorder(event_list, new_node(new_event));
            strcpy(current_event->dtstart, "");
            strcpy(current_event->start_time, "");
            strcpy(current_event->dtend, "");
            strcpy(current_event->end_time, "");
            strcpy(current_event->frequency, "");
            strcpy(current_event->repeat_until, "");
            strcpy(current_event->summary, "");
            strcpy(current_event->location, "");
        }
        else
        {
            current_event = parse_line(label, description, current_event);
        }
        strcpy(label, "");
        strcpy(description, "");
    }
    fclose(cal);
    free(line);
    free(label);
    free(description);
    free(current_event);

    return event_list;
}

/*
 * Function parse_line:
 *
 * Takes a line from the ics file and stores the description in a 
 * event object based on the label.
 * 
 * Returns an event_t pointer.
 * 
 */
event_t *parse_line(char *label, char *description, event_t *current_event)
{
    //Store the start date and time in an event struct. If event has unique date, add that date to the dates array.
    if (strcmp(label, "DTSTART") == 0)
    {
        //Store the start date and time in an event struct.
        sscanf(description, "%[0-9]T%[0-9]", current_event->dtstart, current_event->start_time);
    }
    //Store the end date and time in an event struct.
    else if (strcmp(label, "DTEND") == 0)
    {
        sscanf(description, "%[0-9]T%[0-9]", current_event->dtend, current_event->end_time);
    }
    //Store the location in an event struct.
    else if (strcmp(label, "LOCATION") == 0)
    {
        strcpy(current_event->location, description);
    }
    //Store summary in an event struct.
    else if (strcmp(label, "SUMMARY") == 0)
    {
        strcpy(current_event->summary, description);
    }
    //Store the frequency and repeat until date in an event struct.
    else if (strcmp(label, "RRULE") == 0)
    {
        char *temp = (char *)emalloc(sizeof(char) * SUMMARY_LEN);
        sscanf(description, "FREQ=%[A-Z];%[A-Z;=]%[0-9];", current_event->frequency, temp, current_event->repeat_until);
        free(temp);
    }

    return current_event;
}
/*
 * Function generate_reoccuring_events:
 *
 * Evaluate each event. If it is reoccuring, duplicate event, increment days,
 * and add the new event to the events array. 
 * 
 * * Returns a node_t pointer.
 *
 */
node_t *generate_reoccuring_events(node_t *event_list)
{
    char *new_date = (char *)emalloc(sizeof(char) * SUMMARY_LEN);
    node_t *repeat_events = NULL;

    //Find events that are reoccuring in event list, increment date and add to repeat_events list.
    node_t *temp_n = event_list;
    while (temp_n != NULL)
    {
        //Evaluates events with reoccuring events only.
        if (strcmp(temp_n->val->frequency, "WEEKLY") == 0)
        {

            strcpy(new_date, temp_n->val->dtstart);
            dt_increment(new_date, new_date, 7);
            //Checks if new date is still within the bounds given by the command line.
            while (strcmp(new_date, temp_n->val->repeat_until) < 0)
            {
                //Adds incremented date to the events array.
                event_t *new_event = copy_event(temp_n->val);
                strcpy(new_event->dtstart, new_date);
                repeat_events = add_inorder(repeat_events, new_node(new_event));
                dt_increment(new_date, new_date, 7);
            }
        }
        temp_n = temp_n->next;
    }

    //Add repeat_events list to main event_list.
    temp_n = repeat_events;
    while (temp_n != NULL)
    {
        event_t *new_event = copy_event(temp_n->val);
        event_list = add_inorder(event_list, new_node(new_event));
        temp_n = temp_n->next;
    }

    // Free temporary repeat_events list.
    free_list(repeat_events);
    free(new_date);

    return event_list;
}

/*
 * Function evaluate_events:
 *
 * Evaluates an event and decides how it should be printed to the terminal. 
 *
 */
void evaluate_events(node_t *event_list, Event_bounds *bounds)
{
    int count = 0;        //Keeps track of the number of days printed to the terminal.
    bool visited = false; //Keeps track of if a unique day has been printed already.
    node_t *temp_n = event_list;

    //Iterate through list.
    while (temp_n != NULL)
    {
        //Check if current event is within bounds.
        if ((strcmp(temp_n->val->dtstart, bounds->from_date) >= 0) && (strcmp(temp_n->val->dtstart, bounds->to_date) <= 0))
        {
            print_event(temp_n, visited, count);
            count++; //Increment count to print new lines appropriately.

            if (!visited)
                visited = true; //Set visited to true when multiple events occur on the same day.

            if (temp_n->next != NULL && (strcmp(temp_n->val->dtstart, temp_n->next->val->dtstart) != 0))
            {
                visited = false; //Reset visited when next event is on a different day.
            }
        }
        temp_n = temp_n->next;
    }
}

/*
 * Function print_event:
 *
 * Prints the contents of an event struct to the terminal.
 * If the dates of the current event has been visited already it will skip 
 * printing the formatted date and dashed line. 
 * 
 *
 */
void print_event(node_t *event, bool visited, const int count)
{
    char *formatted_date = (char *)emalloc(sizeof(char) * SUMMARY_LEN);
    char *formatted_start_time = (char *)emalloc(sizeof(char) * SUMMARY_LEN);
    char *formatted_end_time = (char *)emalloc(sizeof(char) * SUMMARY_LEN);

    // Print current event date when it is the first event evaluated on that day.
    if (visited == false)
    {
        if (count > 0)
            printf("\n");
        dt_format(formatted_date, event->val->dtstart, SUMMARY_LEN);
        printf("%s\n", formatted_date);
        generate_dashedline(formatted_date);
    }

    time_format(formatted_start_time, event->val->start_time, SUMMARY_LEN);
    time_format(formatted_end_time, event->val->end_time, SUMMARY_LEN);
    printf("%8s to %8s: %s {{%s}}\n", formatted_start_time, formatted_end_time, event->val->summary, event->val->location);

    free(formatted_date);
    free(formatted_start_time);
    free(formatted_end_time);
}

/*
 * Function create_bounds:
 *
 * Creates a bounds struct based on bay, month, and year integer data.
 * 
 * Returns a event_bounds pointer. 
 * 
 */
Event_bounds *create_bounds(int fy, int fm, int fd, int ty, int tm, int td)
{
    Event_bounds *bounds = (Event_bounds *)emalloc(sizeof(Event_bounds));
    bounds->from_yy = fy;
    bounds->from_mm = fm;
    bounds->from_dd = fd;
    bounds->to_yy = ty;
    bounds->to_mm = tm;
    bounds->to_dd = td;
    concat_date(fy, fm, fd, bounds->from_date);
    concat_date(ty, tm, td, bounds->to_date);
    return bounds;
}

/*
 * Function copy_event:
 *
 * Copy the contents of an event into a new event and return it. 
 * 
 * Returns an event_t pointer.
 *
 */
event_t *copy_event(event_t *old_event)
{
    event_t *e = (event_t *)emalloc(sizeof(event_t));
    strcpy(e->dtstart, old_event->dtstart);
    strcpy(e->dtend, old_event->dtend);
    strcpy(e->start_time, old_event->start_time);
    strcpy(e->end_time, old_event->end_time);
    strcpy(e->frequency, old_event->frequency);
    strcpy(e->summary, old_event->summary);
    strcpy(e->location, old_event->location);
    strcpy(e->repeat_until, old_event->repeat_until);
    return e;
}
/*
 * Function concat_date:
 *
 * Given a year, month, and day. Concatonate into a string that represents 
 * the date with leading zeros. 
 * 
 */
void concat_date(int year, int month, int day, char *date)
{
    char y[DT_LEN];
    char m[DT_LEN];
    char d[DT_LEN];
    char temp_date[DT_LEN] = "";

    sprintf(y, "%d", year);
    strcat(temp_date, y);
    if (month > 9)
    {
        sprintf(m, "%d", month);
        strcat(temp_date, m);
    }
    else
    {
        strcat(temp_date, "0");
        sprintf(m, "%d", month);
        strcat(temp_date, m);
    }
    if (day > 9)
    {
        sprintf(d, "%d", day);
        strcat(temp_date, d);
    }
    else
    {
        strcat(temp_date, "0");
        sprintf(d, "%d", day);
        strcat(temp_date, d);
    }
    strcpy(date, temp_date);
}

/*
 * Function dt_format.
 *
 * Given a date-time, creates a more readable version of the
 * calendar date by using some C-library routines. For example,
 * if the string in "dt_time" corresponds to:
 *
 *   20190520T111500
 *
 * then the string stored at "formatted_time" is:
 *
 *   May 20, 2019 (Mon).
 *
 */
void dt_format(char *formatted_time, const char *dt_time, const int len)
{
    struct tm temp_time;
    time_t full_time;

    memset(&temp_time, 0, sizeof(struct tm));
    sscanf(dt_time, "%4d%2d%2d",
           &temp_time.tm_year, &temp_time.tm_mon, &temp_time.tm_mday);
    temp_time.tm_year -= 1900;
    temp_time.tm_mon -= 1;
    full_time = mktime(&temp_time);
    strftime(formatted_time, len, "%B %d, %Y (%a)",
             localtime(&full_time));
}

/*
 * Function time_format:
 *
 * Given a string in military time. 
 * Convert to a string with AM/PM
 *
 */
void time_format(char *formatted_time, const char *time, const int len)
{
    struct tm temp_time;
    time_t full_time;

    memset(&temp_time, 0, sizeof(struct tm));
    sscanf(time, "%2d%2d%2d",
           &temp_time.tm_hour, &temp_time.tm_min, &temp_time.tm_sec);
    temp_time.tm_year -= 1900;
    temp_time.tm_mon -= 1;
    full_time = mktime(&temp_time);
    strftime(formatted_time, len, "%-2I:%M %p",
             localtime(&full_time));
}

/*
 * Function dt_increment:
 *
 * Given a date-time, it adds the number of days in a way that
 * results in the correct year, month, and day. For example,
 * if the string in "before" corresponds to:
 *
 *   20190520T111500
 *
 * then the datetime string stored in "after", assuming that
 * "num_days" is 100, will be:
 *
 *   20190828T111500
 *
 * which is 100 days after May 20, 2019 (i.e., August 28, 2019).
 *
 */
void dt_increment(char *after, const char *before, int const num_days)
{
    struct tm temp_time;
    time_t full_time;

    memset(&temp_time, 0, sizeof(struct tm));
    sscanf(before, "%4d%2d%2dT%2d%2d%2d", &temp_time.tm_year,
           &temp_time.tm_mon, &temp_time.tm_mday, &temp_time.tm_hour,
           &temp_time.tm_min, &temp_time.tm_sec);
    temp_time.tm_year -= 1900;
    temp_time.tm_mon -= 1;
    temp_time.tm_mday += num_days;

    full_time = mktime(&temp_time);
    after[0] = '\0';
    strftime(after, 16, "%Y%m%d", localtime(&full_time));
    strncpy(after + 16, before + 16, SUMMARY_LEN - 16);
    after[SUMMARY_LEN - 1] = '\0';
}

/*
 * Function generate_dashedline:
 *
 * Prints a dashed line equal to the length of a given string.
 *
 */
void generate_dashedline(char *formatted_date)
{
    char line[SUMMARY_LEN] = "";
    for (int i = 0; formatted_date[i] != '\0'; i++)
    {
        strcat(line, "-");
    }
    printf("%s\n", line);
}

/*
 * Function free_list
 *
 * Deallocated memory associated with a node_t list that contains event_t structs.
 *
 */
void free_list(node_t *list)
{
    for (node_t *temp_n = NULL; list != NULL; list = temp_n)
    {
        temp_n = list->next;
        free(list->val);
        free(list);
    }
}