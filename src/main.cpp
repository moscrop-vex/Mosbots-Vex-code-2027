/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       errortoast                                                */
/*    Created:      10/2/2026, 2:26:42 PM                                     */
/*    Description:  V5 project - Robot Henry                                   */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#include <vex.h>

using namespace vex;

// vex system devices
brain Brain;
controller Controller1 = controller(primary);

// competition instance
competition Competition;

// PIN DEFINITIONS
int leftSideMotorPort1 = 1;   // Left side motor 1
int rightSideMotorPort1 = 2;  // Right side motor 1
int leftSideMotorPort2 = 9;   // Left side motor 2
int rightSideMotorPort2 = 10; // Right side motor 2
int rightLiftPin = 6;         // Right side lift
int leftLiftPin = 7;          // Left side lift
int clawUpDownPin = 3;        // Claw up/down
int intakePin = 8;            // Intake motor

// Motor definitions
motor DriveLeft1(leftSideMotorPort1, false);
motor DriveRight1(rightSideMotorPort1, true);
motor DriveLeft2(leftSideMotorPort2, false);
motor DriveRight2(rightSideMotorPort2, true);

motor RightLift(rightLiftPin);
motor LeftLift(leftLiftPin);

// Claw motor on port 3 (replacing three_wire)
motor ClawMotor(clawUpDownPin);

// Motor speed constants
int driveSpeed = 75;
int liftSpeed = 60;
int intakeSpeed = 80;
int clawSpeed = 40;

// Claw state
bool clawOpen = false;
bool clawPressed = false;

/*---------------------------------------------------------------------------*/
/*                          Pre-Autonomous Functions                         */
/*---------------------------------------------------------------------------*/

void pre_auton(void) {
  DriveLeft1.setStopping(brakeType::brake);
  DriveRight1.setStopping(brakeType::brake);
  DriveLeft2.setStopping(brakeType::brake);
  DriveRight2.setStopping(brakeType::brake);
  
  LeftLift.setStopping(vex::brakeType::brake);
  RightLift.setStopping(vex::brakeType::brake);
  
  ClawMotor.setStopping(brakeType::brake);
}

/*---------------------------------------------------------------------------*/
/*                              Autonomous Task                              */
/*---------------------------------------------------------------------------*/

void autonomous(void) {
}

/*---------------------------------------------------------------------------*/
/*                              User Control Task                            */
/*---------------------------------------------------------------------------*/

void usercontrol(void) {
  while (true) {
    // Drive control - right stick up/down for forward/back, left stick left/right to turn
    int driveY = Controller1.Axis3.position();
    int turnX = Controller1.Axis2.position();
    
    double leftSpeed = driveY + turnX;
    double rightSpeed = driveY - turnX;
    
    DriveLeft1.spin(directionType::fwd, leftSpeed * 0.85, velocityUnits::pct);
    DriveLeft2.spin(directionType::fwd, leftSpeed * 0.85, velocityUnits::pct);
    DriveRight1.spin(directionType::fwd, rightSpeed * 0.85, velocityUnits::pct);
    DriveRight2.spin(directionType::fwd, rightSpeed * 0.85, velocityUnits::pct);
    
    // Lift control - R1 up, R2 down
    if (Controller1.ButtonR1.pressing()) {
      LeftLift.spin(directionType::fwd, liftSpeed, velocityUnits::pct);
      RightLift.spin(directionType::fwd, liftSpeed, velocityUnits::pct);
    } else if (Controller1.ButtonR2.pressing()) {
      LeftLift.spin(directionType::rev, liftSpeed, velocityUnits::pct);
      RightLift.spin(directionType::rev, liftSpeed, velocityUnits::pct);
    } else {
      LeftLift.stop();
      RightLift.stop();
    }
    
    // Intake control - L1 intake in, L2 intake out
    if (Controller1.ButtonL1.pressing()) {
      DriveLeft1.spin(directionType::fwd, intakeSpeed, velocityUnits::pct);
      DriveRight1.spin(directionType::rev, intakeSpeed, velocityUnits::pct);
    } else if (Controller1.ButtonL2.pressing()) {
      DriveLeft1.spin(directionType::rev, intakeSpeed, velocityUnits::pct);
      DriveRight1.spin(directionType::fwd, intakeSpeed, velocityUnits::pct);
    } else {
      DriveLeft1.stop();
      DriveRight1.stop();
    }
    
    // Side lift control - A left side up, B right side up, X left down, Y right down
    if (Controller1.ButtonA.pressing()) {
      LeftLift.spin(directionType::fwd, liftSpeed, velocityUnits::pct);
    } else if (Controller1.ButtonX.pressing()) {
      LeftLift.spin(directionType::rev, liftSpeed, velocityUnits::pct);
    } else {
      LeftLift.stop();
    }
    
    if (Controller1.ButtonB.pressing()) {
      RightLift.spin(directionType::fwd, liftSpeed, velocityUnits::pct);
    } else if (Controller1.ButtonY.pressing()) {
      RightLift.spin(directionType::rev, liftSpeed, velocityUnits::pct);
    } else {
      RightLift.stop();
    }
    
    // Claw control - left stick up/down for claw, left bumper toggle open/close
    if (Controller1.Axis2.position() < -20) {
      ClawMotor.spin(directionType::fwd, clawSpeed, velocityUnits::pct);
    } else if (Controller1.Axis2.position() > 20) {
      ClawMotor.spin(directionType::rev, clawSpeed, velocityUnits::pct);
    } else if (Controller1.ButtonL1.pressing() && !clawPressed) {
      clawOpen = !clawOpen;
      clawPressed = true;
      wait(300, msec);
      ClawMotor.spin(directionType::fwd, 50, velocityUnits::pct);
      wait(300, msec);
      ClawMotor.stop();
    } else if (!Controller1.ButtonL1.pressing()) {
      clawPressed = false;
    } else {
      ClawMotor.stop();
    }
    
    wait(20, msec);
  }
}

int main() {
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);
  pre_auton();
  
  while (true) {
    wait(100, msec);
  }
}
