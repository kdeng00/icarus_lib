#ifndef BASE_MANAGER_H_
#define BASE_MANAGER_H_


namespace icarus_lib::manager
{

template<typename config_detail>
class base_manager
{
protected:
    base_manager() = default;
    base_manager(const config_detail &config) :
        m_config(config)
    {
    }

    config_detail m_config;
private:
};
    
}


#endif