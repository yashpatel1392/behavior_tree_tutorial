#include <ros/package.h>
#include "behaviortree_cpp_v3/bt_factory.h"

//xml tree file created in here
static const char* xml_text = R"(
 <root>
     <BehaviorTree>
        <MoveTo  goal="-1;3;0.5" />
     </BehaviorTree>
 </root>
 )";

struct Point3D{double x,y,z;};

namespace BT{
    template <> Point3D convertFromString(StringView str){
        auto parts = BT::splitString(str, ';');
        if(parts.size() != 3){
            throw RuntimeError("invalid input");
        }
        else{
            Point3D output;
            output.x = convertFromString<double>(parts[0]);
            output.y = convertFromString<double>(parts[1]);
            output.z = convertFromString<double>(parts[2]);
            return output;
        }
    }
}
class MyLegacyMoveTo
{
public:
    bool go(Point3D goal)
    {
        printf("Going to: %f %f %f\n", goal.x, goal.y, goal.z);
        return true; // true means success in my legacy code
    }
};

int main(){
    using namespace BT;
    MyLegacyMoveTo move_to;

    auto MoveToWrapperWithLambda = [&move_to](TreeNode& parent_node) -> NodeStatus {
        Point3D goal;
        parent_node.setOutput("goal", goal);
        bool res = move_to.go(goal);
        return res ? NodeStatus::SUCCESS : NodeStatus::FAILURE;

    };
    BehaviorTreeFactory bt_factory;

    PortsList ports = {BT::InputPort<Point3D>("goal")};
    bt_factory.registerSimpleAction("MoveTo", MoveToWrapperWithLambda, ports);

    auto tree = bt_factory.createTreeFromText(xml_text);
    tree.root_node->executeTick();

    return 0;
}
