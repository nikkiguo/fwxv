#pragma once
#include "can_fifo.h"
#include "can_msg.h"
#include "bms_carrier_unpack_msg.h"


#define BMS_CARRIER_BPS_HEARTBEAT_ID 0

#define BMS_CARRIER_BATTERY_VT_ID 30

#define BMS_CARRIER_BATTERY_AGGREGATE_VC_ID 33

#define BMS_CARRIER_BATTERY_FAN_STATE_ID 57

#define BMS_CARRIER_BATTERY_RELAY_STATE_ID 58

  

void can_receive_all() {
    CanFifo msg_queue;
    can_fifo_init(&msg_queue);
    CanMessage msg = {0};
    while(can_fifo_pop(&msg_queue, &msg)){
        switch (msg.msg_id){
            
             case BMS_CARRIER_BPS_HEARTBEAT_ID:
                can_unpack_bps_heartbeat(&msg, &bms_carrier_receive_struct.bps_heartbeat_status);
                break;
            
             case BMS_CARRIER_BATTERY_VT_ID:
                can_unpack_battery_vt(&msg, &bms_carrier_receive_struct.battery_vt_module_id, &bms_carrier_receive_struct.battery_vt_voltage, &bms_carrier_receive_struct.battery_vt_temperature);
                break;
            
             case BMS_CARRIER_BATTERY_AGGREGATE_VC_ID:
                can_unpack_battery_aggregate_vc(&msg, &bms_carrier_receive_struct.battery_aggregate_vc_voltage, &bms_carrier_receive_struct.battery_aggregate_vc_current);
                break;
            
             case BMS_CARRIER_BATTERY_FAN_STATE_ID:
                can_unpack_battery_fan_state(&msg, &bms_carrier_receive_struct.battery_fan_state_fan_1, &bms_carrier_receive_struct.battery_fan_state_fan_2, &bms_carrier_receive_struct.battery_fan_state_fan_3, &bms_carrier_receive_struct.battery_fan_state_fan_4, &bms_carrier_receive_struct.battery_fan_state_fan_5, &bms_carrier_receive_struct.battery_fan_state_fan_6, &bms_carrier_receive_struct.battery_fan_state_fan_7, &bms_carrier_receive_struct.battery_fan_state_fan_8);
                break;
            
             case BMS_CARRIER_BATTERY_RELAY_STATE_ID:
                can_unpack_battery_relay_state(&msg, &bms_carrier_receive_struct.battery_relay_state_hv, &bms_carrier_receive_struct.battery_relay_state_gnd);
                break;
              
        }
    }
}