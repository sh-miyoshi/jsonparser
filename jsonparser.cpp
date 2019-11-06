#include "jsonparser.h"
using namespace json;

Error Value::SetObject(std::string key, std::shared_ptr<Value> value){
    if (type == eTYPE_NULL || type == eTYPE_OBJECT) {
        type = eTYPE_OBJECT;
        obj.insert(std::make_pair(key, value));
    } else {
        Error err;
        err.success=false;
        err.message="Failed to set object due to unexpected type";
        return err;
    }
    return Error();// return success
}

Error Value::SetArray(std::shared_ptr<Value> value){
    if (type == eTYPE_NULL || type == eTYPE_ARRAY) {
        type = eTYPE_ARRAY;
        ary.push_back(value);
    } else {
        Error err;
        err.success=false;
        err.message="Failed to set array due to unexpected type";
        return err;
    }
    return Error();// return success
}

Error Value::SetString(std::string value){
    if (type == eTYPE_NULL) {
        type = eTYPE_STRING;
        str = value;
    } else {
        Error err;
        err.success=false;
        err.message="Failed to set string due to unexpected type";
        return err;
    }
    return Error();// return success
}

Value::Type Value::GetType() const {
    return type;
}
std::string Value::GetString(){
    return str;
}

std::map<std::string, std::shared_ptr<Value>> Value::GetObject(){
    return obj;
}

std::vector<std::shared_ptr<Value>> Value::GetArray(){
    return ary;
}