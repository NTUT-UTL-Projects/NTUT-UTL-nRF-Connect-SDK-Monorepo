add_library(stepper_motor STATIC)

target_sources(stepper_motor PRIVATE 
  src/stepper_motor.c
)

target_include_directories(stepper_motor INTERFACE
  .
  ./src
)
