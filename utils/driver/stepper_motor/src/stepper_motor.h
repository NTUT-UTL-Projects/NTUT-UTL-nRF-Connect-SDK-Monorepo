#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

// User-defined GPIO control function type
typedef void (*gpio_write_fn)(uint8_t pin, bool value);

// User-defined delay function type (unit: microseconds)
typedef void (*delay_us_fn)(uint32_t delay_us);

// Drive mode
typedef enum {
    STEPPER_WAVE_DRIVE = 0,  // Single-phase excitation (wave drive)
    STEPPER_FULL_STEP  = 1,  // Two-phase excitation (full step)
    STEPPER_HALF_STEP  = 2   // Half step (alternating single/two-phase)
} stepper_drive_mode_t;

// Stepper motor context structure
typedef struct {
    uint8_t  coil_count;               // Number of coils (phases) N
    uint8_t  seq_len;                  // Sequence length per electrical cycle
    uint8_t *sequence;                 // Flat array, length = seq_len * coil_count, values 0/1

    stepper_drive_mode_t mode;         // Drive mode
    gpio_write_fn gpio_write;          // GPIO control function
    delay_us_fn  delay_us;             // Delay function

    int base_steps_per_rev;            // Mechanical “full-step” steps per revolution (based on FULL/WAVE)
    int effective_steps_per_rev;       // Actual steps per revolution depending on mode (half-step doubles it)
    int current_step_index;            // Current sequence index
} StepperMotor;

// Initialize (automatically generate sequence based on coil count and mode)
void StepperMotor_Init(StepperMotor *stp,
                       uint8_t coil_count,
                       stepper_drive_mode_t mode,
                       gpio_write_fn gpio_write,
                       delay_us_fn delay_us,
                       int base_steps_per_rev);

// Release dynamic resources
void StepperMotor_Deinit(StepperMotor *stp);

// Control movement by number of steps (positive/negative)
void StepperMotor_Step(StepperMotor *stp, int steps, int delay_us);

// Rotate to a specific angle (degree can be positive/negative)
void StepperMotor_RotateToAngle(StepperMotor *stp, float degree, int delay_us);

#ifdef __cplusplus
}
#endif
