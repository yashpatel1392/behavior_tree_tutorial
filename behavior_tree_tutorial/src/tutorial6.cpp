#include "crossdoor_nodes.h"
#include "ros/package.h"
#include "behaviortree_cpp_v3/loggers/bt_cout_logger.h"
#include "behaviortree_cpp_v3/loggers/bt_minitrace_logger.h"
#include "behaviortree_cpp_v3/loggers/bt_file_logger.h"
#include "behaviortree_cpp_v3/bt_factory.h"

using namespace BT;

int main(int argc, char** argv)
{
    BT::BehaviorTreeFactory factory;
    CrossDoor::RegisterNodes(factory);

    auto tree = factory.createTreeFromFile(ros::package::getPath("behavior_tree_tutorial") + "/subtrees_tutorial_tree.xml");

    StdCoutLogger logger_cout(tree);

    FileLogger logger_file(tree, "bt_trace.fbl");
    MinitraceLogger logger_minitrace(tree, "bt_trace.json");

    printTreeRecursively(tree.root_node);

    const bool LOOP = ( argc == 2 && strcmp( argv[1], "loop") == 0);

    do
    {
        NodeStatus status = NodeStatus::RUNNING;
        // Keep on ticking until you get either a SUCCESS or FAILURE state
        while( status == NodeStatus::RUNNING)
        {
            status = tree.root_node->executeTick();
            CrossDoor::SleepMS(1);   // optional sleep to avoid "busy loops"
        }
        CrossDoor::SleepMS(1000);
    }
    while(LOOP);

    return 0;
}