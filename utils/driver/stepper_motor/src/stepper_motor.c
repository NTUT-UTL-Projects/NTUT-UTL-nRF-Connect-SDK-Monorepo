#include "stepper_motor.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define SEQ_AT(stp, step_idx, coil_idx)  ((stp)->sequence[(step_idx) * (stp)->coil_count + (coil_idx)])

// Generate step sequence based on mode and coil_count
static bool generate_sequence(StepperMotor *stp)
{
    uint8_t N = stp->coil_count;
    if (N == 0) return false;

    // Determine sequence length
    switch (stp->mode) {
        case STEPPER_WAVE_DRIVE: stp->seq_len = N;     break; // One phase on per step
        case STEPPER_FULL_STEP:  stp->seq_len = N;     break; // Two adjacent phases on per step
        case STEPPER_HALF_STEP:  stp->seq_len = 2 * N; break; // Alternate single/two-phase
        default: return false;
    }

    // Allocate and initialize with zeros
    size_t total = (size_t)stp->seq_len * (size_t)N;
    stp->sequence = (uint8_t*)malloc(total);
    if (!stp->sequence) return false;
    memset(stp->sequence, 0, total);

    // Fill sequence data
    if (stp->mode == STEPPER_WAVE_DRIVE) {
        // s: 0..N-1, only the s-th phase is ON
        for (uint8_t s = 0; s < N; ++s) {
            SEQ_AT(stp, s, s) = 1;
        }
    } else if (stp->mode == STEPPER_FULL_STEP) {
        // s: 0..N-1, turn on the s-th and (s+1)%N phases
        for (uint8_t s = 0; s < N; ++s) {
            SEQ_AT(stp, s, s) = 1;
            SEQ_AT(stp, s, (uint8_t)((s + 1) % N)) = 1;
        }
    } else { // STEPPER_HALF_STEP
        // s: 0..2N-1
        // Even: only k-th phase; Odd: k-th and (k+1)%N phases
        for (uint8_t s = 0; s < 2 * N; ++s) {
            uint8_t k = (uint8_t)(s / 2);
            if ((s % 2) == 0) {
                SEQ_AT(stp, s, k) = 1;
            } else {
                SEQ_AT(stp, s, k) = 1;
                SEQ_AT(stp, s, (uint8_t)((k + 1) % N)) = 1;
            }
        }
    }

    return true;
}

static void write_step(const StepperMotor *stp, int idx)
{
    // Output each coil state according to the step sequence
    for (uint8_t c = 0; c < stp->coil_count; ++c) {
        bool on = SEQ_AT(stp, (uint8_t)idx, c) ? true : false;
        stp->gpio_write(c, on);
    }
}

void StepperMotor_Init(StepperMotor *stp,
                       uint8_t coil_count,
                       stepper_drive_mode_t mode,
                       gpio_write_fn gpio_write,
                       delay_us_fn delay_us,
                       int base_steps_per_rev)
{
    stp->coil_count = coil_count;
    stp->mode = mode;
    stp->gpio_write = gpio_write;
    stp->delay_us = delay_us;
    stp->base_steps_per_rev = base_steps_per_rev;

    // In half-step mode, steps per revolution are doubled; otherwise same as base
    stp->effective_steps_per_rev =
        (mode == STEPPER_HALF_STEP) ? (base_steps_per_rev * 2) : base_steps_per_rev;

    stp->current_step_index = 0;
    stp->sequence = NULL;
    stp->seq_len = 0;

    // Automatically generate sequence based on parameters
    if (!generate_sequence(stp)) {
        // If generation fails, reset to safe zero state
        stp->coil_count = 0;
        stp->seq_len = 0;
    }
}

void StepperMotor_Deinit(StepperMotor *stp)
{
    if (stp->sequence) {
        free(stp->sequence);
        stp->sequence = NULL;
    }
}

void StepperMotor_Step(StepperMotor *stp, int steps, int delay_us)
{
    if (stp->seq_len == 0) return;

    int direction = (steps >= 0) ? +1 : -1;
    int count = (steps >= 0) ? steps : -steps;

    for (int i = 0; i < count; i++) {
        stp->current_step_index =
            (stp->current_step_index + direction + stp->seq_len) % stp->seq_len;

        write_step(stp, stp->current_step_index);
        stp->delay_us((uint32_t)delay_us);
    }
}

void StepperMotor_RotateToAngle(StepperMotor *stp, float degree, int delay_us)
{
    // degree can be positive or negative; steps calculated based on drive mode
    float steps_f = (degree / 360.0f) * (float)stp->effective_steps_per_rev;
    int step_count = (int)lrintf(steps_f); // round to nearest integer
    StepperMotor_Step(stp, step_count, delay_us);
}
