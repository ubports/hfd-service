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

#include <QtCore/QObject>
#include <QtGui/QColor>

#include <memory>

class ComLomiriHfdLedsInterface;
class Leds: public QObject
{
    Q_OBJECT
    Q_PROPERTY(State state READ state  WRITE setState NOTIFY stateChanged)
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)
    Q_PROPERTY(int onMillisec READ onMillisec WRITE setOnMillisec NOTIFY onMillisecChanged)
    Q_PROPERTY(int offMillisec READ offMillisec WRITE setOffMillisec NOTIFY offMillisecChanged)

public:
    enum State {
        Off = 0,
        On = 1,
    };
    Q_ENUM(State)

    explicit Leds(QObject *parent = 0);

    void setState(State newState);
    State state() const;

    void setColor(const QColor &color);
    QColor color() const;

    int onMillisec() const;
    void setOnMillisec(int onMs);

    int offMillisec() const;
    void setOffMillisec(int offMs);

Q_SIGNALS:
    void stateChanged(State newState);
    void colorChanged(const QColor &color);
    void onMillisecChanged(int onMs);
    void offMillisecChanged(int offMs);

private:
    std::shared_ptr<ComLomiriHfdLedsInterface> m_interface;

    QColor m_color;
    State m_state;
    int m_onMs;
    int m_offMs;

    void turnOff();
    void turnOn();
};
