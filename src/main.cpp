/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       errortoast                                                */
/*    Created:      10/2/2026, 2:26:42 PM                                     */
/*    Description:  V5 project - Mosbots 2027 Robot - Henry                   */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#include <vex.h>

using namespace vex;

// vex system devices
brain Brain;
controller Controller1 = controller(primary);

// competition instance
competition Competition;

// Motor definitions (vex::PORTn is zero-indexed; use the enum, never raw ints)
// Drive
motor DriveLeft1(vex::PORT1, vex::ratio18_1, false);   // left back
motor DriveLeft2(vex::PORT9, vex::ratio18_1, false);   // left front
motor DriveRight1(vex::PORT2, vex::ratio18_1, true);   // right back
motor DriveRight2(vex::PORT10, vex::ratio18_1, true);  // right front

// Lift
motor RightLift(vex::PORT6, vex::ratio18_1, false);    // right lift
motor LeftLift(vex::PORT7, vex::ratio18_1, false);     // left lift

// Intake + claw up/down
motor IntakeMotor(vex::PORT8, vex::ratio6_1, false);       // intake (blue 600rpm)
motor ClawUpDownMotor(vex::PORT3, vex::ratio36_1, false);  // claw up/down (red 100rpm)

// Claw open/close servo on three wire port H
servo ClawServo(Brain.ThreeWirePort.H);

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
  
  IntakeMotor.setStopping(brakeType::coast);
  ClawUpDownMotor.setStopping(brakeType::hold);
  
  ClawServo.setPosition(clawOpen ? 100 : 0, percentUnits::pct);
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
    int driveY = Controller1.Axis2.position();
    int turnX = Controller1.Axis4.position();
    
    double leftSpeed = driveY + turnX;
    double rightSpeed = driveY - turnX;
    
    DriveLeft1.spin(directionType::fwd, leftSpeed * 0.85, velocityUnits::pct);
    DriveLeft2.spin(directionType::fwd, leftSpeed * 0.85, velocityUnits::pct);
    DriveRight1.spin(directionType::fwd, rightSpeed * 0.85, velocityUnits::pct);
    DriveRight2.spin(directionType::fwd, rightSpeed * 0.85, velocityUnits::pct);
    
    // Lift control - R1 raises both, R2 lowers both, A/B/X/Y trim each side
    bool leftUp = Controller1.ButtonR1.pressing() || Controller1.ButtonA.pressing();
    bool leftDown = Controller1.ButtonR2.pressing() || Controller1.ButtonX.pressing();
    bool rightUp = Controller1.ButtonR1.pressing() || Controller1.ButtonB.pressing();
    bool rightDown = Controller1.ButtonR2.pressing() || Controller1.ButtonY.pressing();
    
    if (leftUp) {
      LeftLift.spin(directionType::fwd, liftSpeed, velocityUnits::pct);
    } else if (leftDown) {
      LeftLift.spin(directionType::rev, liftSpeed, velocityUnits::pct);
    } else {
      LeftLift.stop();
    }
    
    if (rightUp) {
      RightLift.spin(directionType::fwd, liftSpeed, velocityUnits::pct);
    } else if (rightDown) {
      RightLift.spin(directionType::rev, liftSpeed, velocityUnits::pct);
    } else {
      RightLift.stop();
    }
    
    // Intake control - L1 intake in, L2 intake out
    if (Controller1.ButtonL1.pressing()) {
      IntakeMotor.spin(directionType::fwd, intakeSpeed, velocityUnits::pct);
    } else if (Controller1.ButtonL2.pressing()) {
      IntakeMotor.spin(directionType::rev, intakeSpeed, velocityUnits::pct);
    } else {
      IntakeMotor.stop();
    }
    
    // Claw up/down - up arrow raises, down arrow lowers
    if (Controller1.ButtonUp.pressing()) {
      ClawUpDownMotor.spin(directionType::fwd, clawSpeed, velocityUnits::pct);
    } else if (Controller1.ButtonDown.pressing()) {
      ClawUpDownMotor.spin(directionType::rev, clawSpeed, velocityUnits::pct);
    } else {
      // Actively hold position whenever the claw stops moving
      ClawUpDownMotor.stop(brakeType::hold);
    }
    
    // Claw open/close - left arrow toggles
    if (Controller1.ButtonLeft.pressing() && !clawPressed) {
      clawPressed = true;
      clawOpen = !clawOpen;
      ClawServo.setPosition(clawOpen ? 100 : 0, percentUnits::pct);
    } else if (!Controller1.ButtonLeft.pressing()) {
      clawPressed = false;
    }
    
    wait(20, msec);
  }
}

int main() {
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);
  pre_auton();
  
  Brain.Screen.clearScreen();
  
  while (true) {
    wait(100, msec);
  }
}
