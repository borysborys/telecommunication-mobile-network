#include "NetConfAgent.hpp"
#include <iostream>
#include "MobileClient.hpp"

using namespace std;

namespace
{

    const int MAX_LEN = 100;  /// for xpath max length

} ///end of anonymus namespace

namespace NetConfAgent_ns
{

    bool NetConfAgent::initSysrepo()
    {
        _conn = make_unique<sysrepo::Connection>();
        ///cout << "connection id is "<< conn.get()<< endl;
        uint32_t cc;
        sr_connection_count(&cc);
        cout<< "there are already "<<cc<<" connections" <<endl;
        _sess = make_shared<sysrepo::Session>(move(_conn));
        ///cout << "session id is " << sess.get()<< endl; 

        _subscribe = make_shared<sysrepo::Subscribe>(_sess);

        return true;
    };

    bool NetConfAgent::closeSysrepo()
    {
        _sess->session_stop();
        _sess.reset();
        _conn.reset();
        _subscribe.reset();

        return true;
    }

    bool NetConfAgent::subscribeForModelChandes(const string & strXpath, MobileClient_ns::MobileClient &mobClient)
    {       

        try
            {       
            const char *path = strXpath.c_str();

            auto cb = [&mobClient](sysrepo::S_Session session, const char *module_name, const char *xpath, sr_event_t event, uint32_t request_id) {
                char change_path[MAX_LEN];        

                    if (event == SR_EV_DONE)
                    {                    
                        snprintf(change_path, MAX_LEN, "/%s:*//.", module_name);

                        auto it = session->get_changes_iter(change_path);
                        
                        map <string, string> mapOfNewData;

                        while (auto change = session->get_change_next(it)) {
                            
                            if (change->oper()!= SR_OP_DELETED)
                            {
                                string value = change->new_val()->to_string();
                                value = value.substr((value.find_last_of("=") + 2),(value.length() - value.find_last_of("=") - 2 - 1));
                                
                                string leaf = change->new_val()->to_string();
                                leaf = leaf.substr((leaf.find_last_of("/") + 1),(leaf.find_last_of("=") - leaf.find_last_of("/") - 2));
                                
                                mapOfNewData.insert(make_pair(leaf, value));
                                
                                mobClient.handleModuleChange(mapOfNewData);

                            }
                            else
                            {
                                cout <<"somthing deleted in data instanse...."<< endl;
                            }
                    }
                    }
                    
                return SR_ERR_OK;
            };

            _subscribe->module_change_subscribe("mobile-network", cb, path);

        
            return true;  
        }
        catch(const std::exception& e)
        {
            cerr << e.what() << endl;
            return false;
        }

    };

    bool NetConfAgent::fetchData(const string &str_xpath, string &str_value, sr_datastore_t ds)
    {
        const char *xpath = str_xpath.c_str();

        try
        {
            _sess->session_switch_ds(ds);       ///switching active session for a chosen datastore
                
            str_value = _sess->get_item(xpath)->val_to_string();                

        }
        catch(const std::exception& e)
        {
            cerr << e.what() << endl;
            return false;
        }
        
        return true;

    };

    bool NetConfAgent::changeData(const string &str_xpath, const string &str_entry, const string &param)
    {
        if (param == "change")
        {
            try
            {
                const char *xpath = str_xpath.c_str();
                const char *entry = str_entry.c_str();

                auto value = std::make_shared<sysrepo::Val>((char *)entry);

                _sess->set_item(xpath, value);
                _sess->apply_changes();
            }
            catch(const std::exception& e)
            {
                std::cerr << e.what() << endl;
                return false;
            }
            
            return true;
        }
        if (param == "delete")
        {
            try
            {
                const char *xpath = str_xpath.c_str();
                const char *entry = str_entry.c_str();

                auto value = std::make_shared<sysrepo::Val>((char *)entry);

                _sess->delete_item(xpath);
                _sess->apply_changes();
            }
            catch(const std::exception& e)
            {
                std::cerr << e.what() << endl;
                return false;
            }
            
            return true;
        }

    };

    bool NetConfAgent::registerOperData(const string & strXpath,MobileClient_ns::MobileClient &mobClient)
    {
        try
        {
            string str_module_name = strXpath;
            str_module_name = str_module_name.substr((str_module_name.find_first_of("/") + 1),((str_module_name.find_first_of(":"))-(str_module_name.find_first_of("/")) - 1));

            string str_path = strXpath;
            str_path = str_path.substr((str_path.find_first_of("/")),((str_path.find_last_of("/"))-(str_path.find_first_of("/"))));   
            
            auto cb = [strXpath, &mobClient] (sysrepo::S_Session session, const char *module_name, const char *path, const char *request_xpath,
            uint32_t request_id, libyang::S_Data_Node &parent) 
            {
                
                cout << "\n\n ========== CALLBACK CALLED TO PROVIDE \"" << strXpath << "\" DATA ==========\n" << endl;
                
                string str_value;
                mobClient.handleOperData(str_value);


                const char* value = str_value.c_str();
                const char* xpath = strXpath.c_str();


                libyang::S_Context ctx = session->get_context();
                libyang::S_Module mod = ctx->get_module(module_name);

                parent->new_path(ctx, xpath, value, LYD_ANYDATA_CONSTSTRING, 0);

                return SR_ERR_OK;
            };


            
            const char* module_name = str_module_name.c_str();  
            const char* path = str_path.c_str();
        
            _subscribe->oper_get_items_subscribe(module_name, cb, path);
            
            return true;
        }
        catch(const std::exception& e)
        {
            cerr << e.what() << endl;
            return false;
        }

    };

    bool NetConfAgent::notifySysrepo(const string &str_value1, const string &str_value2)
    {
        const char *value1 = str_value1.c_str();
        const char *value2 = str_value2.c_str();

        auto in_vals = std::make_shared<sysrepo::Vals>(2);

        in_vals->val(0)->set("/mobile-network:test-notif/val1", value1, SR_STRING_T);
        in_vals->val(1)->set("/mobile-network:test-notif/val2", value2, SR_STRING_T);

        cout << "\n ========== START NOTIF SEND ==========\n" << endl;
        _sess->event_notif_send("/mobile-network:test-notif", in_vals);

        return true;
    };

    bool NetConfAgent::subscribeForRpc(const string &str_rpc_xpath, const string &str_value)
    {
        auto cbVals = [str_value](sysrepo::S_Session session, const char* op_path, const sysrepo::S_Vals input, sr_event_t event, uint32_t request_id, sysrepo::S_Vals_Holder output) {
            cout << "\n ========== RPC CALLED ==========\n" << endl;

            const char* value = str_value.c_str();

            auto out_vals = output->allocate(1);
            out_vals->val(0)->set("/mobile-network:do-something/i-do", value , SR_STRING_T);

            for(size_t n = 0; n < input->val_cnt(); ++n){
                cout << (input->val(n))->data()->get_string()<< endl;
            }

            return SR_ERR_OK;
        };

        cout << "\n ========== SUBSCRIBE TO RPC CALL ==========\n" << endl;
        
        const char* rpc_xpath = str_rpc_xpath.c_str();

        _subscribe->rpc_subscribe(rpc_xpath, cbVals, 1);

        return true;

    };
} ///end of NetConfAgent_ns
