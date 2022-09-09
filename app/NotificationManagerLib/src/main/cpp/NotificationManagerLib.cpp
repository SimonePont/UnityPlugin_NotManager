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

#define NOTIF_TO_SCHED 5 //number of notifications to scheduled at every call
#define SEC_BETWEEN_NOTIF 10 //seconds between the scheduling of one notification and the following

using namespace std;

//Global std::vector to store all the scheduled notifications
vector<Notification> NotificationsVec;
//Global variable to store the system clock
chrono::system_clock sys_clk;
//Global variable to store the next ID to be used
char next_ID=1;
//Global arrays containing the title and texts used for the notification
string titles_arr[21]={"8 Ball Pool",
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
                       "Agario",};
string texts_arr[21]={"The World's #1 multiplayer pool game!",
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
                      "Are you predator or prey?",};

int GetNotIndexByID(char ID){
    int index=0;
    bool found=false;

    while(!found && index<NotificationsVec.size()){
        if(NotificationsVec[index].GetID()==ID){
            found=true;
        }
        index++;
    }
    if(!found){
        index=-1;
    }
    return index;
}

//Implemented JNI functions

/// \brief Function used to check all the scheduled notifications and identify if one or more has to be notify
extern "C"
JNIEXPORT jchar JNICALL
Java_com_example_notificationmanagerlib_NotificationManagerClass_CheckAllNotification(JNIEnv *env, jobject jobj)
{
    char tmp_title[50]={0,};
    char tmp_text[200]={0,};
    //char tmp_title[100];
    bool notified=false;
    int n=0;
    string tmp_str;
    jchar ID=0;
    jstring title;
    jstring text;
    chrono::system_clock::time_point enter_time=sys_clk.now();

    //wait three seconds
    //while((float)clock()/CLOCKS_PER_SEC < enter_time+10.1){}

    //cycle all the scheduled notification and check if one has to be notify (only the first one is considered at each method call)
    while(n<NotificationsVec.size() && !notified){
        if(NotificationsVec[n].IsNotificationTime()){
            notified=true;
            NotificationsVec[n].Notified();
            //msg[7]=NotificationsVec[n].GetID();
            //__android_log_write(ANDROID_LOG_INFO,"CCCCC",msg);

            //fill the arguments
            ID=(jchar)NotificationsVec[n].GetID();

            tmp_str=NotificationsVec[n].GetTitle();
            for(int c=0; c<tmp_str.size();c++){
                tmp_title[c]=tmp_str[c];
            }
            title = (*env).NewStringUTF(tmp_title);

            tmp_str=NotificationsVec[n].GetText();
            for(int c=0; c<tmp_str.size();c++){
                tmp_text[c]=tmp_str[c];
            }
            text = (*env).NewStringUTF(tmp_text);

            //Delete the notification from the vector
            //NotificationsVec.erase(NotificationsVec.begin()+n);
            /* invoke the Notify method using the JNI */
            /*
            jclass jcls = env->FindClass("com/example/notificationmanagerlib/NotificationManagerClass");
            jmethodID mid = env->GetMethodID(jcls,"Notify", "(CLjava/lang/String;Ljava/lang/String;)V");
            //jmethodID mid = env->GetMethodID(jcls,"Test", "()V");
            env->CallVoidMethod(jobj,mid,ID,title,text);
            */
        }
        n++;
    }

    return ID;
}

/// \brief Function used to schedule a set of NOTIF_TO_SCHED notifications adding them to the related vector. The NOTIF_TO_SCHED notifications are scheduled one every SEC_BETWEEN_NOTIF.
extern "C"
JNIEXPORT void JNICALL
Java_com_example_notificationmanagerlib_NotificationManagerClass_AddNotifications(JNIEnv *env, jobject jobj)
{
    string title=to_string(next_ID);
    string text="Notification number "+to_string(next_ID);
    int img_ID=1;

    srand (time(NULL));
    img_ID=rand()%15;

    //cycle to add the new notification to the vector (and schedule them)
    for(int n=0; n<NOTIF_TO_SCHED; n++){
        title=titles_arr[img_ID];
        text=texts_arr[img_ID];
        NotificationsVec.push_back(Notification(next_ID,SEC_BETWEEN_NOTIF*(n+1), title,
                                                text,img_ID));
        next_ID++;
        img_ID++;
    }
}

extern "C"
JNIEXPORT int JNICALL
Java_com_example_notificationmanagerlib_NotificationManagerClass_DeleteAllNotifications(JNIEnv *env, jobject jobj)
{
    int num=NotificationsVec.size();
    NotificationsVec.clear();
    next_ID=1; //reset the ID to the first one (not mandatory)
    return num;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_notificationmanagerlib_NotificationManagerClass_DeleteSingleNotification( JNIEnv *env, jobject thiz, jchar id) {
    int index = GetNotIndexByID(id);

    if (index >= 0) {
        NotificationsVec.erase(NotificationsVec.begin() + index);
    }
}

extern "C"
JNIEXPORT jstring JNICALL
Java_com_example_notificationmanagerlib_NotificationManagerClass_GetNotTitleByID(JNIEnv *env, jobject thiz, jchar id) {
    int index=GetNotIndexByID(id);
    string tmp_str;
    jstring title;
    char tmp_title[50]={0,};

    if(index>=0) {
        tmp_str = NotificationsVec[index].GetTitle();
        for (int c = 0; c < tmp_str.size(); c++) {
            tmp_title[c] = tmp_str[c];
        }
    }
    title = (*env).NewStringUTF(tmp_title);
    return title;
}
extern "C"
JNIEXPORT jstring JNICALL
Java_com_example_notificationmanagerlib_NotificationManagerClass_GetNotTextByID(JNIEnv *env, jobject thiz, jchar id) {
    int index=GetNotIndexByID(id);
    string tmp_str;
    jstring text;
    char tmp_text[200]={0,};

    if(index>=0) {
        tmp_str = NotificationsVec[index].GetText();
        for (int c = 0; c < tmp_str.size(); c++) {
            tmp_text[c] = tmp_str[c];
        }
    }
    text = (*env).NewStringUTF(tmp_text);
    return text;
}
extern "C"
JNIEXPORT jint JNICALL
Java_com_example_notificationmanagerlib_NotificationManagerClass_GetNotImgIDByID(JNIEnv *env, jobject thiz, jchar id) {
    int index= GetNotIndexByID(id);
    int img_id=0;
    if(index>=0){
        img_id=NotificationsVec[index].GetImageID();
    }
    return img_id;
}
extern "C"
JNIEXPORT jboolean JNICALL
Java_com_example_notificationmanagerlib_NotificationManagerClass_SwapNotify(JNIEnv *env, jobject thiz, jchar id_1, jchar id_2) {
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