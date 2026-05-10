import rclpy
from rclpy.node import Node
from rclpy.qos import QoSProfile, ReliabilityPolicy, DurabilityPolicy
from geometry_msgs.msg import Twist
from nav_msgs.msg import Odometry
import serial
import time
import threading

class RoverSerialBridge(Node):
    def __init__(self):
        super().__init__('rover_serial_bridge')
        self.get_logger().info('=== ROVER BRIDGE STARTING ===')
        
        self.port = '/dev/ttyACM0'
        self.baud = 115200
        
        try:
            self.ser = serial.Serial(self.port, self.baud, timeout=0.1)
            time.sleep(2.5)
            self.get_logger().info('✅ Serial opened successfully')
        except Exception as e:
            self.get_logger().error(f'Failed to open serial: {e}')
            return

        # Publishers & Subscribers
        self.odom_pub = self.create_publisher(Odometry, 'odom', 10)
        
        qos = QoSProfile(depth=10, reliability=ReliabilityPolicy.RELIABLE)
        self.create_subscription(Twist, 'cmd_vel', self.cmd_vel_callback, qos)
        self.get_logger().info('✅ Subscribed to /cmd_vel')

        self.create_timer(0.1, self.request_encoders)
        
        self.left_ticks = 0
        self.right_ticks = 0

        self.read_thread = threading.Thread(target=self.read_serial, daemon=True)
        self.read_thread.start()
        self.get_logger().info('📡 Bridge fully started and running')

    def request_encoders(self):
        try:
            self.ser.write(b'ENC\n')
        except:
            pass

    def cmd_vel_callback(self, msg: Twist):
        cmd = f"VEL {msg.linear.x:.3f} {msg.angular.z:.3f}\n"
        try:
            self.ser.write(cmd.encode('utf-8'))
            self.get_logger().info(f'→ Sent to ESP: VEL {msg.linear.x:.3f} {msg.angular.z:.3f}')
        except Exception as e:
            self.get_logger().error(f'Write failed: {e}')

    def read_serial(self):
        while rclpy.ok():
            try:
                if self.ser.in_waiting > 0:
                    line = self.ser.readline().decode('utf-8', errors='ignore').strip()
                    if line.startswith("ENC "):
                        self.parse_enc(line)
                    elif line and not any(x in line for x in ["Motor", "-----"]):
                        self.get_logger().debug(f'ESP: {line}')
            except:
                time.sleep(0.01)

    def parse_enc(self, line):
        try:
            parts = line.split()
            left = int(parts[1])
            right = int(parts[2])
            delta_l = left - self.left_ticks
            delta_r = right - self.right_ticks
            self.left_ticks = left
            self.right_ticks = right

            odom = Odometry()
            odom.header.stamp = self.get_clock().now().to_msg()
            odom.header.frame_id = 'odom'
            odom.child_frame_id = 'base_link'
            odom.twist.twist.linear.x = (delta_l + delta_r) * 0.005
            odom.twist.twist.angular.z = (delta_r - delta_l) * 0.008
            self.odom_pub.publish(odom)
            
            self.get_logger().info(f'ENC → L:{left} R:{right}  Δ:{delta_l}/{delta_r}')
        except:
            pass

    def destroy_node(self):
        try:
            self.ser.write(b"VEL 0.0 0.0\n")
            self.ser.close()
        except:
            pass
        super().destroy_node()


def main(args=None):
    rclpy.init(args=args)
    node = RoverSerialBridge()
    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass
    finally:
        node.destroy_node()
        rclpy.shutdown()

if __name__ == '__main__':
    main()