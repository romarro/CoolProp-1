#include "Configuration.h"

namespace CoolProp
{
    
std::string config_key_to_string(configuration_keys keys)
{
    switch (keys)
    {
        /* ***MAGIC WARNING**!!
         * See http://stackoverflow.com/a/148610
         * See http://stackoverflow.com/questions/147267/easy-way-to-use-variables-of-enum-types-as-string-in-c#202511
         */
         #define X(Enum, String, Default) \
            case Enum:  return String; break;
            CONFIGURATION_KEYS_ENUM
         #undef X
    }
    return ""; // will never get here, just to make compiler happy
}; 

/// Go from string to enum key
configuration_keys config_string_to_key(std::string &s)
{
    /* See http://stackoverflow.com/a/148610
     * See http://stackoverflow.com/questions/147267/easy-way-to-use-variables-of-enum-types-as-string-in-c#202511
     */
    #define X(Enum, String, Default) \
        if (s == String){ return Enum; }
        CONFIGURATION_KEYS_ENUM
    #undef X

    // Nothing else has fired
    throw ValueError();
};
    
static Configuration config;

bool get_config_bool(configuration_keys key){ 
    return static_cast<bool>(config.get_item(key)); 
}
double get_config_double(configuration_keys key){ 
    return static_cast<double>(config.get_item(key)); 
}
std::string get_config_string(configuration_keys key){ 
    return static_cast<std::string>(config.get_item(key)); 
}
void get_config_as_json(rapidjson::Document &doc){
    // Get the items
    std::map<configuration_keys, ConfigurationItem> items = config.get_items();
    for (std::map<configuration_keys, ConfigurationItem>::iterator it = items.begin(); it != items.end(); ++it){
        it->second.add_to_json(doc, doc);
    }
}
std::string get_config_as_json_string(){
    rapidjson::Document doc;
    doc.SetObject();
    get_config_as_json(doc);
    return cpjson::to_string(doc);
}
void set_config_as_json(rapidjson::Value &val){
    
    // First check that all keys are valid
    for (rapidjson::Value::MemberIterator it = val.MemberBegin(); it != val.MemberEnd(); ++it){
        try{
            // Try to get the key for the string
            std::string s = std::string(it->name.GetString());
            configuration_keys key = config_string_to_key(s);
            // Try to retrieve the item from the config for this key
            config.get_item(key);
        }
        catch(std::exception &e)
        {
            throw ValueError(format("Unable to parse json file with error: %s", e.what()));
        }
    }
    
    // Now we actually set the values
    for (rapidjson::Value::MemberIterator it = val.MemberBegin(); it != val.MemberEnd(); ++it){
        // Try to get the key for the string
        std::string s = std::string(it->name.GetString());
        configuration_keys key = config_string_to_key(s);
        // Try to retrieve the item from the config for this key
        ConfigurationItem & item = config.get_item(key);
        try{
            // Set the value from what is stored in the json value
            item.set_from_json(it->value);
        }
        catch(std::exception &e)
        {
            throw ValueError(format("Unable to parse json file with error: %s", e.what()));
        }
    }
}
void set_config_as_json_string(std::string &s){
    // Init the rapidjson doc
    rapidjson::Document doc;
    doc.Parse<0>(s.c_str());
    set_config_as_json(doc);
}

}

