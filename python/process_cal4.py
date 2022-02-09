#!/usr/bin/env python3
'''
    This is a module for reading, processing and storing the information contained within an ics file. 

    Classes:
        Event: This is a class for defining an event stored in an ics file.

            Functions:
                __init__: Initializes an Event objected based on a dictionary containing information about the event. 
                __repr__: Constructs a string that represents an event.
                get_start_date: Get the date of given Event with start time.
                get_date: Get the date of the given Event without time. 
        
        Day: This is the class for defining a given day with one or more events.

            Functions:
                __init__: Initializes a Day object based on an Event object and puts the event into a list.
                __repr__: Constructs a multi-line string that represents the day. 
                add_event: Adds an Event to the event list in the day object.
                get_date: Get the date of the given Day. 
                get_events: Get the events list for the given Day.

        process_cal: This is a class for reading, processing and storing the information contained within an ics file.

            Functions:
                __init__: Initializes a process_cal object based on an ics file.
                add_event: Adds an event to the days array by either creating a new day or appending the event to an existing day. 
                format_date: Returns a datetime object based on a string representation of a date.
                read_file: Attempts to open and read a filename stored in process_cal. If successful, parses the file.
                parse_file: Parses an ics file and populates days and events in the process_cal object. 
                generate_reoccurring_events: Generates the reoccuring events of a given event based on the frequency and adds them to the day list.
                sort_by_time: Sorts the events information in each day based on the start time. 
                get_events_for_day: Determine if there is an entry for the day given and return that day.


    '''
from datetime import datetime
import datetime as dt
import sys
import re


class Event:
    '''
    This is a class for defining an event stored in an ics file.

    Attributes:
        __start_date (datetime): A datetime object of the start date and time of the event.
        __date (datetime): A datetime object of the start date with the hours and minutes initialized to zero.
        __end_date (datetime): A datetime object of the end date and time of the event.
        __location (string): Stores the location of the event.
        __summary (string): Stores the summary of the event.  
    '''

    def __init__(self, event):
        '''
        Initializes an Event objected based on a dictionary containing information about the event. 

        Args:
            event (Dict): Contains information about an event.

        Attributes: 
            __start_date (datetime): Stores the start date and time of the event.
            __date (datetime): Stores the start date without time.
            __end_date (datetime): Stores the end date and time of the event.
            __location (string): Stores location string.
            __summary (string): Stores summary string. 
        '''
        self.__start_date = event['start_dt']
        self.__date = event['date']
        self.__end_date = event['end_dt']
        self.__location = event['location']
        self.__summary = event['summary']

    def __repr__(self):
        '''
        Constructs a string that represents an event. 

        Returns:
            string: Representation including time, summary and location.
        '''
        return '%s to %s: %s {{%s}}' % (self.__start_date.strftime("%-2I:%M %p"),
                                        self.__end_date.strftime(
            "%-2I:%M %p"),
            self.__summary,
            self.__location)

    def get_start_date(self):
        '''
        Get the date of the given Event with time. 

        Returns:
            datetime: Datetime object associated with the event.
        '''
        return self.__start_date

    def get_date(self):
        '''
        Get the date of the given Event without time. 

        Returns:
            datetime: Datetime object associated with the event.
        '''
        return self.__date


class Day:
    '''
    This is the class for defining a given day with one or more events.

    Attributes:
        __date (datetime): A datetime object that stores the date of the day without a time associated. 
        __events (list): Stores a list of events (time, location, and summary) on the given day.
    '''

    def __init__(self, event):
        ''' 
        Initializes a Day object based on an Event object and puts the event into a list.

        Args:
            event (Event): An event object to be stored in the events list.

        Attributes:
            start (datetime): Stores the date information with no time initialized
            events (List): Stores a list of events, appending the first event given as an argument.

        '''
        self.__date = event.get_date()
        self.__events = []
        self.__events.append(event)

    def __repr__(self):
        '''
        Constructs a multi-line string that represents the day. 

        Returns:
            string: String representation of a day object with date of the day and the time, summary and location of the events on that day.
        '''
        date = self.__date.strftime("%B %2d, %Y (%a)")
        dashes = '-'*len(self.__date.strftime("%B %2d, %Y (%a)"))
        output = f'{date}\n{dashes}\n'
        # Add a line for each event that takes play on the given day.
        for i in range(len(self.__events)):
            output = output + self.__events[i].__repr__()
            if i is not (len(self.__events)-1):
                output = output + '\n'

        return output

    def add_event(self, event):
        '''
        Adds an Event to the event list in the day object. 

        Args:
            event (dict/Event): Dictionary or Event to be added to the events list.
        '''
        if isinstance(event, Event):
            self.__events.append(event)
        else:
            self.__events.append(Event(event))

    def get_date(self):
        '''
        Get the date of the given Day. 

        Returns:
            datetime: Datetime object associated with the day.
        '''
        return self.__date

    def get_events(self):
        '''
        Get the events list for the given Day.

        Returns:
            list: List of events (time, location, and summary) of a given Day.
        '''
        return self.__events


