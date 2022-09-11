package com.example.notificationmanagerlib;

import androidx.annotation.RequiresApi;
import androidx.appcompat.app.AlertDialog;
import androidx.core.app.ActivityCompat;
import androidx.core.app.NotificationCompat;
import androidx.core.app.NotificationManagerCompat;
import androidx.core.content.ContextCompat;
import android.content.ComponentName;

import android.Manifest;
import android.app.Activity;
import android.app.Application;
import android.app.Notification;
import android.app.NotificationChannel;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.SharedPreferences;
import android.content.pm.PackageManager;
import android.content.res.Resources;
import android.os.Build;
import android.widget.Toast;
import static android.Manifest.permission.ACTIVITY_RECOGNITION;
import static androidx.core.app.ActivityCompat.requestPermissions;

/// \brief Class used to interface the unity environment with this Android library
public final class NotificationManagerClass extends Application {

    private String CHANNEL_ID="chN"; //identifier of the notification channel
    private static Activity _unity_activity; //variable used to store the unity Activity that uses this library
    private static Context _unity_context; //variable used to store the unity Context that uses this library
    private static Context _plugin_context; //variable used to store the Context of the current library as unity plugin
    private static Intent _service_intent; //variable used to store the intent of the generated service
    private boolean _check_needed=false; //flag to indicate if some scheduled notifications are present and their check must be enabled (when 'true') or not (when 'false').

    //Native methods implemented in the C++ file "NotificationManagerLib.cpp"
    private native char CheckAllNotification();
    private native void AddNotifications(int not_number, int sec_between_not);
    private native int DeleteAllNotifications();
    private native void DeleteSingleNotification(char id);
    private native String GetNotTitleByID(char id);
    private native String GetNotTextByID(char id);
    private native int GetNotImgIDByID(char id);
    private native boolean SwapNotify(char id_1, char id_2);

    //Load the .cpp file where the native methods are implemented
    static {
        System.loadLibrary("NotificationManagerLib");
    }

    /// \brief Method called at the creation of a new object
    @Override
    public void onCreate() {
        super.onCreate();
        NotificationManagerClass._plugin_context=getApplicationContext();

    }

    /// \brief Encapsulation of the native private method used to schedule a set of 'not_number' new notifications, one every 'sec_between_not' seconds.
    /// \param not_number is an integer representing the number of notifications that has to be scheduled.
    /// \param sec_between_not is an integer representing the number of seconds that must elapse between two consecutive notifications of the set.
    public void AddNot(int not_number, int sec_between_not){
        AddNotifications(not_number,sec_between_not);
        _check_needed=true; //if new notifications are scheduled the check must be enabled
        Toast.makeText(_unity_activity, "Notifications scheduled", Toast.LENGTH_SHORT).show();
    }

    /// \brief Encapsulation of the native private method used to check if the scheduled time of a notification is elapsed and it must be sent.
    /// \return The method returns the univocal identifier (ID) of the notification that has to be sent.
    public char CheckNot(){
        char ret=0;
        if(_check_needed){ //the check is done only when necessary
            ret=CheckAllNotification();
        }
        return ret;
    }

    /// \brief Encapsulation of the native private method used to delete all the scheduled notifications.
    public void DelAllNot(){
        int res=0;
        res=DeleteAllNotifications();
        _check_needed=false; //if all the notifications are deleted the check is no more needed
        Toast.makeText(_unity_activity, "All notifications deleted! "+res+" in total.", Toast.LENGTH_SHORT).show();
    }

    /// \brief Encapsulation of the native private method used to delete a single notification.
    /// \param ID is the univocal identifier (ID) of the notification that has to be deleted.
    public void DelSingleNot(char ID){
        DeleteSingleNotification(ID);
        Toast.makeText(_unity_activity, "Notifications "+ID+" deleted!", Toast.LENGTH_SHORT).show();
    }

    /// \brief Encapsulation of the native private method used to get the title of a single notification.
    /// \param ID is the univocal identifier (ID) of the requested notification.
    /// \return The method returns a string containing the title of the requested notification.
    public String GetNotTitle(char ID){
        return GetNotTitleByID(ID);
    }

    /// \brief Encapsulation of the native private method used to get the text of a single notification.
    /// \param ID is the univocal identifier (ID) of the requested notification.
    /// \return The method returns a string containing the text of the requested notification.
    public String GetNotText(char ID){
        return GetNotTextByID(ID);
    }

    /// \brief Encapsulation of the native private method used to get the image identifier of a single notification.
    /// \param ID is the univocal identifier (ID) of the requested notification.
    /// \return The method returns an integer representing identifier of the image used for the requested notification.
    public int GetNotImgID(char ID){
        return GetNotImgIDByID(ID);
    }

    /// \brief Method used to manually set the internal flag that enables/disables the check of the scheduled notifications.
    /// \param val is a boolean indicating the value that has to be set for the flag: 'true' means check enabled, 'false' means check disabled.
    public void SetCheckFlg(boolean val){
        _check_needed=val;
    }

