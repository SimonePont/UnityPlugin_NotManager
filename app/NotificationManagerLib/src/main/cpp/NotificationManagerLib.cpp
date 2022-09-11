// Write C++ code here.
//
// Do not forget to dynamically load the C++ library into your application.
//
// For instance,
//
// In MainActivity.java:
//    static {
//       System.loadLibrary("NotificationManagerLib");
//    }
//
// Or, in MainActivity.kt:
//    companion object {
//      init {
//         System.loadLibrary("NotificationManagerLib")
//      }
//    }


#include "Notification.h"
#include <vector>
#include <android/log.h>
#include <iostream>
#include <fstream>

#define MAX_ELEMENTS 20 //max number of titles and texts used for the notifications
#define MAX_TITLE_LEN 50 //max length of a possible title
#define MAX_TEXT_LEN 200 //max length of a possible text

using namespace std;

//Global std::vector to store all the scheduled notifications
vector<Notification> NotificationsVec;
//Global flag to indicate if there are some scheduled notifications to check (when 'true') or not (when 'false').
bool notifications_to_check=false;
//Global variable to store the system clock
chrono::system_clock sys_clk;
//Global variable to store the next ID to be used
char next_ID=1;
//Global arrays containing the title and texts used for the notification
string titles_arr[MAX_ELEMENTS]={"8 Ball Pool",
                       "Carrom Pool",
                       "Subway Surfers",
                       "Mini Basketball",
                       "Mini Football",
                       "Football Strike",
                       "Head Ball 2",
                       "Online Soccer Manager",
                       "Soccer Stars",
                       "Basketball Stars",
                       "Basketball Arena",
                       "Golf Battle",
                       "Ultimate Golf",
                       "Sniper Strike",
                       "Pure Sniper",
                       "Mini Militia",
                       "Motorsport Manager Online",
                       "Darts of Fury",
                       "Cricket League",
                       "Agario"};
string texts_arr[MAX_ELEMENTS]={"The World's #1 multiplayer pool game!",
                      "Carrom Pool is an easy-to-play multiplayer board game. Are you up for the challenge?",
                      "Have you got what it takes to help Jake and his friends escape the grumpy guard in one of the most exciting endless runners of all time?",
                      "Put on your sneakers, and dominate the court! Experience Basketball like never before!",
                      "Wild football fun at your fingertips!",
                      "Make a name for yourself in the world's best free-kick face-off football game!",
                      "Experience the thrill of using your head to score and become the champion!",
                      "Start your football manager career by signing the contract at your favorite football club.",
                      "Easy to pick up and fun to play. Will you take the cup home?",
                      "Dribble, shoot, score, WIN, in the world's best multiplayer basketball mobile game!",
                      "Dominate the Arena! Climb your way to the top!",
                      "Beat your opponents in this innovative 6-player real time golf game",
                      "Don't waste any time waiting for your opponent to take their shot, tee-off in this fantastic real-time multiplayer golf game!",
                      "Intercept, eliminate and extract. It's time to engage the enemy",
                      "Step into the ultimate 3D FPS action adventure! Do you have what it takes?",
                      "Battle with up to 6 players online in this fun cartoon themed 2D game.",
                      "Motorsport Manager Online takes the acclaimed race strategy game to mobile for the first time!",
                      "Play amazing darts arenas and take on real players from all around the world!",
                      "Bat, bowl and field your way to the top of the league. Compete in this real-time multiplayer cricket game!",
                      "Are you predator or prey?"};

/// \brief Function used to obtain the index of the std::vector containing the Notification object with a certain identifier (ID)
/// \param ID is the univocal identifier of the requested notification.
/// \return The function returns an integer representing the index of the NotificationsVec element that contains the wanted Notification object.
int GetNotIndexByID(char ID){
    int index=0;
    bool found=false;

    while(!found && index<NotificationsVec.size()){
        if(NotificationsVec[index].GetID()==ID){
            found=true;
        }
        else{
            index++;
        }
    }
    if(!found){
        index=-1;
    }
    return index;
}

//Implemented JNI functions

