#include "power_distribution_power_sequence_task.h"

#include "delay.h"
#include "fsm2.h"
#include "log.h"
#include "notify.h"
#include "task.h"

/*
  Notes:
  - fsm_transition takes you to the next state -> have this at the end of each input function
  - we need a state list
  - we need a transition list


  TODO:
  - add transition list
  - clean up existing
*/

FSM(power_sequence, NUM_POWER_SEQUENCE_STATES);

void prv_power_sequence_init_state_input(Fsm *fsm, void *context)
{
    Fsm *fsm1 = context;
    uint32_t notif;
    // Only transition if FSM1 has finished state 1 and sent notification
    notify_get(&notif);
    if (notify_check_event(&notif, FSM1_STATE_0_CMPL))
    {
        fsm_transition(fsm, FSM1_STATE_1);
        LOG_DEBUG("New FSM Cycle\n");
    }
    else
    {
        // TODO(mitchellostler): Make this use GPIO_IT notification
        fsm_transition(fsm, FSM1_STATE_1);
    }
}

static void prv_power_sequence_init_state_output(void *context)
{
    LOG_DEBUG("Transitioned to FSM1 state0\n");
    // Tell fsm1 that we've completed state0
    notify(fsm2, FSM1_STATE_0_CMPL);
}

static void prv_power_sequence_driver_controls_state_input(Fsm *fsm, void *context)
{
    Fsm *fsm1 = context;
    uint32_t notif;
    // Only transition if FSM1 has finished state 1 and sent notification
    notify_get(&notif);
    if (notify_check_event(&notif, FSM2_STATE_1_CMPL))
    {
        fsm_transition(fsm, FSM1_STATE_2);
    }
}

static void prv_power_sequence_driver_controls_state_output(void *context)
{
    LOG_DEBUG("Transitioned to FSM1 state1\n");
    // Tell fsm1 that we've completed state0
    notify(fsm2, FSM1_STATE_1_CMPL);
}

static void prv_power_sequence_power_main_everything_state_input(Fsm *fsm, void *context)
{
    Fsm *fsm1 = context;
    uint32_t notif;
    // Only transition if FSM1 has finished state 1 and sent notification
    notify_get(&notif);
    if (notify_check_event(&notif, FSM1_STATE_2_CMPL))
    {
        fsm_transition(fsm, FSM1_STATE_0);
    }
}

static void prv_power_sequence_power_main_everything_state_output(void *context)
{
    LOG_DEBUG("Transitioned to FSM1 state2\n");
    // Tell fsm1 that we've completed state0
    notify(fsm2, FSM1_STATE_2_CMPL);
}

static void prv_power_sequence_main_operation_state_input(Fsm *fsm, void *context)
{
    Fsm *fsm1 = context;
    uint32_t notif;
    // Only transition if FSM1 has finished state 1 and sent notification
    notify_get(&notif);
    if (notify_check_event(&notif, FSM1_STATE_2_CMPL))
    {
        fsm_transition(fsm, FSM1_STATE_0);
    }
}

static void prv_power_sequence_main_operation_state_output(void *context)
{
    LOG_DEBUG("Transitioned to FSM1 state2\n");
    // Tell fsm1 that we've completed state0
    notify(fsm2, FSM1_STATE_2_CMPL);
}

// Declare states in state list
static FsmState s_test1_state_list[NUM_POWER_SEQUENCE_STATES] = {
    STATE(POWER_SEQUENCE_INIT_STATE, prv_power_sequence_init_state_input,
          prv_power_sequence_init_state_output),
    STATE(POWER_SEQUENCE_DRIVER_CONTROLS_STATE, prv_power_sequence_driver_controls_state_input,
          prv_power_sequence_driver_controls_state_output),
    STATE(POWER_SEQUENCE_POWER_MAIN_EVERYTHING_STATE,
          prv_power_sequence_power_main_everything_state_input,
          prv_power_sequence_power_main_everything_state_output),
    STATE(POWER_SEQUENCE_MAIN_OPERATION_STATE, prv_power_sequence_main_operation_state_input,
          prv_power_sequence_main_operation_state_output),
};

// Declares transition for state machine, must match those in input functions
static FsmTransition s_test1_transitions[NUM_POWER_SEQUENCE_TRANSITIONS] = {
    // Transitions for init state
    TRANSITION(POWER_SEQUENCE_INIT_STATE, POWER_SEQUENCE_POWER_MAIN_EVERYTHING_STATE),
    TRANSITION(POWER_SEQUENCE_INIT_STATE, POWER_SEQUENCE_DRIVER_CONTROLS_STATE),

    // Transitions for driver controls state
    TRANSITION(POWER_SEQUENCE_DRIVER_CONTROLS_STATE, POWER_SEQUENCE_POWER_MAIN_EVERYTHING_STATE),
    TRANSITION(POWER_SEQUENCE_DRIVER_CONTROLS_STATE, POWER_SEQUENCE_INIT_STATE),

    // Transitions for power main everything on state
    TRANSITION(POWER_SEQUENCE_POWER_MAIN_EVERYTHING_STATE, POWER_SEQUENCE_MAIN_OPERATION_STATE),

    // Transitions for main operation state
    TRANSITION(POWER_SEQUENCE_MAIN_OPERATION_STATE, POWER_SEQUENCE_MAIN_OPERATION_STATE),
    TRANSITION(POWER_SEQUENCE_MAIN_OPERATION_STATE, POWER_SEQUENCE_INIT_STATE),
};

StatusCode init_power_sequence(void)
{
    FsmSettings settings = {
        .state_list = s_test1_state_list,
        .transitions = s_test1_transitions,
        .num_transitions = NUM_POWER_SEQUENCE_TRANSITIONS,
        .initial_state = POWER_SEQUENCE_INIT_STATE,
    };
    fsm_init(power_sequence, settings, NULL);
    return STATUS_CODE_OK;
}
