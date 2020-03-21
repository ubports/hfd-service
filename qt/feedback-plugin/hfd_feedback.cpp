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

#include "hfd_feedback.h"

#include <qfeedbackactuator.h>

#include <QtCore/QtPlugin>
#include <QtCore/QDebug>
#include <QtCore/QStringList>
#include <QtCore/QCoreApplication>
#include <QtCore/QFile>
#include <QtCore/QVariant>
#include <QtCore/QTimer>
#include <QtCore/QProcess>
#include <QtCore/QFileInfo>
#include <QDBusInterface>
#include <QDBusPendingCall>
#include <QDBusPendingCallWatcher>
#include <QDBusPendingReply>

#include "hfdInterface.h"

hfd::Feedback::Feedback() : QObject(),
                             enabled(false),
                             state(QFeedbackEffect::Stopped)
{
    m_interface = std::make_shared<com::lomiri::hfd::Vibrator>("com.lomiri.hfd", "/com/lomiri/hfd",
                                                            QDBusConnection::systemBus(), this);
    actuatorList << createFeedbackActuator(this, 42);
}

hfd::Feedback::~Feedback()
{
}

QFeedbackInterface::PluginPriority hfd::Feedback::pluginPriority()
{
    return PluginHighPriority;
}

QList<QFeedbackActuator*> hfd::Feedback::actuators()
{
    return actuatorList;
}

void hfd::Feedback::setActuatorProperty(const QFeedbackActuator&, ActuatorProperty prop, const QVariant &value)
{
    switch (prop)
    {
    case Enabled:
        enabled = value.toBool();
        break;
    default:
        break;
    }
}

QVariant hfd::Feedback::actuatorProperty(const QFeedbackActuator &actuator, ActuatorProperty prop)
{
    QVariant result;

    switch (prop)
    {
    case Name: result = QString::fromLocal8Bit("Hfd Vibrator"); break;
    case State: result = actuator.isValid() ? QFeedbackActuator::Ready : QFeedbackActuator::Unknown; break;
    case Enabled: result = enabled; break;
    }

    return result;
}

bool hfd::Feedback::isActuatorCapabilitySupported(const QFeedbackActuator &, QFeedbackActuator::Capability cap)
{
    bool result = false;

    switch(cap)
    {
    case QFeedbackActuator::Envelope: result = true; break;
    case QFeedbackActuator::Period: result = false; break;
    }

    return result;
}

void hfd::Feedback::updateEffectProperty(const QFeedbackHapticsEffect *, EffectProperty)
{
}

void hfd::Feedback::hapticsVibrateReply(QDBusPendingCallWatcher *watcher, int period, int repeat)
{
    QDBusPendingReply<> reply = *watcher;
    if (reply.isError()) {
        qWarning() << "Failed to vibrate with pattern:" << reply.error().message();
        state = QFeedbackEffect::Stopped;
    } else {
        if ((repeat == QFeedbackEffect::Infinite) || (--repeat > 0))
            QTimer::singleShot(period*2, [=]() { vibrate(period, repeat); });
        else
            state = QFeedbackEffect::Stopped;
    }

    watcher->deleteLater();
}

void hfd::Feedback::vibrate(int period, int repeat)
{
    if (!(period && repeat))
        state = QFeedbackEffect::Stopped;

    if (state != QFeedbackEffect::Running) {
        // Maybe stopped/paused before this async call.
        return;
    }


    QDBusInterface iface("com.lomiri.hfd",
                         "/com/lomiri/hfd",
                         "com.lomiri.hfd.Vibrator");

    QDBusPendingCall call = m_interface->vibrate(period);
    QDBusPendingCallWatcher *watcher = new QDBusPendingCallWatcher(call, this);
    connect(watcher, &QDBusPendingCallWatcher::finished,
            [=](){ hapticsVibrateReply(watcher, period, repeat); });
}

void hfd::Feedback::startVibration(const QFeedbackHapticsEffect *effect)
{
    int duration = effect->duration();
    if (duration == 0)
        duration = 150;

    int period = effect->period();
    int repeat;
    if ((duration == QFeedbackEffect::Infinite) || (duration < 0)) {
        // If duration is set to QFeedbackEffect::Infinite or a negative
        // value, we repeat this effect forever until stopped. The
        // effective period should have been set to a positive value or
        // 150ms by default.
        duration = QFeedbackEffect::Infinite;
        repeat = QFeedbackEffect::Infinite;
        if (period <= 0)
            period = 150;
    } else if (period <= 0) {
        // If duration is set to a positive value and period is invalid,
        // then use duration as period.
        repeat = 1;
        period = duration;
    } else {
        // Otherwise, repeat this effect as many times as the duration
        // may cover the effect period.
        repeat = (duration + period - 1) / period;
    }

    vibrate(period, repeat);
}

void hfd::Feedback::setEffectState(const QFeedbackHapticsEffect *effect, QFeedbackEffect::State state)
{
    this->state = state;
    switch (state)
    {
    case QFeedbackEffect::Stopped:
        break;
    case QFeedbackEffect::Paused:
        break;
    case QFeedbackEffect::Running:
        QTimer::singleShot(0, [=]() { startVibration(effect); });
        break;
    case QFeedbackEffect::Loading:
        break;
    }
}

QFeedbackEffect::State hfd::Feedback::effectState(const QFeedbackHapticsEffect *)
{
    return state;
}
