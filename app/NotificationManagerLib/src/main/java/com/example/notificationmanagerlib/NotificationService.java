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

    /// \brief Private method used to create a notification channel used by the service.
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
    }

    /// \brief Private method used to check if a certain notification has to be sent. It is the main functionality of this service.
    private void startNotificationCheck(){
        /*
            Not implemented because it does not work with the created unity activity.
            It should substitute the "CheckAllNotification()" method of the "NotificationManagerClass" class working in background and sending notifications when needed.
         */
    }

    /// \brief Method called when the service is started, it creates the notification channel and launch its background work to send the notifications when they are scheduled.
    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {
        createNotificationChannel();
        startNotificationCheck();
        super.onCreate();
        return START_NOT_STICKY;
    }
}
