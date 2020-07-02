#include "behaviortree_cpp_v3/loggers/bt_cout_logger.h"
#include "behaviortree_cpp_v3/bt_factory.h"
#include "ros/package.h"
#include "movebase_node.h"
#include "dummy_nodes.h"

using namespace BT;
using namespace DummyNodes;

int main()
{
    BehaviorTreeFactory factory;

    factory.registerNodeType<SaySomething>("SaySomething");
    factory.registerNodeType<MoveBaseAction>("MoveBase");

    auto tree = factory.createTreeFromFile(ros::package::getPath("behavior_tree_tutorial") + "/ports_remapping_tree.xml");

    NodeStatus status = NodeStatus::RUNNING;
    while( status == NodeStatus::RUNNING)
    {
        status = tree.root_node->executeTick();
        SleepMS(1);
    }

    std::cout << "--------------" << std::endl;
    tree.blackboard_stack[0]->debugMessage();
    std::cout << "--------------" << std::endl;
    tree.blackboard_stack[1]->debugMessage();
    std::cout << "--------------" << std::endl;

    return 0;
}