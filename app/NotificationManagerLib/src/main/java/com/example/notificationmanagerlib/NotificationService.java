package com.example.notificationmanagerlib;

import android.app.Notification;
import android.app.NotificationChannel;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.app.Service;
import android.content.Context;
import android.content.Intent;
import android.content.res.Resources;
import android.os.Build;
import android.os.IBinder;
import android.widget.Toast;

import androidx.annotation.Nullable;
import androidx.core.app.NotificationCompat;
import androidx.core.app.NotificationManagerCompat;


public class NotificationService extends Service {
    @Nullable
    @Override
    public IBinder onBind(Intent intent) {
        return null;
    }

    private void createNotificationChannel() {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
            final NotificationChannel notificationChannel = new NotificationChannel(
                    "PedometerLib",
                    "Service Channel",
                    NotificationManager.IMPORTANCE_DEFAULT
            );
            final NotificationManagerCompat notificationManager = NotificationManagerCompat.from(this);
            notificationManager.createNotificationChannel(notificationChannel);
        }
        Toast.makeText(this, "Channel created", Toast.LENGTH_SHORT).show();
    }

    private void startNotification(){
        String input = "Counting your steps...";
        Resources res = this.getResources();
        int icon = res.getIdentifier("notification_icon", "drawable", "com.example.notificationmanagerlib");
        Intent notificationIntent = new Intent(this,com.example.notificationmanagerlib.NotificationManagerClass.class);
        PendingIntent pendingIntent = PendingIntent.getActivity(this,
                0, notificationIntent, 0);
        Toast.makeText(this, "Notification on creation", Toast.LENGTH_SHORT).show();
        Notification notification = new NotificationCompat.Builder(this, "PedometerLib")
                .setContentTitle("Background Walking Service")
                .setContentText(input)
                .setSmallIcon(icon)
                .setContentIntent(pendingIntent)
                .build();
        startForeground(112, notification);
        Toast.makeText(this, "Notification on service created", Toast.LENGTH_SHORT).show();
        /*
        final NotificationManagerCompat notificationManager = NotificationManagerCompat.from(this);
        if(notificationManager != null){
            Toast.makeText(this, "Notification prepared", Toast.LENGTH_SHORT).show();
        }
        else{
            Toast.makeText(this, "Notification NOT prepared", Toast.LENGTH_SHORT).show();
        }
        notificationManager.notify(3,notification);
    */
    }


    @Override
    public void onCreate() {

    }

    @Override
    public void onTaskRemoved(Intent rootIntent) {

    }

    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {
        createNotificationChannel();
        startNotification();
        super.onCreate();
        return START_NOT_STICKY;
    }
}
