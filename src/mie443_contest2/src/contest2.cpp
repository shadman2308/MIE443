#include <boxes.h>
#include <navigation.h>
#include <robot_pose.h>
#include <imagePipeline.h>
#include <math.h>
#include <cmath>

int main(int argc, char** argv) {
    // Setup ROS.
    ros::init(argc, argv, "contest2");
    ros::NodeHandle n;

    // Robot pose object + subscriber.
    RobotPose robotPose(0,0,0);
    ros::Subscriber amclSub = n.subscribe("/amcl_pose", 1, &RobotPose::poseCallback, &robotPose);

    // Initialize box coordinates and templates
    Boxes boxes; 
    if(!boxes.load_coords() || !boxes.load_templates()) 
    {
        std::cout << "ERROR: could not load coords or templates" << std::endl;
        return -1;
    }

    for(int i = 0; i < boxes.coords.size(); ++i) 
    {
        std::cout << "Box coordinates: " << std::endl;
        std::cout << i << " x: " << boxes.coords[i][0] << " y: " << boxes.coords[i][1] << " z: " 
                  << boxes.coords[i][2] << std::endl;
    }

    // Initialize image objectand subscriber.
    //ImagePipeline imagePipeline(n);

    // Initalize navigator
    Navigation navigator;

    unsigned boxInd = 0;

    // Execute strategy.
    while(ros::ok()) 
    {
        ros::spinOnce();
        /***YOUR CODE HERE***/
        // Use: boxes.coords
        // Use: robotPose.x, robotPose.y, robotPose.phi

        float phi = boxes.coords[boxInd][2];
        float x = boxes.coords[boxInd][0] + 0.7*std::cos(phi);
        float y = boxes.coords[boxInd][1] + 0.7*std::sin(phi);
        phi = fmod(phi + 3.14, 6.28);

        if (!navigator.moveToGoal(x, y, phi))
        {
            return -1;
        }

        ++boxInd;
        if (boxInd > static_cast<int>(boxes.coords.size()))
        {
            std::cout << "Found all boxes" << std::endl;
            return 0;
        }

        //simagePipeline.getTemplateID(boxes);
        //ros::Duration(0.01).sleep();
    }
    return 0;
}