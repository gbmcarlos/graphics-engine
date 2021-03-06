#include "EventBus.h"

#include "Event.h"

namespace engine {

    void EventBus::publishEvent(Event& event) {
        if (eventCallback) {
            eventCallback(event);
        }
    }

    void EventBus::setEventCallback(const std::function<void(Event&)>& callback) {
        eventCallback = callback;
    }

}