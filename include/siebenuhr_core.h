
class S7Controller 
{
public:
    static S7Controller* getInstance();

    void initialize();
    void update();

private:
    S7Controller() = default;

    static S7Controller* s_instance;
};