from launch import LaunchDescription
from launch_ros.actions import Node
from launch.actions import ExecuteProcess, TimerAction
from ament_index_python.packages import get_package_share_directory
import os
import xacro


def generate_launch_description():

    pkg_share = get_package_share_directory('bot_description')

    xacro_file = os.path.join(pkg_share, 'urdf', 'omnidrive.xacro')
    robot_desc = xacro.process_file(xacro_file).toxml()

    return LaunchDescription([

        # Start Gazebo properly
        ExecuteProcess(
            cmd=['gz', 'sim', '-r'],
            output='screen'
        ),

        # Robot state publisher
        Node(
            package='robot_state_publisher',
            executable='robot_state_publisher',
            parameters=[{'robot_description': robot_desc}]
        ),

        # Spawn robot
        Node(
            package='ros_gz_sim',
            executable='create',
            arguments=[
                '-topic', 'robot_description',
                '-name', 'omnidrive'
            ],
            output='screen'
        ),

        # Delay controller loading (VERY IMPORTANT)
        TimerAction(
            period=5.0,
            actions=[
                Node(
                    package="controller_manager",
                    executable="spawner",
                    arguments=["joint_state_broadcaster"],
                    output="screen"
                ),

                Node(
                    package="controller_manager",
                    executable="spawner",
                    arguments=["mecanum_drive_controller"],
                    output="screen"
                ),
            ]
        ),
    ])
