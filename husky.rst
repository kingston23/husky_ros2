Husky Mobile Robot with DiffDriveController in Gazebo Simulator in ROS2 
========================================================================


**Goal:** Launch a Pan-Tilt Camera in Gazebo, learn how to controll two joints — pan (yaw) and tilt (pitch), and visualize image from camera in Rviz

**Tutorial level:** Basics

This project demonstrates how to build and simulate a Pan-Tilt Camera system using ROS2 and Gazebo. The system features two controllable joints — pan (yaw) and tilt (pitch) — with a virtual RGB camera mounted on the pan-tilt system. Users can control the camera orientation by publishing joint commands and visualize the camera stream in real-time.

The pan-tilt camera setup is commonly used in real-world applications such as surveillance, robotic inspection, teleoperation, and vision-based tracking. This tutorial provides a scalable foundation that can be easily extended with advanced perception or control algorithms.

By the end of this tutorial, you will be able to understand the basics of a pan-tilt camera system and how to make controllers to move the camera right/left and up/down directions. The main ROS2 concepts and skills covered in this tutorial are:

``URDF Modeling (Unified Robot Description Format)``
URDF is an XML format used to describe the physical configuration of robots, including links, joints, sensors, and their relationships. In this tutorial, you'll learn to define a pan-tilt camera system's geometry and kinematics using URDF, enabling accurate simulation and visualization in Rviz.

``Controller Configuration (ros2_control Framework)``
The ros2_control framework provides a modular way to implement, configure, and manage robot controllers in both simulation and real hardware. You'll learn how to set up joint controllers, configure them via YAML, and manage them using the Controller Manager, which handles loading, starting, and stopping controllers.

``Gazebo Plugins``
Gazebo plugins extend the simulation environment by adding sensors, actuators, and interfaces to ROS2. You'll use plugins such as gazebo_ros_camera to simulate an RGB camera and interface its data with ROS2 topics, enabling real-time visualization and interaction

``ROS2 Topics``
Topics are the primary communication mechanism in ROS2, allowing nodes to publish and subscribe to streams of data such as joint states and camera images. You'll learn to interact with these topics to command joint positions and visualize sensor data.

.. contents:: Contents
   :depth: 2
   :local:


.. note::

   These instructions are about the current `Gazebo <https://gazebosim.org/>`__ (previously known as Ignition), not  `Gazebo Classic <https://classic.gazebosim.org/>`__.
 

How the Camera Works
--------------------

In this tutorial RealSense camera is modeled as a set of URDF/Xacro links and joints. It is attached to the robot via fixed joints and includes a Gazebo camera sensor plugin. This setup allows the camera to generate and publish simulated image data (RGB, and optionally depth/pointcloud) during simulation, making it available to ROS nodes for visualization or processing.



System Requirements
-------------------

OS: Ubuntu 24.04

ROS2:  `Jazzy <https://docs.ros.org/en/jazzy/Installation.html>`__

Gazebo: Harmonic

