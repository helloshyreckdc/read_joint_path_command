#include "ros/ros.h"
#include"std_msgs/String.h"
#include "trajectory_msgs/JointTrajectory.h"
#include "trajectory_msgs/JointTrajectoryPoint.h"
#include <sstream>
#include <vector>
#include <cstdio>
#include "cubicSpline.h"
#
#define AXIS_COUNT 6
void chatterCB(const trajectory_msgs::JointTrajectory& msg)
{
    cubicSpline spline;
     double time_nsecs=0.0;
     int points_count=0;
     double joint_positions[6] {0.0};
     double right_bound=0;
    ROS_INFO("get the joint_path_command ");
    ofstream originFile;
    originFile.open("origin2.csv",ios::out | ios::trunc);
    originFile<<"time"<<","<<"pos2"<<","<<"vel2"<<","<<"ace2"<<","<<endl;
    std::basic_string<char> jointname=msg.joint_names[0] ;
    points_count= msg.points.size();                        //获取规划后点的个数
    double *Time=new double[points_count];              //划分数组
    double *data_arr=new double[points_count];
   for (int j=0; j < AXIS_COUNT; j++)
    {
         for (int i{}; i < points_count; i++)
         {
         
             data_arr[i]=msg.points[i].positions[j];
             Time[i]=((double)msg.points[i].time_from_start.toSec());
             ROS_INFO("%f",((double)msg.points[i].time_from_start.toSec()));
             originFile<<msg.points[i].time_from_start.toSec()<<","<<msg.points[i].positions[j]<<","<<msg.points[i].velocities[j]<<","<<msg.points[i].accelerations[j]<<endl;
         } 
         //将各轴数据分开
        originFile<<" "<<","<<" "<<","<<" "<<","<<" "<<endl;
        spline.loadData(Time,data_arr,points_count,0,0, cubicSpline::BoundType_First_Derivative);
        double x_out = 0;
        double y_out = 0;
        ROS_INFO("%f",(double)msg.points[points_count-1].time_from_start.toSec()); 
       for(double i=0; i<=(((double)msg.points[points_count-1].time_from_start.toSec())); i=i+0.004)
         {
            x_out = x_out + 0.004;
            spline.getYbyX(x_out, y_out); //x是时间，y是点
         }
         spline.oFile<<" "<<","<<" "<<","<<" "<<","<<" "<<endl;
     }
 

    
   
}
 int main(int argc, char *argv[])
{
    /* code for main function */
    ros::init(argc, argv, "joint_path_command_subscriber");
    ros::NodeHandle n;
    ros::Subscriber sub=n.subscribe("joint_path_command",10,chatterCB);
    ros::spin();
    return 0;
}
