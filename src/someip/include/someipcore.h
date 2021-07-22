#include <vsomeip/vsomeip.hpp>

class SomeipCore {
public:
    SomeipCore(std::string name) :
                    rtm_(vsomeip::runtime::get()),
                    app_(rtm_->create_application(name))
    {
    }
    std::shared_ptr<vsomeip::runtime> GetRtm(){
        return rtm_;
    }
    std::shared_ptr<vsomeip::application> GetApp(){
        return app_;
    }
private:
    std::shared_ptr<vsomeip::runtime> rtm_;
    std::shared_ptr<vsomeip::application> app_;
};