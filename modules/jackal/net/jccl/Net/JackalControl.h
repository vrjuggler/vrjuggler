




namespace jccl {

class Connect;
class JackalServer;

class JackalControl {

public:

    virtual void setJackalServer (JackalServer* js) {;}

    //: Notification from JackalServer for a new connection.
    virtual void addConnect (Connect* c) {;}

    //: Notification from JackalServer for a closed connection.
    virtual void removeConnect (Connect* c) {;}

};

};
