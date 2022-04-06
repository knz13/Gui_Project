#include "structures.h"
#include "../kv.h"








EventReceiver::~EventReceiver() {
    if(m_SubscribedEvents.size() == 0){
        return;
    }
    auto it = m_SubscribedEvents.begin();
    while(it != m_SubscribedEvents.end()){
        it->second();
        it = m_SubscribedEvents.begin();
    }
}

