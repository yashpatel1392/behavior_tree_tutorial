#include "behaviortree_cpp_v3/bt_factory.h"
#include "ros/package.h"

struct Position2d{ double x,y; };
using namespace BT;

namespace BT {
    template<>
    inline Position2d convertFromString(StringView str) {
        printf("Converting string: \"%s\"\n", str.data());
        auto parts = splitString(str, ';');
        if (parts.size() != 2) {
            throw RuntimeError("invalid input");
        } else {
            Position2d output;
            output.x = convertFromString<double>(parts[0]);
            output.y = convertFromString<double>(parts[1]);
            return output;
        }
    }
}
//input class
class PrintTarget: public SyncActionNode{
public:
    PrintTarget(const std::string& name, const NodeConfiguration& config):
            SyncActionNode(name,config)
    {}
    static PortsList providedPorts(){
        const char* description = "Simply print the goal on the console";
        return {InputPort<Position2d>("target", description)};
    }
    NodeStatus tick() override {
        auto response = getInput<Position2d>("target");
        if(!response){
            throw RuntimeError("Error: ", response.error());
        }
        Position2d target = response.value();
        printf("Target positions: [ %.1f, %.1f ]\n", target.x, target.y );
        return NodeStatus::SUCCESS;
    }
};
//output class
class CalculateGoal: public SyncActionNode{
public:
    CalculateGoal(const std::string& name, const NodeConfiguration& config):
            SyncActionNode(name,config)
    {}
    static PortsList providedPorts(){
        return{OutputPort<Position2d>("goal")};
    }
    NodeStatus tick() override {
        Position2d mygoal = {1.1, 2.2};
        setOutput("goal", mygoal);
        return NodeStatus::SUCCESS;
    }
};
int main(){
    BehaviorTreeFactory bt_factory;

    bt_factory.registerNodeType<PrintTarget>("PrintTarget");
    bt_factory.registerNodeType<CalculateGoal>("CalculateGoal");

    auto tree = bt_factory.createTreeFromFile(ros::package::getPath("behavior_tree_tutorial") + "/generic_ports_tutorial_tree.xml");
    tree.root_node->executeTick();
    return 0;
}

