#include "structures.h"
#include "../kv.h"




EventReceiver& EventReceiver::operator=(const EventReceiver& other) {
    for(auto& [ptr,prop] : other.m_SubscribedEvents){
        prop.m_CopyFunc(this);
    }
    return *this;
}



EventReceiver::~EventReceiver() {
    if(m_SubscribedEvents.size() == 0){
        return;
    }
    auto it = m_SubscribedEvents.begin();
    while(it != m_SubscribedEvents.end()){
        it->second.m_DeleteFunc(it->first,std::hash<void*>()((void*)this));
        it = m_SubscribedEvents.begin();
    }
}

