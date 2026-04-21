paketi za instalaciju uz ros-jazzy-desktop potrebni za control i podizanje simulatora:  
sudo apt-get install ros-jazzy-xacro  
sudo apt-get install ros-jazzy-controller-interface  
sudo apt-get install ros-jazzy-controller-manager  
sudo apt-get install ros-jazzy-ros-gz-sim  
sudo apt-get install ros-jazzy-ros-gz-bridge  
sudo apt-get install ros-jazzy-robot-localization  
sudo apt-get install ros-jazzy-interactive-marker-twist-server  
sudo apt-get install ros-jazzy-twist-mux  
sudo apt-get install ros-jazzy-gz-ros2-control  
sudo apt-get install ros-jazzy-ros2-control  
sudo apt-get install ros-jazzy-ros2-controllers  
sudo apt-get install ros-jazzy-joint-state-publisher
sudo apt-get install ros-jazzy-joint-state-broadcaster
sudo apt-get install ros-jazzy-hardware-interface
sudo apt-get install ros-jazzy-joint-trajectory-controller
sudo apt-get install ros-jazzy-diff-drive-controller
sudo apt-get install ros-jazzy-tf-transformations 

plugin za kameru:  
sudo apt-get install ros-jazzy-realsense2-camera  
sudo apt-get install ros-jazzy-realsense2-description

plugin za pan/tilt:
sudo apt-get install ros-jazzy-position-controllers
testiranje pan/tilt: 
ros2 topic pub /effort_group_controller/commands std_msgs/msg/Float64MultiArray "data: [0.1, -0.2]"


plugin za korištenje nav2:  
sudo apt-get install ros-jazzy-nav2-bringup  
sudo apt-get install ros-jazzy-navigation2

za lidar:
sudo apt-get install ros-jazzy-velodyne
sudo apt-get install ros-jazzy-velodyne-description


* napraviti source za gazebo pakete: source /opt/ros/jazzy/setup.bash  
  * ako nije dodan u .bashrc, dodati na kraj tog file: gedit ~/.bashrc  
* ako baci grešku kod instalacije paketa, treba napraviti update: sudo apt-get update

za ROS Humble i laserski senzor:  
instalirati lms1xx paket:
sudo apt-get install ros-humble-lms1xx  
za ROS Jazzy skinuti ručno i dodati u src, ali treba prilagoditi plugin za novi gazebo

pokretanje simulacije:  
ros2 launch husky_gazebo husky.launch.py


publish brzine na topic:  
ros2 topic pub /husky_velocity_controller/cmd_vel geometry_msgs/msg/TwistStamped "{ header: { stamp: {sec: 0, nanosec: 0}, frame_id: 'base_link' }, twist: { linear: {x: 1.5, y: 0.0, z: 0.0}, angular: {x: 0.0, y: 0.0, z: 0.5} } }" --rate 10

platform:
ros2 launch ur_robotiq_gazebo husky.launch.py
ros2 launch  husky_viz view_navigation.launch.py
~/rob2_ws/src/ur_robotiq-main/ur_robotiq-main/ur_robotiq_gazebo/launch$ python3 twistToTwistStamped.py 
u StateMachines_All:
python3 sm_mision_old25goals.py
python3 navigation_client.py 
python3 fake_sender.py 

python3 logging_server.py
python3 task_status_server.py




moguca greska kod build: --- stderr: husky_base, ali se opet colcon build i sve ok
