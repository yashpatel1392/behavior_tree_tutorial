#include <ros/package.h>

#include "behaviortree_cpp_v3/bt_factory.h"
#include "dummy_nodes.h"

using namespace BT;

class ThinkWhatToSay: public BT::SyncActionNode{
public:
    ThinkWhatToSay(const std::string& name, const BT::NodeConfiguration& config)
            : BT::SyncActionNode(name, config)
    { }
    static BT::PortsList providedPorts(){
        return{BT::OutputPort<std::string>("result")};
    }
    BT::NodeStatus tick(){
        setOutput("result", "the answer is 40");
        return BT::NodeStatus::SUCCESS;
    }
};
int main(){
    using namespace DummyNodes;

    BehaviorTreeFactory bt_factory;

    //SaySomething class is declared in DummyNodes
    bt_factory.registerNodeType<SaySomething>("SaySomething");
    bt_factory.registerNodeType<ThinkWhatToSay>("ThinkWhatToSay");

    BT::PortsList say_something2_ports = {BT::InputPort<std::string>("msg")};
    bt_factory.registerSimpleAction("SaySomething2", SaySomethingSimple, say_something2_ports);
    auto tree = bt_factory.createTreeFromFile(ros::package::getPath("behavior_tree_tutorial") + "/ports_tutorial_tree.xml");
    tree.root_node->executeTick();
    return 0;
}