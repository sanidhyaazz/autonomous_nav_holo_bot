#include <micro_ros_platformio.h>
#include <Arduino.h>
#include <rcl/rcl.h>
#include <rcl/error_handling.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>
#include <geometry_msgs/msg/twist.h>

// ---------------- Pins ----------------
#define LED_PIN 13

#define RPWM1 23   // W1 Front-Left
#define LPWM1 22 

#define RPWM2 18   // W2 Front-Right
#define LPWM2 19 

#define RPWM3 4    // W3 Back-Left
#define LPWM3 2  

#define RPWM4 21    // W4 Back-Right
#define LPWM4 5 

// ---------------- micro-ROS ----------------
rcl_subscription_t twist_subscriber;
geometry_msgs__msg__Twist twist_msg;   
rclc_executor_t executor;
rclc_support_t support;
rcl_allocator_t allocator;
rcl_node_t node;

#define MAX_PWM 255
float R = 0.15;   // wheel -> center distance (meters)

// ---------------- Error ----------------
#define RCCHECK(fn) { rcl_ret_t rc = fn; if(rc != RCL_RET_OK){error_loop();}}
#define RCSOFTCHECK(fn) { rcl_ret_t rc = fn; }

void error_loop(){
  while(1){
    digitalWrite(LED_PIN, !digitalRead(LED_PIN));
    delay(100);
  }
}

// ---------------- Motor Driver ----------------
void bts_driver(int rpwm, int lpwm, int pwm) {
  pwm = constrain(pwm, -MAX_PWM, MAX_PWM);

  if (pwm > 0) {
    analogWrite(rpwm, pwm);
    analogWrite(lpwm, 0);
  } 
  else if (pwm < 0) {
    analogWrite(rpwm, 0);
    analogWrite(lpwm, abs(pwm));
  } 
  else {
    analogWrite(rpwm, 0);
    analogWrite(lpwm, 0);
  }
}

// ---------------- Omni IK Callback ----------------
void cmd_vel_callback(const void * msgin) {
  const geometry_msgs__msg__Twist * msg = (const geometry_msgs__msg__Twist *)msgin;

  float Vf = msg->linear.x;     // Forward/Backward
  float Vs = msg->linear.y;     // Side-to-side (Strafe)
  float w  = msg->angular.z;    // Rotation

  // Standard Mecanum IK
  float v1 = Vf - Vs - (R * w);   // Front Left
  float v2 = Vf + Vs + (R * w);   // Front Right
  float v3 = Vf + Vs - (R * w);   // Back Left
  float v4 = Vf - Vs + (R * w);   // Back Right

  // Find max value to maintain ratio if we exceed PWM limits
  float maxv = abs(v1);
  if (abs(v2) > maxv) maxv = abs(v2);
  if (abs(v3) > maxv) maxv = abs(v3);
  if (abs(v4) > maxv) maxv = abs(v4);

  // Map to PWM (0-255)
  // Only scale down if the calculated value exceeds a threshold, 
  // otherwise, use a multiplier that fits your motor's max speed.
  float scale = (maxv > 1.0) ? (MAX_PWM / maxv) : MAX_PWM;

  int pwm1 = v1 * scale;
  int pwm2 = v2 * scale;
  int pwm3 = v3 * scale;
  int pwm4 = v4 * scale;

  // Drive motors
  bts_driver(RPWM1, LPWM1, pwm1);
  bts_driver(RPWM2, LPWM2, pwm2);
  bts_driver(RPWM3, LPWM3, pwm3);
  bts_driver(RPWM4, LPWM4, pwm4);
}

// ---------------- Setup ----------------
void setup() {
  Serial.begin(115200);

  pinMode(RPWM1, OUTPUT); pinMode(LPWM1, OUTPUT);
  pinMode(RPWM2, OUTPUT); pinMode(LPWM2, OUTPUT);
  pinMode(RPWM3, OUTPUT); pinMode(LPWM3, OUTPUT);
  pinMode(RPWM4, OUTPUT); pinMode(LPWM4, OUTPUT);
  pinMode(LED_PIN, OUTPUT);

  set_microros_serial_transports(Serial);
  delay(2000);

  allocator = rcl_get_default_allocator();
  RCCHECK(rclc_support_init(&support, 0, NULL, &allocator));
  RCCHECK(rclc_node_init_default(&node, "omni_cmd_vel_node", "", &support));

  RCCHECK(rclc_subscription_init_default(
    &twist_subscriber,
    &node,
    ROSIDL_GET_MSG_TYPE_SUPPORT(geometry_msgs, msg, Twist),
    "cmd_vel"));

  RCCHECK(rclc_executor_init(&executor, &support.context, 1, &allocator));
  RCCHECK(rclc_executor_add_subscription(&executor,
                                         &twist_subscriber,
                                         &twist_msg,
                                         &cmd_vel_callback,
                                         ON_NEW_DATA));
}

// ---------------- Loop ----------------
void loop() {
  RCSOFTCHECK(rclc_executor_spin_some(&executor, RCL_MS_TO_NS(10)));
}