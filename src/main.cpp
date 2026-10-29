/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       errortoast                                                */
/*    Created:      10/2/2026, 2:26:42 PM                                     */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#include "vex.h"

using namespace vex;

// A global instance of competition
competition Competition;

// PIN DEFINITIONS

int intakeMotorPort = 1; // Intake motor port
int liftMotorPort = 2; // Lift motor port

int intakeSpeed = 80; // Intake speed percentage
int liftSpeed = 40; // Lift speed percentage

bool intakeReversed = false; // Intake direction flag
bool liftLeftReversed = false; // Lift direction flag
bool liftRightReversed = false; // Lift direction flag

// DEVICES
motor Intake(intakeMotorPort, gearSetting::ratio18_1, intakeReversed);
motor liftLeft(liftMotorPort, gearSetting::ratio18_1, liftLeftReversed);
motor liftRight(liftMotorPort, gearSetting::ratio18_1, liftRightReversed);

// define your global instances of motors and other devices here

/*---------------------------------------------------------------------------*/
/*                          Pre-Autonomous Functions                         */
/*                                                                           */
/*  You may want to perform some actions before the competition starts.      */
/*  Do them in the following function.  You must return from this function   */
/*  or the autonomous and usercontrol tasks will not be started.  This       */
/*  function is only called once after the V5 has been powered on and        */
/*  not every time that the robot is disabled.                               */
/*---------------------------------------------------------------------------*/

void pre_auton(void) {

  // All activities that occur before the competition starts
  // Example: clearing encoders, setting servo positions, ...
  Intake.setStopping(brakeType::coast);
  liftLeft.setStopping(brakeType::hold);
  liftRight.setStopping(brakeType::hold);
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              Autonomous Task                              */
/*                                                                           */
/*  This task is used to control your robot during the autonomous phase of   */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

void autonomous(void) {
  // ..........................................................................
  // Insert autonomous user code here.
  // ..........................................................................
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              User Control Task                            */
/*                                                                           */
/*  This task is used to control your robot during the user control phase of */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

void usercontrol(void) {
  while (true) {

    // Intake
    if (Controller1.ButtonR1.pressing()) {
      Intake.spin(directionType::fwd, INTAKE_SPEED,
                  velocityUnits::pct);
    } else if (Controller1.ButtonR2.pressing()) {
      Intake.spin(directionType::rev, INTAKE_SPEED,
                  velocityUnits::pct);
    } else {
      Intake.stop();
    }

    // Two-motor cascade lift
    if (Controller1.ButtonL1.pressing()) {
      Lift.spin(directionType::fwd, LIFT_SPEED,
                velocityUnits::pct);
    } else if (Controller1.ButtonL2.pressing()) {
      Lift.spin(directionType::rev, LIFT_SPEED,
                velocityUnits::pct);
    } else {
      Lift.stop(brakeType::hold);
    }

    wait(20, msec);
  }
}

//
// Main will set up the competition functions and callbacks.
//
int main() {
  // Set up callbacks for autonomous and driver control periods.
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  // Run the pre-autonomous function.
  pre_auton();

  // Prevent main from exiting with an infinite loop.
  while (true) {
    wait(100, msec);
  }
}
