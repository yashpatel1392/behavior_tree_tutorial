#include <ros/package.h>

#include "behaviortree_cpp_v3/bt_factory.h"
#include "dummy_nodes.h"

using namespace BT;

BT::NodeStatus SayHello(){
    std::cout<<"Hello World"<<std::endl;
    return BT::NodeStatus::SUCCESS;
}
int main()
{
    BehaviorTreeFactory factory;
    using namespace DummyNodes;
    factory.registerNodeType<ApproachObject>("ApproachObject");
    factory.registerSimpleCondition("SayHello", std::bind(SayHello));
    factory.registerSimpleCondition("CheckBattery", std::bind(CheckBattery));
    GripperInterface gripper_interface;
    factory.registerSimpleAction("OpenGripper", std::bind(&GripperInterface::open, &gripper_interface));
    factory.registerSimpleAction("CloseGripper", std::bind(&GripperInterface::close, &gripper_interface));
    auto tree = factory.createTreeFromFile(ros::package::getPath("behavior_tree_tutorial") + "/my_tree.xml");
    tree.root_node->executeTick();
    return 0;
}