/// \brief Implementation of the Java function used to check all the scheduled notifications and identify if one has to be notified.
/// \param env is a pointer to a structure storing all JNI function pointers (automatically defined at compiling and linking time)
/// \param jobj is the object of the class where this function is used (automatically defined at compiling and linking time).
/// \return The function returns a jchar (char) value representing the identifier (ID) of the first notification that has to be sent.
extern "C"
JNIEXPORT jchar JNICALL
Java_com_example_notificationmanagerlib_NotificationManagerClass_CheckAllNotification(JNIEnv *env, jobject jobj)
{
    bool notify=false;
    bool last_notify=true;
    int n=0;
    string tmp_str;
    jchar ID=0;

    //the check must be done only when necessary (there are still notifications to be sent)
    if(notifications_to_check) {
        //cycle all the scheduled notification and check if one has to be notify (only the first one is considered at each method call)
        while (n < NotificationsVec.size() && !notify) {
            if (NotificationsVec[n].IsNotificationTime()) {
                notify = true;
                NotificationsVec[n].Notified();
                ID = NotificationsVec[n].GetID();
                //invoke the Notify method using the JNI
                /* !!!!NOT WORKING IN UNITY PROJECT!!!!
                jclass jcls = env->FindClass("com/example/notificationmanagerlib/NotificationManagerClass");
                jmethodID mid = env->GetMethodID(jcls,"Notify", "(CLjava/lang/String;Ljava/lang/String;)V");
                //jmethodID mid = env->GetMethodID(jcls,"Test", "()V");
                env->CallVoidMethod(jobj,mid,ID,title,text);
                */
            }
            n++;
        }
        //when a notification has to be sent a cycle is started to check if it is the last one
        if(notify){
            for(n=0; n<NotificationsVec.size(); n++){
                if(!NotificationsVec[n].IsSent()){
                    last_notify=false;
                }
            }
            //if all the notifications has been sent the check must be temporary stopped (until new notification are added)
            if(last_notify){
                notifications_to_check=false;
                //invoke the SetCheckFlg method using the JNI (to disable the check until new notifications are scheduled)
                jclass jcls = env->FindClass("com/example/notificationmanagerlib/NotificationManagerClass");
                jmethodID mid = env->GetMethodID(jcls,"SetCheckFlg", "(Z)V");
                env->CallVoidMethod(jobj,mid,false);
            }
        }
    }

    return ID;
}

/// \brief Implementation of the Java function used to schedule a set of 'not_number' notifications adding them to the related vector. The 'not_number' notifications are scheduled one every 'sec_between_not'.
/// \param env is a pointer to a structure storing all JNI function pointers (automatically defined at compiling and linking time)
/// \param jobj is the object of the class where this function is used (automatically defined at compiling and linking time).
/// \param not_number is an integer value representing the number of notifications that has to be scheduled.
/// \param sec_between_not is an integer value representing the time (expressed in seconds) that must pass between a notification and the next one.
extern "C"
JNIEXPORT void JNICALL
Java_com_example_notificationmanagerlib_NotificationManagerClass_AddNotifications(JNIEnv *env, jobject jobj, jint not_number, jint sec_between_not)
{
    string title=to_string(next_ID);
    string text="Notification number "+to_string(next_ID);
    int img_ID=1;

    srand (time(NULL));
    img_ID=rand()%(MAX_ELEMENTS-not_number);

    //cycle to add the new notification to the vector (and schedule them)
    for(int n=0; n<not_number; n++){
        title=titles_arr[img_ID];
        text=texts_arr[img_ID];
        NotificationsVec.push_back(Notification(next_ID,sec_between_not*(n+1), title,text,img_ID));
        next_ID++;
        img_ID++;
    }
    //set the global flag to enable the check for the notification time
    notifications_to_check=true;
}

/// \brief Implementation of the Java function used to delete all the scheduled notifications.
/// \param env is a pointer to a structure storing all JNI function pointers (automatically defined at compiling and linking time)
/// \param jobj is the object of the class where this function is used (automatically defined at compiling and linking time).
extern "C"
JNIEXPORT int JNICALL
Java_com_example_notificationmanagerlib_NotificationManagerClass_DeleteAllNotifications(JNIEnv *env, jobject jobj)
{
    int num=NotificationsVec.size();
    NotificationsVec.clear();
    next_ID=1; //reset the ID to the first one (not mandatory)
    return num;
}

/// \brief Implementation of the Java function used to delete a single scheduled notification.
/// \param env is a pointer to a structure storing all JNI function pointers (automatically defined at compiling and linking time)
/// \param jobj is the object of the class where this function is used (automatically defined at compiling and linking time).
/// \param id is the univocal identifier (ID) of the notification that has to be deleted.
extern "C"
JNIEXPORT void JNICALL
Java_com_example_notificationmanagerlib_NotificationManagerClass_DeleteSingleNotification( JNIEnv *env, jobject jobj, jchar id) {
    int index = GetNotIndexByID(id);

    if (index >= 0) {
        NotificationsVec.erase(NotificationsVec.begin() + index);
    }
}

