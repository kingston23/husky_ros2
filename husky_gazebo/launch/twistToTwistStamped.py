import rclpy
from rclpy.node import Node
from geometry_msgs.msg import Twist, TwistStamped

class TwistToTwistStamped(Node):
    def __init__(self):
        super().__init__('twist_to_twist_stamped')
        self.publisher_ = self.create_publisher(TwistStamped, 'husky_velocity_controller/cmd_vel', 10)
        self.subscription = self.create_subscription(
            Twist,
            'cmd_vel',
            self.callback,
            10
        )

    def callback(self, msg):
        twist_stamped = TwistStamped()
        twist_stamped.header.stamp = self.get_clock().now().to_msg()
        twist_stamped.header.frame_id = 'base_link'  # Set frame id
        twist_stamped.twist = msg
        self.publisher_.publish(twist_stamped)

def main(args=None):
    rclpy.init(args=args)
    node = TwistToTwistStamped()
    rclpy.spin(node)
    node.destroy_node()
    rclpy.shutdown()

if __name__ == '__main__':
    main()
