//
// Created by Pont Simone on 01/09/2022.
//

#ifndef NOTIFICATIONMANAGER_NOTIFICATION_H
#define NOTIFICATIONMANAGER_NOTIFICATION_H

#include <string>
#include <chrono>
#include <jni.h>
#include <iostream>
//#include <iosfwd>

using namespace std;

class Notification {

private:
    //unique number used to identify a single notification
    unsigned char _ID;
    //float value storing the time (expressed in elapsed seconds) when the notification is created (set by the constructor)
    chrono::system_clock::time_point _creation_time;
    //float value indicating the time (expressed in elapsed seconds) when the notification is scheduled
    chrono::system_clock::time_point _notification_time;
    //string containing the title of the notification
    string _title;
    //string containing the text of the notification
    string _text;
    //integer representing the ID of the image used for the notification
    int _image_ID;
    //flag to indicate if the notification is already sent (when 'true') or not (when 'false')
    bool _is_sent=false;

    /// \brief Private method used to calculate the time corresponding to a certain clock value
    /// \param clk is a clock_t variable representing the elapsed clock ticks of the system from the start of the program
    /// \return The method returns a floating point number representing the time corresponding to the elapsed clock ticks (expressed in seconds)
    float _CalculateTimeFromClk(clock_t clk){
        return ((float)clk)/CLOCKS_PER_SEC;
    }

public:
    //constructor
    Notification(char ID, int sched_time=60, string title="", string text="", int img_ID=0);

    //public methods used to set the internal private variables
    void SetID(unsigned char ID);
    void SetNotTime(chrono::system_clock::time_point not_time);
    void SetTitle(string title);
    void SetText(string text);
    void SetImageID(int image_ID);

    //public methods used to get the information stored inside the class
    char GetID(void);
    chrono::system_clock::time_point GetCreationTime(void);
    chrono::system_clock::time_point GetNotTime(void);
    string GetTitle(void);
    string GetText(void);
    int GetImageID(void);

    //method used to check if the current notification must be notified
    bool IsNotificationTime(void);
    //method used to mark the current notification is sent
    void Notified(void);
    //method used to find if the current notification has been already sent
    bool IsSent(void);

};


#endif //NOTIFICATIONMANAGER_NOTIFICATION_H
