
#include <iostream>
#include <sys/time.h>

#include "trick/VariableServer.hh"
#include "trick/TrickConstant.hh"

int Trick::VariableServer::copy_data_scheduled() {

    long long next_call_tics ;
    std::map < pthread_t , VariableServerSession * >::iterator it ;

    next_call_tics = TRICK_MAX_LONG_LONG ;

    pthread_mutex_lock(&map_mutex) ;
    for ( it = var_server_sessions.begin() ; it != var_server_sessions.end() ; it++ ) {
        auto session = (*it).second ;
        session->copy_data_scheduled(copy_data_job->next_tics) ;
        if ( session->get_next_tics() < next_call_tics ) {
            next_call_tics = session->get_next_tics() ;
        }
    }
    pthread_mutex_unlock(&map_mutex) ;

    //reschedule the current job. TODO: a call needs to be created to do this the OO way
    if ( copy_data_job != NULL ) {
        copy_data_job->next_tics = next_call_tics ;
    }

    return(0) ;
}
