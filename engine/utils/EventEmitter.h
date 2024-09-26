#include <iostream>
#include <unordered_map>
#include <vector>
#include <functional>
#include <string>
#include <algorithm>
#include <sstream>

class EventEmitter {
public:
    EventEmitter() {
        // Initialize the base namespace
        callbacks["base"] = {};
    }

    EventEmitter& on(const std::string& _names, std::function<void()> callback) {
        if (_names.empty()) {
            std::cout << "wrong names" << std::endl;
            return *this;
        }

        if (!callback) {
            std::cout << "wrong callback" << std::endl;
            return *this;
        }

        std::vector<std::string> names = resolveNames(_names);

        for (const std::string& _name : names) {
            auto name = resolveName(_name);

            // Ensure the outer map contains the namespace
            callbacks[name.namespace_][name.value].push_back(callback);
        }

        return *this;
    }

    EventEmitter& off(const std::string& _names) {
        if (_names.empty()) {
            std::cout << "wrong names" << std::endl;
            return *this;
        }

        std::vector<std::string> names = resolveNames(_names);

        for (const std::string& _name : names) {
            auto name = resolveName(_name);

            if (name.namespace_ == "base") {
                // Remove all callbacks from base namespace
                callbacks["base"].clear();
            }
            else {
                // Erase the specific callback
                auto it = callbacks.find(name.namespace_);
                if (it != callbacks.end()) {
                    it->second.erase(name.value);
                    if (it->second.empty()) {
                        callbacks.erase(it);
                    }
                }
            }
        }

        return *this;
    }

    void trigger(const std::string& _name, const std::vector<std::string>& args = {}) {
        if (_name.empty()) {
            std::cout << "wrong name" << std::endl;
            return;
        }

        auto name = resolveName(resolveNames(_name)[0]);

        if (name.namespace_ == "base") {
            for (auto& pair : callbacks) {
                auto& callbackMap = pair.second;
                if (callbackMap.find(name.value) != callbackMap.end()) {
                    for (auto& callback : callbackMap[name.value]) {
                        callback();
                    }
                }
            }
        }
        else if (callbacks.find(name.namespace_) != callbacks.end()) {
            auto& callbackMap = callbacks[name.namespace_];
            if (callbackMap.find(name.value) != callbackMap.end()) {
                for (auto& callback : callbackMap[name.value]) {
                    callback();
                }
            }
        }
        else {
            std::cout << "wrong name" << std::endl;
        }
    }

private:
    struct Name {
        std::string original;
        std::string value;
        std::string namespace_;
    };

    std::vector<std::string> resolveNames(const std::string& _names) {
        std::string names = _names;
        names.erase(std::remove_if(names.begin(), names.end(),
            [](char c) { return !isalnum(c) && c != ' ' && c != ',' && c != '.'; }),
            names.end());
        std::replace(names.begin(), names.end(), ',', ' ');
        std::replace(names.begin(), names.end(), '/', ' ');

        std::vector<std::string> result;
        std::istringstream iss(names);
        for (std::string s; iss >> s;) {
            result.push_back(s);
        }

        return result;
    }

    Name resolveName(const std::string& name) {
        Name newName;
        newName.original = name;

        size_t pos = name.find('.');
        if (pos == std::string::npos) {
            newName.value = name;
            newName.namespace_ = "base";
        }
        else {
            newName.value = name.substr(0, pos);
            newName.namespace_ = name.substr(pos + 1);
        }

        return newName;
    }

    std::unordered_map<std::string, std::unordered_map<std::string, std::vector<std::function<void()>>>> callbacks;
};
