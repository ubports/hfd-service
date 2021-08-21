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

#include "dbusAdaptor.h"
#include "vibrator.h"
#include "leds.h"
#include "utils.h"

#include "hfdadaptor.h"

#include <memory>
#include <iostream>

#include <QCoreApplication>

class DbusAdaptorService : public DbusAdaptor {
    Q_OBJECT
public:
    DbusAdaptorService(std::shared_ptr<hfd::Vibrator> vibrator, std::shared_ptr<hfd::Leds> leds)
        : DbusAdaptor()
        , m_vibrator(vibrator)
        , m_leds(leds)
    {}

public Q_SLOTS:
    void vibrate() override { m_vibrator->vibrate(); };
    void vibrate(unsigned int durationMs) override { m_vibrator->vibrate(durationMs); };
    void vibratePattern(unsigned int durationMs[], unsigned int repeat) override { m_vibrator->vibrate(durationMs, repeat)};
    void rumble(unsigned int durationMs, unsigned int repeat) override { m_vibrator->rumble(durationMs, repeat); };

    void setState(int state) override { m_leds->setState(hfd::utils::toState(state)); };
    void setColor(unsigned int color) override { m_leds->setColor(color); }
    void setOnMs(int ms) override { m_leds->setOnMs(ms); };
    void setOffMs(int ms) override { m_leds->setOffMs(ms); };

private:
    std::shared_ptr<hfd::Vibrator> m_vibrator;
    std::shared_ptr<hfd::Leds> m_leds;
};

#include "service.moc"


int main(int argc, char *argv[]) {
    QCoreApplication app(argc, argv);

    // Environment variables to switch between implementations
    const char* vibrator_impl_env = getenv("HFD_VIBRATOR_IMPL");
    const char* leds_impl_env = getenv("HFD_LEDS_IMPL");

    std::cout << "Starting vibrator impl" << std::endl;
    std::shared_ptr<hfd::Vibrator> vibrator;
    if (vibrator_impl_env)
        vibrator = hfd::Vibrator::create(vibrator_impl_env);
    else
        vibrator = hfd::Vibrator::create();

    std::cout << "Starting leds impl" << std::endl;
    std::shared_ptr<hfd::Leds> leds;
    if (leds_impl_env)
        leds = hfd::Leds::create(leds_impl_env);
    else
        leds = hfd::Leds::create();

    std::cout << "done" << std::endl;
    auto dbusAdaptor = new DbusAdaptorService(vibrator, leds);

    new VibratorAdaptor(dbusAdaptor);
    new LedsAdaptor(dbusAdaptor);
    QDBusConnection connection = QDBusConnection::systemBus();
    connection.registerObject("/com/lomiri/hfd", dbusAdaptor);
    connection.registerService("com.lomiri.hfd");

    if (connection.lastError().isValid()) {
        std::cout << "Not connected to DBus!!!" << std::endl;
        qWarning() << connection.lastError();
        return 1;
    }

    std::cout << "Started dbus conn" << std::endl;

    return app.exec();

}
