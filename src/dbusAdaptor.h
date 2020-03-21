/*
 * Copyright 2020 UBports foundation
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; version 3.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Author: Marius Gripsgard <marius@ubports.com>
 */

#pragma once

#include <QObject>

class DbusAdaptor : public QObject
{
    Q_OBJECT
public:
    DbusAdaptor(QObject *parent = 0) : QObject(parent) {};

public Q_SLOTS:
    virtual void vibrate() = 0;
    virtual void vibrate(int durationMs) = 0;
    virtual void rumble(int durationMs, int repeat) = 0;

    virtual void setState(int state) = 0;
    virtual void setColor(unsigned int color) = 0;
    virtual void setOnMs(int ms) = 0;
    virtual void setOffMs(int ms) = 0;

};
