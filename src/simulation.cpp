
// ros libs
#include "ros/ros.h"
#include "std_msgs/String.h"
#include <geometry_msgs/Twist.h>

// vrep libs
#include "vrep_common/VrepInfo.h"
#include "vrep_common/simRosStartSimulation.h"
#include "vrep_common/simRosStopSimulation.h"
#include "vrep_common/simRosEnablePublisher.h"
#include "vrep_common/simRosEnableSubscriber.h"
#include "vrep_common/simRosGetObjectHandle.h"
#include "vrep_common/JointSetStateData.h"
#include "../include/v_repConst.h"

bool simulationRunning=true;
float simulationTime=0.0f;
float speedMotorFront, speedMotorRear, speedMotorLeft, speedMotorRight = 0;

void infoCallback(const vrep_common::VrepInfo::ConstPtr& info)
{
  simulationTime=info->simulationTime.data;
  simulationRunning=(info->simulatorState.data&1)!=0;
}

void startStopSim(ros::NodeHandle n_, int s){

  if (s == 0){
    ros::ServiceClient client_simStart = n_.serviceClient<vrep_common::simRosStartSimulation>("/vrep/simRosStartSimulation");
    vrep_common::simRosStartSimulation srv_simStart;
    if(client_simStart.call(srv_simStart)){
      ROS_INFO("Simulation Started!"); // TODO check if service call response is correct
    }

  }

  else if (s == 1){
    ros::ServiceClient client_simStop = n_.serviceClient<vrep_common::simRosStopSimulation>("/vrep/simRosStopSimulation");
    vrep_common::simRosStopSimulation srv_simStop;
    if(client_simStop.call(srv_simStop)){
      ROS_INFO("Simulation Stopped!"); // TODO check if service call response is correct
    }

  }

}




int main(int argc, char **argv)
{

  ros::init(argc, argv, "simulation");
  ros::NodeHandle n_;
  ros::Rate rate(100);
  geometry_msgs::Twist vel_msg;

  ros::Subscriber subInfo=n_.subscribe("/vrep/info", 1, infoCallback);
  ros::Publisher vel_pub =n_.advertise<geometry_msgs::Twist>("cmd_vel", 10);

  startStopSim(n_,0);
  vel_msg.linear.x = 0.1;

    while (ros::ok() && simulationRunning) {


      // vel_pub.publish(vel_msg);
      ros::spinOnce();
      rate.sleep();
    }
    startStopSim(n_,1); // ROS terminated, stop simulation
    ros::shutdown();
    return(0);



}