/// \brief Implementation of the Java function used to get the title of a single scheduled notification.
/// \param env is a pointer to a structure storing all JNI function pointers (automatically defined at compiling and linking time)
/// \param jobj is the object of the class where this function is used (automatically defined at compiling and linking time).
/// \param id is the univocal identifier (ID) of the notification that has to be considered.
/// \return The function returns a jstring (string) containing the title of the wanted notification.
extern "C"
JNIEXPORT jstring JNICALL
Java_com_example_notificationmanagerlib_NotificationManagerClass_GetNotTitleByID(JNIEnv *env, jobject jobj, jchar id) {
    int index=GetNotIndexByID(id);
    string tmp_str;
    jstring title;
    char tmp_title[MAX_TITLE_LEN]={0,};

    if(index>=0) {
        tmp_str = NotificationsVec[index].GetTitle();
        for (int c = 0; c < tmp_str.size(); c++) {
            tmp_title[c] = tmp_str[c];
        }
    }
    title = (*env).NewStringUTF(tmp_title);
    return title;
}

/// \brief Implementation of the Java function used to get the text of a single scheduled notification.
/// \param env is a pointer to a structure storing all JNI function pointers (automatically defined at compiling and linking time)
/// \param jobj is the object of the class where this function is used (automatically defined at compiling and linking time).
/// \param id is the univocal identifier (ID) of the notification that has to be considered.
/// \return The function returns a jstring (string) containing the text of the wanted notification.
extern "C"
JNIEXPORT jstring JNICALL
Java_com_example_notificationmanagerlib_NotificationManagerClass_GetNotTextByID(JNIEnv *env, jobject jobj, jchar id) {
    int index=GetNotIndexByID(id);
    string tmp_str;
    jstring text;
    char tmp_text[MAX_TEXT_LEN]={0,};

    if(index>=0) {
        tmp_str = NotificationsVec[index].GetText();
        for (int c = 0; c < tmp_str.size(); c++) {
            tmp_text[c] = tmp_str[c];
        }
    }
    text = (*env).NewStringUTF(tmp_text);
    return text;
}

/// \brief Implementation of the Java function used to get the identifier of the image related to a single scheduled notification.
/// \param env is a pointer to a structure storing all JNI function pointers (automatically defined at compiling and linking time)
/// \param jobj is the object of the class where this function is used (automatically defined at compiling and linking time).
/// \param id is the univocal identifier (ID) of the notification that has to be considered.
/// \return The function returns a jint (int) containing the identifier of the image related to the wanted notification.
extern "C"
JNIEXPORT jint JNICALL
Java_com_example_notificationmanagerlib_NotificationManagerClass_GetNotImgIDByID(JNIEnv *env, jobject jobj, jchar id) {
    int index= GetNotIndexByID(id);
    int img_id=0;
    if(index>=0){
        img_id=NotificationsVec[index].GetImageID();
    }
    return img_id;
}

/// \brief Implementation of the Java function used to swap two scheduled notification (their notification times are swapped).
/// \param env is a pointer to a structure storing all JNI function pointers (automatically defined at compiling and linking time)
/// \param jobj is the object of the class where this function is used (automatically defined at compiling and linking time).
/// \param id_1 is the univocal identifier (ID) of the first notification that has to be considered.
/// \param id_2 is the univocal identifier (ID) of the second notification that has to be considered.
/// \return The function returns 'true' if the two notifications exist and are correctly swapped, it returns 'false' otherwise.
extern "C"
JNIEXPORT jboolean JNICALL
Java_com_example_notificationmanagerlib_NotificationManagerClass_SwapNotify(JNIEnv *env, jobject jobj, jchar id_1, jchar id_2) {
    bool res=false;
    int index1= GetNotIndexByID(id_1);
    int index2= GetNotIndexByID(id_2);
    chrono::system_clock::time_point tmp_time;

    if(index1>=0 && index2>=0){
        tmp_time=NotificationsVec[index1].GetNotTime();
        NotificationsVec[index1].SetNotTime(NotificationsVec[index2].GetNotTime());
        NotificationsVec[index2].SetNotTime(tmp_time);
        res= true;
    }
    return res;
}