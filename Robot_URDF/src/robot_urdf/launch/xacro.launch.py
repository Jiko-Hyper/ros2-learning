#可以理解成：“我要创建一份启动清单。”
from launch import LaunchDescription
#ROS2 节点启动对象
from launch_ros.actions import Node
#帮我们找到 ROS2 package 在哪里。
#get_package_share_directory('my_robot_description')
# ROS2 会帮你找到~/robot_ws/install/my_robot_description/share/my_robot_description
from ament_index_python.packages import get_package_share_directory
#它拼文件路径
import os

from launch.substitutions import Command

from launch_ros.parameter_descriptions import ParameterValue

#ROS2 launch 的固定入口
def generate_launch_description():

    # 准备东西
    package_path = get_package_share_directory('robot_urdf')
    xacro_path = os.path.join(package_path, 'urdf', 'robot.urdf.xacro')
    # package_path
    # │
    # └── urdf
    #      │
    #      └── robot.urdf

    # with open(urdf_path, 'r') as file:
    #     robot_description = file.read() #打开 robot.urdf, 把里面所有文字读出来

    robot_description = ParameterValue(           #       Command
            Command(['xacro ',xacro_path]),       #          ↓
            value_type=str                        #       执行xacro
    )
                                                  #    ParameterValue
                                                  #         ↓
                                                  #把结果作为字符串参数交给节点
            
    
    

    # 创建节点

    # 启动 ROS2 提供的 robot_state_publisher 节点
    robot_state_publisher_node = Node(
        package='robot_state_publisher',           #功能包
        executable='robot_state_publisher',        #真正运行的程序
        parameters=[
            {'robot_description':robot_description}
        ]                                          #给 robot_state_publisher 一个参数
                                                   #参数名字:robot_description
                                                   #参数内容:URDF全文
    )  

    #告诉 robot_state_publisher 当前关节角度
    joint_state_publisher_node = Node(
        package='joint_state_publisher',
        executable='joint_state_publisher'
    )

    rviz_node = Node(
        package='rviz2',
        executable='rviz2'
    )


    return LaunchDescription([
         # 要启动的东西
         robot_state_publisher_node,
         rviz_node,
         joint_state_publisher_node
    ])