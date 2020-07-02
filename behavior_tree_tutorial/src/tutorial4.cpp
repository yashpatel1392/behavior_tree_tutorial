#include "ros/package.h"
#include "behaviortree_cpp_v3/bt_factory.h"
#include "movebase_node.h"
#include "dummy_nodes.h"
struct Pose2d{ double x, y, theta;};
using namespace BT;

void Assert(bool condition)
{
    if (!condition)
        throw RuntimeError("this is not what I expected");
}

int main(){
    using namespace DummyNodes;

    BehaviorTreeFactory bt_factory;

    bt_factory.registerSimpleCondition("BatteryOK", std::bind(CheckBattery));
    bt_factory.registerNodeType<MoveBaseAction>("MoveBase");
    bt_factory.registerNodeType<SaySomething>("SaySomething");

    for(auto& xml_tree : {ros::package::getPath("behavior_tree_tutorial") + "/sequence_tree.xml",
                          ros::package::getPath("behavior_tree_tutorial") + "/reactive_sequence_tree.xml"} )
    {
        std::cout<<"----Building a new tree----"<<std::endl;

        auto tree = bt_factory.createTreeFromFile(xml_tree);
        NodeStatus status;

        std::cout << "\n--- 1st executeTick() ---"<<xml_tree<< std::endl;
        status = tree.root_node->executeTick();
        Assert(status == NodeStatus::RUNNING);

        SleepMS(150);
        std::cout << "\n--- 2nd executeTick() ---" << std::endl;
        status = tree.root_node->executeTick();
        Assert(status == NodeStatus::RUNNING);

        SleepMS(150);
        std::cout << "\n--- 3rd executeTick() ---" << std::endl;
        status = tree.root_node->executeTick();
        Assert(status == NodeStatus::SUCCESS);

        std::cout << std::endl;

    }
}