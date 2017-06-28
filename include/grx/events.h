/*
 * events.h
 *
 * Copyright (c) 2016 David Lechner <david@lechnology.com>
 * This file is part of the GRX graphics library.
 *
 * The GRX graphics library is free software; you can redistribute it
 * and/or modify it under some conditions; see the "copying.grx" file
 * for details.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

#ifndef __GRX_EVENT_H__
#define __GRX_EVENT_H__

#include <glib.h>
#include <glib-object.h>
#include <gio/gio.h>

#include <grx/common.h>
#include <grx/device.h>
#include <grx/input_keysyms.h>

/**
 * SECTION:events
 * @short_description: Event handling
 * @title: Events
 * @section_id: events
 * @include: grx-3.0.h
 *
 * TODO
 */

/**
 * GrxEventType:
 * @GRX_EVENT_TYPE_NONE: indicates there is no event
 * @GRX_EVENT_TYPE_APP_ACTIVATE: application activated event, for example,
 * this is triggered by console switching by the linuxfb video driver or when
 * using a graphical desktop driver such as gtk3, this is triggered when the
 * window becomes the active window on the desktop
 * window is the active window 
 * @GRX_EVENT_TYPE_APP_DEACTIVATE: application deactivated event, for example,
 * this is triggered by console switching by the linuxfb video driver or when
 * using a graphical desktop driver such as gtk3, this is triggered when the
 * window is no longer the active window on the desktop
 * @GRX_EVENT_TYPE_APP_QUIT: application request to quit event, for example,
 * this is triggered the window is closed in a desktop application
 * @GRX_EVENT_TYPE_KEY_DOWN: key press event
 * @GRX_EVENT_TYPE_KEY_UP: key release event
 * @GRX_EVENT_TYPE_POINTER_MOTION: pointer (mouse) motion event
 * @GRX_EVENT_TYPE_BUTTON_PRESS: button press event
 * @GRX_EVENT_TYPE_BUTTON_RELEASE: button release event
 * @GRX_EVENT_TYPE_BUTTON_DOUBLE_PRESS: button double-press event
 * @GRX_EVENT_TYPE_TOUCH_DOWN: begin touch event
 * @GRX_EVENT_TYPE_TOUCH_MOTION: touch motion event
 * @GRX_EVENT_TYPE_TOUCH_UP: end touch event
 * @GRX_EVENT_TYPE_TOUCH_CANCEL: cancel touch event
 *
 * Indicates the type of event.
 */
typedef enum {
    GRX_EVENT_TYPE_NONE,
    GRX_EVENT_TYPE_APP_ACTIVATE,
    GRX_EVENT_TYPE_APP_DEACTIVATE,
    GRX_EVENT_TYPE_APP_QUIT,
    GRX_EVENT_TYPE_KEY_DOWN,
    GRX_EVENT_TYPE_KEY_UP,
    GRX_EVENT_TYPE_POINTER_MOTION,
    GRX_EVENT_TYPE_BUTTON_PRESS,
    GRX_EVENT_TYPE_BUTTON_RELEASE,
    GRX_EVENT_TYPE_BUTTON_DOUBLE_PRESS,
    GRX_EVENT_TYPE_TOUCH_DOWN,
    GRX_EVENT_TYPE_TOUCH_MOTION,
    GRX_EVENT_TYPE_TOUCH_UP,
    GRX_EVENT_TYPE_TOUCH_CANCEL,
} GrxEventType;

/**
 * GrxAnyEvent:
 * @type: the event type
 *
 * Common structure shared by all events.
 */
typedef struct {
    GrxEventType type;
} GrxAnyEvent;

/**
 * GrxKeyEvent:
 * @type: @GRX_EVENT_TYPE_KEY_DOWN or @GRX_EVENT_TYPE_KEY_UP
 * @keysym: the key symbol (translated using keymap)
 * @unichar: the UTF-32 character
 * @code: the platform dependent raw key code
 * @device: the originating device
 *
 * Structure that holds information about a keyboard key event.
 */
typedef struct {
    GrxEventType type;
    GrxKey keysym;
    gunichar unichar;
    guint32 code;
    GrxDevice *device;
} GrxKeyEvent;

/**
 * GrxMotionEvent:
 * @type: @GRX_EVENT_TYPE_POINTER_MOTION
 * @x: the pointer position along the x axis
 * @y: the pointer position along the y axis
 * @device: the originating device
 *
 * Structure that holds information about a pointer motion event.
 */
typedef struct {
    GrxEventType type;
    gint32 x;
    gint32 y;
    GrxDevice *device;
} GrxMotionEvent;

/**
 * GrxButtonEvent:
 * @type: @GRX_EVENT_TYPE_BUTTON_PRESS, @GRX_EVENT_TYPE_BUTTON_RELEASE
 * or @GRX_EVENT_TYPE_BUTTON_DOUBLE_PRESS
 * @button: the platform dependent button code
 * @device: the originating device
 *
 * Structure that holds information about a pointer button press event.
 */
typedef struct {
    GrxEventType type;
    guint32 button;
    GrxDevice *device;
} GrxButtonEvent;

/**
 * GrxTouchEvent:
 * @type: @GRX_EVENT_TYPE_TOUCH_DOWN, @GRX_EVENT_TYPE_TOUCH_MOTION,
 * @GRX_EVENT_TYPE_TOUCH_UP or @GRX_EVENT_TYPE_TOUCH_CANCEL
 * @id: the touch point id (for multi-touch devices)
 * @x: the X coordinate of the touch event
 * @y: the Y coordinate of the touch event
 * @device: the originating device
 *
 * Structure that holds information about a touch event.
 */
typedef struct {
    GrxEventType type;
    gint32 id;
    gint32 x;
    gint32 y;
    GrxDevice *device;
} GrxTouchEvent;

union _GrxEvent {
    GrxEventType type;
    GrxAnyEvent any;
    GrxKeyEvent key;
    GrxMotionEvent motion;
    GrxButtonEvent button;
    GrxTouchEvent touch;
};


#define GRX_TYPE_EVENT grx_event_get_type()

GType grx_event_get_type (void);
GrxEvent *grx_event_new (GrxEventType type);
gboolean grx_events_pending (void);
GrxEvent *grx_event_peek (void);
GrxEvent *grx_event_get (void);
void grx_event_put (GrxEvent *event);
GrxEvent *grx_event_copy (GrxEvent *event);
void grx_event_free (GrxEvent *event);

GrxEventType grx_event_get_event_type(GrxEvent *event);

/**
 * GrxEventHandlerFunc:
 * @event: pointer to the event
 * @user_data: data passed to the function, set when the source was created
 *
 * Specifies the type of function passed to grx_event_handler_add().
 */
typedef void (*GrxEventHandlerFunc)(GrxEvent *event, gpointer user_data);

GSource *grx_event_handler_source_new (void);
guint grx_event_handler_add (GrxEventHandlerFunc callback, gpointer user_data,
                             GDestroyNotify notify);

#endif /* __GRX_EVENT_H__ */