Python 3.10+ (if you're adding custom control nodes)


Dependencies & Packages
-----------------------

You should install ROS2 packages: 

.. code-block:: python

   sudo apt install \
      ros-jazzy-xacro \
      ros-jazzy-controller-interface \
      ros-jazzy-controller-manager \
      ros-jazzy-ros2-control \
      ros-jazzy-ros2-controllers \
      ros-jazzy-gz-ros2-control \
      ros-jazzy-joint-state-publisher \
      ros-jazzy-joint-state-publisher-gui \
      ros-jazzy-joint-state-broadcaster \
      ros-jazzy-hardware-interface \
      ros-jazzy-joint-trajectory-controller \
      ros-jazzy-diff-drive-controller \
      ros-jazzy-robot-localization \
      ros-jazzy-interactive-marker-twist-server \
      ros-jazzy-twist-mux \
      ros-jazzy-gazebo-ros-pkgs \
      ros-jazzy-ros-gz-sim \
      ros-jazzy-ros-gz-bridge \
      ros-jazzy-tf-transformations

Short descriptions for each custom package in Husky ROS2 project:

``husky_description``
Contains the URDF files and 3D meshes that define the physical structure, joints, and visual appearance of the pan-tilt camera system for simulation and visualization.

``husky_bringup``
Provides launch files that automate the startup of the robot description, controllers, and Gazebo simulation environment, making it easy to bring up the complete system with a single command.


``husky_control``
Implements example ROS2 nodes or scripts for sending joint commands and demonstrating custom control logic for the pan and tilt movements of the camera system.

``husky_gazebo``
Implements example ROS2 nodes or scripts for sending joint commands and demonstrating custom control logic for the pan and tilt movements of the camera system.

``husky_viz``
Implements example ROS2 nodes or scripts for sending joint commands and demonstrating custom control logic for the pan and tilt movements of the camera system.


.. code-block:: console

   $ git clone pan_tilt_camera_description/
   $ git clone pan_tilt_camera_bringup/
   $ git clone pan_tilt_camera_control/
   $ git clone pan_tilt_viz


Make the Workspace and Sourcing the Setup Script
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

To build your ROS2 workspace and set up your environment, follow these steps:

1. Create the workspace and src directory:

.. code-block:: console

    $ mkdir -p ~/husky_ws/src
    $ cd ~/husky_ws/src

2. Clone your packages into src, not the workspace root:

.. code-block:: console

    $ git clone <your_repo_url>/pan_tilt_camera_description.git
    $ git clone <your_repo_url>/pan_tilt_camera_bringup.git
    $ git clone <your_repo_url>/pan_tilt_camera_control.git

3. Go back to the workspace root, build the workspace and source the setup script:

.. code-block:: console

    $ cd ..
    $ colcon build
    $ source install/setup.bash

Run source the setup script in every new terminal before using your workspace.

.. note::

   If you’re new to ROS2 workspaces or want a detailed step-by-step guide, see the official ROS2 tutorial: `Creating a workspace (ROS2 Documentation) <https://docs.ros.org/en/jazzy/Tutorials/Beginner-Client-Libraries/Creating-A-Workspace/Creating-A-Workspace.html>`__.

Husky Description (URDF Modeling)
---------------------------------


**URDF Example:**



Enabling Control with ros2_control
----------------------------------

The joint_state_broadcaster is a special controller that publishes the current state of all robot joints (position, velocity, effort) as ROS messages (specifically, the joint_states topic). It reads the state interfaces from the hardware (via ros2_control) and broadcasts them, making the information available for visualization, logging, and other nodes in the system

To control Husky mobile robot in simulation (Gazebo), we use the `ros2_control` framework. This enables real-time joint control using controllers defined in both the URDF and external configuration files.

Launch file 
^^^^^^^^^^^^

This code is a ROS 2 launch file written in Python, designed to launch an older Husky robot model in Gazebo Sim using ROS 2

**Launch Example:**
These are the basic ROS 2 launch tools used to define launch arguments, include other launch files, set paths and environment variables, and launch nodes:

.. code-block:: python

   from launch import LaunchDescription
   from launch.actions import DeclareLaunchArgument, IncludeLaunchDescription
   from launch.launch_description_sources import PythonLaunchDescriptionSource
   from launch.substitutions import EnvironmentVariable, LaunchConfiguration, PathJoinSubstitution
   from launch_ros.actions import Node
   from launch_ros.substitutions import FindPackageShare

These allow you to customize your launch:
   1. rviz: whether to launch RViz (true/false)
   2. world: choose the Gazebo world to launch (warehouse by default)
   3. use_sim_time: toggle ROS simulation time usage (default true)

.. code-block:: python

   ARGUMENTS = [
      DeclareLaunchArgument('rviz', default_value='false', choices=['true', 'false'], description='Start rviz.'),
      DeclareLaunchArgument('world', default_value='warehouse', description='Gazebo World'),
      DeclareLaunchArgument('use_sim_time', default_value='true', choices=['true', 'false'], description='use_sim_time')]




Fetches the installed path of the husky_gazebo package:

.. code-block:: python

   pkg_clearpath_gz = get_package_share_directory('husky_gazebo')


Resolves paths to two important launch files:
   1. gz_sim.launch.py: likely launches Gazebo with simulation settings.
   2. gazebo.launch.py: likely spawns the Husky robot.

.. code-block:: python

   gz_sim_launch = PathJoinSubstitution([pkg_clearpath_gz, 'launch', 'gz_sim.launch.py'])
   gazebo_launch = PathJoinSubstitution([FindPackageShare("husky_gazebo"), "launch", "gazebo.launch.py"])


Launches the Gazebo simulation (with optional world, rviz, and use_sim_time settings):

.. code-block:: python

   gz_sim = IncludeLaunchDescription(
      PythonLaunchDescriptionSource([gz_sim_launch]),
      launch_arguments=[
         ('world', LaunchConfiguration('world')),
         ('rviz', LaunchConfiguration('rviz')),
         ('use_sim_time', LaunchConfiguration('use_sim_time')),
      ]
   )

Spawns the Husky robot into the Gazebo world:

.. code-block:: python

   robot_spawn = IncludeLaunchDescription(
      PythonLaunchDescriptionSource([gazebo_launch]),
      launch_arguments=[
         ('world', LaunchConfiguration('world')),
         ('rviz', LaunchConfiguration('rviz')),
         ('use_sim_time', LaunchConfiguration('use_sim_time')),
      ]
   )

Launches a ROS <-> Gazebo bridge node to forward /tf messages between the two ecosystems using ros_gz_bridge:

.. code-block:: python

   bridge = Node(
      executable='parameter_bridge',
      package='ros_gz_bridge',
      arguments=[
         "/tf@tf2_msgs/msg/TFMessage@gz.msgs.Pose_V",               
      ],
      output="screen",
   )

This constructs the complete launch description and returns it to ROS2:

.. code-block:: python

   ld = LaunchDescription(ARGUMENTS)
   ld.add_action(gz_sim)
   ld.add_action(bridge)
   ld.add_action(robot_spawn)
   return ld

   

Controller Configuration (YAML)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

The actual controllers are defined in a `control.yaml` file located in the `husky_control/config/` directory. We use one `DiffDriveController`.

**control.yaml:**

.. code-block:: yaml

   controller_manager:
   ros__parameters:
      update_rate: 50 # Hz
      use_sim_time: False

      joint_state_broadcaster:
         type: joint_state_broadcaster/JointStateBroadcaster

      husky_velocity_controller:
         type: diff_drive_controller/DiffDriveController


   husky_velocity_controller:
   ros__parameters:
      use_sim_time: False
      left_wheel_names: ["front_left_wheel_joint", "rear_left_wheel_joint"]
      right_wheel_names: ["front_right_wheel_joint", "rear_right_wheel_joint"]

      wheel_separation: 0.512 #0.1  # 0.256  # 0.512
      wheels_per_side: 1 # actually 2, but both are controlled by 1 signal
      wheel_radius: 0.1651 # 0.015

      wheel_separation_multiplier: 1.875 # default: 1.0
      left_wheel_radius_multiplier: 1.0
      right_wheel_radius_multiplier: 1.0

      publish_rate: 50.0
      odom_frame_id: odom
      base_frame_id: base_link
      pose_covariance_diagonal: [0.001, 0.001, 0.001, 0.001, 0.001, 0.03]
      twist_covariance_diagonal: [0.001, 0.001, 0.001, 0.001, 0.001, 0.03]

      open_loop: false
      # Odometry fused with IMU is published by robot_localization, so
      # no need to publish a TF based on encoders alone.
      enable_odom_tf: false

      cmd_vel_timeout: 0.25
      #publish_limited_velocity: true
      use_stamped_vel: false
      velocity_rolling_window_size: 2

      # Preserve turning radius when limiting speed/acceleration/jerk
      preserve_turning_radius: true

      # Publish limited velocity
      publish_cmd: true

      # Publish wheel data
      publish_wheel_data: true

      # Velocity and acceleration limits
      # Whenever a min_* is unspecified, default to -max_*
      linear.x.has_velocity_limits: true
      linear.x.has_acceleration_limits: true
      linear.x.has_jerk_limits: false
      linear.x.max_velocity: 1.0
      linear.x.min_velocity: -1.0
      linear.x.max_acceleration: 3.00
      linear.x.max_jerk: 0.0
      linear.x.min_jerk: 0.0

      angular.z.has_velocity_limits: true
      angular.z.has_acceleration_limits: true
      angular.z.has_jerk_limits: false
      angular.z.max_velocity: 2.0
      angular.z.min_velocity: -2.0
      angular.z.max_acceleration: 6.0
      angular.z.min_acceleration: -6.0
      angular.z.max_jerk: 0.0
      angular.z.min_jerk: 0.0


Launching the Controllers
^^^^^^^^^^^^^^^^^^^^^^^^^


To start the controllers when launching the simulation, we use the `controller_manager` spawner node. This is typically done inside your `gazebo.launch.py` launch file.

**Launch Snippet (Python):**

.. code-block:: python

   # Directory Setup
   pkg_clearpath_gz = get_package_share_directory('husky_gazebo')
   gps_wpf_dir = get_package_share_directory('husky_control')
   params_dir = os.path.join(gps_wpf_dir, "config")
   launch_dir = os.path.join(gps_wpf_dir, 'launch')

   #  Set GZ_SIM_RESOURCE_PATH
   gz_resource_path = SetEnvironmentVariable(name='GZ_SIM_RESOURCE_PATH', value=[
      os.path.join(pkg_clearpath_gz, 'worlds'), ':',
      str(Path(get_package_share_directory('husky_description')).parent.resolve())
   ])

   #  Load Parameters
   world_path = LaunchConfiguration('world_path')
   prefix = LaunchConfiguration('prefix')  

   #  URDF & robot_description
   robot_description_content = Command([
      PathJoinSubstitution([FindExecutable(name="xacro")]),
      " ",
      PathJoinSubstitution([FindPackageShare("husky_description"), "urdf", "husky.urdf.xacro"]),
      " name:=husky prefix:='' is_sim:=true gazebo_controllers:=",
      config_husky_velocity_controller,
   ])
   robot_description = {"robot_description": robot_description_content}

   #  Set robot_state_publisher
   node_robot_state_publisher = Node(
      package="robot_state_publisher",
      executable="robot_state_publisher",
      output="screen",
      parameters=[{'use_sim_time': True}, robot_description],
   )

   #  Joint State Broadcaster - Launches a controller that publishes joint states (needed for visualization and TF).
   spawn_joint_state_broadcaster = Node(
      package='controller_manager',
      executable='spawner',
      arguments=['joint_state_broadcaster', '-c', '/controller_manager'],
      output='screen',
   )
   # Husky Velocity Controller - Starts the differential drive controller that controls Husky’s movement via /cmd_vel.
   spawn_husky_velocity_controller = Node(
    package='controller_manager',
    executable='spawner',
    arguments=['husky_velocity_controller', '-c', '/controller_manager'],
    output='screen',
   )

    # Make sure spawn_husky_velocity_controller starts after spawn_joint_state_broadcaster
    diffdrive_controller_spawn_callback = RegisterEventHandler(
        event_handler=OnProcessExit(
            target_action=spawn_joint_state_broadcaster,
            on_exit=[spawn_husky_velocity_controller],
        )
    )

    # Spawn the Robot in Gazebo
    diffdrive_controller_spawn_callback = RegisterEventHandler(
        event_handler=OnProcessExit(
            target_action=spawn_joint_state_broadcaster,
            on_exit=[spawn_husky_velocity_controller],
        )
    )

    # Launch robot_localization
   launch_husky_control = IncludeLaunchDescription(
      PythonLaunchDescriptionSource(PathJoinSubstitution([
         FindPackageShare("husky_control"), 'launch', 'control.launch.py'
      ]))
   )


It is essential that the `JointStateBroadcaster` is fully started before the pan and tilt controllers. This is done using an event handler that triggers after the `spawn_joint_state_broadcaster` completes:



Publish velocity to topic
^^^^^^^^^^^^^^^^^^^^^^^^^

Gazebo camera plugin details


How to see the feed:

 ros2 run rqt_image_view rqt_image_view


List topics: ros2 topic list

ros2 topic info /husky_velocity_controller/cmd_vel
Type: geometry_msgs/msg/TwistStamped
Publisher count: 1
Subscription count: 1

Twist to TwistStamped
^^^^^^^^^^^^^^^^^^^^^^

If cmd_vel topic has msg type TwistStamped you need to convert Twist to TwistStamped like this:

.. code-block:: python

   # Imports
   import rclpy
   from rclpy.node import Node
   from geometry_msgs.msg import Twist, TwistStamped

   # Defines a node class that inherits from rclpy.node.Node.
   class TwistToTwistStamped(Node):
      def __init__(self):
         super().__init__('twist_to_twist_stamped') # Initializes the node with the name twist_to_twist_stamped.
   
         # Creates a publisher that sends messages of type TwistStamped to the topic where 10 is the queue size for outgoing messages.
         self.publisher_ = self.create_publisher(TwistStamped, 'husky_velocity_controller/cmd_vel', 10)

         # Subscribes to the topic cmd_vel, which typically provides velocity commands as Twist messages. The callback self.callback will be called when a message is received.
         self.subscription = self.create_subscription(
               Twist,
               'cmd_vel',
               self.callback,
               10
         )
      # his function is triggered whenever a Twist message is received.
      def callback(self, msg):
         twist_stamped = TwistStamped()
         twist_stamped.header.stamp = self.get_clock().now().to_msg() # sets the current ROS 2 time.
         twist_stamped.header.frame_id = 'base_link'  # Set frame id
         twist_stamped.twist = msg # copies the original Twist message.
         self.publisher_.publish(twist_stamped) # Publishe the new TwistStamped message to the husky_velocity_controller/cmd_vel topic.

   # Main Function - Initializes the ROS2 Python system.
   def main(args=None):
      rclpy.init(args=args)
      node = TwistToTwistStamped()
      rclpy.spin(node)
      node.destroy_node()
      rclpy.shutdown()

   if __name__ == '__main__':
      main()


Testing the System 
------------------

URDF + controllers + Gazebo in one go


Launch example and keyboard control:

.. code-block:: console

   $ ros2 launch husky_gazebo husky.launch.py 

   $ /husky_withoutSensors_ws/src/husky_gazebo/launch$ python3 twistToTwistStamped.py 



Gazebo visualization:

.. image:: gzSim.png 
   :height: 1080 px
   :width: 1440 px
   :scale: 100 %
   :loading: embed
   :alt: Gazebo visualization
   :align: left

Rviz visualization:

.. image:: rviz.png
   :height: 1080 px
   :width: 1440 px
   :scale: 100 %
   :loading: embed
   :alt: Rviz visualization
   :align: left

Open a new terminal and write to control Husky with keyboard:

.. code-block:: console

   $ ros2 run teleop_twist_keyboard teleop_twist_keyboard 


Open a new terminal and write to check husky_velocity_controller assigning velocity:

.. code-block:: console

   $ ros2 topic pub /husky_velocity_controller/cmd_vel geometry_msgs/msg/TwistStamped "{ header: { stamp: {sec: 0, nanosec: 0}, frame_id: 'base_link' }, twist: { linear: {x: 1.5, y: 0.0, z: 0.0}, angular: {x: 0.0, y: 0.0, z: 0.5} } }" --rate 10

Husky should move in circle like this:

.. video:: pan_tilt.mp4

.. image:: https://img.youtube.com/vi/YOUTUBE_VIDEO_ID_HERE/maxresdefault.jpg
    :alt: IMAGE ALT TEXT HERE
    :target: https://www.youtube.com/watch?v=xGfjsGp3I2o

Further Resources
-----------------

Once Gazebo is installed and is all clear on the last quick test, you can move to the `Gazebo tutorials <https://gazebosim.org/docs/harmonic/tutorials>`__ to try out building your own robot!

If you use a different version of Gazebo than the recommended version, make sure to use the dropdown to select the correct version of documentation.

Summary
-------

In this tutorial, you have installed Gazebo and set-up your workspace to start with the `Gazebo tutorials <https://gazebosim.org/docs/harmonic/tutorials>`__.