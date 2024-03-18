/*
 * Copyright (C) 2015   Jeremy Chen jeremy_cz@yahoo.com
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "common_defs.h"
#include "CBaseSysDep.h"

#ifndef __FDBUS_C_COMMON_H__
#define __FDBUS_C_COMMON_H__
 
#ifdef __cplusplus
extern "C"
{
#endif

typedef struct fdb_subscribe_item_tag
{
    FdbMsgCode_t event_code;
    const char *topic;
} fdb_subscribe_item_t;

typedef struct fdb_message_tag
{
    FdbSessionId_t sid;
    FdbMsgCode_t msg_code;
    uint8_t *msg_data;
    int32_t data_size;
    int32_t status;
    const char *topic;
    void *user_data;
    enum EFdbQOS qos;

    void *_msg_buffer; // internal used only!!!
}fdb_message_t;

#ifdef __cplusplus
}
#endif

#endif

