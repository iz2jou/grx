#!/usr/bin/env python3
#
# intl.py - Demonstration of drawing text in various languages/scripts
#
# Copyright (c) 2017 David Lechner <david@lechnology.com>
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

# TODO: need to implement scrolling for small screens

import os
from subprocess import check_output

import gi
gi.require_version('GLib', '2.0')
from gi.repository import GLib
gi.require_version('Grx', '3.0')
from gi.repository import Grx


class App(Grx.Application):
    FONT_TABLE = (
        # (<language code>, <script code>, <hello world>)
        ('en', None, 'Hello world'),
        ('zh-cn', None, '你好，世界'),
        ('ja', None, 'こんにちは世界'),
        ('ko', None, '안녕 세상'),
        ('ru', None, 'Привет мир'),
        ('el', None, 'Γειά σου Κόσμε'),
        ('vi', None, 'Chào thế giới'),
        ('hi', None, 'नमस्ते दुनिया'),
        ('ka', None, 'გამარჯობა მსოფლიო'),
        (None, 'Runr', 'ᚻᛖᛚᛚᚩ ᚹᚩᚱᛚᛞ'),
    )
    FONT_SIZE = 12

    def __init__(self):
        super(Grx.Application, self).__init__()
        self.init()
        self.WHITE = Grx.color_get_white()
        self.BLACK = Grx.color_get_black()
        self.font_index = 0
        self.next_index = 0
        self.prev_index = []

    def do_activate(self):
        self.show_next_font(0)

    def do_input_event(self, event):
        if event.type == Grx.EventType.KEY_DOWN:
            if event.key.keysym in (Grx.KEY_q, Grx.KEY_BackSpace, Grx.KEY_Escape):
                self.quit()

    def show_next_font(self, start_index):
        Grx.clear_screen(self.WHITE)
        offset = 0
        try:
            for item in self.FONT_TABLE:
                font = Grx.font_load_full(
                    None, self.FONT_SIZE, Grx.get_dpi(), Grx.FontWeight.REGULAR,
                    Grx.FontSlant.REGULAR, Grx.FontWidth.REGULAR, False, item[0], item[1])
                text_opt = Grx.TextOptions.new_full(
                    font, self.BLACK, self.WHITE, Grx.TextHAlign.CENTER, Grx.TextVAlign.TOP)
                Grx.draw_text(item[2], Grx.get_width() / 2, offset, text_opt)
                offset += font.get_height() + 0
        except GLib.Error as err:
            Grx.draw_text(str(err), 10, 10, self.default_font)

if __name__ == '__main__':
    app = App()
    app.run()
