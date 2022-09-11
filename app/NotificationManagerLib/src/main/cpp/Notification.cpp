//
// Created by Pont Simone on 01/09/2022.
//

#include "Notification.h"
#include <jni.h>

/// \brief Constructor used to set all the internal variables
/// \param ID is a char representing the unique ID of the notification. It MUST be different from the other notification objects!!!
/// \param sched_time is a float value representing the time (expressed in seconds) that must pass between the creation of the object and its notification (by default it is set to 60 seconds).
/// \param title is a string representing the title of the notification (by default it is an empty string).
/// \param text is a string representing the text of the notification (by default it is an empty string).
/// \param img_ID is an integer representing the identifier of the image associated to the notification (by default it is set to 0).
Notification::Notification(char ID, int sched_time, string title, string text, int img_ID) {
    chrono::seconds sec_between_not(sched_time);
    _ID=ID;
    _creation_time=chrono::system_clock::now();
    _notification_time=_creation_time+sec_between_not;
    _title=title;
    _text=text;
    _image_ID=img_ID;
    _is_sent=false;
}

/// \brief Method used to set the ID of the current notification object.
/// \param ID is a char representing the unique ID of the current notification.
void Notification::SetID(unsigned char ID) {
    _ID=ID;
}

/// \brief Method used to set the time when the notification must be launched.
/// \param not_time is a floating point value indicating the time (expressed in seconds) when the notification has to be sent.
void Notification::SetNotTime(chrono::system_clock::time_point not_time) {
    _notification_time=not_time;
}

/// \brief Method used to set the title of the current notification object.
/// \param title is a string containing the title of the notification.
void Notification::SetTitle(string title) {
    _title=title;
}

/// \brief Method used to set the text of the current notification object.
/// \param text is a string containing the text of the notification.
void Notification::SetText(string text) {
    _text=text;
}

/// \brief Method used to set the image ID of the current notification object.
/// \param image_ID is an integer value representing the identifier of the image used for the notification.
void Notification::SetImageID(int image_ID) {
    _image_ID=image_ID;
}

/// \brief Method used to get the ID of the current notification object.
/// \return The method returns the ID of the notification.
char Notification::GetID() {
    return _ID;
}

/// \brief Method used to get the creation time of the current notification object.
/// \return The method returns the time (expressed in seconds) when the notification has been created.
chrono::system_clock::time_point Notification::GetCreationTime() {
    return _creation_time;
}

/// \brief Method used to get the notification time of the current notification object.
/// \return The method returns the time (expressed in seconds) when the notification is scheduled.
chrono::system_clock::time_point Notification::GetNotTime() {
    return _notification_time;
}

/// \brief Method used to get the title of the current notification object.
/// \return The method returns the title of the notification.
string Notification::GetTitle() {
    return _title;
}

/// \brief Method used to get the text of the current notification object.
/// \return The method returns the text of the notification.
string Notification::GetText() {
    return _text;
}

/// \brief Method used to get the image ID of the current notification object.
/// \return The method returns the identifier of then image related to the notification.
int Notification::GetImageID() {
    return _image_ID;
}

/// \brief Method used to check if the time when the notification is scheduled is elapsed or not.
/// return The method returns 'true' if the time is elapsed (and the notification has to be sent), it returns 'false' otherwise.
bool Notification::IsNotificationTime() {
    bool res= false;
    //float current_time=_CalculateTimeFromClk(clock());
    chrono::system_clock::time_point current_time=chrono::system_clock::now();

    //if the current time is higher or equal to the scheduled one the notification must be sent (if not already sent)
    if(current_time>=_notification_time && _is_sent==false){
        res=true;
    }
    return res;
}

/// \brief Method used to indicate that the notification is sent and change the internal flag storing this information.
void Notification::Notified() {
    _is_sent=true;
}

/// \brief Method used to know if the notification has been already sent or not.
/// \brief The method returns 'true' if the notifications has been already sent, it return 'false' if it is still waiting.
bool Notification::IsSent() {
    return _is_sent;
}