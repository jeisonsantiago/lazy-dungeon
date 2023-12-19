#ifndef EVENT_H
#define EVENT_H

#include <functional>
#include <vector>

template<typename ...Args>
class Event
{
public:
    Event() = default;
    Event(const Event&) = default;
    using Callback = std::function<void(Args...)>;

    void subscribe(Callback callback){
        m_callbacks.push_back(callback);
    }

    void notify(Args... args)const{
        for(const auto& callback:m_callbacks){
            callback(args...);
        }
    }

private:
    std::vector<Callback> m_callbacks;
};

#endif // EVENT_H