    /// \brief Method used to get the internal flag that enables/disables the check of the scheduled notifications.
    /// \return The method returns a boolean indicating the value of the internal flag: 'true' means check enabled, 'false' means check disabled.
    public boolean GetCheckFlg(){
        return _check_needed;
    }

    /// \brief Method used to receive the activity from Unity
    /// \param unity_act is the unity Activity this library has to work on.
    public static void ReceiveUnityActivity(Activity unity_act){
        String[] perms= new String[1];
        perms[0]=Manifest.permission.ACTIVITY_RECOGNITION;
        _unity_activity=unity_act;
        if (ContextCompat.checkSelfPermission(_unity_activity, Manifest.permission.ACTIVITY_RECOGNITION)
                != PackageManager.PERMISSION_GRANTED) {
            ActivityCompat.requestPermissions(NotificationManagerClass._unity_activity, perms,1);
        }

    }
    /// \brief Method used to receive the context from Unity
    /// \param unity_act is the unity Context this library has to work on.
    public static void ReceiveUnityContext(Context unity_cont){
        _unity_context=unity_cont;
    }

    /// \brief Method used ot create a channel used to send the notifications (needed from API level 26).
    /// \return The function returns 'true' if the channel is successfully created, it returns 'false' otherwise.
    @RequiresApi(api = Build.VERSION_CODES.O)
    public boolean createNotificationChannel() {
        boolean ret=false;
        // Create the NotificationChannel, but only on API 26+ because
        // the NotificationChannel class is new and not in the support library
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O && _unity_activity!=null) {
            String description = "channel for notifications";
            int importance = NotificationManager.IMPORTANCE_DEFAULT;
            NotificationChannel channel = new NotificationChannel(CHANNEL_ID,"channel_name", importance);
            channel.setDescription(description);
            // Register the channel with the system; you can't change the importance
            // or other notification behaviors after this
            NotificationManager notificationManager = _unity_activity.getSystemService(NotificationManager.class);
            notificationManager.createNotificationChannel(channel);
        }

        Toast.makeText(_unity_activity, "Notification channel created", Toast.LENGTH_SHORT).show();
        return ret;
    }

    /// \brief Method used to send a notification through the channel previously created.
    /// \param not_ID is the univocal identifier (ID) of the notification that has to be sent.
    /// \param title is a String containing the title of the notification that has to be sent.
    /// \param text is a String containing the text of the notification that has to be sent.
    /// !!!!NOT WORKING IN UNITY PROJECT!!!!
    public void Notify(char not_ID, String title, String text){
        Resources res = _unity_activity.getResources();
        int icon = res.getIdentifier("notification_icon", "drawable", "com.example.notificationmanagerlib");

        // Create an explicit intent for an Activity in your app
        Intent intent = new Intent(this, NotificationManagerClass.class);
        intent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK | Intent.FLAG_ACTIVITY_CLEAR_TASK);
        PendingIntent pendingIntent = PendingIntent.getActivity(_unity_activity, 0, intent, PendingIntent.FLAG_IMMUTABLE);

        NotificationCompat.Builder builder = new NotificationCompat.Builder(_unity_activity.getApplicationContext())
                //.setSmallIcon(R.drawable.notification_icon)
                .setSmallIcon(icon)
                .setContentTitle(title)
                .setContentText(text)
                // Set the intent that will fire when the user taps the notification
                .setContentIntent(pendingIntent)
                .setPriority(NotificationCompat.PRIORITY_DEFAULT)
                .setAutoCancel(true);

        final NotificationManagerCompat notificationManager = NotificationManagerCompat.from(this);
        if(notificationManager != null){
            Toast.makeText(_unity_activity, "Notification prepared", Toast.LENGTH_SHORT).show();
        }
        else{
            Toast.makeText(_unity_activity, "Notification NOT prepared", Toast.LENGTH_SHORT).show();
        }
        notificationManager.notify(not_ID,builder.build());

        Toast.makeText(_unity_activity, "Notification sent", Toast.LENGTH_SHORT).show();
    }


    /// \brief Method used to start a background "NotificationService" that checks if a new notification has to be sent.
    /// \return The method returns 'true' if the service is correctly started, it return 'false' otherwise.
    /// !!!!NOT WORKING IN UNITY PROJECT!!!!
    public static boolean StartNotService() {
        boolean ret=false;
        _service_intent=new Intent(_unity_activity, NotificationService.class);
        if (_unity_activity != null) {
            _unity_activity.startService(_service_intent);
            Toast.makeText(_unity_activity, "Service created", Toast.LENGTH_SHORT).show();
            ret=true;
        }
        return ret;
    }

    /// \brief Method used to stop the background "NotificationService" previously created.
    /// !!!!NOT WORKING IN UNITY PROJECT!!!!
    public static void StopNotService(){
        _unity_activity.stopService(_service_intent);
    }
}
