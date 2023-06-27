// Copyright 2023 The Chromium Authors
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

package org.chromium.ui;

import android.view.MotionEvent;

import androidx.annotation.Nullable;

import org.chromium.base.StrictModeContext;
import org.chromium.base.TraceEvent;

import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;

/**
 * Class with helper methods for MotionEvent.
 */
public class MotionEventUtils {
    /**
     * Returns the time in nanoseconds of the given MotionEvent.
     *
     * This method exists as a utility pre API 34 (Android U) there was no public method to get
     * nanoseconds. So we call the hidden SDK method "getEventTimeNano" via reflection. If the
     * reflection fails, the time in milliseconds extended to nanoseconds will be returned.
     *
     * TODO(b/286064744): Add in Android U support once the SDK is publicly in the chrome builders.
     */
    public static long getEventTimeNanos(MotionEvent event) {
        long timeNs = 0;
        // We are calling a method that was set as maxSDK=P so need to ignore strictmode violations.
        try (StrictModeContext ignored = StrictModeContext.allowAllVmPolicies()) {
            if (sGetTimeNanoMethod == null) {
                Class<?> cls = Class.forName("android.view.MotionEvent");
                sGetTimeNanoMethod = cls.getMethod("getEventTimeNano");
            }
            timeNs = (long) sGetTimeNanoMethod.invoke(event);
        } catch (IllegalAccessException | NoSuchMethodException | ClassNotFoundException
                | InvocationTargetException e) {
            TraceEvent.instant("MotionEventUtils::getEventTimeNano error", e.toString());
            timeNs = event.getEventTime() * 1_000_000;
        }
        return timeNs;
    }

    /**
     * Returns the time in nanoseconds, but with precision to milliseconds, of the given
     * MotionEvent. There is no SDK method which returns the event time in nanoseconds, pre Android
     * API 34 (Android U) so we just extend milliseconds to nanoseconds in that case.
     *
     * TODO(b/286064744): Add in Android U support once the SDK is publicly in the chrome builders.
     */
    public static long getHistoricalEventTimeNanos(MotionEvent event, int pos) {
        return event.getHistoricalEventTime(pos) * 1_000_000;
    }

    private MotionEventUtils() {}

    @Nullable
    private static Method sGetTimeNanoMethod;
}
