#include "behaviortree_cpp_v3/bt_factory.h"
#include "ros/package.h"

using namespace BT;

class MyAsyncAction: public CoroActionNode
{
public:
    MyAsyncAction(const std::string& name):
            CoroActionNode(name, {})
    {}

private:
    NodeStatus tick() override
    {
        std::cout << name() <<": Started. Send Request to server." << std::endl;

        auto Now = [](){ return std::chrono::high_resolution_clock::now(); };

        TimePoint initial_time = Now();
        TimePoint time_before_reply = initial_time + std::chrono::milliseconds(100);

        int count = 0;
        bool reply_received = false;

        while( !reply_received )
        {
            if( count++ == 0)
            {
                std::cout << name() <<": Waiting Reply..." << std::endl;
            }
            if( Now() >= time_before_reply )
            {
                reply_received = true;
            }
            if( !reply_received )
            {
                setStatusRunningAndYield();
            }
        }
        std::cout << name() <<": Done. 'Waiting Reply' loop repeated "
                  << count << " times" << std::endl;
        cleanup(false);
        return NodeStatus::SUCCESS;
    }
    void cleanup(bool halted)
    {
        if( halted )
        {
            std::cout << name() <<": cleaning up after an halt()\n" << std::endl;
        }
        else{
            std::cout << name() <<": cleaning up after SUCCESS\n" << std::endl;
        }
    }
    void halt() override
    {
        std::cout << name() <<": Halted." << std::endl;
        cleanup(true);
        CoroActionNode::halt();
    }
};
int main() {
    BehaviorTreeFactory factory;
    factory.registerNodeType<MyAsyncAction>("MyAsyncAction");

    auto tree = factory.createTreeFromFile(ros::package::getPath("behavior_tree_tutorial") + "/coroutines_tutorial_tree.xml");

    while (tree.root_node->executeTick() == NodeStatus::RUNNING) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    return 0;
}
