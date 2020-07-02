#include <ros/package.h>
#include "behaviortree_cpp_v3/bt_factory.h"

using namespace BT;
class Action_A: public SyncActionNode
{
public:
    Action_A(const std::string& name, const NodeConfiguration& config,
             int arg1, double arg2, std::string arg3 ):
            SyncActionNode(name, config),
            _arg1(arg1),
            _arg2(arg2),
            _arg3(arg3) {}

    NodeStatus tick() override
    {
        std::cout << "Action_A: " << _arg1 << " / " << _arg2 << " / " << _arg3 << std::endl;
        return NodeStatus::SUCCESS;
    }
    static PortsList providedPorts() { return {}; }

private:
    int _arg1;
    double _arg2;
    std::string _arg3;
};
class Action_B: public SyncActionNode {

public:
    Action_B(const std::string &name, const NodeConfiguration &config) :
            SyncActionNode(name, config) {}

    void init(int arg1, double arg2, std::string arg3) {
        _arg1 = (arg1);
        _arg2 = (arg2);
        _arg3 = (arg3);
    }

    NodeStatus tick() override {
        std::cout << "Action_B: " << _arg1 << " / " << _arg2 << " / " << _arg3 << std::endl;
        return NodeStatus::SUCCESS;
    }

    static PortsList providedPorts() { return {}; }

private:
    int _arg1;
    double _arg2;
    std::string _arg3;
};
int main()
{
    BehaviorTreeFactory factory;
    NodeBuilder builder_A = [](const std::string& name, const NodeConfiguration& config)
    {
        return std::make_unique<Action_A>( name, config, 40, 3.1428, "hello world" );
    };

    factory.registerBuilder<Action_A>( "Action_A", builder_A);
    factory.registerNodeType<Action_B>( "Action_B" );

    auto tree = factory.createTreeFromFile(ros::package::getPath("behavior_tree_tutorial") + "/class_parameters_tutorial_tree.xml");

    for( auto& node: tree.nodes )
    {
        if( auto action_B_node = dynamic_cast<Action_B*>( node.get() ))
        {
            action_B_node->init( 10, 9.99, "interesting_value" );
        }
    }

    tree.root_node->executeTick();

    return 0;
}
