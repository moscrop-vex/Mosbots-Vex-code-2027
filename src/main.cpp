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

// Claw open/close pnumatik on three wire port A
pneumatics ClawPiston(Brain.ThreeWirePort.A);

constexpr double kDriveDeadband = 8.0;
constexpr int kDriveMaxPct = 100;
constexpr int driveSpeed = 75;
constexpr int slowDriveSpeed = 45;
constexpr int liftSpeed = 60;
constexpr int intakeSpeed = 75;
constexpr int clawSpeed = 45;

inline double clampPct(double value) {
  if (value > kDriveMaxPct) {
    return kDriveMaxPct;
  }
  if (value < -kDriveMaxPct) {
    return -kDriveMaxPct;
  }
  return value;
}

inline double applyDeadband(int input) {
  if (input > -kDriveDeadband && input < kDriveDeadband) {
    return 0.0;
  }
  return static_cast<double>(input);
}

inline void driveTank(int forward, int turn, bool slowMode) {
  const double scale = slowMode ? (slowDriveSpeed / 100.0) : (driveSpeed / 100.0);
  double leftSpeed = clampPct((applyDeadband(forward) + applyDeadband(turn)) * scale);
  double rightSpeed = clampPct((applyDeadband(forward) - applyDeadband(turn)) * scale);

  DriveLeft1.spin(directionType::fwd, leftSpeed, velocityUnits::pct);
  DriveLeft2.spin(directionType::fwd, leftSpeed, velocityUnits::pct);
  DriveRight1.spin(directionType::fwd, rightSpeed, velocityUnits::pct);
  DriveRight2.spin(directionType::fwd, rightSpeed, velocityUnits::pct);
}

inline void setLiftMotor(motor &motorRef, bool up, bool down, int speed) {
  if (up) {
    motorRef.spin(directionType::fwd, speed, velocityUnits::pct);
  } else if (down) {
    motorRef.spin(directionType::rev, speed, velocityUnits::pct);
  } else {
    motorRef.stop();
  }
}

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
  ClawPiston.close();
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
    // Use the right stick for forward/back and the left stick for turning.
    int driveY = Controller1.Axis3.position();
    int turnX = Controller1.Axis1.position();
    bool slowMode = Controller1.ButtonX.pressing();

    driveTank(driveY, turnX, slowMode);

    bool leftUp = Controller1.ButtonR1.pressing() || Controller1.ButtonA.pressing();
    bool leftDown = Controller1.ButtonR2.pressing();
    bool rightUp = Controller1.ButtonR1.pressing() || Controller1.ButtonB.pressing();
    bool rightDown = Controller1.ButtonR2.pressing() || Controller1.ButtonY.pressing();

    setLiftMotor(LeftLift, leftUp, leftDown, liftSpeed);
    setLiftMotor(RightLift, rightUp, rightDown, liftSpeed);

    if (Controller1.ButtonL1.pressing()) {
      IntakeMotor.spin(directionType::fwd, intakeSpeed, velocityUnits::pct);
    } else if (Controller1.ButtonL2.pressing()) {
      IntakeMotor.spin(directionType::rev, intakeSpeed, velocityUnits::pct);
    } else {
      IntakeMotor.stop();
    }

    if (Controller1.ButtonUp.pressing()) {
      ClawUpDownMotor.spin(directionType::fwd, clawSpeed, velocityUnits::pct);
    } else if (Controller1.ButtonDown.pressing()) {
      ClawUpDownMotor.spin(directionType::rev, clawSpeed, velocityUnits::pct);
    } else {
      ClawUpDownMotor.stop(brakeType::hold);
    }

    if (Controller1.ButtonRight.pressing()) {
      clawOpen = true;
      ClawPiston.open();
      clawPressed = false;
    } else if (Controller1.ButtonLeft.pressing() && !clawPressed) {
      clawPressed = true;
      clawOpen = !clawOpen;
      if (clawOpen) {
        ClawPiston.open();
      } else {
        ClawPiston.close();
      }
    } else if (!Controller1.ButtonLeft.pressing() && !Controller1.ButtonRight.pressing()) {
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
