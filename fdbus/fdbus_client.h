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
#include "fdbus_c_common.h"

#ifndef __FDBUS_CLIENT_H__
#define __FDBUS_CLIENT_H__

#ifdef __cplusplus
extern "C"
{
#endif

struct fdb_client_tag;
typedef void (*fdb_client_online_fn_t)(struct fdb_client_tag *self, FdbSessionId_t sid, enum EFdbQOS qos);
typedef void (*fdb_client_offline_fn_t)(struct fdb_client_tag *self, FdbSessionId_t sid, enum EFdbQOS qos);
typedef void (*fdb_client_reply_fn_t)(struct fdb_client_tag *self, fdb_message_t *msg);
typedef void (*fdb_message_reply_fn_t)(struct fdb_client_tag *self, fdb_message_t *msg);
typedef void (*fdb_client_get_event_fn_t)(struct fdb_client_tag *self, fdb_message_t *msg);
typedef void (*fdb_client_broadcast_fn_t)(struct fdb_client_tag *self, fdb_message_t *msg);

typedef struct fdb_client_handles_tag
{
    fdb_client_online_fn_t on_online_func;
    fdb_client_offline_fn_t on_offline_func;
    fdb_client_reply_fn_t on_reply_func;
    fdb_client_get_event_fn_t on_get_event_func;
    fdb_client_broadcast_fn_t on_broadcast_func;
}fdb_client_handles_t;

typedef struct fdb_client_tag
{
    void *native_handle;
    void *user_data;
    const fdb_client_handles_t *handles;
}fdb_client_t;

LIB_EXPORT
fdb_client_t *fdb_client_create(const char *name, void *user_data);
LIB_EXPORT
fdb_client_t *fdb_client_create_with_handle(void *user_data, void *client_handle);
LIB_EXPORT
void *fdb_client_get_user_data(fdb_client_t *handle);
LIB_EXPORT
void fdb_client_register_event_handle(fdb_client_t *handle, const fdb_client_handles_t *handles);

LIB_EXPORT
void fdb_client_destroy(fdb_client_t *handle);
LIB_EXPORT
fdb_bool_t fdb_client_connect(fdb_client_t *handle, const char *url);
LIB_EXPORT
fdb_bool_t fdb_client_disconnect(fdb_client_t *handle);
LIB_EXPORT
fdb_bool_t fdb_client_invoke_async(fdb_client_t *handle,
                                   FdbMsgCode_t msg_code,
                                   const uint8_t *msg_data,
                                   int32_t data_size,
                                   int32_t timeout,
                                   enum EFdbQOS qos,
                                   void *user_data,
                                   const char *log_data);
LIB_EXPORT
fdb_bool_t fdb_client_invoke_callback(fdb_client_t *handle,
                                   FdbMsgCode_t msg_code,
                                   const uint8_t *msg_data,
                                   int32_t data_size,
                                   int32_t timeout,
                                   enum EFdbQOS qos,
                                   fdb_message_reply_fn_t reply_fn,
                                   void *user_data,
                                   const char *log_data);
LIB_EXPORT
fdb_bool_t fdb_client_invoke_sync(fdb_client_t *handle,
                                  FdbMsgCode_t msg_code,
                                  const uint8_t *msg_data,
                                  int32_t data_size,
                                  int32_t timeout,
                                  enum EFdbQOS qos,
                                  const char *log_data,
                                  fdb_message_t *ret_msg);

LIB_EXPORT
void fdb_client_release_return_msg(fdb_message_t *ret_msg);

LIB_EXPORT
fdb_bool_t fdb_client_send(fdb_client_t *handle,
                           FdbMsgCode_t msg_code,
                           const uint8_t *msg_data,
                           int32_t data_size,
                           enum EFdbQOS qos,
                           const char *log_data);

LIB_EXPORT
fdb_bool_t fdb_client_subscribe(fdb_client_t *handle,
                                const fdb_subscribe_item_t *sub_items,
                                int32_t nr_items,
                                enum EFdbQOS qos);

LIB_EXPORT
fdb_bool_t fdb_client_unsubscribe(fdb_client_t *handle,
                                  const fdb_subscribe_item_t *sub_items,
                                  int32_t nr_items,
                                  enum EFdbQOS qos);

LIB_EXPORT
fdb_bool_t fdb_client_publish(fdb_client_t *handle,
                              FdbMsgCode_t event,
                              const char *topic,
                              const uint8_t *event_data,
                              int32_t data_size,
                              enum EFdbQOS qos,
                              const char *log_data,
                              fdb_bool_t always_update);

LIB_EXPORT
fdb_bool_t fdb_client_get_event_async(fdb_client_t *handle,
                                      FdbMsgCode_t event,
                                      const char *topic,
                                      int32_t timeout,
                                      enum EFdbQOS qos,
                                      void *user_data);

fdb_bool_t fdb_client_get_event_sync(fdb_client_t *handle,
                                     FdbMsgCode_t event,
                                     const char *topic,
                                     int32_t timeout,
                                     enum EFdbQOS qos,
                                     fdb_message_t *ret_msg);

#ifdef __cplusplus
}
#endif
                          
#endif

