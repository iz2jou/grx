#!/usr/bin/env python3
#
# input.py
#
# Copyright (c) 2016 David Lechner <david@lechnology.com>
# This file is part of the GRX graphics library.
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, version.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
# General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program. If not, see <http://www.gnu.org/licenses/>.
#

import gi
gi.require_version('GLib', '2.0')
from gi.repository import GLib
gi.require_version('Grx', '3.0')
from gi.repository import Grx


class InputApplication(Grx.Application):
    def __init__(self):
        super(Grx.Application, self).__init__()
        self.init()
        self.hold()
        self.color = Grx.color_get_white()
        self.last_touch = None

    def do_activate(self):
        print("application started")

    def do_event(self, event):
        if Grx.Application.do_event(self, event):
            return True

        t = event.type
        if t in (Grx.EventType.KEY_DOWN, Grx.EventType.KEY_UP):
            key_event = event.key
            print("key keysym", key_event.keysym)
            print("key unichar", key_event.unichar)
            print("key code", key_event.code)
            print("modifiers", key_event.modifiers)
            if key_event.keysym in (Grx.Key.LCASE_Q, Grx.Key.BACK_SPACE, Grx.Key.ESCAPE):
                self.quit()
        elif t in (Grx.EventType.BUTTON_PRESS, Grx.EventType.BUTTON_RELEASE):
            button_event = event.button
            print("button", button_event.button)
            print("modifiers", button_event.modifiers)
            Grx.clear_screen(Grx.color_get_black())
        elif t == Grx.EventType.BUTTON_DOUBLE_PRESS:
            button_event = event.button
            print("button double-press", button_event.button)
            print("modifiers", button_event.modifiers)
        elif t == Grx.EventType.TOUCH_DOWN:
            touch_event = event.touch
            Grx.draw_pixel(touch_event.x, touch_event.y, self.color)
            self.last_touch = (touch_event.x, touch_event.y)
            print("touch", touch_event.x, touch_event.y)
            print("modifiers", touch_event.modifiers)
        elif t == Grx.EventType.TOUCH_MOTION:
            touch_event = event.touch
            Grx.draw_line(self.last_touch[0], self.last_touch[1], touch_event.x, touch_event.y, self.color)
            self.last_touch = (touch_event.x, touch_event.y)
        else:
            return False

        return True

if __name__ == '__main__':
    GLib.set_prgname('input.py')
    GLib.set_application_name('GRX3 Input Demo')
    app = InputApplication()
    app.run()
