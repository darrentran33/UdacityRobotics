//ros is the official library for ROS. Provides much of the functionality required for intercfa
#include "ros/ros.h"
// Contains the primitive message types in ROS, will publish Float64 messages to command topics for each joint of the arm
#include "std_msgs/Float64.h"

int main(int argc, char** argv)
{
    // Initialize the arm_mover node registered with the ROS master. Here arm_mover is the name of the node. 
    //argc and argv arguments areboth taken in and passed into the init() function
    ros::init(argc, argv, "arm_mover");

    // Create a handle to the arm_mover node. node handle object will fully initialize the node and permit it to communicate with the ROS Master.
    ros::NodeHandle n;

    // Create a publisher that can publish a std_msgs::Float64 message on the /simple_arm/joint_1_position_controller/command topic
    ros::Publisher joint1_pub = n.advertise<std_msgs::Float64>("/simple_arm/joint_1_position_controller/command", 10);
    // Create a publisher that can publish a std_msgs::Float64 message on the /simple_arm/joint_2_position_controller/command topic
    ros::Publisher joint2_pub = n.advertise<std_msgs::Float64>("/simple_arm/joint_2_position_controller/command", 10);

    // Set loop frequency of 10Hz. Choosing a rate too high may result in unnecessary CPU usage while using a value too low will result in high latency.
    ros::Rate loop_rate(10);

    int start_time, elapsed;

    // Get ROS start time as the current time. Want to know how much time has elapsed
    while (not start_time) {
        start_time = ros::Time::now().toSec();
    }

    while (ros::ok()) {
        // Get ROS elapsed time, ros-TIme-now will initially return 0, until first message recieved on the /clock. Polled until nonzero value is returned.
        elapsed = ros::Time::now().toSec() - start_time;

        // Set the arm joint angles. Joint angles are sampled from a sine wave with a period of 10 second, and in magnitude from [-pi/2, +pi/2]
        std_msgs::Float64 joint1_angle, joint2_angle;
        joint1_angle.data = sin(2 * M_PI * 0.1 * elapsed) * (M_PI / 2);
        joint2_angle.data = sin(2 * M_PI * 0.1 * elapsed) * (M_PI / 2);

        // Publish the arm joint angles
        joint1_pub.publish(joint1_angle);
        joint2_pub.publish(joint2_angle);

        // Sleep for the time remaining until 10 Hz is reached
        loop_rate.sleep();
    }

    return 0;
}