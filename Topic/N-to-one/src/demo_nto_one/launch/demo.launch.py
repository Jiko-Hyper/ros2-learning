from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
    return LaunchDescription([
        Node(
            package='demo_nto_one',
            executable='demo_temperature_publisher',
            name='demo_temperature_publisher',
            output='screen',
            emulate_tty=True,
            parameters=[{'publish_frequency': 2.0}]
        ),
        Node(
            package='demo_nto_one',
            executable='demo_humidity_publisher',
            name='demo_humidity_publisher',
            output='screen',
            emulate_tty=True,
            parameters=[{'publish_frequency': 2 / 3.0}]
        ),
        Node(
            package='demo_nto_one',
            executable='demo_smart_subscriber',
            name='demo_smart_subscriber',
            output='screen',
            emulate_tty=True
        )
    ])