# main function of this file is to create a node and publish the initial pos of the robot 
# amcl subscribe to this topic

import time
import rclpy
from rclpy.node import Node
# import transforms3d
from math import sin, cos
from geometry_msgs.msg import PoseWithCovarianceStamped

class InitAmclPosePublisher(Node):
    def __init__(self):
        super().__init__("init_amcl_pose_publisher")

        self.declare_parameter("x", value=0.0)
        self.declare_parameter("y", value=0.0)
        self.declare_parameter("theta", value=0.0)
        self.declare_parameter("cov", value=0.5**2) #covariance value
        
        self.publisher = self.create_publisher(
            PoseWithCovarianceStamped, 
            '/initialpose',
            10,
        )

        # this function stops the node from publishing until the AMCL subscribe to the topic
        self.sent = False
        self.timer = self.create_timer(4.0, self.send_once)

    def send_once(self):
        if self.sent:
            return

        self.get_logger().info("Publishing initial pose...")
        self.send_init_pose()
        self.sent = True
    
    def send_init_pose(self):
        x = self.get_parameter("x").value
        y = self.get_parameter("y").value
        theta = self.get_parameter("theta").value
        cov = self.get_parameter("cov").value

        msg = PoseWithCovarianceStamped()
        msg.header.frame_id = "map"
        msg.header.stamp = self.get_clock().now().to_msg()
        msg.pose.pose.position.x = x
        msg.pose.pose.position.y = y

        # quat = transforms3d.euler.euler2quat(0,0,theta) #converting from euler to quaterion
        qx = 0.0
        qy = 0.0
        qz = sin(theta / 2.0)
        qw = cos(theta / 2.0)
        msg.pose.pose.orientation.x = qx
        msg.pose.pose.orientation.y = qy
        msg.pose.pose.orientation.z = qz
        msg.pose.pose.orientation.w = qw

        # msg.pose.pose.orientation.w = quat[0]
        # msg.pose.pose.orientation.x = quat[1]
        # msg.pose.pose.orientation.y = quat[2]
        # msg.pose.pose.orientation.z = quat[3]

        msg.pose.covariance = [
        cov, 0.0, 0.0, 0.0, 0.0, 0.0,  # Pos X
        0.0, cov, 0.0, 0.0, 0.0, 0.0,  # Pos Y
        0.0, 0.0, 0.0, 0.0, 0.0, 0.0,  # Pos Z
        0.0, 0.0, 0.0, 0.0, 0.0, 0.0,  # Rot X
        0.0, 0.0, 0.0, 0.0, 0.0, 0.0,  # Rot Y
        0.0, 0.0, 0.0, 0.0, 0.0, cov   # Rot Z
        ]

        self.publisher.publish(msg)

def main(arg=None):
    rclpy.init()                                        #initiated the ros-python bridge

    node = InitAmclPosePublisher()     #created an object

    rclpy.spin(node)                   #spinning it

    node.destroy_node()
    rclpy.shutdown()