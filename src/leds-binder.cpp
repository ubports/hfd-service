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
 * Author: Erfan Abdi <erfangplus@gmail.com>
 */

#include "leds-binder.h"

#include <iostream>
#include <unistd.h>

#define BINDER_LIGHT_SERVICE_DEVICE "/dev/hwbinder"
#define BINDER_LIGHT_SERVICE_IFACE "android.hardware.light@2.0::ILight"
#define BINDER_LIGHT_SERVICE_FQNAME BINDER_LIGHT_SERVICE_IFACE "/default"

namespace hfd {

bool LedsBinder::usable() {
    GBinderServiceManager *sm;
    GBinderRemoteObject *remote;
    GBinderClient *client;
    GBinderRemoteReply *reply;
    int status;

    sm = gbinder_servicemanager_new(BINDER_LIGHT_SERVICE_DEVICE);
    if (!sm)
        return false;

    remote = gbinder_servicemanager_get_service_sync(sm, BINDER_LIGHT_SERVICE_FQNAME, NULL);
    if (!remote) {
        gbinder_servicemanager_unref(sm);
        return false;
    }
    client = gbinder_client_new(remote, BINDER_LIGHT_SERVICE_IFACE);
    if (!client) {
        gbinder_remote_object_unref(remote);
        gbinder_servicemanager_unref(sm);
        return false;
    }

    GBinderLocalRequest *req = gbinder_client_new_request(client);
    reply = gbinder_client_transact_sync_reply(client,
                                               2 /* getSupportedTypes */, req, &status);
    gbinder_local_request_unref(req);

    if (status == GBINDER_STATUS_OK) {
        GBinderReader reader;
        guint value;

        gbinder_remote_reply_init_reader(reply, &reader);
        status = gbinder_reader_read_uint32(&reader, &value);
        if (value != GBINDER_STATUS_OK) {
            gbinder_remote_object_unref(remote);
            gbinder_servicemanager_unref(sm);
            return false;
        }

        gsize count = 0;
        gsize vecSize = 0;
        const int32_t *types;
        types = (const int32_t*) gbinder_reader_read_hidl_vec(&reader, &count, &vecSize);
        for (int i = 0; i < count; i++) {
            if (types[i] == LIGHT_TYPE_NOTIFICATIONS) {
                gbinder_remote_object_unref(remote);
                gbinder_servicemanager_unref(sm);
                return true;
            }
        }
    }
    gbinder_remote_object_unref(remote);
    gbinder_servicemanager_unref(sm);
    return false;
}

LedsBinder::LedsBinder() : Leds() {
    mSm = gbinder_servicemanager_new(BINDER_LIGHT_SERVICE_DEVICE);
    if (!mSm)
        return;

    mRemote = gbinder_servicemanager_get_service_sync(mSm, BINDER_LIGHT_SERVICE_FQNAME, NULL);
    if (!mRemote) {
        gbinder_servicemanager_unref(mSm);
        return;
    }
    mClient = gbinder_client_new(mRemote, BINDER_LIGHT_SERVICE_IFACE);
    if (!mClient) {
        gbinder_remote_object_unref(mRemote);
        gbinder_servicemanager_unref(mSm);
        return;
    }

    // Get up to date
    configure();
}

LedsBinder::~LedsBinder()
{
    gbinder_remote_object_unref(mRemote);
    gbinder_servicemanager_unref(mSm);
}

void LedsBinder::configure() {
    if (m_state == State::On)
        turnOn();
    else
        turnOff();
}

void LedsBinder::turnOn()
{
    GBinderRemoteReply *reply;
    GBinderWriter writer;
    LightState *notification_state;
    int status;
    GBinderLocalRequest *req = gbinder_client_new_request(mClient);

    gbinder_local_request_init_writer(req, &writer);
    notification_state = gbinder_writer_new0(&writer, LightState);
    int alpha = (m_color >> 24) & 0xFF;
    int red = (m_color >> 16) & 0xFF;
    int green = (m_color >> 8) & 0xFF;
    int blue = m_color & 0xFF;
    alpha = 0xff; //TODO: Support alpha
    int newcolor = ((alpha & 0xff) << 24) + ((red & 0xff) << 16) + ((green & 0xff) << 8) + (blue & 0xff);
    notification_state->color = newcolor;
    notification_state->flashMode = FLASH_TYPE_TIMED;
    notification_state->flashOnMs = m_onMs;
    notification_state->flashOffMs = m_offMs;
    notification_state->brightnessMode = BRIGHTNESS_MODE_USER;

    gbinder_writer_append_int32(&writer, LIGHT_TYPE_NOTIFICATIONS);
    gbinder_writer_append_buffer_object(&writer, notification_state,
                                        sizeof(*notification_state));

    reply = gbinder_client_transact_sync_reply(mClient,
                                               1 /* setLight */, req, &status);
    gbinder_local_request_unref(req);

    if (status == GBINDER_STATUS_OK) {
        GBinderReader reader;
        guint value;

        gbinder_remote_reply_init_reader(reply, &reader);
        status = gbinder_reader_read_uint32(&reader, &value);
    }
}

void LedsBinder::turnOff()
{
    GBinderRemoteReply *reply;
    GBinderWriter writer;
    LightState *notification_state;
    int status;
    GBinderLocalRequest *req = gbinder_client_new_request(mClient);

    gbinder_local_request_init_writer(req, &writer);
    notification_state = gbinder_writer_new0(&writer, LightState);
    notification_state->color = 0;
    notification_state->flashMode = FLASH_TYPE_NONE;
    notification_state->flashOnMs = 0;
    notification_state->flashOffMs = 0;
    notification_state->brightnessMode = BRIGHTNESS_MODE_USER;

    gbinder_writer_append_int32(&writer, LIGHT_TYPE_NOTIFICATIONS);
    gbinder_writer_append_buffer_object(&writer, notification_state,
                                        sizeof(*notification_state));

    reply = gbinder_client_transact_sync_reply(mClient,
                                               1 /* setLight */, req, &status);
    gbinder_local_request_unref(req);

    if (status == GBINDER_STATUS_OK) {
        GBinderReader reader;
        guint value;

        gbinder_remote_reply_init_reader(reply, &reader);
        status = gbinder_reader_read_uint32(&reader, &value);
    }
}
}
