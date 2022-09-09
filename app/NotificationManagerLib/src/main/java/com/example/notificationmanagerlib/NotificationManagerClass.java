package com.example.notificationmanagerlib;

import androidx.annotation.RequiresPermission;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.NotificationCompat;
import androidx.core.app.NotificationManagerCompat;

import android.Manifest;
import android.app.Activity;
import android.app.Notification;
import android.app.NotificationChannel;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.res.Resources;
import android.os.Build;
import android.os.Bundle;
import android.widget.Toast;


public class NotificationManagerClass {

    private String CHANNEL_ID="chN";
    private static Activity _unity_activity;
    private static Context _unity_context;

    private native char CheckAllNotification();
    private native void AddNotifications();
    private native int DeleteAllNotifications();
    private native void DeleteSingleNotification(char id);
    private native String GetNotTitleByID(char id);
    private native String GetNotTextByID(char id);
    private native int GetNotImgIDByID(char id);
    private native boolean SwapNotify(char id_1, char id_2);

    static {
        System.loadLibrary("NotificationManagerLib");
    }

    public void AddNot(){
        AddNotifications();
        Toast.makeText(_unity_activity, "Notifications scheduled", Toast.LENGTH_SHORT).show();
    }

    public char CheckNot(){
        char ret=0;
        ret=CheckAllNotification();
        return ret;
    }

    public void DelAllNot(){
        int res=0;
        res=DeleteAllNotifications();
        Toast.makeText(_unity_activity, "All notifications deleted! "+res+" in total.", Toast.LENGTH_SHORT).show();
    }

    public void DelSingleNot(char ID){
        DeleteSingleNotification(ID);
        Toast.makeText(_unity_activity, "Notifications "+ID+" deleted!", Toast.LENGTH_SHORT).show();
    }

    public String GetNotTitle(char ID){
        return GetNotTitleByID(ID);
    }

    public String GetNotText(char ID){
        return GetNotTextByID(ID);
    }

    public int GetNotImgID(char ID){
        return GetNotImgIDByID(ID);
    }

    public static void ReceiveUnityActivity(Activity unity_act){
        _unity_activity=unity_act;
    }

    public static void ReceiveUnityContext(Context unity_cont){
        _unity_context=unity_cont;
    }

    public int createNotificationChannel() {
        // Create the NotificationChannel, but only on API 26+ because
        // the NotificationChannel class is new and not in the support library
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
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
        return 1;
    }

    public void Notify(char not_ID, String title, String text){
        Resources res = _unity_context.getResources();
        int icon = res.getIdentifier("notification_icon", "drawable", "com.example.notificationmanagerlib");
        Toast.makeText(_unity_activity, "Notification time "+(int)not_ID, Toast.LENGTH_SHORT).show();
        // Create an explicit intent for an Activity in your app
        /*
        Intent intent = new Intent(this, NotificationManagerClass.class);
        intent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK | Intent.FLAG_ACTIVITY_CLEAR_TASK);
        PendingIntent pendingIntent = PendingIntent.getActivity(_unity_activity, 0, intent, PendingIntent.FLAG_IMMUTABLE);

        NotificationCompat.Builder builder = new NotificationCompat.Builder(_unity_activity.getApplicationContext())
                //.setSmallIcon(R.drawable.notification_icon)
                .setSmallIcon(icon)
                .setContentTitle(title)
                .setContentText(text)
                // Set the intent that will fire when the user taps the notification
                //.setContentIntent(pendingIntent)
                .setPriority(NotificationCompat.PRIORITY_DEFAULT)
                .setAutoCancel(true);

         */
        Notification notification = new Notification(icon, text, System.currentTimeMillis());
        //NotificationManagerCompat notificationManager = NotificationManagerCompat.from(_unity_activity);
        NotificationManager notificationManager =(NotificationManager)_unity_context.getSystemService(Context.NOTIFICATION_SERVICE);
        if(notificationManager != null){
            Toast.makeText(_unity_activity, "Notification prepared", Toast.LENGTH_SHORT).show();
        }
        else{
            Toast.makeText(_unity_activity, "Notification NOT prepared", Toast.LENGTH_SHORT).show();
        }
        //notificationManager.notify(not_ID,notification);

        Toast.makeText(_unity_activity, "Notification sent", Toast.LENGTH_SHORT).show();
    }

}
