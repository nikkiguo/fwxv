#pragma once

#include <stddef.h>

#include "can.h"
#include "can_ack.h"
#include "can_msg_defs.h"
#include "can_pack.h"

StatusCode can_transmit_bps_heartbeat(const CanAckRequest *ack_ptr, uint8_t status_u8) {
{                                                       
    CanMessage msg = { 0 };                            
    can_pack_bps_heartbeat(&msg, status_u8);         
    StatusCode status = can_transmit(&msg, ack_ptr); 
    return status;                                            
}
  
StatusCode can_transmit_battery_vt(const CanAckRequest *ack_ptr, uint16_t module_id_u16, uint16_t voltage_u16, uint16_t temperature_u16) {
{                                                       
    CanMessage msg = { 0 };                            
    can_pack_battery_vt(&msg, module_id_u16, voltage_u16, temperature_u16);         
    StatusCode status = can_transmit(&msg, ack_ptr); 
    return status;                                            
}
  
StatusCode can_transmit_battery_aggregate_vc(const CanAckRequest *ack_ptr, uint32_t voltage_u32, uint32_t current_u32) {
{                                                       
    CanMessage msg = { 0 };                            
    can_pack_battery_aggregate_vc(&msg, voltage_u32, current_u32);         
    StatusCode status = can_transmit(&msg, ack_ptr); 
    return status;                                            
}
  
StatusCode can_transmit_battery_fan_state(const CanAckRequest *ack_ptr, uint8_t fan_1_u8, uint8_t fan_2_u8, uint8_t fan_3_u8, uint8_t fan_4_u8, uint8_t fan_5_u8, uint8_t fan_6_u8, uint8_t fan_7_u8, uint8_t fan_8_u8) {
{                                                       
    CanMessage msg = { 0 };                            
    can_pack_battery_fan_state(&msg, fan_1_u8, fan_2_u8, fan_3_u8, fan_4_u8, fan_5_u8, fan_6_u8, fan_7_u8, fan_8_u8);         
    StatusCode status = can_transmit(&msg, ack_ptr); 
    return status;                                            
}
  
StatusCode can_transmit_battery_relay_state(const CanAckRequest *ack_ptr, uint8_t hv_u8, uint8_t gnd_u8) {
{                                                       
    CanMessage msg = { 0 };                            
    can_pack_battery_relay_state(&msg, hv_u8, gnd_u8);         
    StatusCode status = can_transmit(&msg, ack_ptr); 
    return status;                                            
}
  
