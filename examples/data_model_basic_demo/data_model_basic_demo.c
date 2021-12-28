#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>  

#include "cJSON.h"
#include "tuya_cacert.h"
#include "tuya_log.h"
#include "tuya_error_code.h"
#include "system_interface.h"
#include "mqtt_client_interface.h"
#include "tuyalink_core.h"

const char productId[] = "t1tlm6p13aouheta";
const char deviceId[] = "6cf918e90b12f7b1ffwiuz";
const char deviceSecret[] = "a5f23a3fb341edbd";

tuya_mqtt_context_t client_instance;

//写文件
int write_file(char str[])
{
    FILE *fp = NULL; 
    fp = fopen("/tmp/from_platform.txt", "w");
    fprintf(fp, "%s", str);
    fclose(fp);
    printf("deviceid_action_time: %s\n", str); 
    return 0;
}

//读文件
int read_file(int num,char return_data[])
{
    FILE *fp = NULL;
  switch (num)
    {
    case 1:
        fp = fopen("/tmp/1.txt", "r");
        if(fp == NULL)
        {
           printf("open /tmp/1.txt error!\n");
           return 0;
        }
        fgets(return_data, 255, (FILE*)fp);
        fclose(fp);
        break;
    case 2:
        fp = fopen("/tmp/2.txt", "r");
        if(fp == NULL)
        {
           printf("open /tmp/2.txt error!\n");
           return 0;
        }
        fgets(return_data, 255, (FILE*)fp);
        fclose(fp);
        break;
    case 3:
        fp = fopen("/tmp/3.txt", "r");
        if(fp == NULL)
        {
           printf("open /tmp/3.txt error!\n");
           return 0;
        }
        fgets(return_data, 255, (FILE*)fp);  
        fclose(fp); 
        break;
    default:
        break;
    }
    printf("return_data:%s\n",return_data);
    return 0;
}

//写一个线程函数 void *函数名（void *arg）
void *thread_worker1(void *arg) 
{  
    char json_temp_hum[255];
    char json_door_state[255];
    char json_data[255];
    char json_data1[255];
        while(1)
        {  
            if(read_file(2,json_data) == 0)
            {
                sprintf(json_temp_hum,"%s",json_data);
                printf("json_temp_hum:%s\r\n",json_temp_hum);
                tuyalink_thing_property_report_with_ack(arg, NULL, json_temp_hum);
                memset(json_data, 0, sizeof(json_data));  
                
            }
            else
            {
                printf("read temp_hum error\r\n");

            }
            if(read_file(3,json_data1) == 0)
            {
                sprintf(json_door_state,"%s",json_data1);
                printf("json_door_state:%s\r\n",json_door_state);
                tuyalink_thing_event_trigger(arg, NULL, json_door_state); 
                memset(json_data1, 0, sizeof(json_data1));  
            }
            else
            {
                printf("read door_state error\r\n");

            }
            sleep(2); 
        }  
}

void on_connected(tuya_mqtt_context_t* context, void* user_data)
{ 
    int error=0;
    pthread_t t1;
    tuyalink_subdevice_bind(context, "[{\"productId\":\"snigjkwkheaxueqa\",\"nodeId\":\"255\",\"clientId\":\"1\"}]");//继电器
    tuyalink_subdevice_bind(context, "[{\"productId\":\"dtoqgbr5azgwvga3\",\"nodeId\":\"254\",\"clientId\":\"2\"}]");//门磁
    tuyalink_subdevice_bind(context, "[{\"productId\":\"6jmmnuwavyxkcv1x\",\"nodeId\":\"1\",\"clientId\":\"3\"}]");//温湿度
    error=pthread_create(&t1,NULL,thread_worker1,context);
    if(error)
       {
          printf("create pthread error!\n");
          return;     
       } 
    
}

void on_disconnect(tuya_mqtt_context_t* context, void* user_data)
{
    TY_LOGI("on disconnect");
}


void on_messages(tuya_mqtt_context_t* context, void* user_data, const tuyalink_message_t* msg)
{
    char json_relay_state[255];
    TY_LOGI("on message id:%s, type:%d, code:%d", msg->msgid, msg->type, msg->code);
    switch (msg->type) {
        case THING_TYPE_MODEL_RSP:
            TY_LOGI("Model data:%s", msg->data_string);
            break;

        case THING_TYPE_PROPERTY_SET:
            TY_LOGI("property set:%s", msg->data_string);
            break;

        case  THING_TYPE_PROPERTY_REPORT_RSP:
           break;

        case THING_TYPE_ACTION_EXECUTE:
              TY_LOGI("action execute:%s", msg->data_string);
              if(write_file(msg->data_string) == 0)
                 {
                    printf ("write ok\r\n");
                    printf ("data_string:%s\r\n",msg->data_string);
                 }
             else
                {
                    printf ("write error\r\n");
                }
                 if(read_file(1,json_relay_state) == 0)
                 {
                   sprintf(msg->data_string,"%s",json_relay_state);
                   printf("json_relay_state:%s\r\n",json_relay_state);
                   tuyalink_thing_property_report(context, NULL, msg->data_string);  
                   memset(json_relay_state, 0, sizeof(json_relay_state));      
                 }
                 else
                 {
                  printf ("read relay_state error\r\n");
                 }     
            break;   

        default:
            break;
    }
    printf("\r\n");
}

int main(int argc, char** argv)
{
    int ret = OPRT_OK;
    tuya_mqtt_context_t* client = &client_instance;
    ret = tuya_mqtt_init(client, &(const tuya_mqtt_config_t) {
        .host = "m2.tuyacn.com",
        .port = 8883,
        .cacert = tuya_cacert_pem,
        .cacert_len = sizeof(tuya_cacert_pem),
        .device_id = deviceId,
        .device_secret = deviceSecret,
        .keepalive = 60,
        .timeout_ms = 2000,
        .on_connected = on_connected,
        .on_disconnect = on_disconnect,
        .on_messages = on_messages
    });
    assert(ret == OPRT_OK);
    ret = tuya_mqtt_connect(client);
    assert(ret == OPRT_OK);
      for (;;) 
       {
        /* Loop to receive packets, and handles client keepalive */
        tuya_mqtt_loop(client);
       }
    return ret;
}
       
   