class process_cal:
    '''
    This is a class for reading, processing and storing the information contained within an ics file.

    Attributes:
        filename (string): Name o the ics file to be processed. 
    '''

    def __init__(self, filename):
        '''
        Initializes a process_cal object based on an ics file.

        Args:
            filename (string): Name of the ics file to be processed. 

        Attributes:
            __filename (string): Name of the ics file to be processed. 
            __days (List): Stores the events over many days.

        '''
        self.__filename = filename
        self.__days = []
        self.read_file()
        self.sort_by_time()

    def add_event(self, event):
        '''
        Adds an event to the days array by either creating a new day or appending the event to an existing day. 

        Args:
            event (Event): Event to be added to the days list. 
        '''
        for day in self.__days:
            if day.get_date() == event.get_date():
                day.add_event(event)
                return

        self.__days.append(Day(event))

    def format_date(self, date, hrs=0, mins=0):
        ''' 
        Returns a datetime object based on a string representation of a date. 

        Args:
            date (string): string representation of a date wither with or without the time.
            hrs (int, optional): Set the hour of the date if necessary. Defaults to 0.
            mins (int, optional): Set the minute of the date if necessary. Defaults to 0.

        Returns:
            datetime: datetime object set with given date from string and optional paramenters
        '''
        if len(date) > 8:
            return datetime.strptime(date, '%Y%m%d%H%M%S')
        else:
            return datetime.strptime(date, '%Y%m%d') + dt.timedelta(hours=hrs, minutes=mins)

    def read_file(self):
        '''
        Attempts to open and read a filename stored in process_cal. If successful, parses the file.
        '''
        if not re.search(r'^.*\.ics$', self.__filename):
            print(f'{self.__filename} is not an ical file.')
            sys.exit(1)
        try:
            file = open(self.__filename, "r")
        except:
            print(f'{self.__filename} is not readable.')
            sys.exit(1)

        self.parse_file(file)
        file.close()

    def parse_file(self, file):
        '''
            Parses an ics file and populates days and events in the process_cal object. 
        '''
        event = {}
        number_pattern = re.compile(r'\d+')
        string_pattern = re.compile(r'^.*:(.+)')
        start_pattern = re.compile(r'DTSTART:')
        end_pattern = re.compile(r'DTEND:')
        rrule_pattern = re.compile(r'RRULE:')
        location_pattern = re.compile(r'LOCATION:')
        summary_pattern = re.compile(r'SUMMARY:')
        done_pattern = re.compile(r'END:VEVENT')

        # Add key value pair to the event dictionary depending on the contents of the line.
        for line in file:
            if re.match(start_pattern, line):
                event['start_dt'] = self.format_date(
                    ''.join(map(str, re.findall(number_pattern, line))))
                event['date'] = dt.datetime(
                    event['start_dt'].year, event['start_dt'].month, event['start_dt'].day)

            elif re.match(end_pattern, line):
                event['end_dt'] = self.format_date(
                    ''.join(map(str, re.findall(number_pattern, line))))

            elif re.match(rrule_pattern, line):
                event['frequency'] = "WEEKLY"
                event['repeat_until'] = self.format_date(
                    ''.join(map(str, re.findall(number_pattern, line))))

            elif re.match(location_pattern, line):
                m = string_pattern.search(line)
                if m:
                    event['location'] = m.group(1)

            elif re.match(summary_pattern, line):
                m = string_pattern.search(line)
                if m:
                    event['summary'] = m.group(1)

            elif re.match(done_pattern, line):
                self.add_event(Event(event))
                self.generate_reoccurring_events(event)
                event.clear()
                event['location'] = ""
        file.close()

    def generate_reoccurring_events(self, event):
        '''
        Generates the reoccuring events of a given event based on the frequency and adds them to the day list. 

        Args:
            event (Event): Event to be repeated. 
        '''
        if 'frequency' in event.keys():
            new_date = event['start_dt'] + dt.timedelta(days=7)
            # Add repeating events to the events list until the date crosses the end bounds.
            while event['repeat_until'] >= new_date:
                new_event = {**event, 'date': dt.datetime(
                    new_date.year, new_date.month, new_date.day), 'start_dt': new_date}
                self.add_event(Event(new_event))
                new_date += dt.timedelta(days=7)

    def sort_by_time(self):
        '''
        Sorts the events information in each day based on the start time. 
        '''
        for day in self.__days:
            day.get_events().sort(key=lambda x: x.get_start_date())

    def get_events_for_day(self, curr_dt):
        '''
        Determine if there is an entry for the day given and return that day. 

        Args:
            curr_dt (datetime): datetime object.

        Returns:
            Day: Day object with events that take place on that day.
        '''
        for day in self.__days:
            if day.get_date() == curr_dt:
                return day
        return None
