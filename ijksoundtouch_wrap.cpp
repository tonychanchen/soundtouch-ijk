/*
 * Copyright (c) 2017 Bilibili
 * copyright (c) 2017 Raymond Zheng <raymondzheng1412@gmail.com>
 *
 * This file is part of ijkPlayer.
 *
 * ijkPlayer is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * ijkPlayer is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with ijkPlayer; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#include "SoundTouch.h"

using namespace std;

using namespace soundtouch;
extern "C" void* ijk_soundtouch_create(float speed, int pitch, int n_channel, int n_sampleRate);
extern "C" int ijk_soundtouch_translate(void *handle, short* data, int len, int bytes_per_sample, int n_channel);
extern "C" void ijk_soundtouch_destroy(void *handle);

void* ijk_soundtouch_create(float speed, int pitch, int n_channel, int n_sampleRate) {
    SoundTouch *handle_ptr = new SoundTouch();
    
    handle_ptr->setTempo(1.0);
    handle_ptr->setPitchSemiTones(pitch);
    handle_ptr->setRate(1.0);

    handle_ptr->setSampleRate(16000);
    handle_ptr->setChannels(n_channel);
    
    handle_ptr->setSetting(SETTING_SEQUENCE_MS, 40);
    handle_ptr->setSetting(SETTING_SEEKWINDOW_MS, 16);
    handle_ptr->setSetting(SETTING_OVERLAP_MS, 8);
    
    return handle_ptr;
}

int ijk_soundtouch_translate(void *handle, short* data, int len, int bytes_per_sample, int n_channel) {
    SoundTouch *handle_ptr = (SoundTouch*)handle;
    int put_n_sample = len / n_channel;
    int nb = 0;
    int pcm_data_size = 0;
    if (handle_ptr == NULL)
        return 0;

//    handle_ptr->setTempo(1.0);
//    handle_ptr->setPitchSemiTones(pitch);
//    handle_ptr->setRate(1.0);
//
//    handle_ptr->setSampleRate(n_sampleRate);
//    handle_ptr->setChannels(n_channel);
//
//    handle_ptr->setSetting(SETTING_SEQUENCE_MS, 40);
//    handle_ptr->setSetting(SETTING_SEEKWINDOW_MS, 16);
//    handle_ptr->setSetting(SETTING_OVERLAP_MS, 8);
    
    handle_ptr->putSamples((SAMPLETYPE*)data, put_n_sample);

    do {
        nb = handle_ptr->receiveSamples((SAMPLETYPE*)data, 2048);
        pcm_data_size += nb * n_channel * bytes_per_sample;
//        printf("pcm_size_callback_tran: %d === %d \n",nb,pcm_data_size);
    } while (nb != 0);

    return pcm_data_size;
}

void ijk_soundtouch_destroy(void *handle) {
    SoundTouch *handle_ptr = (SoundTouch*)handle;
    if (handle_ptr == NULL)
        return;
    handle_ptr->clear();
    delete handle_ptr;
}
