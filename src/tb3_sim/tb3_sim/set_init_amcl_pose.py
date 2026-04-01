# main function of this file is to create a node and publish the initial pos of the robot 
# amcl subscribe to this topic

import time
import rclpy
from rclpy.node import Node
import transforms3d
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
        while(self.publisher.get_subscription_count() == 0):
            self.get_logger().info("waiting for AMCL Intial Pose Subscriber")
            time.sleep(1.0)
    
    def send_init_pose(self):
        x= self.get_parameter
def main (arg=None):
    rclpy.init()                                        #initiated the ros-python bridge
    InitAmclPosePublisher = InitAmclPosePublisher()     #created an object
    rclpy.spin(InitAmclPosePublisher)                   #spinning it
    InitAmclPosePublisher.destroy_node()
    rclpy.shutdown()